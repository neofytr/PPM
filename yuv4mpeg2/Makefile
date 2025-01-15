# Compiler and flags
CC = gcc
CFLAGS = -O2 -I./header
SRCDIR = ./source
BINDIR = ./binaries

# Find all source files in source directory
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Generate object file names from sources
OBJECTS = $(BINDIR)/ppm.o

# Create binary directory if it doesn't exist
$(shell mkdir -p $(BINDIR))

# Main target
all: $(OBJECTS)

# Compile all source files into single object file
$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES) -o $@

# Clean target
clean:
	rm -f $(OBJECTS)
	rm -rf $(BINDIR)

.PHONY: all clean