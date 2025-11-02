#!/bin/bash
###############################################################################
# Personal Firewall Installation Script
###############################################################################

set -euo pipefail

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_header() {
    echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║      Personal Firewall Installation Script              ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

check_root() {
    if [[ $EUID -ne 0 ]]; then
        echo -e "${RED}Error: This script requires root privileges${NC}"
        echo -e "${YELLOW}Please run with: sudo bash install.sh${NC}"
        exit 1
    fi
}

check_dependencies() {
    echo -e "${YELLOW}Checking dependencies...${NC}"
    
    local missing_deps=0
    
    if ! command -v gcc &> /dev/null; then
        echo -e "${RED}  ❌ GCC compiler not found${NC}"
        missing_deps=1
    else
        echo -e "${GREEN}  ✓ GCC compiler found${NC}"
    fi
    
    if ! command -v iptables &> /dev/null; then
        echo -e "${RED}  ❌ iptables not found${NC}"
        missing_deps=1
    else
        echo -e "${GREEN}  ✓ iptables found${NC}"
    fi
    
    if ! command -v make &> /dev/null; then
        echo -e "${RED}  ❌ Make not found${NC}"
        missing_deps=1
    else
        echo -e "${GREEN}  ✓ Make found${NC}"
    fi
    
    if [[ $missing_deps -eq 1 ]]; then
        echo ""
        echo -e "${RED}Please install missing dependencies and try again${NC}"
        echo "Install on Ubuntu/Debian: sudo apt-get install build-essential iptables"
        echo "Install on Fedora: sudo dnf install gcc make iptables"
        exit 1
    fi
    
    echo ""
}

compile_project() {
    echo -e "${YELLOW}Compiling project...${NC}"
    
    if make clean; then
        echo -e "${GREEN}  ✓ Clean complete${NC}"
    fi
    
    if make; then
        echo -e "${GREEN}  ✓ Compilation successful${NC}"
    else
        echo -e "${RED}  ❌ Compilation failed${NC}"
        exit 1
    fi
    echo ""
}

install_files() {
    echo -e "${YELLOW}Installing files...${NC}"
    
    # Install binary
    cp firewall /usr/local/bin/firewall
    chmod +x /usr/local/bin/firewall
    echo -e "${GREEN}  ✓ Installed firewall binary${NC}"
    
    # Install script
    cp scripts/firewall.sh /usr/local/bin/firewall.sh
    chmod +x /usr/local/bin/firewall.sh
    echo -e "${GREEN}  ✓ Installed firewall script${NC}"
    
    # Create config directory
    mkdir -p /etc/personal-firewall
    echo -e "${GREEN}  ✓ Created config directory${NC}"
    
    # Create default config if it doesn't exist
    if [[ ! -f /etc/personal-firewall/firewall.conf ]]; then
        cat > /etc/personal-firewall/firewall.conf << 'EOF'
# Personal Firewall Configuration

[general]
chain=INPUT
policy=ACCEPT
logging=enabled
log_file=/var/log/personal-firewall.log

[rules]
# Add your rules here

EOF
        echo -e "${GREEN}  ✓ Created default configuration${NC}"
    fi
    
    # Create log directory
    touch /var/log/personal-firewall.log
    chmod 644 /var/log/personal-firewall.log
    echo -e "${GREEN}  ✓ Created log file${NC}"
    
    echo ""
}

verify_installation() {
    echo -e "${YELLOW}Verifying installation...${NC}"
    
    if command -v firewall &> /dev/null; then
        echo -e "${GREEN}  ✓ firewall command is available${NC}"
    else
        echo -e "${RED}  ❌ firewall command not found${NC}"
    fi
    
    if command -v firewall.sh &> /dev/null; then
        echo -e "${GREEN}  ✓ firewall.sh command is available${NC}"
    else
        echo -e "${RED}  ❌ firewall.sh command not found${NC}"
    fi
    
    if [[ -d /etc/personal-firewall ]]; then
        echo -e "${GREEN}  ✓ Config directory exists${NC}"
    else
        echo -e "${RED}  ❌ Config directory not found${NC}"
    fi
    
    echo ""
}

print_success() {
    echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║         Installation Complete Successfully!            ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"
    echo ""
    echo "To use the firewall:"
    echo "  Interactive mode:     sudo firewall.sh"
    echo "  Command-line mode:    sudo firewall list"
    echo ""
    echo "For help:"
    echo "  firewall --help"
    echo ""
}

main() {
    print_header
    check_root
    check_dependencies
    compile_project
    install_files
    verify_installation
    print_success
}

main "$@"

