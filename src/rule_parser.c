#include "firewall.h"
#include <sys/wait.h>

// Global rules array
FirewallRule rules[1000];
int rule_count = 0;

/**
 * Parse a rule string into a FirewallRule structure
 * Format: action=ACCEPT,source=192.168.1.1,port=80,protocol=TCP
 */
int parse_rule_string(const char *rule_string, FirewallRule *rule) {
    if (!rule_string || !rule) {
        return -1;
    }

    // Initialize rule
    memset(rule, 0, sizeof(FirewallRule));
    rule->active = 1;

    // Copy and sanitize rule string
    char *rule_copy = strdup(rule_string);
    if (!rule_copy) {
        return -1;
    }

    // Tokenize by comma
    char *token = strtok(rule_copy, ",");
    while (token != NULL) {
        // Remove leading/trailing whitespace
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';

        // Parse key=value pairs
        char *equals = strchr(token, '=');
        if (equals) {
            *equals = '\0';
            char *key = token;
            char *value = equals + 1;

            // Remove quotes if present
            if (value[0] == '"' || value[0] == '\'') {
                value++;
                value[strlen(value) - 1] = '\0';
            }

            // Assign values based on key
            if (strcmp(key, "action") == 0) {
                strncpy(rule->action, value, MAX_ACTION_LENGTH - 1);
            } else if (strcmp(key, "source") == 0) {
                strncpy(rule->source, value, MAX_IP_LENGTH - 1);
            } else if (strcmp(key, "destination") == 0 || strcmp(key, "dest") == 0) {
                strncpy(rule->dest, value, MAX_IP_LENGTH - 1);
            } else if (strcmp(key, "port") == 0 || strcmp(key, "dport") == 0) {
                strncpy(rule->port, value, MAX_PORT_LENGTH - 1);
            } else if (strcmp(key, "protocol") == 0) {
                strncpy(rule->protocol, value, MAX_PROTOCOL_LENGTH - 1);
            } else if (strcmp(key, "interface") == 0 || strcmp(key, "i") == 0) {
                strncpy(rule->interface, value, 63);
            } else if (strcmp(key, "comment") == 0) {
                strncpy(rule->comment, value, MAX_COMMENT_LENGTH - 1);
            }
        }

        token = strtok(NULL, ",");
    }

    free(rule_copy);
    return 0;
}

/**
 * Add a firewall rule
 */
int add_firewall_rule(const char *rule_string) {
    if (!rule_string) {
        fprintf(stderr, "Error: Empty rule string\n");
        return -1;
    }

    if (rule_count >= 1000) {
        fprintf(stderr, "Error: Maximum rule limit reached\n");
        return -1;
    }

    FirewallRule rule;
    if (parse_rule_string(rule_string, &rule) != 0) {
        fprintf(stderr, "Error: Failed to parse rule\n");
        return -1;
    }

    // Validate the rule
    if (rule.action[0] == '\0') {
        fprintf(stderr, "Error: Action is required\n");
        return -1;
    }

    if (!validate_action(rule.action)) {
        fprintf(stderr, "Error: Invalid action: %s\n", rule.action);
        return -1;
    }

    if (rule.source[0] && !validate_ip(rule.source) && !validate_cidr(rule.source)) {
        fprintf(stderr, "Error: Invalid source IP: %s\n", rule.source);
        return -1;
    }

    if (rule.dest[0] && !validate_ip(rule.dest) && !validate_cidr(rule.dest)) {
        fprintf(stderr, "Error: Invalid destination IP: %s\n", rule.dest);
        return -1;
    }

    if (rule.port[0] && !validate_port(rule.port)) {
        fprintf(stderr, "Error: Invalid port: %s\n", rule.port);
        return -1;
    }

    if (rule.protocol[0] && !validate_protocol(rule.protocol)) {
        fprintf(stderr, "Error: Invalid protocol: %s\n", rule.protocol);
        return -1;
    }

    // Assign ID and add rule
    rule.id = rule_count + 1;
    rules[rule_count] = rule;
    rule_count++;

    // Apply to iptables if we have root
    if (check_root_privileges()) {
        apply_rule_to_iptables(&rule);
    }

    printf("Rule added successfully with ID: %d\n", rule.id);
    return rule.id;
}

/**
 * Remove a firewall rule by ID
 */
int remove_firewall_rule(int rule_id) {
    if (rule_id < 1 || rule_id > rule_count) {
        fprintf(stderr, "Error: Invalid rule ID\n");
        return -1;
    }

    int index = rule_id - 1;

    // Apply removal to iptables if we have root
    if (check_root_privileges() && rules[index].active) {
        remove_rule_from_iptables(&rules[index]);
    }

    // Remove from array by shifting
    for (int i = index; i < rule_count - 1; i++) {
        rules[i] = rules[i + 1];
        rules[i].id = i + 1;
    }

    rule_count--;
    printf("Rule %d removed successfully\n", rule_id);
    return 0;
}

/**
 * List all firewall rules
 */
int list_firewall_rules(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                     FIREWALL RULES                               ║\n");
    printf("╠══════════════════════════════════════════════════════════════════╣\n");

    if (rule_count == 0) {
        printf("║  No rules configured                                            ║\n");
        printf("╚══════════════════════════════════════════════════════════════════╝\n");
        return 0;
    }

    for (int i = 0; i < rule_count; i++) {
        printf("║  ID: %-3d | Status: %s\n", rules[i].id, rules[i].active ? "ACTIVE" : "DISABLED");
        
        if (rules[i].action[0]) {
            printf("║  Action:    %s\n", rules[i].action);
        }
        if (rules[i].source[0]) {
            printf("║  Source:    %s\n", rules[i].source);
        }
        if (rules[i].dest[0]) {
            printf("║  Dest:      %s\n", rules[i].dest);
        }
        if (rules[i].port[0]) {
            printf("║  Port:      %s\n", rules[i].port);
        }
        if (rules[i].protocol[0]) {
            printf("║  Protocol:  %s\n", rules[i].protocol);
        }
        if (rules[i].comment[0]) {
            printf("║  Comment:   %s\n", rules[i].comment);
        }
        if (i < rule_count - 1) {
            printf("╠══════════════════════════════════════════════════════════════════╣\n");
        }
    }

    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    printf("\nTotal rules: %d\n", rule_count);
    return 0;
}

/**
 * Get rule count
 */
int get_rule_count(void) {
    return rule_count;
}

/**
 * Get rule by ID
 */
FirewallRule* get_rule_by_id(int rule_id) {
    if (rule_id < 1 || rule_id > rule_count) {
        return NULL;
    }
    return &rules[rule_id - 1];
}

