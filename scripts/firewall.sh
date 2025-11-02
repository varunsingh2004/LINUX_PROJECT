#!/bin/bash
###############################################################################
# Personal Firewall Frontend
# Shell script interface for firewall rule management
###############################################################################

set -euo pipefail

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Path to firewall binary
FIREWALL_BIN="./firewall"
if [ ! -f "$FIREWALL_BIN" ]; then
    FIREWALL_BIN="firewall"
fi

###############################################################################
# Functions
###############################################################################

print_banner() {
    clear
    echo -e "${CYAN}"
    echo "╔══════════════════════════════════════════════════════════════════╗"
    echo "║                  PERSONAL FIREWALL MANAGER                      ║"
    echo "╠══════════════════════════════════════════════════════════════════╣"
    echo "║                    User-Friendly Interface                       ║"
    echo "╚══════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
}

print_menu() {
    echo ""
    echo -e "${BLUE}╔══════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║  MAIN MENU                                                      ║${NC}"
    echo -e "${BLUE}╠══════════════════════════════════════════════════════════════════╣${NC}"
    echo -e "${BLUE}║${NC}  1. ${GREEN}Add a new firewall rule${NC}                              ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  2. ${GREEN}Remove a firewall rule${NC}                             ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  3. ${GREEN}List all firewall rules${NC}                            ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  4. ${GREEN}Show firewall status${NC}                               ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  5. ${GREEN}Flush all rules${NC}                                     ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  6. ${GREEN}Reload configuration${NC}                                ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  7. ${GREEN}Backup configuration${NC}                                ${BLUE}║${NC}"
    echo -e "${BLUE}║${NC}  8. ${YELLOW}Exit${NC}                                                ${BLUE}║${NC}"
    echo -e "${BLUE}╚══════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

check_root() {
    if [[ $EUID -ne 0 ]]; then
        echo -e "${RED}Error: This script requires root privileges${NC}"
        echo -e "${YELLOW}Please run with: sudo $0${NC}"
        exit 1
    fi
}

add_rule_menu() {
    echo -e "${CYAN}=== Add New Firewall Rule ===${NC}"
    echo ""
    
    echo -e "${BLUE}Select action:${NC}"
    echo "1. ACCEPT (Allow traffic)"
    echo "2. DROP (Block traffic silently)"
    echo "3. REJECT (Block with error)"
    read -p "Choice [1-3]: " action_choice
    
    case $action_choice in
        1) action="ACCEPT" ;;
        2) action="DROP" ;;
        3) action="REJECT" ;;
        *) echo -e "${RED}Invalid choice${NC}"; return ;;
    esac
    
    echo ""
    echo -e "${BLUE}Add source IP? (Optional)${NC}"
    read -p "Source IP or CIDR (leave empty to skip): " source_ip
    
    echo ""
    echo -e "${BLUE}Add destination IP? (Optional)${NC}"
    read -p "Destination IP or CIDR (leave empty to skip): " dest_ip
    
    echo ""
    echo -e "${BLUE}Add port? (Optional)${NC}"
    read -p "Port number or range (e.g., 80 or 8000:8999): " port
    
    echo ""
    echo -e "${BLUE}Add protocol? (Optional)${NC}"
    echo "1. TCP"
    echo "2. UDP"
    echo "3. ICMP"
    read -p "Choice [1-3] or leave empty: " protocol_choice
    
    protocol=""
    case $protocol_choice in
        1) protocol="TCP" ;;
        2) protocol="UDP" ;;
        3) protocol="ICMP" ;;
        *) protocol="" ;;
    esac
    
    echo ""
    echo -e "${BLUE}Add comment? (Optional)${NC}"
    read -p "Comment: " comment
    
    # Build rule string
    rule_string="action=${action}"
    
    if [[ -n "$source_ip" ]]; then
        rule_string="${rule_string}, source=${source_ip}"
    fi
    
    if [[ -n "$dest_ip" ]]; then
        rule_string="${rule_string}, dest=${dest_ip}"
    fi
    
    if [[ -n "$port" ]]; then
        rule_string="${rule_string}, port=${port}"
    fi
    
    if [[ -n "$protocol" ]]; then
        rule_string="${rule_string}, protocol=${protocol}"
    fi
    
    if [[ -n "$comment" ]]; then
        rule_string="${rule_string}, comment=\"${comment}\""
    fi
    
    echo ""
    echo -e "${YELLOW}Generated rule: ${rule_string}${NC}"
    read -p "Apply this rule? [y/N]: " confirm
    
    if [[ "$confirm" =~ ^[Yy]$ ]]; then
        if sudo "$FIREWALL_BIN" add "$rule_string"; then
            echo -e "${GREEN}Rule added successfully!${NC}"
        else
            echo -e "${RED}Failed to add rule${NC}"
        fi
    else
        echo -e "${YELLOW}Rule not added${NC}"
    fi
    
    read -p "Press Enter to continue..."
}

