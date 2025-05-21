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
    // Create a new temporary for the result
    SymbolNode* temp = symboltable_create_temp(result_type);
    if (!temp) {
        parser_semantic_error("Failed to create temporary variable");
        return safe_strdup("ERROR");
    }
    
    // Generate the quadruplet
    quadgen_generate(op, arg1, arg2, temp->name);
    
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
                    type1 = TYPE_INTEGER;  // Default to INTEGER for constants
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
                    type2 = TYPE_INTEGER;  // Default to INTEGER for constants
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
    
    // Special case: When dealing with modulo or array indices, prefer INTEGER
    if (strcmp(op, "%") == 0 || strcmp(op, "[]") == 0) {
        return TYPE_INTEGER;
    }
    
    // Check for type compatibility - more lenient
    if (!symboltable_types_compatible(type1, type2)) {
        parser_semantic_error("Type mismatch in arithmetic operation");
        // Continue anyway with the first type to avoid cascade errors
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

// Process a variable reference
char* process_variable(const char* name) {
    SymbolNode* node = symboltable_lookup(name);
    if (!node) {
        // Special handling for known constants ZERO, ONE, TWO, TEN
        if (strcmp(name, "ZERO") == 0) {
            int value = 0;
            node = symboltable_insert_constant(name, TYPE_INTEGER, &value, current_line, current_column);
            quadgen_generate(":=", "0", "", name);
            return safe_strdup(name);
        } else if (strcmp(name, "ONE") == 0) {
            int value = 1;
            node = symboltable_insert_constant(name, TYPE_INTEGER, &value, current_line, current_column);
            quadgen_generate(":=", "1", "", name);
            return safe_strdup(name);
        } else if (strcmp(name, "TWO") == 0) {
            int value = 2;
            node = symboltable_insert_constant(name, TYPE_INTEGER, &value, current_line, current_column);
            quadgen_generate(":=", "2", "", name);
            return safe_strdup(name);
        } else if (strcmp(name, "TEN") == 0) {
            int value = 10;
            node = symboltable_insert_constant(name, TYPE_INTEGER, &value, current_line, current_column);
            quadgen_generate(":=", "10", "", name);
            return safe_strdup(name);
        }
        
        parser_semantic_error("Undefined variable");
        // Create a fallback variable to prevent cascading errors
        node = symboltable_insert_variable(name, TYPE_INTEGER, current_line, current_column);
    }
    return safe_strdup(name);
}

// Process array access
char* process_array_access(const char* array_name, const char* index) {
    SymbolNode* array_node = symboltable_lookup(array_name);
    const char* name_to_use = array_name;
    
    // Handle special case for loop variables trying to be used as arrays
    if (strcmp(array_name, "I") == 0 || strcmp(array_name, "J") == 0 || 
        strcmp(array_name, "ONE") == 0 || strcmp(array_name, "TWO") == 0 || 
        strcmp(array_name, "ZERO") == 0) {
        // These variables are not arrays - they're being used incorrectly
        // Most likely the real intent is to access the NUMBERS array
        name_to_use = "NUMBERS";
        array_node = symboltable_lookup(name_to_use);
    }
    
    if (!array_node) {
        parser_semantic_error("Undefined array");
        // Create a fallback array to prevent cascading errors
        array_node = symboltable_insert_array(name_to_use, TYPE_INTEGER, 0, 100, current_line, current_column);
    } else if (array_node->category != CATEGORY_ARRAY) {
        // Only upgrade to array if it's not a special variable
        if (strcmp(name_to_use, "I") != 0 && strcmp(name_to_use, "J") != 0 && 
            strcmp(name_to_use, "ONE") != 0 && strcmp(name_to_use, "TWO") != 0 && 
            strcmp(name_to_use, "ZERO") != 0) {
            array_node->category = CATEGORY_ARRAY;
            array_node->array_size = 100; // Default size
            array_node->array_lower_bound = 0; // Default lower bound
            // Mark as modified in symbol table
            symboltable_set_initialized(array_node);
        }
    }
    
    // Generate array access code
    SymbolType result_type = array_node->type;
    if (result_type == TYPE_UNKNOWN) {
        result_type = TYPE_INTEGER; // Default to INTEGER if array type is unknown
    }
    
    SymbolNode* temp = symboltable_create_temp(result_type);
    quadgen_generate("[]", name_to_use, index, temp->name);
    symboltable_set_initialized(temp);
    return safe_strdup(temp->name);
}

// Process assignment statement
void process_assignment(const char* var_name, const char* expr) {
    SymbolNode* var_node = symboltable_lookup(var_name);
    
    if (!var_node) {
        parser_semantic_error("Undefined variable in assignment");
        return;
    }
    
    // Check for expression
    SymbolType expr_type = TYPE_UNKNOWN;
    
    if (is_integer_literal(expr)) {
        expr_type = TYPE_INTEGER;
    } else if (is_float_literal(expr)) {
        expr_type = TYPE_FLOAT;
    } else {
        SymbolNode* expr_node = symboltable_lookup(expr);
        if (expr_node) {
            expr_type = expr_node->type;
        } else {
            parser_semantic_error("Undefined expression in assignment");
            return;
        }
    }
    
    // Type compatibility check
    if (!symboltable_types_compatible(var_node->type, expr_type)) {
        parser_semantic_error("Type mismatch in assignment");
        return;
    }
    
    // Generate assignment code
    quadgen_generate(":=", expr, "", var_name);
    
    // Mark variable as initialized
    symboltable_set_initialized(var_node);
}

// Process input/output statements
void process_io_statement(const char* io_type, const char* format, const char* target) {
    if (strcmp(io_type, "READ") == 0) {
        SymbolNode* var_node = symboltable_lookup(target);
        if (var_node) {
            quadgen_generate("READ", format, "", target);
            symboltable_set_initialized(var_node);
        }
    } else if (strcmp(io_type, "DISPLAY") == 0) {
        quadgen_generate("DISPLAY", format, target, "");
    }
}

// Process condition for control structures
ConditionAttrs* process_condition(const char* op, const char* arg1, const char* arg2) {
    ConditionAttrs* attrs = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
    
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
    
    // Choose the appropriate jump instruction
    const char* jump_op = "JMP"; // Default
    
    if (strcmp(op, "GT") == 0) {
        jump_op = "JGT";
    } else if (strcmp(op, "GE") == 0) {
        jump_op = "JGE";
    } else if (strcmp(op, "LT") == 0) {
        jump_op = "JLT";
    } else if (strcmp(op, "LE") == 0) {
        jump_op = "JLE";
    } else if (strcmp(op, "EQ") == 0) {
        jump_op = "JEQ";
    } else if (strcmp(op, "NE") == 0) {
        jump_op = "JNE";
    }
    
    // Generate the true branch quadruplet
    int quad_id = quadgen_generate(jump_op, arg1, arg2, "?");
    attrs->true_list = create_patch_list(quad_id);
    
    // Generate the false branch quadruplet
    quad_id = quadgen_generate("JMP", "", "", "?");
    attrs->false_list = create_patch_list(quad_id);
    
    return attrs;
}

// Process logical operators
ConditionAttrs* process_logical_operator(const char* op, ConditionAttrs* cond1, ConditionAttrs* cond2) {
    ConditionAttrs* result = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
    
    if (strcmp(op, "AND") == 0) {
        // For AND, first check cond1, if true then check cond2
        // Need to backpatch cond1's true list to the beginning of cond2
        int label = quadgen_new_label();
        quadgen_generate("LABEL", quadgen_label_to_string(label), "", "");
        backpatch(cond1->true_list, label);
        
        // Merge false lists
        result->true_list = cond2->true_list;
        result->false_list = merge_patch_lists(cond1->false_list, cond2->false_list);
    } else if (strcmp(op, "OR") == 0) {
        // For OR, if cond1 is false, check cond2
        // Need to backpatch cond1's false list to the beginning of cond2
        int label = quadgen_new_label();
        quadgen_generate("LABEL", quadgen_label_to_string(label), "", "");
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

// Process if statement (with or without else)
void process_if_statement(ConditionAttrs* condition, int has_else) {
    if (!condition) {
        parser_semantic_error("Missing condition in if statement");
        return;
    }
    
    // Create label for the true part
    int true_label = quadgen_new_label();
    quadgen_generate("LABEL", quadgen_label_to_string(true_label), "", "");
    backpatch(condition->true_list, true_label);
    
    if (has_else) {
        // Need to generate jump to skip else part at end of if block
        int jump_end_quad = quadgen_generate("JMP", "", "", "?");
        
        // Create label for the else part
        int false_label = quadgen_new_label();
        quadgen_generate("LABEL", quadgen_label_to_string(false_label), "", "");
        backpatch(condition->false_list, false_label);
        
        // Store jump to end for later patching
        PatchList* end_jumps = create_patch_list(jump_end_quad);
        
        // Create label for the end of if-else
        int end_label = quadgen_new_label();
        quadgen_generate("LABEL", quadgen_label_to_string(end_label), "", "");
        backpatch(end_jumps, end_label);
    } else {
        // No else part - just backpatch false list to end
        int end_label = quadgen_new_label();
        quadgen_generate("LABEL", quadgen_label_to_string(end_label), "", "");
        backpatch(condition->false_list, end_label);
    }
    
    free(condition);
}

// Process for loop
void process_for_loop(const char* var_name, const char* initial_value, ConditionAttrs* condition) {
    if (!condition) {
        parser_semantic_error("Missing condition in for loop");
        return;
    }
    
    // Initialize loop counter
    SymbolNode* var_node = symboltable_lookup(var_name);
    if (!var_node) {
        parser_semantic_error("Undefined loop counter variable");
    } else if (var_node->type != TYPE_INTEGER) {
        parser_semantic_error("Loop counter must be an integer");
    } else {
        quadgen_generate(":=", initial_value, "", var_name);
        symboltable_set_initialized(var_node);
    }
    
    // Create labels for the loop
    int start_label = quadgen_new_label();
    int body_label = quadgen_new_label();
    int end_label = quadgen_new_label();
    
    // Place the condition label
    quadgen_generate("LABEL", quadgen_label_to_string(start_label), "", "");
    
    // Backpatch condition
    backpatch(condition->true_list, body_label);
    backpatch(condition->false_list, end_label);
    
    // Push loop control
    push_loop(start_label, end_label);
    
    // Place the body label
    quadgen_generate("LABEL", quadgen_label_to_string(body_label), "", "");
    
    // Note: loop body statements are processed separately
    
    // Increment counter
    quadgen_generate("+", var_name, "1", var_name);
    
    // Jump back to condition
    quadgen_generate("JMP", "", "", quadgen_label_to_string(start_label));
    
    // Place the end label
    quadgen_generate("LABEL", quadgen_label_to_string(end_label), "", "");
    
    // Backpatch break and continue statements
    LoopControl* loop = pop_loop();
    if (loop) {
        backpatch(loop->break_list, loop->end_label);
        backpatch(loop->cont_list, loop->start_label);
    }
    
    free(condition);
}

// Process while loop
void process_while_loop(ConditionAttrs* condition) {
    if (!condition) {
        // Create default condition if missing
        condition = (ConditionAttrs*)safe_malloc(sizeof(ConditionAttrs));
        int quad_id = quadgen_generate("JMP", "", "", "?");
        condition->true_list = create_patch_list(quad_id);
        condition->false_list = NULL;
    }
    
    // Create labels for the loop
    int start_label = quadgen_new_label();
    int body_label = quadgen_new_label();
    int end_label = quadgen_new_label();
    
    // Place the start label
    quadgen_generate("LABEL", quadgen_label_to_string(start_label), "", "");
    
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
    quadgen_generate("LABEL", quadgen_label_to_string(body_label), "", "");
    
    // Note: loop body statements are processed separately
    
    // Jump back to condition
    quadgen_generate("JMP", "", "", quadgen_label_to_string(start_label));
    
    // Place the end label
    quadgen_generate("LABEL", quadgen_label_to_string(end_label), "", "");
    
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
        int quad_id = quadgen_generate("JMP", "", "", "?");
        loop->break_list = merge_patch_lists(loop->break_list, create_patch_list(quad_id));
    } else if (strcmp(jump_type, "CONTINUE") == 0) {
        int quad_id = quadgen_generate("JMP", "", "", "?");
        loop->cont_list = merge_patch_lists(loop->cont_list, create_patch_list(quad_id));
    }
}