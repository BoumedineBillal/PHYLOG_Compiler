# Makefile for the PHYLOG compiler

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lfl

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target binary
TARGET = $(BIN_DIR)/phylog

# Create necessary directories
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Files to compile
OBJECTS = \
    $(BUILD_DIR)/lex.yy.o \
    $(BUILD_DIR)/parser.tab.o \
    $(BUILD_DIR)/common.o \
    $(BUILD_DIR)/symboltable.o \
    $(BUILD_DIR)/quadgen.o \
    $(BUILD_DIR)/parser_helpers.o \
    $(BUILD_DIR)/optimizer.o \
    $(BUILD_DIR)/codegen.o \
    $(BUILD_DIR)/main.o

# Default target
all: $(TARGET)

# Link all objects into final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Generate and compile lexer
$(BUILD_DIR)/lex.yy.o: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.h
	flex -o $(BUILD_DIR)/lex.yy.c $(SRC_DIR)/lexer.l
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(BUILD_DIR)/lex.yy.c -o $(BUILD_DIR)/lex.yy.o

# Generate and compile parser
$(BUILD_DIR)/parser.tab.o: $(SRC_DIR)/parser.y
	bison -d -o $(BUILD_DIR)/parser.tab.c $(SRC_DIR)/parser.y
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $(BUILD_DIR)/parser.tab.c -o $(BUILD_DIR)/parser.tab.o

# Parser header file
$(BUILD_DIR)/parser.tab.h: $(BUILD_DIR)/parser.tab.o
	@true

# Compile other C files - each gets its own rule to avoid automatic variables
$(BUILD_DIR)/common.o: $(SRC_DIR)/common.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/common.c -o $(BUILD_DIR)/common.o

$(BUILD_DIR)/symboltable.o: $(SRC_DIR)/symboltable.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/symboltable.c -o $(BUILD_DIR)/symboltable.o

$(BUILD_DIR)/quadgen.o: $(SRC_DIR)/quadgen.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/quadgen.c -o $(BUILD_DIR)/quadgen.o

$(BUILD_DIR)/parser_helpers.o: $(SRC_DIR)/parser_helpers.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/parser_helpers.c -o $(BUILD_DIR)/parser_helpers.o

$(BUILD_DIR)/optimizer.o: $(SRC_DIR)/optimizer.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/optimizer.c -o $(BUILD_DIR)/optimizer.o

$(BUILD_DIR)/codegen.o: $(SRC_DIR)/codegen.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/codegen.c -o $(BUILD_DIR)/codegen.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(BUILD_DIR)/parser.tab.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(BUILD_DIR) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f *.ir.csv *.symtab.csv *.opt*.csv *.asm

.PHONY: all clean