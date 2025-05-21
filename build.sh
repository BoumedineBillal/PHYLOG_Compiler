#!/bin/bash
# Simple build script for PHYLOG compiler

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

if [ "$1" == "-c" ] || [ "$1" == "--clean" ]; then
  echo -e "${YELLOW}Cleaning build artifacts...${NC}"
  make clean
  echo -e "${GREEN}Clean completed.${NC}"
  exit 0
fi

echo -e "${YELLOW}Building PHYLOG compiler...${NC}"
make

if [ $? -eq 0 ]; then
  echo -e "${GREEN}Build successful!${NC}"
  echo -e "Compiler binary is at: ${YELLOW}bin/phylog${NC}"
else
  echo -e "${RED}Build failed.${NC}"
  exit 1
fi