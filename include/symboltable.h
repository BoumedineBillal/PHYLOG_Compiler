/*
 * symboltable.h - Symbol table for the PHYLOG compiler
 */

#ifndef PHYLOG_SYMBOLTABLE_H
#define PHYLOG_SYMBOLTABLE_H

#include "common.h"

// Data types
typedef enum {
    TYPE_UNKNOWN,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING
} SymbolType;

// Symbol categories
typedef enum {
    CATEGORY_UNKNOWN,
    CATEGORY_VARIABLE,    // Regular variable
    CATEGORY_CONSTANT,    // Constant value
    CATEGORY_ARRAY,       // Array (vector)
    CATEGORY_TEMP         // Temporary variable
} SymbolCategory;

// Symbol table entry
typedef struct SymbolNode {
    char name[32];              // Symbol name
    SymbolCategory category;    // Symbol category
    SymbolType type;            // Symbol data type
    int is_initialized;         // Flag indicating initialization
    
    // Value union for different types
    union {
        int int_value;
        float float_value;
        char char_value;
        char* string_value;     // Dynamically allocated
    } value;
    
    // Array-specific fields
    int array_size;             // Size of array (number of elements)
    int array_lower_bound;      // Lower index bound
    
    // Source location info
    int line;                   // Line where declared
    int column;                 // Column where declared
    
    struct SymbolNode* next;    // Next symbol in chain
} SymbolNode;

// Initialize the symbol table
void symboltable_init();

// Insert a variable into the symbol table
SymbolNode* symboltable_insert_variable(const char* name, SymbolType type, int line, int column);

// Insert a constant into the symbol table
SymbolNode* symboltable_insert_constant(const char* name, SymbolType type, void* value, int line, int column);

// Insert an array into the symbol table
SymbolNode* symboltable_insert_array(const char* name, SymbolType type, int lower_bound, int size, int line, int column);

// Create a new temporary variable
SymbolNode* symboltable_create_temp(SymbolType type);

// Look up a symbol by name
SymbolNode* symboltable_lookup(const char* name);

// Update a variable's value
void symboltable_set_value(SymbolNode* node, void* value);

// Mark a variable as initialized
void symboltable_set_initialized(SymbolNode* node);

// Check if two types are compatible
int symboltable_types_compatible(SymbolType type1, SymbolType type2);

// Get string representation of a type
const char* symboltable_type_to_string(SymbolType type);

// Get string representation of a category
const char* symboltable_category_to_string(SymbolCategory category);

// Display the symbol table
void symboltable_display();

// Write the symbol table to a file (CSV format)
void symboltable_write_to_file(const char* filename);

// Free memory used by the symbol table
void symboltable_cleanup();

// Get the total number of symbols in the symbol table
int symboltable_get_count(void);

// Get a symbol by index (for iterating through all symbols)
SymbolNode* symboltable_get_by_index(int index);

// Remove a symbol from the symbol table
int symboltable_remove(const char* name);

#endif // PHYLOG_SYMBOLTABLE_H