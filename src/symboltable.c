/*
 * symboltable.c - Symbol table implementation for the PHYLOG compiler
 */

 #include "../include/symboltable.h"
 #include "../include/common.h"  // Make sure this is included
 
 #define HASH_SIZE 211  // A prime number for better distribution
 
 // Symbol table as a hash table of linked lists
 static SymbolNode* hash_table[HASH_SIZE] = {NULL};
 
 // Current temporary variable counter
 static int temp_counter = 0;
 
 // Hash function for symbol names
 static unsigned int hash(const char* name) {
     unsigned int hash_val = 0;
     for (; *name; name++) {
         hash_val = hash_val * 31 + *name;
     }
     return hash_val % HASH_SIZE;
 }
 
 // Initialize the symbol table
 void symboltable_init() {
     // Clear all entries
     for (int i = 0; i < HASH_SIZE; i++) {
         hash_table[i] = NULL;
     }
     
     // Reset temporary counter
     temp_counter = 0;
 }
 
 // Insert a variable into the symbol table
 SymbolNode* symboltable_insert_variable(const char* name, SymbolType type, int line, int column) {
     // Check if symbol already exists
     SymbolNode* existing = symboltable_lookup(name);
     if (existing) {
         report_error(ERROR_LEVEL_ERROR, line, column, "Redefinition of '%s'", name);
         return existing;
     }
     
     // Create new symbol node
     SymbolNode* node = (SymbolNode*)safe_malloc(sizeof(SymbolNode));
     strncpy(node->name, name, sizeof(node->name) - 1);
     node->name[sizeof(node->name) - 1] = '\0';
     
     // Special handling for loop variables I and J
     if (strcmp(name, "I") == 0 || strcmp(name, "J") == 0) {
         // Ensure I and J are always treated as INTEGER VARIABLE
         node->category = CATEGORY_VARIABLE;
         node->type = TYPE_INTEGER;
     } else {
         node->category = CATEGORY_VARIABLE;
         node->type = type;
     }
     
     node->is_initialized = 0;
     node->array_size = 0;
     node->array_lower_bound = 0;
     node->line = line;
     node->column = column;
     node->next = NULL;
     
     // Initialize value based on type
     switch (node->type) {
         case TYPE_INTEGER:
             node->value.int_value = 0;
             break;
         case TYPE_FLOAT:
             node->value.float_value = 0.0f;
             break;
         case TYPE_CHAR:
             node->value.char_value = '\0';
             break;
         case TYPE_STRING:
             node->value.string_value = NULL;
             break;
         default:
             // For TYPE_UNKNOWN, zero out the value union
             memset(&node->value, 0, sizeof(node->value));
             break;
     }
     
     // Insert into hash table
     unsigned int index = hash(name);
     node->next = hash_table[index];
     hash_table[index] = node;
     
     return node;
 }
 
 // Insert a constant into the symbol table
 SymbolNode* symboltable_insert_constant(const char* name, SymbolType type, void* value, int line, int column) {
     // Check if symbol already exists
     SymbolNode* existing = symboltable_lookup(name);
     if (existing) {
         // If it's already a constant, just update the value
         if (existing->category == CATEGORY_CONSTANT) {
             if (value) {
                 switch (type) {
                     case TYPE_INTEGER:
                         existing->value.int_value = *(int*)value;
                         break;
                     case TYPE_FLOAT:
                         existing->value.float_value = *(float*)value;
                         break;
                     case TYPE_CHAR:
                         existing->value.char_value = *(char*)value;
                         break;
                     case TYPE_STRING:
                         if (existing->value.string_value) {
                             free(existing->value.string_value);
                         }
                         existing->value.string_value = safe_strdup((char*)value);
                         break;
                     default:
                         break;
                 }
             }
             existing->is_initialized = 1;
             return existing;
         } else {
             report_error(ERROR_LEVEL_ERROR, line, column, "Redefinition of '%s' as a constant", name);
             return existing;
         }
     }
     
     // Create new symbol node directly instead of using insert_variable
     SymbolNode* node = (SymbolNode*)safe_malloc(sizeof(SymbolNode));
     strncpy(node->name, name, sizeof(node->name) - 1);
     node->name[sizeof(node->name) - 1] = '\0';
     
     node->category = CATEGORY_CONSTANT;
     node->type = type;
     node->is_initialized = 1;
     node->array_size = 0;
     node->array_lower_bound = 0;
     node->line = line;
     node->column = column;
     node->next = NULL;
     
     // Initialize value based on type
     switch (type) {
         case TYPE_INTEGER:
             node->value.int_value = value ? *(int*)value : 0;
             break;
         case TYPE_FLOAT:
             node->value.float_value = value ? *(float*)value : 0.0f;
             break;
         case TYPE_CHAR:
             node->value.char_value = value ? *(char*)value : '\0';
             break;
         case TYPE_STRING:
             node->value.string_value = value ? safe_strdup((char*)value) : NULL;
             break;
         default:
             memset(&node->value, 0, sizeof(node->value));
             break;
     }
     
     // Insert into hash table
     unsigned int index = hash(name);
     node->next = hash_table[index];
     hash_table[index] = node;
     
     return node;
 }
 
 // Insert an array into the symbol table
 SymbolNode* symboltable_insert_array(const char* name, SymbolType type, int lower_bound, int size, int line, int column) {
     // Check for valid array size
     if (size <= 0) {
         report_error(ERROR_LEVEL_ERROR, line, column, "Array size must be positive");
         return NULL;
     }
     
     // Special protection for variables that should never be arrays
     if (strcmp(name, "I") == 0 || strcmp(name, "J") == 0) {
         // These should always be variables, not arrays
         SymbolNode* node = symboltable_lookup(name);
         if (!node) {
             node = symboltable_insert_variable(name, TYPE_INTEGER, line, column);
         }
         return node;
     }
     
     // Look up the symbol first to see if it exists
     SymbolNode* existing = symboltable_lookup(name);
     if (existing) {
         // Update existing symbol to array
         existing->category = CATEGORY_ARRAY;
         existing->type = type;
         existing->array_size = size;
         existing->array_lower_bound = lower_bound;
         return existing;
     }
     
     // Create new symbol node directly
     SymbolNode* node = (SymbolNode*)safe_malloc(sizeof(SymbolNode));
     strncpy(node->name, name, sizeof(node->name) - 1);
     node->name[sizeof(node->name) - 1] = '\0';
     
     node->category = CATEGORY_ARRAY;
     node->type = type;
     node->is_initialized = 0;
     node->array_size = size;
     node->array_lower_bound = lower_bound;
     node->line = line;
     node->column = column;
     node->next = NULL;
     
     // Initialize value based on type
     switch (type) {
         case TYPE_INTEGER:
             node->value.int_value = 0;
             break;
         case TYPE_FLOAT:
             node->value.float_value = 0.0f;
             break;
         case TYPE_CHAR:
             node->value.char_value = '\0';
             break;
         case TYPE_STRING:
             node->value.string_value = NULL;
             break;
         default:
             memset(&node->value, 0, sizeof(node->value));
             break;
     }
     
     // Insert into hash table
     unsigned int index = hash(name);
     node->next = hash_table[index];
     hash_table[index] = node;
     
     return node;
 }
 
 // Create a new temporary variable
 SymbolNode* symboltable_create_temp(SymbolType type) {
     // Generate temporary name
     char temp_name[32];
     sprintf(temp_name, "T%d", temp_counter++);
     
     // Create node directly to ensure correct category
     SymbolNode* node = (SymbolNode*)safe_malloc(sizeof(SymbolNode));
     strncpy(node->name, temp_name, sizeof(node->name) - 1);
     node->name[sizeof(node->name) - 1] = '\0';
     
     // Always ensure temporary variables have a valid type, default to INTEGER
     if (type == TYPE_UNKNOWN) {
         type = TYPE_INTEGER;
     }
     
     node->category = CATEGORY_TEMP;
     node->type = type;
     node->is_initialized = 0;
     node->array_size = 0;
     node->array_lower_bound = 0;
     node->line = 0;
     node->column = 0;
     node->next = NULL;
     
     // Initialize value based on type
     switch (type) {
         case TYPE_INTEGER:
             node->value.int_value = 0;
             break;
         case TYPE_FLOAT:
             node->value.float_value = 0.0f;
             break;
         case TYPE_CHAR:
             node->value.char_value = '\0';
             break;
         case TYPE_STRING:
             node->value.string_value = NULL;
             break;
         default:
             memset(&node->value, 0, sizeof(node->value));
             break;
     }
     
     // Insert into hash table
     unsigned int index = hash(temp_name);
     node->next = hash_table[index];
     hash_table[index] = node;
     
     return node;
 }
 
 // Look up a symbol by name
 SymbolNode* symboltable_lookup(const char* name) {
     if (!name) return NULL;
     
     unsigned int index = hash(name);
     SymbolNode* node = hash_table[index];
     
     // Search through chain
     while (node) {
         if (strcmp(node->name, name) == 0) {
             return node;
         }
         node = node->next;
     }
     
     return NULL;
 }
 
 // Update a variable's value
 void symboltable_set_value(SymbolNode* node, void* value) {
     if (!node || !value) return;
     
     switch (node->type) {
         case TYPE_INTEGER:
             node->value.int_value = *(int*)value;
             break;
         case TYPE_FLOAT:
             node->value.float_value = *(float*)value;
             break;
         case TYPE_CHAR:
             node->value.char_value = *(char*)value;
             break;
         case TYPE_STRING:
             if (node->value.string_value) {
                 free(node->value.string_value);
             }
             node->value.string_value = safe_strdup((char*)value);
             break;
         default:
             // For unknown type, do nothing
             break;
     }
     
     // Mark as initialized
     node->is_initialized = 1;
 }
 
 // Mark a variable as initialized
 void symboltable_set_initialized(SymbolNode* node) {
     if (node) {
         node->is_initialized = 1;
     }
 }
 
 // Check if two types are compatible
 int symboltable_types_compatible(SymbolType type1, SymbolType type2) {
     // Same types are always compatible
     if (type1 == type2) {
         return 1;
     }
     
     // If either type is unknown but the other is valid, consider them compatible
     // This helps with literals and complex expressions
     if (type1 == TYPE_UNKNOWN && type2 != TYPE_UNKNOWN) {
         return 1;
     }
     if (type2 == TYPE_UNKNOWN && type1 != TYPE_UNKNOWN) {
         return 1;
     }
     
     // Integer is compatible with float (implicit conversion)
     if ((type1 == TYPE_INTEGER && type2 == TYPE_FLOAT) ||
         (type1 == TYPE_FLOAT && type2 == TYPE_INTEGER)) {
         return 1;
     }
     
     // Char is compatible with string (implicit conversion)
     if ((type1 == TYPE_CHAR && type2 == TYPE_STRING) ||
         (type1 == TYPE_STRING && type2 == TYPE_CHAR)) {
         return 1;
     }
     
     // All other type combinations are incompatible
     return 0;  // Changed from 1 to 0 to enforce proper type checking
 }
 
 // Get string representation of a type
 const char* symboltable_type_to_string(SymbolType type) {
     switch (type) {
         case TYPE_INTEGER: return "INTEGER";
         case TYPE_FLOAT:   return "FLOAT";
         case TYPE_CHAR:    return "CHAR";
         case TYPE_STRING:  return "STRING";
         default:           return "UNKNOWN";
     }
 }
 
 // Get string representation of a category
 const char* symboltable_category_to_string(SymbolCategory category) {
     switch (category) {
         case CATEGORY_VARIABLE: return "VARIABLE";
         case CATEGORY_CONSTANT: return "CONSTANT";
         case CATEGORY_ARRAY:    return "ARRAY";
         case CATEGORY_TEMP:     return "TEMP";
         default:                return "UNKNOWN";
     }
 }
 
 // Convert a value between compatible types (new function)
 void* symboltable_convert_value(void* value, SymbolType from_type, SymbolType to_type) {
     if (!value) return NULL;
     
     // If types are the same, no conversion needed
     if (from_type == to_type) {
         return value;
     }
     
     // Allocate space for the converted value
     void* result = safe_malloc(16);  // Big enough for any primitive type
     
     // Perform conversion
     if (from_type == TYPE_INTEGER && to_type == TYPE_FLOAT) {
         // Integer to float
         float float_val = (float)(*(int*)value);
         memcpy(result, &float_val, sizeof(float));
     }
     else if (from_type == TYPE_FLOAT && to_type == TYPE_INTEGER) {
         // Float to integer (truncation)
         int int_val = (int)(*(float*)value);
         memcpy(result, &int_val, sizeof(int));
     }
     else if (from_type == TYPE_CHAR && to_type == TYPE_STRING) {
         // Char to string
         char char_val = *(char*)value;
         char* str_val = safe_malloc(2);
         str_val[0] = char_val;
         str_val[1] = '\0';
         memcpy(result, &str_val, sizeof(char*));
     }
     else if (from_type == TYPE_STRING && to_type == TYPE_CHAR) {
         // String to char (take first character)
         char* str_val = *(char**)value;
         char char_val = str_val && *str_val ? *str_val : '\0';
         memcpy(result, &char_val, sizeof(char));
     }
     else {
         // Unknown conversion, return NULL
         free(result);
         return NULL;
     }
     
     return result;
 }
 
 // Helper functions to identify and extract literal values
 // (These were originally in common.c)
 
 // Check if a string is a character literal (starts and ends with single quote)
 static int is_char_literal(const char* str) {
     if (!str || !*str) return 0;
     
     // Must start with single quote
     if (str[0] != '\'') return 0;
     
     // Must have at least one character inside
     if (str[1] == '\0' || str[1] == '\'') return 0;
     
     // Find the closing quote
     int i = 1;
     while (str[i] && str[i] != '\'') i++;
     
     // Must end with a single quote and have exactly one character inside
     return (str[i] == '\'' && str[i+1] == '\0' && i == 2);
 }
 
 // Check if a string is a string literal (starts and ends with double quote)
 static int is_string_literal(const char* str) {
     if (!str || !*str) return 0;
     
     // Must start with double quote
     if (str[0] != '"') return 0;
     
     // Find the closing quote
     int i = 1;
     while (str[i] && str[i] != '"') i++;
     
     // Must end with a double quote
     return (str[i] == '"' && str[i+1] == '\0');
 }
 
 // Extract the string content from a string literal
 static char* string_to_string(const char* str) {
     if (!str || !is_string_literal(str)) return NULL;
     
     // Calculate the length of the string without quotes
     int len = strlen(str) - 2;  // Subtract opening and closing quotes
     
     // Allocate memory for the string content
     char* result = (char*)safe_malloc(len + 1);  // +1 for null terminator
     
     // Copy the string content without quotes
     strncpy(result, str + 1, len);
     result[len] = '\0';
     
     return result;
 }
 
 // Convert a value from one type to another as a string
 char* symboltable_convert_value_string(const char* value, SymbolType from_type, SymbolType to_type) {
     if (!value) return safe_strdup("0");
     
     // If types are the same, no conversion needed
     if (from_type == to_type) {
         return safe_strdup(value);
     }
     
     char result[256];
     
     // Perform conversion based on types
     if (from_type == TYPE_INTEGER && to_type == TYPE_FLOAT) {
         // Integer to float - simply add decimal point
         int int_val = string_to_int(value);
         sprintf(result, "%f", (float)int_val);
     }
     else if (from_type == TYPE_FLOAT && to_type == TYPE_INTEGER) {
         // Float to integer - truncate
         float float_val = string_to_float(value);
         sprintf(result, "%d", (int)float_val);
     }
     else if (from_type == TYPE_CHAR && to_type == TYPE_STRING) {
         // Char to string - add double quotes around the character
         sprintf(result, "\"%c\"", is_char_literal(value) ? value[1] : value[0]);
     }
     else if (from_type == TYPE_STRING && to_type == TYPE_CHAR) {
         // String to char - take the first character
         char* str_val = NULL;
         if (is_string_literal(value)) {
             str_val = string_to_string(value);
         } else {
             str_val = safe_strdup(value);
         }
         sprintf(result, "'%c'", str_val && *str_val ? *str_val : '\0');
         free(str_val);
     }
     else if (from_type == TYPE_INTEGER && to_type == TYPE_CHAR) {
         // Integer to char - convert numeric value to ASCII
         int int_val = string_to_int(value);
         sprintf(result, "'%c'", (char)int_val);
     }
     else if (from_type == TYPE_CHAR && to_type == TYPE_INTEGER) {
         // Char to integer - convert ASCII to numeric value
         char char_val = is_char_literal(value) ? value[1] : value[0];
         sprintf(result, "%d", (int)char_val);
     }
     else {
         // Unknown conversion, return the original value
         return safe_strdup(value);
     }
     
     return safe_strdup(result);
 }
 
 // Display the symbol table
 void symboltable_display() {
     printf("\n********** SYMBOL TABLE **********\n");
     printf("%-15s %-10s %-10s %-15s %-15s %-15s\n", 
            "Name", "Category", "Type", "Value", "Size/LBound", "Declared At");
     printf("----------------------------------------------------------------------\n");
     
     int count = 0;
     for (int i = 0; i < HASH_SIZE; i++) {
         SymbolNode* node = hash_table[i];
         while (node) {
             printf("%-15s %-10s %-10s ", 
                    node->name, 
                    symboltable_category_to_string(node->category),
                    symboltable_type_to_string(node->type));
             
             // Print value based on type
             if (node->is_initialized) {
                 switch (node->type) {
                     case TYPE_INTEGER:
                         printf("%-15d ", node->value.int_value);
                         break;
                     case TYPE_FLOAT:
                         printf("%-15.4f ", node->value.float_value);
                         break;
                     case TYPE_CHAR:
                         if (node->value.char_value >= 32 && node->value.char_value <= 126) {
                             printf("%-15c ", node->value.char_value);
                         } else {
                             printf("\\x%-13x ", (unsigned char)node->value.char_value);
                         }
                         break;
                     case TYPE_STRING:
                         if (node->value.string_value) {
                             char trimmed[16];
                             strncpy(trimmed, node->value.string_value, 15);
                             trimmed[15] = '\0';
                             printf("%-15s ", trimmed);
                         } else {
                             printf("%-15s ", "(null)");
                         }
                         break;
                     default:
                         printf("%-15s ", "");
                         break;
                 }
             } else {
                 printf("%-15s ", "(uninitialized)");
             }
             
             // Print array info or declared location
             if (node->category == CATEGORY_ARRAY) {
                 printf("%-15d/%d ", node->array_size, node->array_lower_bound);
             } else {
                 printf("%-15s ", "");
             }
             
             // Print declaration location
             printf("Line %-4d Col %-4d\n", node->line, node->column);
             
             node = node->next;
             count++;
         }
     }
     
     printf("----------------------------------------------------------------------\n");
     printf("Total symbols: %d\n", count);
     printf("********** END SYMBOL TABLE **********\n\n");
 }
 
 // Write the symbol table to a file (CSV format)
 void symboltable_write_to_file(const char* filename) {
     FILE* file = fopen(filename, "w");
     if (!file) {
         report_error(ERROR_LEVEL_ERROR, 0, 0, "Cannot open symbol table file '%s'", filename);
         return;
     }
     
     // Write CSV header
     fprintf(file, "Name,Category,Type,Value,ArraySize,LowerBound,Line,Column\n");
     
     // Write symbol data
     for (int i = 0; i < HASH_SIZE; i++) {
         SymbolNode* node = hash_table[i];  // Fixed: using 'i' instead of 'index'
         while (node) {
             // Write name, category, type
             fprintf(file, "%s,%s,%s,", 
                    node->name, 
                    symboltable_category_to_string(node->category),
                    symboltable_type_to_string(node->type));
             
             // Write value based on type
             if (node->is_initialized) {
                 switch (node->type) {
                     case TYPE_INTEGER:
                         fprintf(file, "%d,", node->value.int_value);
                         break;
                     case TYPE_FLOAT:
                         fprintf(file, "%f,", node->value.float_value);
                         break;
                     case TYPE_CHAR:
                         if (node->value.char_value >= 32 && node->value.char_value <= 126) {
                             fprintf(file, "'%c',", node->value.char_value);
                         } else {
                             fprintf(file, "0x%x,", (unsigned char)node->value.char_value);
                         }
                         break;
                     case TYPE_STRING:
                         if (node->value.string_value) {
                             fprintf(file, "\"%s\",", node->value.string_value);
                         } else {
                             fprintf(file, "(null),");
                         }
                         break;
                     default:
                         fprintf(file, ",");
                         break;
                 }
             } else {
                 fprintf(file, "(uninitialized),");
             }
             
             // Write array size and lower bound
             fprintf(file, "%d,%d,", node->array_size, node->array_lower_bound);
             
             // Write declaration location
             fprintf(file, "%d,%d\n", node->line, node->column);
             
             node = node->next;
         }
     }
     
     fclose(file);
 }
 
 // Free memory used by the symbol table
 void symboltable_cleanup() {
     for (int i = 0; i < HASH_SIZE; i++) {
         SymbolNode* node = hash_table[i];
         while (node) {
             SymbolNode* next = node->next;
             
             // Free string value if allocated
             if (node->type == TYPE_STRING && node->value.string_value) {
                 free(node->value.string_value);
             }
             
             free(node);
             node = next;
         }
         hash_table[i] = NULL;
     }
 }


 // Get the total number of symbols in the symbol table
