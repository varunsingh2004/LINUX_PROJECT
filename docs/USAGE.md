# Usage Guide

Complete guide to using Personal Firewall.

## Quick Start

### Interactive Mode

The easiest way to manage your firewall:

```bash
sudo firewall.sh
```

This opens an interactive menu where you can:
- Add rules
- Remove rules
- List rules
- Check status
- Manage configuration

### Command-Line Mode

For scripting and automation:

```bash
sudo firewall <command> [options]
```

## Commands

### Add Rule

Add a new firewall rule:

```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=22"
```

Parameters:
- `action`: ACCEPT, DROP, or REJECT
- `source`: Source IP or CIDR
- `dest`: Destination IP or CIDR
- `port`: Port number or range
- `protocol`: TCP, UDP, or ICMP
- `comment`: Optional description

Examples:
```bash
# Allow SSH
sudo firewall add "action=ACCEPT,protocol=TCP,port=22"

# Block an IP
sudo firewall add "action=DROP,source=192.168.1.100"

# Allow port range
sudo firewall add "action=ACCEPT,protocol=TCP,port=8000:8999"

# Allow with comment
sudo firewall add "action=ACCEPT,protocol=TCP,port=80,comment=\"Web Server\""
```

### Remove Rule

Remove a rule by ID:

```bash
sudo firewall remove 1
```

### List Rules

View all configured rules:

```bash
sudo firewall list
```

### Show Status

Display firewall status and active rules:

```bash
sudo firewall status
```

### Flush Rules

Remove all firewall rules:

```bash
sudo firewall flush
```

**Warning**: This will remove all rules!

### Save Configuration

Save current rules to file:

```bash
sudo firewall save
```

### Load Configuration

Load rules from file:

```bash
sudo firewall load
```

## Interactive Menu Guide

### Main Menu Options

```
1. Add a new rule
2. Remove a rule
3. List all rules
4. Show firewall status
5. Flush all rules
6. Reload configuration
7. Backup configuration
8. Exit
```

### Adding a Rule (Interactive)

1. Select option 1
2. Choose action (ACCEPT, DROP, REJECT)
3. Enter source IP (optional)
4. Enter destination IP (optional)
5. Enter port (optional)
6. Select protocol (optional)
7. Add comment (optional)
8. Confirm and apply

### Removing a Rule (Interactive)

1. Select option 2
2. View list of rules
3. Enter rule ID to remove
4. Confirm removal

## Rule Examples

### Common Firewall Rules

**Allow SSH:**
```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=22"
```

**Allow HTTP/HTTPS:**
```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=80"
sudo firewall add "action=ACCEPT,protocol=TCP,port=443"
```

**Block an IP:**
```bash
sudo firewall add "action=DROP,source=192.168.1.100"
```

**Allow local network:**
```bash
sudo firewall add "action=ACCEPT,source=192.168.1.0/24"
```

**Allow ICMP (ping):**
```bash
sudo firewall add "action=ACCEPT,protocol=ICMP"
```

**Block port range:**
```bash
sudo firewall add "action=DROP,protocol=TCP,port=8000:8999"
```

### Advanced Rules

**Allow SSH from specific IP:**
```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=22,source=192.168.1.50"
```

**Forward to specific destination:**
```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=80,dest=10.0.0.1"
```

**Multiple conditions:**
```bash
sudo firewall add "action=ACCEPT,protocol=TCP,port=3306,source=192.168.1.0/24,comment=\"MySQL from local network\""
```

## Best Practices

### Security

1. **Always keep SSH access** before applying rules
2. **Test rules** on non-critical systems first
3. **Log dropped packets** for analysis
4. **Regular backups** of configuration
5. **Review logs** regularly

### Performance

1. **Order matters** - place most common rules first
2. **Use CIDR** for network ranges instead of multiple IPs
3. **Combine conditions** when possible
4. **Keep rule count** reasonable

### Maintenance

1. **Document** your rules with comments
2. **Version control** your configurations
3. **Regular audits** of active rules
4. **Update** regularly
5. **Monitor** firewall logs

## Scripting Examples

### Bash Script

```bash
#!/bin/bash
# Setup basic firewall

sudo firewall flush

# Allow SSH
sudo firewall add "action=ACCEPT,protocol=TCP,port=22"

# Allow HTTP/HTTPS
sudo firewall add "action=ACCEPT,protocol=TCP,port=80"
sudo firewall add "action=ACCEPT,protocol=TCP,port=443"

# Block specific IP
sudo firewall add "action=DROP,source=192.168.1.100"

sudo firewall save
```

### Python Script

```python
#!/usr/bin/env python3
import subprocess

def add_rule(rule_string):
    subprocess.run(['sudo', 'firewall', 'add', rule_string])

# Add rules
add_rule('action=ACCEPT,protocol=TCP,port=22')
add_rule('action=ACCEPT,protocol=TCP,port=80')
add_rule('action=DROP,source=192.168.1.100')
```

## Troubleshooting

### Rule Not Working

1. Check rule syntax
2. Verify rule is applied: `sudo firewall status`
3. Check iptables directly: `sudo iptables -L -n`
4. Review logs: `sudo tail -f /var/log/personal-firewall.log`

### Can't Connect

1. Check if port is blocked
2. Verify source IP isn't blocked
3. Test with firewall status
4. Temporarily disable firewall to test

### Configuration Lost

1. Check backup: `/etc/personal-firewall/backup_*.conf`
2. Restore from backup
3. Check file permissions

## Advanced Usage

### Integration with Other Tools

**With fail2ban:**
```bash
# fail2ban can add rules automatically
```

**With monitoring:**
```bash
# Monitor firewall status
watch -n 1 'sudo firewall status'
```

**With automation:**
```bash
# Add to cron for regular updates
0 2 * * * /usr/local/bin/firewall.sh
```

## Getting Help

- Read this guide
- Check example configurations
- Review firewall logs
- Open GitHub issues

