/*
 * parser_helpers.c - Helper functions for the PHYLOG parser
 */

 #include "../include/parser.h"
 #include "../include/lexer.h"
 
 // Program name storage
 char program_name[32];

 // Function to report syntax errors
 void parser_syntax_error(const char* message) {
     report_error(ERROR_LEVEL_ERROR, current_line, current_column, "Syntax error: %s", message);
 }
 
 // Function to report semantic errors
 void parser_semantic_error(const char* message) {
     report_error(ERROR_LEVEL_ERROR, current_line, current_column, "Semantic error: %s", message);
 }
 
 // Helper function to convert string type to enum
 SymbolType get_type_from_string(const char* type_str) {
     if (strcmp(type_str, "INTEGER") == 0) return TYPE_INTEGER;
     if (strcmp(type_str, "FLOAT") == 0) return TYPE_FLOAT;
     if (strcmp(type_str, "CHAR") == 0) return TYPE_CHAR;
     if (strcmp(type_str, "STRING") == 0) return TYPE_STRING;
     return TYPE_UNKNOWN;
 }
 
 // Process a literal expression
 char* process_literal(const char* literal_str, SymbolType type) {
     // Create a temporary symbol to hold the literal
     SymbolNode* temp = symboltable_create_temp(type);
     if (!temp) {
         parser_semantic_error("Failed to create temporary for literal");
         return safe_strdup("ERROR");
     }
     
     // Set the value based on the type
     if (type == TYPE_INTEGER) {
         int value = string_to_int(literal_str);
         symboltable_set_value(temp, &value);
     } else if (type == TYPE_FLOAT) {
         float value = string_to_float(literal_str);
         symboltable_set_value(temp, &value);
     }
     
     // Generate assignment of the literal to the temporary
     quadgen_generate(":=", literal_str, "", temp->name);
     
     return safe_strdup(temp->name);
 }
 
 // Helper function to create an expression temp and generate quad
 char* gen_expr_quad(const char* op, const char* arg1, const char* arg2, SymbolType result_type) {
     // Check if the operation is valid for the operand types
     SymbolType type1 = get_expression_type(arg1);
     SymbolType type2 = get_expression_type(arg2);
     
     // Special case for modulo: both operands must be integers
     if (strcmp(op, "%") == 0) {
         if (type1 != TYPE_INTEGER || type2 != TYPE_INTEGER) {
             parser_semantic_error("Modulo operation requires integer operands");
             // Continue with integer type for error recovery
             result_type = TYPE_INTEGER;
         }
     }
     
     // Create a new temporary for the result
     SymbolNode* temp = symboltable_create_temp(result_type);
     if (!temp) {
         parser_semantic_error("Failed to create temporary variable");
         return safe_strdup("ERROR");
     }
     
     // Check if type conversion is needed for operands
     char* converted_arg1 = safe_strdup(arg1);
     char* converted_arg2 = safe_strdup(arg2);
     
     // Convert arg1 if needed
     if (type1 != TYPE_UNKNOWN && result_type != TYPE_UNKNOWN && type1 != result_type) {
         // Need conversion - create a temporary and conversion quad
         SymbolNode* conv_temp = symboltable_create_temp(result_type);
         quadgen_generate("CONVERT", arg1, symboltable_type_to_string(result_type), conv_temp->name);
         free(converted_arg1);
         converted_arg1 = safe_strdup(conv_temp->name);
     }
     
     // Convert arg2 if needed
     if (type2 != TYPE_UNKNOWN && result_type != TYPE_UNKNOWN && type2 != result_type) {
         // Need conversion - create a temporary and conversion quad
         SymbolNode* conv_temp = symboltable_create_temp(result_type);
         quadgen_generate("CONVERT", arg2, symboltable_type_to_string(result_type), conv_temp->name);
         free(converted_arg2);
         converted_arg2 = safe_strdup(conv_temp->name);
     }
     
     // Generate the quadruplet with possibly converted operands
     quadgen_generate(op, converted_arg1, converted_arg2, temp->name);
     
     // Free temporary strings
     free(converted_arg1);
     free(converted_arg2);
     
     // Mark temporary as initialized
     symboltable_set_initialized(temp);
     
     return safe_strdup(temp->name);
 }
 
 // Check types for arithmetic operations
 SymbolType check_arithmetic_types(const char* op, const char* arg1, const char* arg2) {
     SymbolNode *node1 = NULL, *node2 = NULL;
     SymbolType type1 = TYPE_UNKNOWN, type2 = TYPE_UNKNOWN;
     
     // Get the types of both operands
     if (is_integer_literal(arg1)) {
         type1 = TYPE_INTEGER;
     } else if (is_float_literal(arg1)) {
         type1 = TYPE_FLOAT;
     } else {
         node1 = symboltable_lookup(arg1);
         if (node1) {
             // For constants, make sure we use INTEGER type
             if (node1->category == CATEGORY_CONSTANT) {
                 if (node1->is_initialized) {
                     // Determine type based on the actual value
                     type1 = node1->type;  // Use the actual type of the constant
                 }
             } else {
                 type1 = node1->type;
             }
         }
     }
     
     if (is_integer_literal(arg2)) {
         type2 = TYPE_INTEGER;
     } else if (is_float_literal(arg2)) {
         type2 = TYPE_FLOAT;
     } else {
         node2 = symboltable_lookup(arg2);
         if (node2) {
             // For constants, make sure we use INTEGER type
             if (node2->category == CATEGORY_CONSTANT) {
                 if (node2->is_initialized) {
                     // Determine type based on the actual value
                     type2 = node2->type;  // Use the actual type of the constant
                 }
             } else {
                 type2 = node2->type;
             }
         }
     }
     
     // Special case: If both types are UNKNOWN, assume INTEGER type
     if (type1 == TYPE_UNKNOWN && type2 == TYPE_UNKNOWN) {
         return TYPE_INTEGER;
     }
     
     // If either type is unknown, use the known type
     if (type1 == TYPE_UNKNOWN) {
         return type2;
     }
     if (type2 == TYPE_UNKNOWN) {
         return type1;
     }
     
     // Special case: Modulo operation requires integer operands
     if (strcmp(op, "%") == 0) {
         if (type1 != TYPE_INTEGER || type2 != TYPE_INTEGER) {
             parser_semantic_error("Modulo operation requires integer operands");
         }
         return TYPE_INTEGER;
     }
     
     // Check for type compatibility
     if (!symboltable_types_compatible(type1, type2)) {
         parser_semantic_error("Type mismatch in arithmetic operation");
         // Continue with the first type for error recovery
         return type1;
     }
     
     // Return the "widest" type
     return (type1 == TYPE_FLOAT || type2 == TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INTEGER;
 }
 
 // Helper function for evaluating integer expressions
 int eval_int_expr(const char* expr) {
     // If it's a direct integer literal, convert and return
     if (is_integer_literal(expr)) {
         return string_to_int(expr);
     }
     
     // Try to lookup in the symbol table
     SymbolNode* node = symboltable_lookup(expr);
     if (node && node->type == TYPE_INTEGER && node->is_initialized) {
         return node->value.int_value;
     }
     
     // Cannot evaluate
     parser_semantic_error("Cannot evaluate integer expression");
     return 0;
 }
 
 // Helper function for evaluating float expressions
 float eval_float_expr(const char* expr) {
     // If it's a direct float literal, convert and return
     if (is_float_literal(expr)) {
         return string_to_float(expr);
     }
     
     // Try to lookup in the symbol table
     SymbolNode* node = symboltable_lookup(expr);
     if (node && node->type == TYPE_FLOAT && node->is_initialized) {
         return node->value.float_value;
     }
     
     // Cannot evaluate
     parser_semantic_error("Cannot evaluate float expression");
     return 0.0f;
 }
 
 // Global loop control stack
 #define MAX_LOOP_DEPTH 32
 static LoopControl loop_stack[MAX_LOOP_DEPTH];
 static int loop_depth = 0;
 
 // Create a new patch list entry
 PatchList* create_patch_list(int quad_id) {
     PatchList* entry = (PatchList*)safe_malloc(sizeof(PatchList));
     entry->quad_id = quad_id;
     entry->next = NULL;
     return entry;
 }
 
 // Merge two patch lists
 PatchList* merge_patch_lists(PatchList* list1, PatchList* list2) {
     if (!list1) return list2;
     if (!list2) return list1;
     
     PatchList* temp = list1;
     while (temp->next) {
         temp = temp->next;
     }
     temp->next = list2;
     
     return list1;
 }
 
 // Backpatch a list with a label
 void backpatch(PatchList* list, int label) {
    char label_str[16];
    sprintf(label_str, "L%d", label);
    
    while (list) {
        // CHANGE THIS - update the target field (arg1) for branch instructions
        quadgen_update(list->quad_id, NULL, label_str, NULL, NULL);
        
        PatchList* temp = list;
        list = list->next;
        free(temp);
    }
}
 
 // Push a new loop onto the stack
 void push_loop(int start_label, int end_label) {
     if (loop_depth >= MAX_LOOP_DEPTH) {
         parser_semantic_error("Loop nesting too deep");
         return;
     }
     
     loop_stack[loop_depth].start_label = start_label;
     loop_stack[loop_depth].end_label = end_label;
     loop_stack[loop_depth].break_list = NULL;
     loop_stack[loop_depth].cont_list = NULL;
     
     loop_depth++;
 }
 
 // Pop a loop from the stack
 LoopControl* pop_loop() {
     if (loop_depth <= 0) {
         parser_semantic_error("Loop stack underflow");
         return NULL;
     }
     
     loop_depth--;
     return &loop_stack[loop_depth];
 }
 
 // Get the current loop
 LoopControl* current_loop() {
     if (loop_depth <= 0) {
         return NULL;
     }
     
     return &loop_stack[loop_depth - 1];
 }
 
 // Process vector declaration with integer bounds and specific type
 void process_array_declaration(const char* array_name, int lower_bound, int upper_bound, SymbolType type) {
     // Calculate size from bounds
     int size = upper_bound - lower_bound + 1;
     
     // Validate size
     if (size <= 0) {
         parser_semantic_error("Vector size must be positive");
         return;
     }
     
     // Create temporary variables for bounds if needed for quadruples
     SymbolNode* temp_lower = symboltable_create_temp(TYPE_INTEGER);
     char lower_str[16];
     sprintf(lower_str, "%d", lower_bound);
     quadgen_generate(":=", lower_str, "", temp_lower->name);
     
     SymbolNode* temp_upper = symboltable_create_temp(TYPE_INTEGER);
     char upper_str[16];
     sprintf(upper_str, "%d", upper_bound);
     quadgen_generate(":=", upper_str, "", temp_upper->name);
     
     // Generate BOUNDS quadruple
     quadgen_generate("BOUNDS", temp_lower->name, temp_upper->name, "");
     
     // Declare the vector
     quadgen_generate("ADEC", array_name, "", "");
     
     // Create or update the vector in symbol table
     SymbolNode* array_node = symboltable_lookup(array_name);
     if (!array_node) {
         // Insert new vector with specified type
         symboltable_insert_array(array_name, type, lower_bound, size, current_line, current_column);
     } else {
         // Update existing node
         array_node->category = CATEGORY_ARRAY;
         array_node->array_lower_bound = lower_bound;
         array_node->array_size = size;
         array_node->type = type;  // Update the type
     }
 }
 
 // Process a variable reference
 char* process_variable(const char* name) {
    SymbolNode* node = symboltable_lookup(name);
    if (!node) {
        parser_semantic_error("Undefined variable");
        // Create a fallback variable to prevent cascading errors
        //node = symboltable_insert_variable(name, TYPE_INTEGER, current_line, current_column);
    }
    return safe_strdup(name);
}
 
 // Process array access
 char* process_array_access(const char* array_name, const char* index) {
    // Check if array exists
    SymbolNode* array_node = symboltable_lookup(array_name);
    
    if (!array_node) {
        char error_msg[256];
        sprintf(error_msg, "Undefined array '%s'", array_name);
        parser_semantic_error(error_msg);
        // Create a temporary for error recovery
        SymbolNode* temp = symboltable_create_temp(TYPE_INTEGER);
        quadgen_generate(":=", "0", "", temp->name); // Set to 0 for recovery
        symboltable_set_initialized(temp);
        return safe_strdup(temp->name);
    }
    
    if (array_node->category != CATEGORY_ARRAY) {
        char error_msg[256];
        sprintf(error_msg, "'%s' is not an array", array_name);
        parser_semantic_error(error_msg);
        // Continue with the variable as if it were scalar
        return safe_strdup(array_name);
    }
    
    // Verify index is an integer expression
    if (is_integer_literal(index)) {
        // Verify constant index is within bounds
        int idx_val = string_to_int(index);
        if (idx_val < array_node->array_lower_bound || 
            idx_val >= array_node->array_lower_bound + array_node->array_size) {
            char error_msg[256];
            sprintf(error_msg, "Array index %d out of bounds [%d:%d]", 
                  idx_val, 
                  array_node->array_lower_bound, 
                  array_node->array_lower_bound + array_node->array_size - 1);
            parser_semantic_error(error_msg);
            // Continue anyway, bounds error will be evident at runtime
        }
    } 
    else {
        // Check variable index type
        SymbolNode* index_node = symboltable_lookup(index);
        if (!index_node) {
            char error_msg[256];
            sprintf(error_msg, "Undefined index expression '%s'", index);
            parser_semantic_error(error_msg);
            // Create a default index for recovery
            SymbolNode* default_idx = symboltable_create_temp(TYPE_INTEGER);
            quadgen_generate(":=", "0", "", default_idx->name);
            symboltable_set_initialized(default_idx);
            index = default_idx->name;
        }
        else if (index_node->type != TYPE_INTEGER) {
            parser_semantic_error("Array index must be INTEGER");
            
            // Try to convert non-integer index to integer if possible
            if (index_node->type == TYPE_FLOAT) {
                // Create a conversion temp
                SymbolNode* int_index = symboltable_create_temp(TYPE_INTEGER);
                quadgen_generate("CONVERT", index, "INTEGER", int_index->name);
                symboltable_set_initialized(int_index);
                index = int_index->name;
            } else {
                // For other types, use a default index
                SymbolNode* default_idx = symboltable_create_temp(TYPE_INTEGER);
                quadgen_generate(":=", "0", "", default_idx->name);
                symboltable_set_initialized(default_idx);
                index = default_idx->name;
            }
        }
    }
    
    // Get the lower bound of the array
    int lower_bound = array_node->array_lower_bound;
    
    // Create a new temporary for the adjusted index
    SymbolNode* adjusted_index = symboltable_create_temp(TYPE_INTEGER);
    symboltable_set_initialized(adjusted_index);
    
    // Generate code to calculate: adjusted_index = index - lower_bound
    char lower_bound_str[16];
    sprintf(lower_bound_str, "%d", lower_bound);
    quadgen_generate("-", index, lower_bound_str, adjusted_index->name);
    
    // Create temporary to hold array element value
    SymbolNode* temp = symboltable_create_temp(array_node->type);
    
    // Format array access according to quadruple format - use adjusted index
    char array_access[256];
    sprintf(array_access, "%s[%s]", array_name, adjusted_index->name);
    
    // Generate the quadruple
    quadgen_generate(":=", array_access, "", temp->name);
    
    symboltable_set_initialized(temp);
    return safe_strdup(temp->name);
}
 
 // Utility function to convert SymbolType to string representation
 const char* get_type_name(SymbolType type) {
     switch (type) {
         case TYPE_INTEGER: return "INTEGER";
         case TYPE_FLOAT: return "FLOAT";
         case TYPE_CHAR: return "CHAR";
         case TYPE_STRING: return "STRING";
         case TYPE_UNKNOWN: return "UNKNOWN";
         default: return "INVALID";
     }
 }
 
 // Helper function to determine expression type
 SymbolType get_expression_type(const char* expr) {
     if (is_integer_literal(expr)) {
         return TYPE_INTEGER;
     } else if (is_float_literal(expr)) {
         return TYPE_FLOAT;
     } else {
         SymbolNode* expr_node = symboltable_lookup(expr);
         if (expr_node) {
             return expr_node->type;
         }
     }
     return TYPE_UNKNOWN;
 }
 
 // Function to handle simple variable assignment
 void process_simple_assignment(const char* var_name, const char* expr_value) {
     SymbolNode* var_node = symboltable_lookup(var_name);
     
     if (!var_node) {
         char error_msg[256];
         sprintf(error_msg, "Undefined variable '%s'", var_name);
         parser_semantic_error(error_msg);
         return;
     }
     
     // Check if the variable is a constant (cannot be modified)
     if (var_node->category == CATEGORY_CONSTANT) {
         char error_msg[256];
         sprintf(error_msg, "Cannot modify constant '%s'", var_name);
         parser_semantic_error(error_msg);
         return;
     }
     
     // Check for expression type
     SymbolType expr_type = get_expression_type(expr_value);
     
     if (expr_type == TYPE_UNKNOWN) {
         char error_msg[256];
         sprintf(error_msg, "Undefined expression '%s' in assignment", expr_value);
         parser_semantic_error(error_msg);
         return;
     }
     
     // Type compatibility check
     if (!symboltable_types_compatible(var_node->type, expr_type)) {
         char error_msg[256];
         sprintf(error_msg, "Type mismatch in assignment: cannot assign %s to %s", 
                 get_type_name(expr_type), get_type_name(var_node->type));
         parser_semantic_error(error_msg);
         return;
     }
     
     // Check if conversion is needed
     if (var_node->type != expr_type && var_node->type != TYPE_UNKNOWN && expr_type != TYPE_UNKNOWN) {
         // Need to convert the expression to match variable type
         SymbolNode* converted = symboltable_create_temp(var_node->type);
         quadgen_generate("CONVERT", expr_value, get_type_name(var_node->type), converted->name);
         symboltable_set_initialized(converted);
         
         // Use the converted value for assignment
         quadgen_generate(":=", converted->name, "", var_name);
     } else {
         // Direct assignment, no conversion needed
         quadgen_generate(":=", expr_value, "", var_name);
     }
     
     // Mark variable as initialized
     symboltable_set_initialized(var_node);
 }
 
 // Function to handle array element assignment
void process_array_assignment(const char* array_name, const char* index, const char* expr_value) {
    // Check if array exists
    SymbolNode* array_node = symboltable_lookup(array_name);
    
    if (!array_node) {
        char error_msg[256];
        sprintf(error_msg, "Undefined array '%s'", array_name);
        parser_semantic_error(error_msg);
        return;
    }
    
    if (array_node->category != CATEGORY_ARRAY) {
        char error_msg[256];
        sprintf(error_msg, "'%s' is not an array", array_name);
        parser_semantic_error(error_msg);
        return;
    }
    
    // Verify index is valid
    if (is_integer_literal(index)) {
        // Verify constant index is within bounds
        int idx_val = string_to_int(index);
        if (idx_val < array_node->array_lower_bound || 
            idx_val >= array_node->array_lower_bound + array_node->array_size) {
            char error_msg[256];
            sprintf(error_msg, "Array index %d out of bounds [%d:%d]", 
                  idx_val, 
                  array_node->array_lower_bound, 
                  array_node->array_lower_bound + array_node->array_size - 1);
            parser_semantic_error(error_msg);
            // Continue anyway for code generation
        }
    } else {
        // Check variable index type
        SymbolNode* index_node = symboltable_lookup(index);
        if (!index_node) {
            char error_msg[256];
            sprintf(error_msg, "Undefined index expression '%s'", index);
            parser_semantic_error(error_msg);
            return;
        }
        else if (index_node->type != TYPE_INTEGER) {
            parser_semantic_error("Array index must be INTEGER");
            
            // Try to convert non-integer index to integer if possible
            const char* original_index = index;
            if (index_node->type == TYPE_FLOAT) {
                // Create a conversion temp
                SymbolNode* int_index = symboltable_create_temp(TYPE_INTEGER);
                quadgen_generate("CONVERT", original_index, "INTEGER", int_index->name);
                symboltable_set_initialized(int_index);
                index = int_index->name;
            } else {
                // For other types, use a default index
                SymbolNode* default_idx = symboltable_create_temp(TYPE_INTEGER);
                quadgen_generate(":=", "0", "", default_idx->name);
                symboltable_set_initialized(default_idx);
                index = default_idx->name;
            }
        }
    }
    
    // Check expression type
    SymbolType expr_type = get_expression_type(expr_value);
    
    if (expr_type == TYPE_UNKNOWN) {
        char error_msg[256];
        sprintf(error_msg, "Undefined expression '%s' in assignment", expr_value);
        parser_semantic_error(error_msg);
        return;
    }
    
    // Type compatibility check
    if (!symboltable_types_compatible(array_node->type, expr_type)) {
        char error_msg[256];
        sprintf(error_msg, "Type mismatch in array element assignment: cannot assign %s to %s array", 
                get_type_name(expr_type), get_type_name(array_node->type));
        parser_semantic_error(error_msg);
        return;
    }
    
    // Get the lower bound of the array
    int lower_bound = array_node->array_lower_bound;
    
    // Create a new temporary for the adjusted index
    SymbolNode* adjusted_index = symboltable_create_temp(TYPE_INTEGER);
    symboltable_set_initialized(adjusted_index);
    
    // Generate code to calculate: adjusted_index = index - lower_bound
    char lower_bound_str[16];
    sprintf(lower_bound_str, "%d", lower_bound);
    quadgen_generate("-", index, lower_bound_str, adjusted_index->name);
    
    // Format the array access for the left side using the adjusted index
    char array_access[256];
    sprintf(array_access, "%s[%s]", array_name, adjusted_index->name);
    
    // Check if conversion is needed
    if (array_node->type != expr_type && array_node->type != TYPE_UNKNOWN && expr_type != TYPE_UNKNOWN) {
        // Need to convert the expression to match array element type
        SymbolNode* converted = symboltable_create_temp(array_node->type);
        quadgen_generate("CONVERT", expr_value, get_type_name(array_node->type), converted->name);
        symboltable_set_initialized(converted);
        
        // Use the converted value for assignment
        quadgen_generate(":=", converted->name, "", array_access);
    } else {
        // Direct assignment, no conversion needed
        quadgen_generate(":=", expr_value, "", array_access);
    }
}
 
 // Process input/output statements
// Process input/output statements
void process_io_statement(const char* io_type, const char* format, const char* target) {
    // Check if target is valid
    SymbolNode* var_node = symboltable_lookup(target);
    if (!var_node) {
        parser_semantic_error("Undefined variable in I/O statement");
        return;
    }
    
    // Verify format matches variable type
    char format_error = 0;
    
    // Extract format specifier from the string
    const char* format_specifier = NULL;
    if (format && strlen(format) > 0) {
        // Look for format specifiers ($, %, #, &) in the string
        format_specifier = strrchr(format, '$'); // Integer
        if (!format_specifier) format_specifier = strrchr(format, '%'); // Float
        if (!format_specifier) format_specifier = strrchr(format, '#'); // String
        if (!format_specifier) format_specifier = strrchr(format, '&'); // Char
    }
    
    // Check if format matches type
    if (format_specifier) {
        switch (*format_specifier) {
            case '$':
                if (var_node->type != TYPE_INTEGER) format_error = 1;
                break;
            case '%':
                if (var_node->type != TYPE_FLOAT) format_error = 1;
                break;
            case '#':
                if (var_node->type != TYPE_STRING) format_error = 1;
                break;
            case '&':
                if (var_node->type != TYPE_CHAR) format_error = 1;
                break;
        }
    }
    
    if (format_error) {
        parser_semantic_error("Format specifier doesn't match variable type");
        return;
    }
    
    if (strcmp(io_type, "READ") == 0) {
        // Check if the variable is a constant (cannot be modified)
        if (var_node->category == CATEGORY_CONSTANT) {
            char error_msg[256];
            sprintf(error_msg, "Cannot modify constant '%s' with READ", target);
            parser_semantic_error(error_msg);
            return;
        }
        
        // Generate the READ quadruple
        quadgen_generate("LIRE", format, "", target);
        symboltable_set_initialized(var_node);
    } else if (strcmp(io_type, "DISPLAY") == 0) {
        // Check if the variable is initialized
        if (!var_node->is_initialized) {
            parser_semantic_error("Cannot display uninitialized variable");
            // Continue anyway for code generation
        }
        
        // Generate the DISPLAY quadruple
        quadgen_generate("AFFICHER", format, target, "");
    }
}
 
 // Process condition for control structures
 ConditionAttrs* process_condition(const char* op, const char* arg1, const char* arg2) {
    ConditionAttrs* attrs = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
    
    // Get operand types for type checking
    SymbolType type1 = get_expression_type(arg1);
    SymbolType type2 = get_expression_type(arg2);
    
    // Check if the types are comparable
    if (!symboltable_types_compatible(type1, type2)) {
        char error_msg[256];
        sprintf(error_msg, "Cannot compare incompatible types (%s and %s) in condition", 
                get_type_name(type1), get_type_name(type2));
        parser_semantic_error(error_msg);
    }
    
    // Make sure both operands are valid before generating code
    if (!is_integer_literal(arg1) && !is_float_literal(arg1)) {
        SymbolNode* node1 = symboltable_lookup(arg1);
        if (!node1) {
            // Create a placeholder for error recovery
            node1 = symboltable_create_temp(TYPE_INTEGER);
            quadgen_generate(":=", "0", "", node1->name);
            symboltable_set_initialized(node1);
            arg1 = node1->name;
        }
    }
    
    if (!is_integer_literal(arg2) && !is_float_literal(arg2)) {
        SymbolNode* node2 = symboltable_lookup(arg2);
        if (!node2) {
            // Create a placeholder for error recovery
            node2 = symboltable_create_temp(TYPE_INTEGER);
            quadgen_generate(":=", "0", "", node2->name);
            symboltable_set_initialized(node2);
            arg2 = node2->name;
        }
    }
    
    // Apply conversion if needed for comparison
    char* conv_arg1 = safe_strdup(arg1);
    char* conv_arg2 = safe_strdup(arg2);
    
    // Use the "widest" type for comparison
    SymbolType compare_type = (type1 == TYPE_FLOAT || type2 == TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INTEGER;
    
    // Convert arg1 if needed
    if (type1 != TYPE_UNKNOWN && type1 != compare_type) {
        SymbolNode* conv_temp = symboltable_create_temp(compare_type);
        quadgen_generate("CONVERT", arg1, get_type_name(compare_type), conv_temp->name);
        symboltable_set_initialized(conv_temp);
        free(conv_arg1);
        conv_arg1 = safe_strdup(conv_temp->name);
    }
    
    // Convert arg2 if needed
    if (type2 != TYPE_UNKNOWN && type2 != compare_type) {
        SymbolNode* conv_temp = symboltable_create_temp(compare_type);
        quadgen_generate("CONVERT", arg2, get_type_name(compare_type), conv_temp->name);
        symboltable_set_initialized(conv_temp);
        free(conv_arg2);
        conv_arg2 = safe_strdup(conv_temp->name);
    }
    
    // Choose the OPPOSITE jump instruction
    const char* jump_op = "SAUT"; // Default
    
    if (strcmp(op, "GT") == 0) {
        jump_op = "BLE"; // Jump if NOT greater than
    } else if (strcmp(op, "GE") == 0) {
        jump_op = "BL";  // Jump if NOT greater or equal
    } else if (strcmp(op, "LT") == 0) {
        jump_op = "BGE"; // Jump if NOT less than
    } else if (strcmp(op, "LE") == 0) {
        jump_op = "BG";  // Jump if NOT less or equal
    } else if (strcmp(op, "EQ") == 0) {
        jump_op = "BNE"; // Jump if NOT equal
    } else if (strcmp(op, "NE") == 0) {
        jump_op = "BE";  // Jump if NOT not-equal
    }
    
    // CHANGE THIS - use correct order (op, target, arg1, arg2)
    int quad_id = quadgen_generate(jump_op, "?", conv_arg1, conv_arg2);
    attrs->false_list = create_patch_list(quad_id);
    
    // Also change this for unconditional jump
    quad_id = quadgen_generate("SAUT", "?", "", "");
    attrs->true_list = create_patch_list(quad_id);
    
    free(conv_arg1);
    free(conv_arg2);
    
    return attrs;
}
 
 // Process logical operators
 ConditionAttrs* process_logical_operator(const char* op, ConditionAttrs* cond1, ConditionAttrs* cond2) {
     ConditionAttrs* result = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
     
     if (strcmp(op, "AND") == 0) {
         // For AND, first check cond1, if true then check cond2
         // Need to backpatch cond1's true list to the beginning of cond2
         int label = quadgen_new_label();
         quadgen_generate("ETIQ", quadgen_label_to_string(label), "", "");
         backpatch(cond1->true_list, label);
         
         // Merge false lists
         result->true_list = cond2->true_list;
         result->false_list = merge_patch_lists(cond1->false_list, cond2->false_list);
     } else if (strcmp(op, "OR") == 0) {
         // For OR, if cond1 is false, check cond2
         // Need to backpatch cond1's false list to the beginning of cond2
         int label = quadgen_new_label();
         quadgen_generate("ETIQ", quadgen_label_to_string(label), "", "");
         backpatch(cond1->false_list, label);
         
         // Merge true lists
         result->true_list = merge_patch_lists(cond1->true_list, cond2->true_list);
         result->false_list = cond2->false_list;
     } else if (strcmp(op, "NOT") == 0) {
         // For NOT, swap true and false lists
         result->true_list = cond1->false_list;
         result->false_list = cond1->true_list;
     }
     
     // Free the input conditions since we've transferred ownership
     if (strcmp(op, "NOT") != 0) {
         free(cond1);
         free(cond2);
     } else {
         free(cond1);
     }
     
     return result;
 }


// Global if-statement context stack (not static to match extern declaration)
IfContext* if_context_stack = NULL;

// Begin an IF statement by evaluating the condition
IfContext* begin_if_statement(ConditionAttrs* condition) {
    if (!condition) {
        parser_semantic_error("Missing condition in if statement");
        return NULL;
    }
    
    // Create new if context
    IfContext* ctx = (IfContext*)safe_malloc(sizeof(IfContext));
    ctx->condition = condition;
    ctx->true_label = quadgen_new_label();
    ctx->end_label = quadgen_new_label();
    ctx->end_jumps = NULL;
    
    // REMOVE THESE TWO LINES:
    // backpatch(condition->true_list, ctx->true_label);
    // backpatch(condition->false_list, ctx->end_label);
    
    // Push onto context stack
    ctx->next = if_context_stack;
    if_context_stack = ctx;
    
    return ctx;
}

// Begin the THEN part of an IF statement
void begin_then_part(void) {
    if (!if_context_stack) {
        parser_semantic_error("No active IF statement");
        return;
    }
    
    // Place the true label
    quadgen_generate("ETIQ", quadgen_label_to_string(if_context_stack->true_label), "", "");
    
    // ADD THIS LINE - backpatch after the label is placed
    backpatch(if_context_stack->condition->true_list, if_context_stack->true_label);
}

// Begin the ELSE part of an IF statement
void begin_else_part(void) {
    if (!if_context_stack) {
        parser_semantic_error("No active IF statement");
        return;
    }
    
    // Generate jump to end of if-else
    int jump_end_quad = quadgen_generate("SAUT", "", "", "");
    if_context_stack->end_jumps = create_patch_list(jump_end_quad);
    
    // Generate false label
    int false_label = quadgen_new_label();
    quadgen_generate("ETIQ", quadgen_label_to_string(false_label), "", "");
    
    // ADD THIS LINE - backpatch after the label is placed
    backpatch(if_context_stack->condition->false_list, false_label);
}

// End an IF statement
void end_if_statement(int has_else) {
    if (!if_context_stack) {
        parser_semantic_error("No active IF statement");
        return;
    }
    
    // Pop context from stack
    IfContext* ctx = if_context_stack;
    if_context_stack = ctx->next;
    
    if (has_else && ctx->end_jumps) {
        // Backpatch jumps to end
        backpatch(ctx->end_jumps, ctx->end_label);
    } else {
        // ADD THIS LINE - for if statements without else, backpatch false list to end
        backpatch(ctx->condition->false_list, ctx->end_label);
    }
    
    // Place end label
    quadgen_generate("ETIQ", quadgen_label_to_string(ctx->end_label), "", "");
    
    // Clean up
    free(ctx->condition); // ADD THIS to free the condition
    free(ctx);
}
 
/* FOR loop context stack */
static ForLoopContext* for_stack = NULL;

/* Initialize a FOR loop counter and set up condition check */
void begin_for_loop_init(const char* var_name, const char* initial_value) {
    // Check loop counter variable
    SymbolNode* var_node = symboltable_lookup(var_name);
    if (!var_node) {
        parser_semantic_error("Undefined loop counter variable");
        return;
    } else if (var_node->type != TYPE_INTEGER) {
        parser_semantic_error("Loop counter must be an integer");
        return;
    } else if (var_node->category == CATEGORY_CONSTANT) {
        parser_semantic_error("Cannot use constant as loop counter");
        return;
    }
    
    // Check if the initial value needs type conversion
    SymbolType init_type = get_expression_type(initial_value);
    if (init_type != TYPE_INTEGER && init_type != TYPE_UNKNOWN) {
        // Create a converted initial value
        SymbolNode* converted = symboltable_create_temp(TYPE_INTEGER);
        quadgen_generate("CONVERT", initial_value, "INTEGER", converted->name);
        symboltable_set_initialized(converted);
        initial_value = converted->name;
    }
    
    // CRUCIAL: Initialize counter BEFORE condition check
    quadgen_generate(":=", initial_value, "", var_name);
    symboltable_set_initialized(var_node);
    
    // Create new for loop context
    ForLoopContext* ctx = (ForLoopContext*)safe_malloc(sizeof(ForLoopContext));
    ctx->var_name = safe_strdup(var_name);
    
    // Create and set condition label
    ctx->cond_label = quadgen_new_label();
    ctx->body_label = 0;  // Will be set later
    ctx->end_label = 0;   // Will be set later
    
    // Push onto stack
    ctx->next = for_stack;
    for_stack = ctx;
    
    // Generate condition label
    quadgen_generate("ETIQ", quadgen_label_to_string(ctx->cond_label), "", "");
}

/* Set up FOR loop condition checking and body label */
void begin_for_loop_condition(ConditionAttrs* condition) {
    // Get current for loop context
    if (!for_stack) {
        parser_semantic_error("FOR loop context stack underflow");
        return;
    }
    
    if (!condition) {
        parser_semantic_error("Missing condition in for loop");
        return;
    }
    
    // Create body and end labels
    for_stack->body_label = quadgen_new_label();
    for_stack->end_label = quadgen_new_label();
    
    // Backpatch condition
    backpatch(condition->true_list, for_stack->body_label);
    backpatch(condition->false_list, for_stack->end_label);
    
    // Generate body label
    quadgen_generate("ETIQ", quadgen_label_to_string(for_stack->body_label), "", "");
    
    // Free condition as it's been processed
    free(condition);
}

/* Complete a FOR loop by generating increment, jump back and end label */
void end_for_loop() {
    // Check for stack underflow
    if (!for_stack) {
        parser_semantic_error("FOR loop context stack underflow");
        return;
    }
    
    // Get context from top of stack
    ForLoopContext* ctx = for_stack;
    for_stack = ctx->next;
    
    // Generate increment
    quadgen_generate("+", ctx->var_name, "1", ctx->var_name);
    
    // Generate jump back to condition
    quadgen_generate("SAUT", "", "", quadgen_label_to_string(ctx->cond_label));
    
    // Generate end label
    quadgen_generate("ETIQ", quadgen_label_to_string(ctx->end_label), "", "");
    
    // Clean up
    free(ctx->var_name);
    free(ctx);
}

 
 // Process while loop
 void process_while_loop(ConditionAttrs* condition) {
     if (!condition) {
         // Create default condition if missing
         condition = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
         int quad_id = quadgen_generate("SAUT", "", "", "");
         condition->true_list = create_patch_list(quad_id);
         condition->false_list = NULL;
     }
     
     // Create labels for the loop
     int start_label = quadgen_new_label();
     int body_label = quadgen_new_label();
     int end_label = quadgen_new_label();
     
     // Place the start label
     quadgen_generate("ETIQ", quadgen_label_to_string(start_label), "", "");
     
     // Backpatch condition
     if (condition->true_list) {
         backpatch(condition->true_list, body_label);
     }
     if (condition->false_list) {
         backpatch(condition->false_list, end_label);
     }
     
     // Push loop control
     push_loop(start_label, end_label);
     
     // Place the body label
     quadgen_generate("ETIQ", quadgen_label_to_string(body_label), "", "");
     
     // Note: loop body statements are processed separately
     
     // Jump back to condition
     quadgen_generate("SAUT", "", "", quadgen_label_to_string(start_label));
     
     // Place the end label
     quadgen_generate("ETIQ", quadgen_label_to_string(end_label), "", "");
     
     // Backpatch break and continue statements
     LoopControl* loop = pop_loop();
     if (loop) {
         backpatch(loop->break_list, loop->end_label);
         backpatch(loop->cont_list, loop->start_label);
     }
     
     free(condition);
 }
 
 // Process jump statements (break/continue)
 void process_jump_statement(const char* jump_type) {
     // Add to the appropriate list of current loop
     LoopControl* loop = current_loop();
     if (!loop) {
         parser_semantic_error("Jump statement outside of loop");
         return;
     }
     
     if (strcmp(jump_type, "BREAK") == 0) {
         int quad_id = quadgen_generate("SAUT", "", "", "");
         loop->break_list = merge_patch_lists(loop->break_list, create_patch_list(quad_id));
     } else if (strcmp(jump_type, "CONTINUE") == 0) {
         int quad_id = quadgen_generate("SAUT", "", "", "");
         loop->cont_list = merge_patch_lists(loop->cont_list, create_patch_list(quad_id));
     }
 }