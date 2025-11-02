# Architecture Documentation

Technical architecture of Personal Firewall.

## System Overview

```
┌────────────────────────────────────────────────────────┐
│                    User Interface                       │
│              (Interactive Shell Menu)                   │
└────────────────────────────────────────────────────────┘
                        ↓
┌────────────────────────────────────────────────────────┐
│                   Shell Script Layer                    │
│           (firewall.sh - Input Validation)              │
└────────────────────────────────────────────────────────┘
                        ↓
┌────────────────────────────────────────────────────────┐
│                    C Backend Layer                      │
│                                                         │
│  ┌──────────────────────────────────────────────┐     │
│  │          Rule Parser (rule_parser.c)         │     │
│  │  - Parse rule strings                        │     │
│  │  - Validate input                            │     │
│  │  - Manage rule data structures               │     │
│  └──────────────────────────────────────────────┘     │
│                        ↓                                │
│  ┌──────────────────────────────────────────────┐     │
│  │      Validator (validator.c)                 │     │
│  │  - IP validation                             │     │
│  │  - Port validation                           │     │
│  │  - Protocol validation                       │     │
│  └──────────────────────────────────────────────┘     │
│                        ↓                                │
│  ┌──────────────────────────────────────────────┐     │
│  │  iptables Manager (iptables_manager.c)       │     │
│  │  - Generate iptables commands                │     │
│  │  - Execute commands safely                   │     │
│  │  - Monitor status                            │     │
│  └──────────────────────────────────────────────┘     │
│                        ↓                                │
│  ┌──────────────────────────────────────────────┐     │
│  │  Config Handler (config_handler.c)           │     │
│  │  - Save/load rules                           │     │
│  │  - Backup/restore                            │     │
│  │  - File I/O                                  │     │
│  └──────────────────────────────────────────────┘     │
└────────────────────────────────────────────────────────┘
                        ↓
┌────────────────────────────────────────────────────────┐
│                  System Integration                     │
│                                                         │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────┐ │
│  │  iptables    │  │  Config      │  │  Logging    │ │
│  │  Kernel      │  │  Files       │  │  System     │ │
│  └──────────────┘  └──────────────┘  └─────────────┘ │
└────────────────────────────────────────────────────────┘
```

## Component Details

### Frontend (Shell Script)

**File**: `scripts/firewall.sh`

**Responsibilities**:
- User interaction
- Menu display
- Input collection
- Result presentation

**Features**:
- Interactive menu system
- Input validation
- Error handling
- User-friendly output

### Backend (C Language)

#### 1. Main Program

**File**: `src/firewall.c`

**Responsibilities**:
- Command-line parsing
- Routing to appropriate functions
- Error handling
- Exit codes

#### 2. Rule Parser

**File**: `src/rule_parser.c`

**Responsibilities**:
- Parse rule strings
- Build rule structures
- Add/remove rules
- List rules

**Key Functions**:
- `parse_rule_string()`: Parse input into structure
- `add_firewall_rule()`: Add new rule
- `remove_firewall_rule()`: Delete rule
- `list_firewall_rules()`: Display all rules

#### 3. Validator

**File**: `src/validator.c`

**Responsibilities**:
- Validate IP addresses
- Validate ports
- Validate protocols
- Check privileges

**Key Functions**:
- `validate_ip()`: Check IP format
- `validate_port()`: Check port number
- `validate_cidr()`: Check CIDR notation
- `validate_action()`: Check action type

#### 4. iptables Manager

**File**: `src/iptables_manager.c`

**Responsibilities**:
- Generate iptables commands
- Execute commands
- Monitor status
- Flush rules

**Key Functions**:
- `execute_iptables_cmd()`: Run iptables
- `apply_rule_to_iptables()`: Add rule
- `remove_rule_from_iptables()`: Delete rule
- `get_firewall_status()`: Show status

#### 5. Config Handler

**File**: `src/config_handler.c`

**Responsibilities**:
- Save rules to file
- Load rules from file
- Backup configurations
- Manage file I/O

**Key Functions**:
- `save_rules_to_file()`: Write rules
- `load_rules_from_file()`: Read rules
- `backup_configuration()`: Create backup

## Data Structures

### FirewallRule

```c
typedef struct {
    int id;                              // Unique identifier
    char action[MAX_ACTION_LENGTH];      // ACCEPT/DROP/REJECT
    char source[MAX_IP_LENGTH];         // Source IP/CIDR
    char dest[MAX_IP_LENGTH];           // Destination IP/CIDR
    char port[MAX_PORT_LENGTH];         // Port or range
    char protocol[MAX_PROTOCOL_LENGTH];  // TCP/UDP/ICMP
    char interface[64];                  // Network interface
    char comment[MAX_COMMENT_LENGTH];    // Description
    int active;                          // Enabled/disabled
} FirewallRule;
```

### Global State

```c
FirewallRule rules[1000];  // Rule storage
int rule_count;            // Current count
```

## File Flow

### Adding a Rule

1. User enters rule in shell
2. Shell validates input format
3. Shell calls C backend: `firewall add <rule_string>`
4. Parser parses rule string
5. Validator checks all components
6. Rule added to array
7. iptables manager applies to kernel
8. Config handler saves to file
9. Success message displayed

### Removing a Rule

1. User selects rule to remove
2. Shell displays confirmation
3. Shell calls C backend: `firewall remove <id>`
4. Parser finds rule by ID
5. iptables manager removes from kernel
6. Rule removed from array
7. Config handler saves updated rules
8. Success message displayed

## Security Considerations

### Input Validation

- All user input validated
- IP addresses checked
- Ports checked for range
- Protocols whitelisted
- Actions restricted

### Command Injection Prevention

- Direct command execution
- No shell interpretation
- Sanitized inputs
- Safe fork/exec pattern

### Privilege Management

- Root check before operations
- Safe privilege dropping
- Audit logging
- Error handling

## Error Handling

### Levels

1. **Input errors**: Invalid format
2. **Validation errors**: Invalid values
3. **System errors**: iptables failures
4. **File errors**: I/O problems

### Strategy

- Graceful degradation
- Clear error messages
- Logging
- User feedback

## Performance

### Optimization

- Array-based storage
- Linear search (small scale)
- Minimal memory allocation
- Efficient file I/O

### Scalability

- Supports 1000 rules
- Can be extended
- Efficient algorithms
- Minimal overhead

## Configuration

### Files

- **Config**: `/etc/personal-firewall/firewall.conf`
- **Rules**: `/etc/personal-firewall/rules.txt`
- **Logs**: `/var/log/personal-firewall.log`

### Format

```
ID. action=ACTION, source=IP, port=PORT, protocol=PROTO, comment="TEXT"
```

### Persistence

- Save on changes
- Load on startup
- Backup capability
- Restore option

## Integration Points

### iptables

- Direct command execution
- Standard format
- Kernel integration
- Chain management

### System Logs

- syslog compatible
- Log rotation ready
- Timestamped
- Formatted

### Shell Interface

- POSIX compliant
- Standard I/O
- Exit codes
- Error messages

## Future Enhancements

### Potential Improvements

- Database storage
- Web interface
- GUI frontend
- Rule templates
- Automation
- Monitoring
- IPv6 support
- Rule testing

## Build System

### Makefile

- Compilation flags
- Dependency tracking
- Clean targets
- Install targets

### Compilation

```bash
gcc -Wall -Wextra -std=c99 -O2 firewall.c ...
```

### Linking

- No external libraries
- Static linking
- Minimal dependencies
- Portable binary

