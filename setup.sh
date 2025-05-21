#!/bin/bash
# Setup script to make all scripts executable
# Run this script once to set up permissions

# Make build script executable
chmod +x build.sh

# Make test scripts executable for part 1
chmod +x test_part1/run_tests.sh
chmod +x test_part1/run_single_test.sh

echo "All scripts are now executable."
echo "To build the compiler, run: ./build.sh"
echo "To run tests for part 1, go to test_part1 directory and run: ./run_tests.sh"