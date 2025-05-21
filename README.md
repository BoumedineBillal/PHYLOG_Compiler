# PHYLOG Compiler - Incremental Implementation

This project implements a compiler for the PHYLOG language in an incremental, modular approach. Each part builds on the previous one, with clean interfaces between components.

- **Year:** Master Degree in Machine Learning and Computer Vision
- **University:** USTHB
- **Students:**
  - Boumedine Billal (181837068863)
  - Tolbat Adel (212131030403)
 
  - 
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
