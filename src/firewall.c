#include "firewall.h"

/**
 * Main entry point for the firewall CLI tool
 */
int main(int argc, char *argv[]) {
    print_banner();

    // Check for root privileges
    if (!check_root_privileges()) {
        fprintf(stderr, "Error: This program requires root privileges\n");
        fprintf(stderr, "Please run with sudo\n");
        return 1;
    }

    // Create config directory if it doesn't exist
    create_config_directory();

    // Load existing rules
    load_rules_from_file(NULL);

    // Parse command-line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [options]\n", argv[0]);
        fprintf(stderr, "Commands:\n");
        fprintf(stderr, "  add <rule>     - Add a new firewall rule\n");
        fprintf(stderr, "  remove <id>    - Remove rule by ID\n");
        fprintf(stderr, "  list           - List all rules\n");
        fprintf(stderr, "  status         - Show firewall status\n");
        fprintf(stderr, "  flush          - Flush all rules\n");
        fprintf(stderr, "  save           - Save rules to file\n");
        fprintf(stderr, "  load           - Load rules from file\n");
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "add") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: Rule string required\n");
            return 1;
        }
        add_firewall_rule(argv[2]);
    }
    else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Error: Rule ID required\n");
            return 1;
        }
        int rule_id = atoi(argv[2]);
        remove_firewall_rule(rule_id);
    }
    else if (strcmp(command, "list") == 0) {
        list_firewall_rules();
    }
    else if (strcmp(command, "status") == 0) {
        get_firewall_status();
    }
    else if (strcmp(command, "flush") == 0) {
        flush_rules();
        rule_count = 0;
    }
    else if (strcmp(command, "save") == 0) {
        save_rules_to_file(NULL);
    }
    else if (strcmp(command, "load") == 0) {
        load_rules_from_file(NULL);
    }
    else {
        fprintf(stderr, "Error: Unknown command: %s\n", command);
        return 1;
    }

    // Save rules after any changes
    if (strcmp(command, "add") == 0 || strcmp(command, "remove") == 0) {
        save_rules_to_file(NULL);
    }

    return 0;
}

