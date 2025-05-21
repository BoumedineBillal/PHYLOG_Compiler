# PHYLOG Compiler

![Compiler Pipline](PHYLOG_Compiler/cp.png)

A modular, incremental compiler implementation for the PHYLOG structured programming language, developed as a project for a Master's Degree in Machine Learning and Computer Vision at USTHB.

**Authors:**
- Boumedine Billal (181837068863)
- Tolbat Adel (212131030403)

## Table of Contents
- [What is PHYLOG?](#what-is-phylog)
- [Compiler Pipeline](#compiler-pipeline)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
- [Usage](#usage)
- [Running Tests](#running-tests)
- [Implementation Details](#implementation-details)
  - [Symbol Table](#symbol-table)
  - [Semantic Analysis](#semantic-analysis)
  - [Optimization Passes](#optimization-passes)
  - [Code Generation](#code-generation)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## What is PHYLOG?

PHYLOG is a structured programming language with the following characteristics:

- Well-organized program structure with DATA/CODE/END sections
- Support for multiple data types: INTEGER, FLOAT, CHAR, and STRING
- Implementation of variables, constants, and arrays (using VECTOR keyword)
- Standard control structures: IF-ELSE, FOR, WHILE, with BREAK and CONTINUE
- Logical operators (.AND., .OR., NOT) and comparison operators (.GE., .G., .L., .LE., .EQ., .DI.)
- I/O operations through READ and DISPLAY statements
- Designed for educational purposes to demonstrate compiler construction techniques

## Compiler Pipeline

The compiler follows a standard pipeline architecture:

1. **Lexical Analysis** - Tokenizes the source code
2. **Syntax Analysis** - Constructs parse tree using grammar rules
3. **Semantic Analysis** - Performs type checking and validates program semantics
4. **IR Generation** - Creates intermediate representation (quadruples)
5. **Optimization** - Applies various optimization techniques
6. **Code Generation** - Produces x86 16-bit assembly code

## Features

- **Modular Design** - Separate components for each compilation phase
- **Robust Error Handling** - Detailed error messages with line and column information
- **Type System** - Static type checking with implicit conversions
- **Array Support** - Arrays with bounds checking and arbitrary lower bounds
- **Symbol Management** - Efficient symbol table implementation
- **Optimization** - Multiple optimization passes for improved code efficiency
- **Intermediate Representation** - Uses quadruples as IR for platform independence
- **Assembly Output** - Generates x86 16-bit assembly code

## Getting Started

### Prerequisites

- GCC compiler
- Make

### Building the Project

To build the compiler:

```bash
./build.sh
```

Or manually:

```bash
make
```

This will create the executable at `bin/phylog`.

To clean build artifacts:

```bash
./build.sh --clean
```

## Usage

```bash
bin/phylog <input_file.phy> [-o <output_prefix>]
```

Example:

```bash
bin/phylog test_part1/sort_array.phy -o output
```

This will produce output files with the specified prefix, such as:
- `output.asm` - Assembly code
- `output.ir` - Intermediate representation
- `output.sym` - Symbol table dump (if enabled)

## Running Tests

Tests are organized by project part in separate directories:

```bash
cd test_part1
./run_tests.sh
```

Or to run a specific test:

```bash
cd test_part1
./run_single_test.sh sort_array.phy
```

Each test part folder contains its own test scripts and expected outputs.

## Implementation Details

### Symbol Table

- Uses a hash table (size 211) with linked lists for collision resolution
- Hash function multiplies each character by 31 for better distribution
- Stores all symbol information including name, type, category, and initialization status
- Supports four symbol categories: VARIABLE, CONSTANT, ARRAY, and TEMP
- Provides lookup, insertion, and deletion operations with O(1) average case complexity
- Tracks source code location (line, column) for better error reporting

### Semantic Analysis

The compiler detects a wide range of semantic errors:

- Redefinition of symbols (variables, constants)
- Type mismatches in expressions and assignments
- Array access with non-integer indices
- Array bounds violations (when detected at compile time)
- Use of uninitialized variables
- Attempting to modify constants
- Type incompatibility in conditions and logical operations
- Format specifier mismatch in I/O operations
- Invalid array declarations (negative or zero size)
- Array access on non-array variables
- Undefined variables or arrays in expressions
- Invalid index expressions in array access
- Incompatible operand types for arithmetic operations
- Modulo operation with non-integer operands
- Division or modulo by zero (when detectable at compile time)
- Constants used in invalid contexts (e.g., as loop counters)
- Invalid operands for logical operators
- Unreachable code detection

### Optimization Passes

| Pass | Description |
|------|-------------|
| Copy Propagation | Replaces variable occurrences with their values, reducing variable usage and memory access |
| Constant Folding | Evaluates constant expressions at compile time, eliminating runtime calculations |
| Constant Propagation | Propagates known constant values to their uses, enabling further optimizations |
| Common Subexpression Elimination | Identifies and removes redundant computations, reusing previously calculated results |
| Algebraic Simplification | Applies algebraic rules to simplify expressions (x+0=x, x*1=x, etc.) |
| Dead Code Elimination | Removes code that computes values never used, reducing program size |
| Code Compaction | Removes NOP instructions and reindexes quadruples, reducing code size |
| Unused Variable Cleanup | Removes variables that are never used in the final IR, optimizing memory usage |

### Code Generation

- **Efficient Register Usage** - Makes good use of the x86 register set (AX, BX, CX, DX)
- **Effective Array Handling** - Implements array access with appropriate indexing and bounds checking
- **Clean Control Flow** - Clear implementation of branching and loops with well-structured jumps
- **I/O Operations** - Uses hooks for input/output operations for compatibility with simulation environments
- **Memory Organization** - Clear separation of data and code sections with appropriate variable declarations
- **Good Commenting** - Generated assembly includes comments linking back to the original IR
- **Compatible Output** - Generates 16-bit x86 assembly suitable for educational purposes

