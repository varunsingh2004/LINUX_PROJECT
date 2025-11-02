#include "firewall.h"

// External references
extern FirewallRule rules[1000];
extern int rule_count;

/**
 * Save rules to configuration file
 */
int save_rules_to_file(const char *filename) {
    FILE *fp;
    
    if (!filename) {
        filename = RULES_FILE;
    }

    fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file for writing: %s\n", filename);
        return -1;
    }

    // Write header
    fprintf(fp, "# Personal Firewall Rules Configuration\n");
    fprintf(fp, "# Auto-generated - Do not edit manually\n");
    fprintf(fp, "# Total rules: %d\n\n", rule_count);

    // Write each rule
    for (int i = 0; i < rule_count; i++) {
        fprintf(fp, "%d. action=%s", i + 1, rules[i].action);
        
        if (rules[i].source[0]) {
            fprintf(fp, ", source=%s", rules[i].source);
        }
        if (rules[i].dest[0]) {
            fprintf(fp, ", dest=%s", rules[i].dest);
        }
        if (rules[i].port[0]) {
            fprintf(fp, ", port=%s", rules[i].port);
        }
        if (rules[i].protocol[0]) {
            fprintf(fp, ", protocol=%s", rules[i].protocol);
        }
        if (rules[i].interface[0]) {
            fprintf(fp, ", interface=%s", rules[i].interface);
        }
        if (rules[i].comment[0]) {
            fprintf(fp, ", comment=\"%s\"", rules[i].comment);
        }
        
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Configuration saved to %s\n", filename);
    return 0;
}

/**
 * Load rules from configuration file
 */
int load_rules_from_file(const char *filename) {
    FILE *fp;
    char line[MAX_CONFIG_LINE];

    if (!filename) {
        filename = RULES_FILE;
    }

    fp = fopen(filename, "r");
    if (!fp) {
        // File doesn't exist - not an error
        printf("No existing configuration found\n");
        return 0;
    }

    // Reset rule count
    rule_count = 0;

    // Read line by line
    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        // Remove trailing newline
        line[strcspn(line, "\n\r")] = '\0';

        // Find the rule part (after "ID. ")
        char *rule_start = strstr(line, ". ");
        if (rule_start) {
            rule_start += 2;
        } else {
            // No ID format, use entire line
            rule_start = line;
        }

        // Parse and add rule
        if (parse_rule_string(rule_start, &rules[rule_count]) == 0) {
            rules[rule_count].id = rule_count + 1;
            rule_count++;

            if (rule_count >= 1000) {
                fprintf(stderr, "Warning: Maximum rules reached\n");
                break;
            }
        }
    }

    fclose(fp);
    printf("Loaded %d rules from %s\n", rule_count, filename);
    return rule_count;
}

/**
 * Backup configuration
 */
int backup_configuration(const char *backup_file) {
    char default_backup[256];
    
    if (!backup_file) {
        snprintf(default_backup, sizeof(default_backup), 
                 "/etc/personal-firewall/backup_%ld.conf", time(NULL));
        backup_file = default_backup;
    }

    return save_rules_to_file(backup_file);
}

/**
 * Restore configuration from backup
 */
int restore_configuration(const char *backup_file) {
    if (!backup_file) {
        fprintf(stderr, "Error: Backup file not specified\n");
        return -1;
    }

    return load_rules_from_file(backup_file);
}

/**
 * Create configuration directory
 */
int create_config_directory(void) {
    struct stat st;
    const char *config_dir = "/etc/personal-firewall";

    if (stat(config_dir, &st) != 0) {
        // Directory doesn't exist, create it
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "mkdir -p %s", config_dir);
        
        if (system(cmd) != 0) {
            fprintf(stderr, "Error: Failed to create config directory\n");
            return -1;
        }
    }

    return 0;
}

