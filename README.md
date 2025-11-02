# Personal Firewall

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Linux](https://img.shields.io/badge/Platform-Linux-blue.svg)](https://www.linux.org/)
[![C](https://img.shields.io/badge/Language-C-green.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Shell](https://img.shields.io/badge/Frontend-Bash-orange.svg)](https://www.gnu.org/software/bash/)

A comprehensive Personal Firewall solution with a **shell script frontend** for easy rule management and a **C backend** that translates rules into `iptables` commands for filtering network traffic based on IPs and ports.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Usage](#usage)
- [Configuration](#configuration)
- [Examples](#examples)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This Personal Firewall application provides:

- **User-Friendly Frontend**: Shell script interface for easy rule management
- **Powerful Backend**: C implementation for efficient iptables command generation
- **Flexible Rules**: Support for IP-based, port-based, and protocol-based filtering
- **Persistent Configuration**: Save and load firewall rules
- **Security**: Input validation and safe command execution

### What You Can Do

- ✅ Add/Remove firewall rules easily
- ✅ Block or allow traffic by IP address
- ✅ Filter traffic by port numbers
- ✅ Specify protocols (TCP, UDP, ICMP)
- ✅ Save and load configurations
- ✅ Monitor firewall status
- ✅ View and manage rules interactively

## ✨ Features

### Core Capabilities

- **Interactive Menu System**: User-friendly CLI interface
- **Rule Management**: Add, remove, list, and modify rules
- **IP Filtering**: Allow/Block specific IP addresses or ranges
- **Port Filtering**: Allow/Block specific ports or port ranges
- **Protocol Support**: TCP, UDP, and ICMP
- **Configuration Persistence**: Save and load rules
- **Status Monitoring**: Check firewall status
- **Logging**: Audit trail of all changes

### Security Features

- **Input Validation**: All inputs validated before execution
- **Command Injection Prevention**: Sanitized command generation
- **Error Handling**: Graceful error recovery
- **Backup System**: Automatic configuration backups
- **Privilege Management**: Proper root access handling

## 🏗️ Architecture

### System Design

```
┌─────────────────────────────────────────────────────────┐
│                  User Interface Layer                    │
│              (Shell Script Frontend)                     │
├─────────────────────────────────────────────────────────┤
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌─────────┐│
│  │Add Rules │  │Remove    │  │List      │  │Status   ││
│  │          │  │Rules     │  │Rules     │  │Check    ││
│  └──────────┘  └──────────┘  └──────────┘  └─────────┘│
└─────────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────────┐
│                   Backend Layer (C)                      │
│                                                          │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │Rule Parser  │  │Validator     │  │Config Handler│  │
│  └─────────────┘  └──────────────┘  └──────────────┘  │
│                                                          │
│  ┌────────────────────────────────────────────┐        │
│  │      iptables Command Generator            │        │
│  └────────────────────────────────────────────┘        │
└─────────────────────────────────────────────────────────┘
                        ↓
┌─────────────────────────────────────────────────────────┐
│                  System Integration                      │
│                                                          │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │iptables     │  │Config Files  │  │Logging       │  │
│  │Commands     │  │              │  │System        │  │
│  └─────────────┘  └──────────────┘  └──────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### Components

1. **Frontend (Shell Script)**: User interaction and rule management
2. **Backend (C)**: Rule parsing, validation, and iptables integration
3. **Configuration System**: Persistent storage of rules
4. **Logging System**: Audit trail and monitoring

## 🔧 Installation

### Prerequisites

```bash
# Required dependencies
- Linux operating system
- iptables installed
- GCC compiler
- Bash shell
- Root/sudo access
- Make build system
```

### Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/personal-firewall.git
cd personal-firewall
```

### Step 2: Build the Project

```bash
# Compile the C backend
make

# Or manually compile
gcc -o firewall src/firewall.c src/rule_parser.c src/iptables_manager.c src/config_handler.c
```

### Step 3: Install

```bash
# Run installation script
sudo bash install.sh

# Or manually install
sudo cp firewall /usr/local/bin/
sudo cp scripts/*.sh /usr/local/bin/
sudo mkdir -p /etc/personal-firewall
sudo cp config/firewall.conf /etc/personal-firewall/
```

### Step 4: Verify Installation

```bash
# Check if installed correctly
which firewall
firewall --version

# Run the firewall
sudo ./scripts/firewall.sh
```

## 🚀 Quick Start

### Basic Usage

```bash
# Start the interactive firewall manager
sudo ./scripts/firewall.sh

# Or use command-line mode
sudo firewall.sh add rule
sudo firewall.sh list
sudo firewall.sh remove 1
```

### Your First Rule

```bash
# Block a specific IP address
sudo ./scripts/firewall.sh add \
  --action DROP \
  --source 192.168.1.100

# Allow SSH (port 22)
sudo ./scripts/firewall.sh add \
  --action ACCEPT \
  --protocol TCP \
  --port 22

# List all rules
sudo ./scripts/firewall.sh list
```

## 📖 Usage

### Interactive Menu

```bash
sudo ./scripts/firewall.sh
```

This will display:
```
╔═══════════════════════════════════════════╗
║         Personal Firewall Manager         ║
╠═══════════════════════════════════════════╣
║  1. Add a new rule                        ║
║  2. Remove a rule                         ║
║  3. List all rules                        ║
║  4. Show firewall status                  ║
║  5. Reload configuration                  ║
║  6. Backup configuration                  ║
║  7. View logs                             ║
║  8. Exit                                  ║
╚═══════════════════════════════════════════╝
```

### Command-Line Interface

```bash
# Add a rule
./firewall.sh add --action DROP --source 192.168.1.100

# Remove a rule by ID
./firewall.sh remove 1

# List all rules
./firewall.sh list

# Show status
./firewall.sh status

# Reload configuration
./firewall.sh reload

# Backup configuration
./firewall.sh backup
```

### Rule Syntax

```
Action:     ACCEPT | DROP | REJECT
Source:     IP address or range (e.g., 192.168.1.100 or 192.168.1.0/24)
Destination: IP address or range
Port:        Port number or range (e.g., 80 or 8000:8999)
Protocol:    TCP | UDP | ICMP
Interface:   Network interface name (optional)
Comment:     Descriptive text (optional)
```

## ⚙️ Configuration

### Configuration File

Location: `/etc/personal-firewall/firewall.conf`

```ini
[general]
chain=INPUT
policy=ACCEPT
logging=enabled
log_file=/var/log/personal-firewall.log

[rules]
# Rule format: ID. action=..., source=..., port=...

1. action=DROP, source=192.168.1.100
2. action=ACCEPT, protocol=TCP, port=22
3. action=DROP, protocol=TCP, port=8000:8999
4. action=ACCEPT, source=192.168.1.0/24
```

### Managing Configuration

```bash
# Load configuration
sudo firewall.sh reload

# Backup configuration
sudo firewall.sh backup

# Restore from backup
sudo cp /etc/personal-firewall/backup.conf /etc/personal-firewall/firewall.conf
sudo firewall.sh reload
```

## 💡 Examples

### Example 1: Basic IP Blocking

```bash
# Block a malicious IP
sudo ./scripts/firewall.sh add \
  --action DROP \
  --source 192.168.1.100 \
  --comment "Blocked malicious IP"
```

### Example 2: Allow Web Server

```bash
# Allow HTTP and HTTPS
sudo ./scripts/firewall.sh add \
  --action ACCEPT \
  --protocol TCP \
  --port 80 \
  --comment "Allow HTTP"

sudo ./scripts/firewall.sh add \
  --action ACCEPT \
  --protocol TCP \
  --port 443 \
  --comment "Allow HTTPS"
```

### Example 3: Block Port Range

```bash
# Block a range of ports
sudo ./scripts/firewall.sh add \
  --action DROP \
  --protocol TCP \
  --port 8000:8999 \
  --comment "Block port range"
```

### Example 4: Network-Wide Rule

```bash
# Allow entire subnet
sudo ./scripts/firewall.sh add \
  --action ACCEPT \
  --source 192.168.1.0/24 \
  --comment "Allow local network"
```

## 📚 Documentation

- [Installation Guide](docs/INSTALLATION.md) - Detailed installation instructions
- [Usage Guide](docs/USAGE.md) - Comprehensive usage documentation
- [Architecture](docs/ARCHITECTURE.md) - System design details
- [API Reference](docs/API.md) - Backend API documentation

## 🧪 Testing

```bash
# Run test suite
make test

# Run specific tests
./tests/test_config.sh
./tests/test_rules.sh
```

## 🛡️ Security Notes

- ⚠️ **Requires Root Access**: Firewall operations require sudo/root privileges
- ⚠️ **Test Rules First**: Always test rules in non-production environments
- ⚠️ **Backup Before Changes**: Always backup before making major changes
- ⚠️ **Lockout Prevention**: Ensure SSH access before blocking ports
- ⚠️ **Validation**: All inputs are validated but test rules carefully

## 🤝 Contributing

We welcome contributions! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Linux iptables project
- Open-source firewall community
- All contributors and testers

---

**Made with ❤️ for network security**

For questions or issues, please open an issue on GitHub.

