/*
 * quadgen.h - Intermediate code generation (quadruplets) for the PHYLOG compiler
 */

#ifndef PHYLOG_QUADGEN_H
#define PHYLOG_QUADGEN_H

#include "common.h"

// Quadruplet structure
typedef struct {
    int id;            // Quadruplet ID number
    char op[16];       // Operator
    char arg1[32];     // First argument
    char arg2[32];     // Second argument
    char result[32];   // Result
} Quadruplet;

// Initialize the quadruplet generator
void quadgen_init();

// Generate a quadruplet and return its ID
int quadgen_generate(const char* op, const char* arg1, const char* arg2, const char* result);

// Update a quadruplet (useful for backpatching)
void quadgen_update(int id, const char* op, const char* arg1, const char* arg2, const char* result);

// Create a new temporary variable name
char* quadgen_new_temp();

// Create a new label ID
int quadgen_new_label();

// Get the text representation of a label
char* quadgen_label_to_string(int label);

// Get the current quadruplet count
int quadgen_get_count();

// Get a specific quadruplet by ID
Quadruplet* quadgen_get(int id);

// Display all generated quadruplets
void quadgen_display();

// Write quadruplets to a file in a format that can be read back
void quadgen_write_to_file(const char* filename);

// Read quadruplets from a file
int quadgen_read_from_file(const char* filename);

// Clean up the quadruplet generator
void quadgen_cleanup();

void quadgen_set_count(int count);

#endif // PHYLOG_QUADGEN_H