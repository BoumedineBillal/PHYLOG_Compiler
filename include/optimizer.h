/*
 * optimizer.h - Code optimization for the PHYLOG compiler
 */

#ifndef PHYLOG_OPTIMIZER_H
#define PHYLOG_OPTIMIZER_H

#include "common.h"
#include "quadgen.h"

// Optimization pass types
typedef enum {
    OPT_PASS_COPY_PROPAGATION,          // Propagate copy operations
    OPT_PASS_CONSTANT_FOLDING,          // Compute constant expressions
    OPT_PASS_CONSTANT_PROPAGATION,      // Propagate constant values
    OPT_PASS_COMMON_SUBEXPR_ELIMINATION, // Eliminate common subexpressions
    OPT_PASS_DEAD_CODE_ELIMINATION,     // Remove dead code
    OPT_PASS_ALGEBRAIC_SIMPLIFICATION,  // Simplify algebraic expressions
    OPT_PASS_CODE_COMPACTION,            // eliminate nops (cleaning)
    OPT_PASS_UNUSED_VAR_CLEANUP         // Remove unused variables
} OptimizationPass;

// Optimization statistics
typedef struct {
    int copies_propagated;
    int constants_folded;
    int constants_propagated;
    int common_subexprs_eliminated;
    int dead_code_eliminated;
    int algebraic_simplifications;
    int nops_eliminated;
} OptimizationStats;

// Initialize the optimizer
void optimizer_init();

// Run a specific optimization pass
int optimizer_run_pass(OptimizationPass pass, const char* input_file, const char* output_file);

// Run all optimization passes in sequence
int optimizer_run_all_passes(const char* input_file, const char* output_file);

// Get optimization statistics
OptimizationStats optimizer_get_stats();

// Display information about optimizations
void optimizer_display_stats();

// Clean up the optimizer
void optimizer_cleanup();

// Run unused variable cleanup (directly checks IR, doesn't modify symbol table)
void optimizer_check_unused_variables(const char* input_file);

#endif // PHYLOG_OPTIMIZER_H