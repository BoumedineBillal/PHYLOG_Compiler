# PHYLOG Compiler V4 Implementation Overview

## Main Components

### 1. Main Program (`main.c`)
- It parses command line arguments and options (like -t for symbol table display, -q for quadruplets display)
- Creates necessary directories and output file paths
- Initializes all compiler components (symbol table, quadgen, etc.)
- Coordinates the compilation phases:
  - Phase 1: Lexical and Syntactic Analysis
  - Phase 2: Optimization (if requested)
  - Phase 3: Code generation (not implemented in this version)
- Manages cleanup of all components

### 2. Lexical Analyzer (`lexer.l` - Flex)
- Defines patterns for all PHYLOG language tokens (keywords, identifiers, literals)
- Tracks line and column numbers for error reporting
- Converts input characters into tokens for the parser
- Handles different token types (INTEGER, FLOAT, CHAR, STRING)
- Processes comments and whitespace
- Reports lexical errors for invalid input

### 3. Parser (`parser.y` - Bison)
- Defines the grammar rules for PHYLOG language
- Handles syntax analysis of tokens from the lexer
- Builds the Abstract Syntax Tree (AST)
- Performs semantic checks (type checking, variable declaration)
- Generates intermediate code (quadruplets) through the quadgen module
- Manages symbol table operations (inserting variables, arrays, constants)
- Implements control structures (if-else, loops, breaks)
- Handles expressions and proper type inference
- Manages backpatching for control flow

### 4. Symbol Table (`symboltable.c`)
- Maintains a hash table of all program symbols (variables, constants, arrays)
- Tracks symbol properties (name, type, value, initialization status)
- Provides functions to insert, lookup, and modify symbols
- Implements type checking and compatibility testing
- Creates temporary variables for intermediate results
- Manages array bounds and dimensions
- Provides functions to display and save the symbol table

### 5. Intermediate Code Generator (`quadgen.c`)
- Generates quadruplets (4-tuples: operation, arg1, arg2, result)
- Provides functions to create, update, and retrieve quadruplets
- Manages labels for control flow (jumps, conditionals)
- Handles display and file I/O for the intermediate code
- Supports reading back intermediate code from files

### 6. Code Optimizer (`optimizer.c`)
- Implements multiple optimization passes:
  - Copy Propagation: Replaces variables with their values
  - Constant Folding: Evaluates constant expressions at compile time
  - Constant Propagation: Propagates known constant values
  - Common Subexpression Elimination: Removes redundant calculations
  - Algebraic Simplifications: Simplifies expressions (x+0 → x)
  - Dead Code Elimination: Removes unused code
- Tracks optimization statistics
- Processes intermediate code in multiple passes
- Manages temporary files for optimization chains

## Build and Execution System

### 1. Makefile
- Defines compilation rules for all components
- Manages build artifacts in the build/ directory
- Provides targets for compilation, testing, and cleanup
- Handles dependencies between components

### 2. Shell Scripts
- `build.sh`: Cleans and rebuilds the compiler
- `test.sh`: Runs test cases in the tests/ directory
- `optimized_test.sh`: Shows optimization in action
- `make_executable.sh`: Makes scripts executable

## Core Processes

### 1. Compilation Process
1. Lexical Analysis: Converts source code to token stream
2. Parsing: Generates abstract syntax tree and checks syntax
3. Semantic Analysis: Type checking and scope validation
4. Intermediate Code Generation: Creates quadruplets
5. Optimization: Applies selected optimization passes
6. (Not implemented) Code Generation: Would translate to assembly

### 2. Optimization Process
1. Copy Propagation: Replaces variables with their copied values
2. Constant Folding: Computes constant expressions at compile time
3. Constant Propagation: Propagates constant values through the code
4. Common Subexpression Elimination: Avoids recalculating the same expressions
5. Algebraic Simplifications: Applies algebraic rules to simplify code
6. Dead Code Elimination: Removes code that doesn't affect program output

### 3. Symbol Table Management
1. Hash table implementation for efficient lookups
2. Handles different symbol types and categories
3. Tracks initialization status and bounds for arrays
4. Creates temporary variables for expression evaluation
5. Manages compatibility between different data types

### 4. Error Handling
1. Tracks line and column numbers for accurate error locations
2. Provides meaningful error messages for various error types
3. Reports lexical, syntax, and semantic errors
4. Handles recovery from errors where possible

## PHYLOG Language Features

1. **Data Types**: INTEGER, FLOAT, CHAR, STRING
2. **Variables and Constants**: Declaration and initialization
3. **Arrays (VECTOR)**: Fixed-size arrays with custom indexing
4. **Arithmetic Operations**: +, -, *, /, %
5. **Logical Operations**: .AND., .OR., NOT
6. **Comparison Operations**: .EQ., .DI., .L., .LE., .G., .GE.
7. **Control Structures**: IF-ELSE, FOR, WHILE, BREAK, CONTINUE
8. **Input/Output**: READ, DISPLAY