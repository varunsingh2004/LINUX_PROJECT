#include "firewall.h"

/**
 * Validate IP address
 */
int validate_ip(const char *ip) {
    if (!ip || strlen(ip) == 0) {
        return 0;
    }

    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result == 1;
}

/**
 * Validate CIDR notation
 */
int validate_cidr(const char *cidr) {
    if (!cidr) {
        return 0;
    }

    char *slash = strchr(cidr, '/');
    if (!slash) {
        return 0;
    }

    // Validate IP part
    char ip[46];
    strncpy(ip, cidr, slash - cidr);
    ip[slash - cidr] = '\0';
    
    if (!validate_ip(ip)) {
        return 0;
    }

    // Validate subnet mask
    int mask = atoi(slash + 1);
    return (mask >= 0 && mask <= 32);
}

/**
 * Validate port number or range
 */
int validate_port(const char *port) {
    if (!port || strlen(port) == 0) {
        return 0;
    }

    // Check for port range (e.g., 8000:8999)
    char *colon = strchr(port, ':');
    if (colon) {
        char range[256];
        strncpy(range, port, sizeof(range) - 1);
        range[sizeof(range) - 1] = '\0';

        *colon = '\0';
        colon++;
        
        int start = atoi(range);
        int end = atoi(colon);
        
        return (start > 0 && start <= 65535 && end > 0 && end <= 65535 && start <= end);
    }

    // Single port
    int port_num = atoi(port);
    return (port_num > 0 && port_num <= 65535);
}

/**
 * Validate action
 */
int validate_action(const char *action) {
    if (!action) {
        return 0;
    }

    return (strcmp(action, "ACCEPT") == 0 ||
            strcmp(action, "DROP") == 0 ||
            strcmp(action, "REJECT") == 0);
}

/**
 * Validate protocol
 */
int validate_protocol(const char *protocol) {
    if (!protocol) {
        return 0;
    }

    return (strcmp(protocol, "TCP") == 0 ||
            strcmp(protocol, "UDP") == 0 ||
            strcmp(protocol, "ICMP") == 0 ||
            strcmp(protocol, "ALL") == 0);
}

/**
 * Validate entire rule string
 */
int validate_rule_string(const char *rule_string) {
    if (!rule_string) {
        return 0;
    }

    // Basic checks
    if (strlen(rule_string) == 0) {
        return 0;
    }

    // Must contain action
    if (strstr(rule_string, "action=") == NULL) {
        return 0;
    }

    // Parse and validate components
    FirewallRule rule;
    if (parse_rule_string(rule_string, &rule) != 0) {
        return 0;
    }

    // Validate each component
    if (!validate_action(rule.action)) {
        return 0;
    }

    if (rule.source[0] && !validate_ip(rule.source) && !validate_cidr(rule.source)) {
        return 0;
    }

    if (rule.dest[0] && !validate_ip(rule.dest) && !validate_cidr(rule.dest)) {
        return 0;
    }

    if (rule.port[0] && !validate_port(rule.port)) {
        return 0;
    }

    if (rule.protocol[0] && !validate_protocol(rule.protocol)) {
        return 0;
    }

    return 1;
}

/**
 * Check if running with root privileges
 */
int check_root_privileges(void) {
    return (geteuid() == 0);
}

/**
 * Log a message
 */
void log_message(const char *message) {
    FILE *fp = fopen("/var/log/personal-firewall.log", "a");
    if (fp) {
        time_t now;
        time(&now);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // Remove newline
        
        fprintf(fp, "[%s] %s\n", time_str, message);
        fclose(fp);
    }
}

/**
 * Print banner
 */
void print_banner(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                     PERSONAL FIREWALL                            ║\n");
    printf("║                 Network Traffic Management                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

