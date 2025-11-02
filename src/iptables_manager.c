#include "firewall.h"
#include <sys/wait.h>

/**
 * Execute an iptables command
 */
int execute_iptables_cmd(const char *cmd) {
    if (!cmd) {
        return -1;
    }

    printf("Executing: iptables %s\n", cmd);

    // Fork and execute
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child process
        char *args[] = {"iptables", (char *)cmd, NULL};
        if (execvp("iptables", args) < 0) {
            perror("execvp");
            exit(1);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return -1;
    }

    return 0;
}

/**
 * Apply a rule to iptables
 */
int apply_rule_to_iptables(const FirewallRule *rule) {
    if (!rule) {
        return -1;
    }

    char cmd[MAX_RULE_LENGTH] = "";
    char temp[256];

    // Build iptables command
    strcat(cmd, "-A INPUT");

    // Add source IP
    if (rule->source[0]) {
        snprintf(temp, sizeof(temp), " -s %s", rule->source);
        strcat(cmd, temp);
    }

    // Add destination IP
    if (rule->dest[0]) {
        snprintf(temp, sizeof(temp), " -d %s", rule->dest);
        strcat(cmd, temp);
    }

    // Add protocol
    if (rule->protocol[0]) {
        snprintf(temp, sizeof(temp), " -p %s", rule->protocol);
        strcat(cmd, temp);
    }

    // Add port
    if (rule->port[0]) {
        if (strcmp(rule->protocol, "TCP") == 0 || strcmp(rule->protocol, "UDP") == 0) {
            snprintf(temp, sizeof(temp), " --dport %s", rule->port);
            strcat(cmd, temp);
        }
    }

    // Add interface
    if (rule->interface[0]) {
        snprintf(temp, sizeof(temp), " -i %s", rule->interface);
        strcat(cmd, temp);
    }

    // Add comment
    if (rule->comment[0]) {
        snprintf(temp, sizeof(temp), " -m comment --comment \"%s\"", rule->comment);
        strcat(cmd, temp);
    } else {
        snprintf(temp, sizeof(temp), " -m comment --comment \"Rule-ID-%d\"", rule->id);
        strcat(cmd, temp);
    }

    // Add action
    snprintf(temp, sizeof(temp), " -j %s", rule->action);
    strcat(cmd, temp);

    return execute_iptables_cmd(cmd);
}

/**
 * Remove a rule from iptables
 */
int remove_rule_from_iptables(const FirewallRule *rule) {
    if (!rule) {
        return -1;
    }

    char cmd[MAX_RULE_LENGTH] = "";
    char temp[256];

    // Build iptables delete command
    strcat(cmd, "-D INPUT");

    // Add source IP
    if (rule->source[0]) {
        snprintf(temp, sizeof(temp), " -s %s", rule->source);
        strcat(cmd, temp);
    }

    // Add destination IP
    if (rule->dest[0]) {
        snprintf(temp, sizeof(temp), " -d %s", rule->dest);
        strcat(cmd, temp);
    }

    // Add protocol
    if (rule->protocol[0]) {
        snprintf(temp, sizeof(temp), " -p %s", rule->protocol);
        strcat(cmd, temp);
    }

    // Add port
    if (rule->port[0]) {
        if (strcmp(rule->protocol, "TCP") == 0 || strcmp(rule->protocol, "UDP") == 0) {
            snprintf(temp, sizeof(temp), " --dport %s", rule->port);
            strcat(cmd, temp);
        }
    }

    // Add interface
    if (rule->interface[0]) {
        snprintf(temp, sizeof(temp), " -i %s", rule->interface);
        strcat(cmd, temp);
    }

    // Add comment
    if (rule->comment[0]) {
        snprintf(temp, sizeof(temp), " -m comment --comment \"%s\"", rule->comment);
        strcat(cmd, temp);
    } else {
        snprintf(temp, sizeof(temp), " -m comment --comment \"Rule-ID-%d\"", rule->id);
        strcat(cmd, temp);
    }

    // Add action
    snprintf(temp, sizeof(temp), " -j %s", rule->action);
    strcat(cmd, temp);

    return execute_iptables_cmd(cmd);
}

/**
 * Flush all rules from iptables
 */
int flush_rules(void) {
    printf("Flushing iptables rules...\n");
    
    // Flush INPUT chain
    int ret = execute_iptables_cmd("-F INPUT");
    if (ret != 0) {
        fprintf(stderr, "Warning: Failed to flush INPUT chain\n");
    }

    // Reset default policy
    execute_iptables_cmd("-P INPUT ACCEPT");
    
    printf("Rules flushed successfully\n");
    return 0;
}

/**
 * Get firewall status
 */
int get_firewall_status(void) {
    printf("\n╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    FIREWALL STATUS                               ║\n");
    printf("╠══════════════════════════════════════════════════════════════════╣\n");

    // Check if iptables is available
    int ret = execute_iptables_cmd("-L -n");
    if (ret == 0) {
        printf("║  Status:     RUNNING                                             ║\n");
    } else {
        printf("║  Status:     NOT AVAILABLE                                       ║\n");
    }

    // List INPUT chain
    printf("╠══════════════════════════════════════════════════════════════════╣\n");
    printf("║  INPUT Chain Rules:                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
    
    execute_iptables_cmd("-L INPUT -n -v --line-numbers");

    return ret;
}

