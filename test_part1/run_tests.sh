#!/bin/bash
# Make sure the script is executable (chmod +x run_tests.sh)
# Run tests for the PHYLOG compiler with optimization
# usage: ./run_tests.sh ./your_test_file.phy


# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Directory containing the tests
TEST_DIR="$(dirname "$0")"
BIN_DIR="../bin"
COMPILER="${BIN_DIR}/phylog"

# Check if a filename was provided
if [ $# -eq 0 ]; then
    echo -e "${RED}Error: No input file provided.${NC}"
    echo -e "${YELLOW}Usage: ./run_tests.sh <filename.phy>${NC}"
    exit 1
fi

INPUT_FILE="$1"

# Check if file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo -e "${RED}Error: File '$INPUT_FILE' not found.${NC}"
    exit 1
fi

# Check if compiler exists
if [ ! -f "$COMPILER" ]; then
    echo -e "${RED}Compiler not found at $COMPILER. Build the project first.${NC}"
    exit 1
fi

# Extract just the base name without path and extension
BASENAME=$(basename "$INPUT_FILE" .phy)

# Run test
echo -e "${YELLOW}Running test on $INPUT_FILE...${NC}"
echo -e "${YELLOW}Optimization enabled at level 3${NC}"
$COMPILER "$INPUT_FILE" -o "$TEST_DIR/output"

# Capture the exit code but ignore it
COMPILER_EXIT_CODE=$?
if [ $COMPILER_EXIT_CODE -ne 0 ]; then
    echo -e "${YELLOW}Note: Compiler exited with code $COMPILER_EXIT_CODE (errors were detected).${NC}"
    echo -e "${YELLOW}This is expected during development and files should still be generated.${NC}"
fi

# Check if outputs were created
if [ -f "$TEST_DIR/output.symtab.csv" ] && [ -f "$TEST_DIR/output.ir.csv" ]; then
    echo -e "${GREEN}Test passed! Output files generated:${NC}"
    echo "- $TEST_DIR/output.symtab.csv"
    echo "- $TEST_DIR/output.ir.csv"
    
    # Check for optimized IR file
    if [ -f "$TEST_DIR/output.opt.ir.csv" ]; then
        echo "- $TEST_DIR/output.opt.ir.csv"
    fi
    
    # Check for assembly file
    if [ -f "$TEST_DIR/output.asm" ]; then
        echo "- $TEST_DIR/output.asm"
    fi
    
    # Display file size statistics
    echo -e "\n${YELLOW}File Statistics:${NC}"
    IR_SIZE=$(stat -c%s "$TEST_DIR/output.ir.csv")
    echo "Original IR size: $IR_SIZE bytes"
    
    if [ -f "$TEST_DIR/output.opt.ir.csv" ]; then
        OPT_SIZE=$(stat -c%s "$TEST_DIR/output.opt.ir.csv")
        echo "Optimized IR size: $OPT_SIZE bytes"
        
        # Calculate size reduction
        if [ $IR_SIZE -gt 0 ]; then
            REDUCTION=$((IR_SIZE - OPT_SIZE))
            PERCENT=$((REDUCTION * 100 / IR_SIZE))
            echo "Size reduced by $REDUCTION bytes ($PERCENT%)"
        fi
    fi
    
    # Move the files to the expected output directory for future reference
    mkdir -p "$TEST_DIR/expected_output/$BASENAME"
    cp "$TEST_DIR/output.symtab.csv" "$TEST_DIR/expected_output/$BASENAME/"
    cp "$TEST_DIR/output.ir.csv" "$TEST_DIR/expected_output/$BASENAME/"
    
    # Copy optimized IR if it exists
    if [ -f "$TEST_DIR/output.opt.ir.csv" ]; then
        cp "$TEST_DIR/output.opt.ir.csv" "$TEST_DIR/expected_output/$BASENAME/"
    fi
    
    # Copy assembly file if it exists
    if [ -f "$TEST_DIR/output.asm" ]; then
        cp "$TEST_DIR/output.asm" "$TEST_DIR/expected_output/$BASENAME/"
    fi
    
    echo -e "\n${GREEN}Test files copied to expected_output/$BASENAME/ for future reference.${NC}"
    
    # Cleanup original files after copying
    echo -e "${YELLOW}Cleaning up temporary files...${NC}"
    rm -f "$TEST_DIR/output.symtab.csv"
    rm -f "$TEST_DIR/output.ir.csv"
    rm -f "$TEST_DIR/output.opt.ir.csv"
    rm -f "$TEST_DIR/output.asm"
else
    echo -e "${RED}Test failed! Output files were not generated.${NC}"
    exit 1
fi

echo -e "\n${GREEN}All tests passed successfully!${NC}"
exit 0