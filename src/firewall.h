#ifndef FIREWALL_H
#define FIREWALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

// External declarations
extern FirewallRule rules[1000];
extern int rule_count;

// Maximum lengths
#define MAX_RULE_LENGTH 1024
#define MAX_IP_LENGTH 46
#define MAX_PORT_LENGTH 20
#define MAX_ACTION_LENGTH 10
#define MAX_PROTOCOL_LENGTH 10
#define MAX_COMMENT_LENGTH 256
#define MAX_CONFIG_LINE 1024
#define CONFIG_FILE "/etc/personal-firewall/firewall.conf"
#define RULES_FILE "/etc/personal-firewall/rules.txt"

// Rule structure
typedef struct {
    int id;
    char action[MAX_ACTION_LENGTH];
    char source[MAX_IP_LENGTH];
    char dest[MAX_IP_LENGTH];
    char port[MAX_PORT_LENGTH];
    char protocol[MAX_PROTOCOL_LENGTH];
    char interface[64];
    char comment[MAX_COMMENT_LENGTH];
    int active;
} FirewallRule;

// Function declarations

// Rule management
int add_firewall_rule(const char *rule_string);
int remove_firewall_rule(int rule_id);
int list_firewall_rules(void);
int enable_firewall_rule(int rule_id);
int disable_firewall_rule(int rule_id);

// Configuration management
int save_rules_to_file(const char *filename);
int load_rules_from_file(const char *filename);
int backup_configuration(const char *backup_file);
int restore_configuration(const char *backup_file);

// Validation functions
int validate_ip(const char *ip);
int validate_port(const char *port);
int validate_cidr(const char *cidr);
int validate_action(const char *action);
int validate_protocol(const char *protocol);
int validate_rule_string(const char *rule_string);

// iptables integration
int execute_iptables_cmd(const char *cmd);
int flush_rules(void);
int get_firewall_status(void);
int apply_rule_to_iptables(const FirewallRule *rule);
int remove_rule_from_iptables(const FirewallRule *rule);

// Utility functions
void print_banner(void);
void log_message(const char *message);
int check_root_privileges(void);
int create_config_directory(void);

// Additional functions
int parse_rule_string(const char *rule_string, FirewallRule *rule);
int get_rule_count(void);
FirewallRule* get_rule_by_id(int rule_id);

#endif // FIREWALL_H

