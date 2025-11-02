# Personal Firewall Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = 
TARGET = firewall
SRCDIR = src
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/firewall.c \
          $(SRCDIR)/rule_parser.c \
          $(SRCDIR)/iptables_manager.c \
          $(SRCDIR)/config_handler.c \
          $(SRCDIR)/validator.c

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Include directories
INCLUDES = -I$(SRCDIR)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete!"

# Compile
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	@echo "Clean complete!"

# Install
install: $(TARGET)
	@echo "Installing $(TARGET)..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo mkdir -p /etc/personal-firewall
	@echo "Installation complete!"

# Uninstall
uninstall:
	@echo "Uninstalling..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation complete!"

# Run tests
test: $(TARGET)
	@echo "Running tests..."
	sudo ./$(TARGET) add "action=ACCEPT,source=127.0.0.1"
	sudo ./$(TARGET) list
	sudo ./$(TARGET) remove 1
	@echo "Tests complete!"

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  test      - Run basic tests"
	@echo "  debug     - Build with debug symbols"
	@echo "  help      - Show this help message"

.PHONY: all clean install uninstall test debug help

