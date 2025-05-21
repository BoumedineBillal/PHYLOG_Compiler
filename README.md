# PHYLOG Compiler - Incremental Implementation

This project implements a compiler for the PHYLOG language in an incremental, modular approach. Each part builds on the previous one, with clean interfaces between components.

## Project Structure

```
.
├── bin/                    # Binary output directory
├── build/                  # Build artifacts
├── include/                # Header files
│   ├── common.h            # Common utilities
│   ├── lexer.h             # Lexical analyzer interface
│   ├── parser.h            # Parser interface
│   ├── parser_defs.h       # Shared parser definitions
│   ├── quadgen.h           # Intermediate code generator
│   └── symboltable.h       # Symbol table manager
├── src/                    # Source files
│   ├── common.c            # Common utilities implementation
│   ├── lexer.l             # Flex lexical analyzer
│   ├── main.c              # Program entry point
│   ├── parser.y            # Bison parser specification
│   ├── parser_helpers.c    # Helper functions for parser
│   ├── quadgen.c           # Intermediate code generator implementation
│   └── symboltable.c       # Symbol table implementation
├── test_part1/             # Tests for Part 1
│   ├── expected_output/    # Expected test outputs
│   ├── run_tests.sh        # Script to run all tests
│   ├── run_single_test.sh  # Script to run a single test
│   └── sort_array.phy      # Example PHYLOG program
└── Makefile                # Build system
```

## Part 1: Basic Compiler Implementation

Part 1 implements the following components:

1. Lexical analysis using Flex
2. Syntactic/semantic analysis using Bison
3. Symbol table management
4. Intermediate code generation (quadruplets)

The output of Part 1 is:
- Symbol table in CSV format
- Intermediate representation (IR) in CSV format

## Building the Project

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

## Running Tests

Tests are organized by project part in separate directories. To run tests for Part 1:

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

## Usage

```bash
bin/phylog <input_file.phy> [-o <output_prefix>]
```

Example:

```bash
bin/phylog test_part1/sort_array.phy -o output
```

This will generate:
- `output.symtab.csv` - Symbol table in CSV format
- `output.ir.csv` - IR code in quadruplet format

## Design Notes

- The parser.y file is kept minimal, with most of the logic moved to external helper functions.
- The symbol table and IR are saved in CSV format for easy parsing in the next compilation phase.
- Clean separation of concerns between different components.
- Each part of the compiler is testable independently.

## Moving to Part 2

For Part 2 (optimization), the compiler will be extended to:

1. Read the symbol table and IR from CSV files
2. Apply optimizations to the IR
3. Output optimized IR

This modular approach ensures that issues in one component don't affect others.