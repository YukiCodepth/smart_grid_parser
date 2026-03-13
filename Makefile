# --- Compiler Settings ---
CC = gcc
CFLAGS = -Wall -Wextra -Iinc

# --- Folders ---
SRC_DIR = src
BUILD_DIR = build

# --- Files ---
# This finds all .c files in the src folder
SOURCES = $(wildcard $(SRC_DIR)/*.c)
# This converts the .c file names into .o (object) file names in the build folder
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# --- Target Executable Name ---
TARGET = $(BUILD_DIR)/telemetry_engine

# --- Build Rules ---
# Default rule when you just type 'make'
all: $(TARGET)

# How to build the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# How to build the individual .o files from .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up rule to delete compiled files
clean:
	rm -rf $(BUILD_DIR)/*