remove_rule_menu() {
    echo -e "${CYAN}=== Remove Firewall Rule ===${NC}"
    echo ""
    
    sudo "$FIREWALL_BIN" list
    echo ""
    
    read -p "Enter rule ID to remove: " rule_id
    
    if [[ ! "$rule_id" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}Invalid rule ID${NC}"
        read -p "Press Enter to continue..."
        return
    fi
    
    read -p "Are you sure you want to remove rule $rule_id? [y/N]: " confirm
    
    if [[ "$confirm" =~ ^[Yy]$ ]]; then
        if sudo "$FIREWALL_BIN" remove "$rule_id"; then
            echo -e "${GREEN}Rule removed successfully!${NC}"
        else
            echo -e "${RED}Failed to remove rule${NC}"
        fi
    else
        echo -e "${YELLOW}Rule not removed${NC}"
    fi
    
    read -p "Press Enter to continue..."
}

list_rules_menu() {
    echo -e "${CYAN}=== Firewall Rules ===${NC}"
    echo ""
    
    sudo "$FIREWALL_BIN" list
    
    read -p "Press Enter to continue..."
}

show_status_menu() {
    echo -e "${CYAN}=== Firewall Status ===${NC}"
    echo ""
    
    sudo "$FIREWALL_BIN" status
    
    read -p "Press Enter to continue..."
}

flush_rules_menu() {
    echo -e "${CYAN}=== Flush All Rules ===${NC}"
    echo ""
    echo -e "${RED}WARNING: This will remove all firewall rules!${NC}"
    echo ""
    
    read -p "Are you absolutely sure? [yes/no]: " confirm
    
    if [[ "$confirm" == "yes" ]]; then
        if sudo "$FIREWALL_BIN" flush; then
            echo -e "${GREEN}All rules flushed successfully!${NC}"
        else
            echo -e "${RED}Failed to flush rules${NC}"
        fi
    else
        echo -e "${YELLOW}Flush cancelled${NC}"
    fi
    
    read -p "Press Enter to continue..."
}

reload_config_menu() {
    echo -e "${CYAN}=== Reload Configuration ===${NC}"
    echo ""
    
    if sudo "$FIREWALL_BIN" load; then
        echo -e "${GREEN}Configuration reloaded successfully!${NC}"
    else
        echo -e "${RED}Failed to reload configuration${NC}"
    fi
    
    read -p "Press Enter to continue..."
}

backup_config_menu() {
    echo -e "${CYAN}=== Backup Configuration ===${NC}"
    echo ""
    
    backup_file="/etc/personal-firewall/backup_$(date +%Y%m%d_%H%M%S).conf"
    
    if sudo "$FIREWALL_BIN" save && sudo cp /etc/personal-firewall/rules.txt "$backup_file"; then
        echo -e "${GREEN}Backup created: $backup_file${NC}"
    else
        echo -e "${RED}Failed to create backup${NC}"
    fi
    
    read -p "Press Enter to continue..."
}

###############################################################################
# Main
###############################################################################

main() {
    check_root
    
    while true; do
        print_banner
        print_menu
        
        read -p "Select an option [1-8]: " choice
        
        case $choice in
            1) add_rule_menu ;;
            2) remove_rule_menu ;;
            3) list_rules_menu ;;
            4) show_status_menu ;;
            5) flush_rules_menu ;;
            6) reload_config_menu ;;
            7) backup_config_menu ;;
            8) 
                echo -e "${GREEN}Goodbye!${NC}"
                exit 0
                ;;
            *)
                echo -e "${RED}Invalid option${NC}"
                sleep 1
                ;;
        esac
    done
}

main "$@"

