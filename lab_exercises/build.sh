#!/bin/bash

# Set directories
SRC_DIR="./c_code"    # Directory where .c files are located
BIN_DIR="./bin"       # Directory where compiled binaries will be stored

# Ensure at least one argument (filename) is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename.c>"
    exit 1
fi

# Get the full path of the source file
SRC_FILE="$SRC_DIR/$1"

# Check if the source file exists
if [ ! -f "$SRC_FILE" ]; then
    echo "Error: Source file '$SRC_FILE' not found!"
    exit 1
fi

# Get the base name of the file (without extension)
BASE_NAME=$(basename "$1" .c)
OUTPUT="$BIN_DIR/$BASE_NAME"  # Output binary

# Ensure the bin directory exists
mkdir -p "$BIN_DIR"

# Compile the C program
gcc "$SRC_FILE" -o "$OUTPUT"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! Binary: $OUTPUT"
    # don't run from here, causing problems :( in file path
    # Change to `lab_exercises/` before running the binary
    # cd "$(dirname "$0")" || exit 1

    # echo "Running the binary..."
    # "./bin/$BASE_NAME"
else
    echo "Compilation failed!"
    exit 1
fi
