# Installation Guide

Complete guide to installing Personal Firewall.

## Prerequisites

### System Requirements

- **Operating System**: Linux (Ubuntu, Fedora, Debian, CentOS, etc.)
- **Kernel**: Linux kernel 2.6+ (iptables support)
- **Architecture**: x86_64 or i386

### Required Software

- **GCC Compiler**: For building the C backend
- **iptables**: Linux firewall tool
- **Make**: Build automation
- **Bash**: Shell for frontend
- **Root Access**: For firewall operations

## Installation Methods

### Method 1: Automated Installation (Recommended)

The fastest way to get started:

```bash
# Clone the repository
git clone https://github.com/yourusername/personal-firewall.git
cd personal-firewall

# Run installation script
sudo bash install.sh
```

The installation script will:
1. Check dependencies
2. Compile the project
3. Install binaries
4. Create configuration directories
5. Set up logging

### Method 2: Manual Installation

Step-by-step manual installation:

#### Step 1: Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential gcc make iptables
```

**Fedora:**
```bash
sudo dnf install gcc make iptables
```

**CentOS/RHEL:**
```bash
sudo yum install gcc make iptables
```

#### Step 2: Build the Project

```bash
cd personal-firewall

# Clean and compile
make clean
make

# Verify compilation
./firewall --help
```

#### Step 3: Install Files

```bash
# Install binary
sudo cp firewall /usr/local/bin/
sudo chmod +x /usr/local/bin/firewall

# Install script
sudo cp scripts/firewall.sh /usr/local/bin/firewall.sh
sudo chmod +x /usr/local/bin/firewall.sh

# Create config directory
sudo mkdir -p /etc/personal-firewall

# Create log file
sudo touch /var/log/personal-firewall.log
sudo chmod 644 /var/log/personal-firewall.log
```

#### Step 4: Verify Installation

```bash
# Test the installation
sudo firewall list
sudo firewall.sh
```

## Post-Installation

### Initial Configuration

1. **Review default configuration:**
   ```bash
   sudo cat /etc/personal-firewall/firewall.conf
   ```

2. **Start interactive mode:**
   ```bash
   sudo firewall.sh
   ```

3. **Add your first rule:**
   - Select option 1 (Add rule)
   - Follow the prompts

### Configuration Files

Default locations:
- **Config**: `/etc/personal-firewall/firewall.conf`
- **Rules**: `/etc/personal-firewall/rules.txt`
- **Logs**: `/var/log/personal-firewall.log`
- **Binary**: `/usr/local/bin/firewall`
- **Script**: `/usr/local/bin/firewall.sh`

### Making Rules Persistent

Rules are saved automatically to `/etc/personal-firewall/rules.txt`.

To ensure rules persist across reboots, you may want to:

1. Create a systemd service
2. Add to /etc/rc.local
3. Use cron to reload rules

## Troubleshooting

### Compilation Errors

**Error: iptables.h not found**
```bash
sudo apt-get install libip6tc-dev
```

**Error: Permission denied**
```bash
chmod +x install.sh
sudo bash install.sh
```

### Runtime Errors

**Error: Requires root privileges**
```bash
sudo firewall.sh
# or
sudo firewall list
```

**Error: iptables not found**
```bash
# Install iptables
sudo apt-get install iptables

# Verify
which iptables
```

**Error: Cannot write to config directory**
```bash
sudo chown -R root:root /etc/personal-firewall
sudo chmod 755 /etc/personal-firewall
```

## Uninstallation

To remove Personal Firewall:

```bash
# Remove binaries
sudo rm -f /usr/local/bin/firewall
sudo rm -f /usr/local/bin/firewall.sh

# Remove config (optional)
sudo rm -rf /etc/personal-firewall

# Remove logs (optional)
sudo rm -f /var/log/personal-firewall.log

# From source directory
make clean
```

## Next Steps

After installation:

1. Read [Usage Guide](USAGE.md)
2. Review example configurations in `examples/`
3. Test with basic rules first
4. Document your custom rules

## Support

For installation issues:
1. Check this guide
2. Review error messages
3. Open an issue on GitHub
4. Check system logs

