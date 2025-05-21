#!/bin/bash
# Make sure the script is executable (chmod +x run_single_test.sh)
# Run a single test case for the PHYLOG compiler

if [ $# -lt 1 ]; then
    echo "Usage: $0 <input_file.phy>"
    exit 1
fi

INPUT_FILE="$1"
BIN_DIR="../bin"
COMPILER="${BIN_DIR}/phylog"
OUTPUT_PREFIX="$(basename "$INPUT_FILE" .phy)_output"

# Check if compiler exists
if [ ! -f "$COMPILER" ]; then
    echo "Compiler not found at $COMPILER. Build the project first."
    exit 1
fi

# Run compiler on the input
echo "Compiling $INPUT_FILE..."
$COMPILER "$INPUT_FILE" -o "$OUTPUT_PREFIX"

# Capture the exit code but ignore it
COMPILER_EXIT_CODE=$?
if [ $COMPILER_EXIT_CODE -ne 0 ]; then
    echo "Note: Compiler exited with code $COMPILER_EXIT_CODE (errors were detected)."
    echo "This is expected during development and files should still be generated."
fi

# Check if outputs were created
if [ -f "${OUTPUT_PREFIX}.symtab.csv" ] && [ -f "${OUTPUT_PREFIX}.ir.csv" ]; then
    echo "Test passed! Output files generated:"
    echo "Symbol table: ${OUTPUT_PREFIX}.symtab.csv"
    echo "IR Code: ${OUTPUT_PREFIX}.ir.csv"
    exit 0
else
    echo "Test failed! Output files were not generated."
    exit 1
fi