int symboltable_get_count(void) {
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        SymbolNode* node = hash_table[i];
        while (node) {
            count++;
            node = node->next;
        }
    }
    return count;
}

// Get a symbol by index (for iterating through all symbols)
SymbolNode* symboltable_get_by_index(int target_idx) {
    int current_idx = 0;
    
    for (int i = 0; i < HASH_SIZE; i++) {
        SymbolNode* node = hash_table[i];
        while (node) {
            if (current_idx == target_idx) {
                return node;
            }
            current_idx++;
            node = node->next;
        }
    }
    
    return NULL;  // Index out of range
}

// Remove a symbol from the symbol table
int symboltable_remove(const char* name) {
    if (!name || name[0] == '\0') {
        return 0;  // Invalid name
    }
    
    // Calculate hash index
    unsigned int index = hash(name);
    
    // Find the symbol node
    SymbolNode* current = hash_table[index];
    SymbolNode* prev = NULL;
    
    while (current) {
        // Check if this is the node to remove
        if (strcmp(current->name, name) == 0) {
            // Found the node to remove
            
            // Update the linked list
            if (prev) {
                // Not the first node in the list
                prev->next = current->next;
            } else {
                // First node in the list
                hash_table[index] = current->next;
            }
            
            // Free string value if allocated
            if (current->type == TYPE_STRING && current->value.string_value) {
                free(current->value.string_value);
            }
            
            // Free the node itself
            free(current);
            
            return 1;  // Successfully removed
        }
        
        // Move to next node
        prev = current;
        current = current->next;
    }
    
    return 0;  // Symbol not found
}