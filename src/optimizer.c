/*
 * optimizer.c - Code optimization for the PHYLOG compiler
 */

 #include "../include/optimizer.h"
 #include "../include/symboltable.h"
 #include <ctype.h>
 
 // Optimization statistics
 static OptimizationStats opt_stats;
 
 // Initialize the optimizer
 void optimizer_init() {
     memset(&opt_stats, 0, sizeof(opt_stats));
 }
 
 // Check if a string is a temporary variable name
 static int is_temporary(const char* var) {
     return var && var[0] == 'T' && isdigit(var[1]);
 }
 
 // Check if a string is a label
 static int is_label(const char* str) {
     return str && str[0] == 'L' && isdigit(str[1]);
 }
 
 // Check if a string is a branch instruction
 static int is_branch_instruction(const char* op) {
     return op && (
         strcmp(op, "BGE") == 0 || strcmp(op, "BG") == 0 ||
         strcmp(op, "BLE") == 0 || strcmp(op, "BL") == 0 ||
         strcmp(op, "BE") == 0 || strcmp(op, "BNE") == 0 ||
         strcmp(op, "SAUT") == 0
     );
 }
 
 // Perform constant folding on a set of quadruplets
 static void perform_constant_folding(Quadruplet* quads, int quad_count) {
     for (int i = 0; i < quad_count; i++) {
         Quadruplet* q = &quads[i];
         
         // Skip non-arithmetic operations or operations without two arguments
         if ((strcmp(q->op, "+") != 0 && strcmp(q->op, "-") != 0 && 
              strcmp(q->op, "*") != 0 && strcmp(q->op, "/") != 0 && 
              strcmp(q->op, "%") != 0) || 
             q->arg2[0] == '\0' || q->result[0] == '\0') {
             continue;
         }
         
         // Check if both operands are constants
         if (is_integer_literal(q->arg1) && is_integer_literal(q->arg2)) {
             int val1 = string_to_int(q->arg1);
             int val2 = string_to_int(q->arg2);
             int result = 0;
             
             // Compute the result
             if (strcmp(q->op, "+") == 0) {
                 result = val1 + val2;
             } else if (strcmp(q->op, "-") == 0) {
                 result = val1 - val2;
             } else if (strcmp(q->op, "*") == 0) {
                 result = val1 * val2;
             } else if (strcmp(q->op, "/") == 0) {
                 if (val2 == 0) continue;  // Skip division by zero
                 result = val1 / val2;
             } else if (strcmp(q->op, "%") == 0) {
                 if (val2 == 0) continue;  // Skip modulo by zero
                 result = val1 % val2;
             }
             
             // Replace with assignment of constant
             strcpy(q->op, ":=");
             sprintf(q->arg1, "%d", result);
             q->arg2[0] = '\0';
             
             opt_stats.constants_folded++;
         } else if (is_float_literal(q->arg1) && is_float_literal(q->arg2)) {
             float val1 = string_to_float(q->arg1);
             float val2 = string_to_float(q->arg2);
             float result = 0.0f;
             
             // Compute the result
             if (strcmp(q->op, "+") == 0) {
                 result = val1 + val2;
             } else if (strcmp(q->op, "-") == 0) {
                 result = val1 - val2;
             } else if (strcmp(q->op, "*") == 0) {
                 result = val1 * val2;
             } else if (strcmp(q->op, "/") == 0) {
                 if (val2 == 0.0f) continue;  // Skip division by zero
                 result = val1 / val2;
             }
             
             // Replace with assignment of constant
             strcpy(q->op, ":=");
             sprintf(q->arg1, "%f", result);
             q->arg2[0] = '\0';
             
             opt_stats.constants_folded++;
         }
     }
 }
 
 // Perform copy propagation on a set of quadruplets
 static void perform_copy_propagation(Quadruplet* quads, int quad_count) {
     int modified;
     
     do {
         modified = 0;
         
         // For each copy operation
         for (int i = 0; i < quad_count; i++) {
             Quadruplet* q = &quads[i];
             
             // Skip branch instructions
             if (is_branch_instruction(q->op)) {
                 continue;
             }
             
             // Look for copy operations (a = b)
             if (strcmp(q->op, ":=") == 0 && q->arg2[0] == '\0' && 
                 !is_integer_literal(q->arg1) && !is_float_literal(q->arg1) &&
                 q->arg1[0] != '\0' && q->result[0] != '\0') {
                 
                 const char* src = q->arg1;
                 const char* dest = q->result;
                 
                 // Skip if either is a label
                 if (is_label(src) || is_label(dest)) {
                     continue;
                 }
                 
                 // Avoid propagating if source or dest is used in array operations
                 int is_array_op = 0;
                 for (int j = 0; j < quad_count; j++) {
                     if (strstr(quads[j].result, "[") != NULL) {
                         char array_name[64] = {0};
                         sscanf(quads[j].result, "%[^[]", array_name);
                         
                         if (strcmp(array_name, src) == 0 || strcmp(array_name, dest) == 0) {
                             is_array_op = 1;
                             break;
                         }
                     }
                 }
                 
                 if (is_array_op) continue;
                 
                 // Propagate copy for subsequent uses of dest
                 for (int j = i + 1; j < quad_count; j++) {
                     Quadruplet* qj = &quads[j];
                     
                     // Stop at redefinition of source or dest
                     if (strcmp(qj->result, src) == 0 || strcmp(qj->result, dest) == 0) {
                         break;
                     }
                     
                     // Skip branch instructions for propagation
                     if (is_branch_instruction(qj->op)) {
                         continue;
                     }
                     
                     // Replace dest with src in arg1
                     if (strcmp(qj->arg1, dest) == 0) {
                         strcpy(qj->arg1, src);
                         modified = 1;
                         opt_stats.copies_propagated++;
                     }
                     
                     // Replace dest with src in arg2
                     if (qj->arg2[0] != '\0' && strcmp(qj->arg2, dest) == 0) {
                         strcpy(qj->arg2, src);
                         modified = 1;
                         opt_stats.copies_propagated++;
                     }
                 }
             }
         }
     } while (modified);
 }
 
 // Perform constant propagation on a set of quadruplets
 static void perform_constant_propagation(Quadruplet* quads, int quad_count) {
     int modified;
     
     do {
         modified = 0;
         
         // For each constant assignment
         for (int i = 0; i < quad_count; i++) {
             Quadruplet* q = &quads[i];
             
             // Skip branch instructions
             if (is_branch_instruction(q->op)) {
                 continue;
             }
             
             // Look for constant assignments (a = const)
             if (strcmp(q->op, ":=") == 0 && q->arg2[0] == '\0' && 
                 (is_integer_literal(q->arg1) || is_float_literal(q->arg1)) &&
                 q->result[0] != '\0') {
                 
                 const char* constant = q->arg1;
                 const char* dest = q->result;
                 
                 // Propagate constant for subsequent uses of dest
                 for (int j = i + 1; j < quad_count; j++) {
                     Quadruplet* qj = &quads[j];
                     
                     // Stop at redefinition of dest
                     if (strcmp(qj->result, dest) == 0) {
                         break;
                     }
                     
                     // Skip branch instructions in propagation
                     if (is_branch_instruction(qj->op)) {
                         // We CAN propagate constants in branch operands
                         if (strcmp(qj->arg2, dest) == 0) {
                             strcpy(qj->arg2, constant);
                             modified = 1;
                             opt_stats.constants_propagated++;
                         }
                         if (qj->result[0] != '\0' && strcmp(qj->result, dest) == 0) {
                             strcpy(qj->result, constant);
                             modified = 1;
                             opt_stats.constants_propagated++;
                         }
                         continue;
                     }
                     
                     // Replace dest with constant in arg1
                     if (strcmp(qj->arg1, dest) == 0) {
                         strcpy(qj->arg1, constant);
                         modified = 1;
                         opt_stats.constants_propagated++;
                     }
                     
                     // Replace dest with constant in arg2
                     if (qj->arg2[0] != '\0' && strcmp(qj->arg2, dest) == 0) {
                         strcpy(qj->arg2, constant);
                         modified = 1;
                         opt_stats.constants_propagated++;
                     }
                 }
             }
         }
     } while (modified);
 }
 
 // Perform common subexpression elimination on a set of quadruplets
 static void perform_common_subexpr_elimination(Quadruplet* quads, int quad_count) {
     for (int i = 0; i < quad_count; i++) {
         Quadruplet* qi = &quads[i];
         
         // Skip non-arithmetic operations or operations without two arguments
         if ((strcmp(qi->op, "+") != 0 && strcmp(qi->op, "-") != 0 && 
              strcmp(qi->op, "*") != 0 && strcmp(qi->op, "/") != 0 && 
              strcmp(qi->op, "%") != 0) || 
             qi->arg2[0] == '\0' || qi->result[0] == '\0' ||
             is_branch_instruction(qi->op)) {
             continue;
         }
         
         // Look for later equivalent expressions
         for (int j = i + 1; j < quad_count; j++) {
             Quadruplet* qj = &quads[j];
             
             // Skip branch instructions
             if (is_branch_instruction(qj->op)) {
                 continue;
             }
             
             // Skip non-arithmetic operations or operations without two arguments
             if ((strcmp(qj->op, "+") != 0 && strcmp(qj->op, "-") != 0 && 
                  strcmp(qj->op, "*") != 0 && strcmp(qj->op, "/") != 0 && 
                  strcmp(qj->op, "%") != 0) || 
                 qj->arg2[0] == '\0' || qj->result[0] == '\0') {
                 continue;
             }
             
             // Check if expressions are equivalent
             int equivalent = 0;
             
             // Direct match
             if (strcmp(qi->op, qj->op) == 0 && 
                 strcmp(qi->arg1, qj->arg1) == 0 && 
                 strcmp(qi->arg2, qj->arg2) == 0) {
                 equivalent = 1;
             }
             
             // Commutative operations (+ and *) can have swapped arguments
             if (!equivalent && (strcmp(qi->op, "+") == 0 || strcmp(qi->op, "*") == 0) &&
                 strcmp(qi->op, qj->op) == 0 && 
                 strcmp(qi->arg1, qj->arg2) == 0 && 
                 strcmp(qi->arg2, qj->arg1) == 0) {
                 equivalent = 1;
             }
             
             // If expressions are equivalent, replace the computation
             if (equivalent) {
                 // Check if any operand has been redefined
                 int redefined = 0;
                 for (int k = i + 1; k < j; k++) {
                     Quadruplet* qk = &quads[k];
                     if (strcmp(qk->result, qi->arg1) == 0 || 
                         strcmp(qk->result, qi->arg2) == 0) {
                         redefined = 1;
                         break;
                     }
                 }
                 
                 if (!redefined) {
                     // Replace the computation with a copy
                     strcpy(qj->op, ":=");
                     strcpy(qj->arg1, qi->result);
                     qj->arg2[0] = '\0';
                     
                     opt_stats.common_subexprs_eliminated++;
                 }
             }
         }
     }
 }
 
 // Perform algebraic simplifications on a set of quadruplets
 static void perform_algebraic_simplifications(Quadruplet* quads, int quad_count) {
     for (int i = 0; i < quad_count; i++) {
         Quadruplet* q = &quads[i];
         
         // Skip branch instructions
         if (is_branch_instruction(q->op)) {
             continue;
         }
         
         // 1. Addition with 0: x + 0 = x, 0 + x = x
         if (strcmp(q->op, "+") == 0) {
             if (strcmp(q->arg1, "0") == 0) {
                 strcpy(q->op, ":=");
                 strcpy(q->arg1, q->arg2);
                 q->arg2[0] = '\0';
                 opt_stats.algebraic_simplifications++;
             } else if (strcmp(q->arg2, "0") == 0) {
                 strcpy(q->op, ":=");
                 q->arg2[0] = '\0';
                 opt_stats.algebraic_simplifications++;
             }
         }
         
         // 2. Subtraction with 0: x - 0 = x
         else if (strcmp(q->op, "-") == 0 && strcmp(q->arg2, "0") == 0) {
             strcpy(q->op, ":=");
             q->arg2[0] = '\0';
             opt_stats.algebraic_simplifications++;
         }
         
         // 3. Multiplication with 0: x * 0 = 0, 0 * x = 0
         else if (strcmp(q->op, "*") == 0) {
             if (strcmp(q->arg1, "0") == 0 || strcmp(q->arg2, "0") == 0) {
                 strcpy(q->op, ":=");
                 strcpy(q->arg1, "0");
                 q->arg2[0] = '\0';
                 opt_stats.algebraic_simplifications++;
             }
         }
         
         // 4. Multiplication with 1: x * 1 = x, 1 * x = x
         else if (strcmp(q->op, "*") == 0) {
             if (strcmp(q->arg1, "1") == 0) {
                 strcpy(q->op, ":=");
                 strcpy(q->arg1, q->arg2);
                 q->arg2[0] = '\0';
                 opt_stats.algebraic_simplifications++;
             } else if (strcmp(q->arg2, "1") == 0) {
                 strcpy(q->op, ":=");
                 q->arg2[0] = '\0';
                 opt_stats.algebraic_simplifications++;
             }
         }
         
         // 5. Division by 1: x / 1 = x
         else if (strcmp(q->op, "/") == 0 && strcmp(q->arg2, "1") == 0) {
             strcpy(q->op, ":=");
             q->arg2[0] = '\0';
             opt_stats.algebraic_simplifications++;
         }
         
         // 6. Division of 0: 0 / x = 0 (for x != 0)
         else if (strcmp(q->op, "/") == 0 && strcmp(q->arg1, "0") == 0) {
             strcpy(q->op, ":=");
             q->arg2[0] = '\0';
             opt_stats.algebraic_simplifications++;
         }
         
         // 7. Modulo of 0: 0 % x = 0 (for x != 0)
         else if (strcmp(q->op, "%") == 0 && strcmp(q->arg1, "0") == 0) {
             strcpy(q->op, ":=");
             q->arg2[0] = '\0';
             opt_stats.algebraic_simplifications++;
         }
     }
 }
 
// Helper function to check if a string contains array access notation [...]
static int is_array_access(const char* str) {
    return str && strchr(str, '[') != NULL && strchr(str, ']') != NULL;
}

// Helper function to extract variable name from array access (e.g., "NUMBERS[T9]" -> "T9")
static void extract_array_index_var(const char* array_access, char* index_var, int max_size) {
    char* open_bracket = strchr(array_access, '[');
    char* close_bracket = strchr(array_access, ']');
    
    if (open_bracket && close_bracket && (close_bracket > open_bracket)) {
        int len = close_bracket - open_bracket - 1;
        if (len < max_size) {
            strncpy(index_var, open_bracket + 1, len);
            index_var[len] = '\0';
        }
    }
}

// Helper function to add a variable to the used_vars array if it's not already there
static int add_to_used_vars(char used_vars[][32], int* used_count, const char* var) {
    if (!var || var[0] == '\0' || is_label(var) || is_integer_literal(var) || is_float_literal(var)) {
        return 0;
    }
    
    // Check if already in the list
    for (int i = 0; i < *used_count; i++) {
        if (strcmp(used_vars[i], var) == 0) {
            return 0;  // Already marked as used
        }
    }
    
    // Add to list if there's room
    if (*used_count < 1000) {
        strcpy(used_vars[*used_count], var);
        (*used_count)++;
        return 1;  // Added successfully
    }
    
    return 0;  // Couldn't add (list full)
}

// Perform dead code elimination on a set of quadruplets
static void perform_dead_code_elimination(Quadruplet* quads, int quad_count) {
    // Mark variables as used
    char used_vars[1000][32] = {{0}};
    int used_count = 0;
    
    // PHASE 1: Mark array access operations for special handling
    char array_indices[1000][32] = {{0}};
    int array_index_count = 0;
    
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        
        // Skip NOP instructions
        if (strcmp(q->op, "NOP") == 0) {
            continue;
        }
        
        // Look for array accesses in result field
        if (is_array_access(q->result)) {
            char index_var[32] = {0};
            extract_array_index_var(q->result, index_var, sizeof(index_var));
            
            if (index_var[0] != '\0') {
                add_to_used_vars(array_indices, &array_index_count, index_var);
            }
        }
        
        // Also look for array accesses in arg1 and arg2
        if (is_array_access(q->arg1)) {
            char index_var[32] = {0};
            extract_array_index_var(q->arg1, index_var, sizeof(index_var));
            
            if (index_var[0] != '\0') {
                add_to_used_vars(array_indices, &array_index_count, index_var);
            }
        }
        
        if (is_array_access(q->arg2)) {
            char index_var[32] = {0};
            extract_array_index_var(q->arg2, index_var, sizeof(index_var));
            
            if (index_var[0] != '\0') {
                add_to_used_vars(array_indices, &array_index_count, index_var);
            }
        }
    }
    
    // PHASE 2: Mark variables used in output and branch operations
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        
        // Skip NOP instructions
        if (strcmp(q->op, "NOP") == 0) {
            continue;
        }
        
        // Output operations and control flow
        if (strcmp(q->op, "AFFICHER") == 0 || strcmp(q->op, "LIRE") == 0 || 
            is_branch_instruction(q->op) || strcmp(q->op, "ETIQ") == 0) {
            
            // Mark arguments as used
            add_to_used_vars(used_vars, &used_count, q->arg1);
            add_to_used_vars(used_vars, &used_count, q->arg2);
            
            // For branch instructions, check result field
            if (is_branch_instruction(q->op)) {
                add_to_used_vars(used_vars, &used_count, q->result);
            }
            
            // If any of the fields contain array accesses, mark the index variable as used
            if (is_array_access(q->arg1)) {
                char index_var[32] = {0};
                extract_array_index_var(q->arg1, index_var, sizeof(index_var));
                add_to_used_vars(used_vars, &used_count, index_var);
            }
            
            if (is_array_access(q->arg2)) {
                char index_var[32] = {0};
                extract_array_index_var(q->arg2, index_var, sizeof(index_var));
                add_to_used_vars(used_vars, &used_count, index_var);
            }
            
            if (is_array_access(q->result)) {
                char index_var[32] = {0};
                extract_array_index_var(q->result, index_var, sizeof(index_var));
                add_to_used_vars(used_vars, &used_count, index_var);
            }
        }
    }
    
    // PHASE 3: Mark all array index variables as used
    for (int i = 0; i < array_index_count; i++) {
        add_to_used_vars(used_vars, &used_count, array_indices[i]);
    }
    
    // PHASE 4: Backward pass to find all variables needed for used variables
    int modified;
    do {
        modified = 0;
        
        for (int i = quad_count - 1; i >= 0; i--) {
            Quadruplet* q = &quads[i];
            
            // Skip NOP, ETIQ, BOUNDS, and ADEC instructions
            if (strcmp(q->op, "NOP") == 0 || strcmp(q->op, "ETIQ") == 0 ||
                strcmp(q->op, "BOUNDS") == 0 || strcmp(q->op, "ADEC") == 0) {
                continue;
            }
            
            // Check if result is used directly or in array access
            int result_used = 0;
            
            // Direct usage
            for (int j = 0; j < used_count; j++) {
                if (strcmp(used_vars[j], q->result) == 0) {
                    result_used = 1;
                    break;
                }
            }
            
            // If result is used, mark operands as used
            if (result_used) {
                // Mark args as used if not constants or labels
                if (add_to_used_vars(used_vars, &used_count, q->arg1)) {
                    modified = 1;
                }
                
                if (add_to_used_vars(used_vars, &used_count, q->arg2)) {
                    modified = 1;
                }
                
                // Check if args contain array accesses
                if (is_array_access(q->arg1)) {
                    char index_var[32] = {0};
                    extract_array_index_var(q->arg1, index_var, sizeof(index_var));
                    if (add_to_used_vars(used_vars, &used_count, index_var)) {
                        modified = 1;
                    }
                }
                
                if (is_array_access(q->arg2)) {
                    char index_var[32] = {0};
                    extract_array_index_var(q->arg2, index_var, sizeof(index_var));
                    if (add_to_used_vars(used_vars, &used_count, index_var)) {
                        modified = 1;
                    }
                }
            }
            
            // Special handling for array stores (e.g., NUMBERS[T8] = T7)
            if (is_array_access(q->result)) {
                char index_var[32] = {0};
                extract_array_index_var(q->result, index_var, sizeof(index_var));
                
                // If the array itself is used, then the store operation and index are used
                char array_name[32] = {0};
                sscanf(q->result, "%[^[]", array_name);
                
                for (int j = 0; j < used_count; j++) {
                    if (strcmp(used_vars[j], array_name) == 0) {
                        // This array is used somewhere, so mark both the array index and value as used
                        if (add_to_used_vars(used_vars, &used_count, index_var)) {
                            modified = 1;
                        }
                        if (add_to_used_vars(used_vars, &used_count, q->arg1)) {
                            modified = 1;
                        }
                        // Also mark the result itself as used
                        if (add_to_used_vars(used_vars, &used_count, q->result)) {
                            modified = 1;
                        }
                        break;
                    }
                }
            }
        }
    } while (modified);
    
    // PHASE 5: Mark unused assignments as NOP
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        
        // Skip instructions that don't produce a result, are already NOPs, or are I/O operations
        if (q->result[0] == '\0' || strcmp(q->op, "NOP") == 0 || 
            strcmp(q->op, "AFFICHER") == 0 || strcmp(q->op, "LIRE") == 0 ||
            is_branch_instruction(q->op) || strcmp(q->op, "ETIQ") == 0 ||
            strcmp(q->op, "BOUNDS") == 0 || strcmp(q->op, "ADEC") == 0) {
            continue;
        }
        
        // If the result is an array access, check if the array itself is used
        if (is_array_access(q->result)) {
            char array_name[32] = {0};
            sscanf(q->result, "%[^[]", array_name);
            
            // If the array is used somewhere, we can't eliminate this store
            for (int j = 0; j < used_count; j++) {
                if (strcmp(used_vars[j], array_name) == 0) {
                    // This is a store to a used array, so keep it
                    continue;
                }
            }
        }
        
        // Check if result is unused
        int result_used = 0;
        for (int j = 0; j < used_count; j++) {
            if (strcmp(used_vars[j], q->result) == 0) {
                result_used = 1;
                break;
            }
        }
        
        // If result is unused, mark instruction as NOP
        if (!result_used) {
            strcpy(q->op, "NOP");
            opt_stats.dead_code_eliminated++;
        }
    }
    
    // Debug output
    printf("Dead code elimination found %d unused instructions.\n", opt_stats.dead_code_eliminated);
    printf("Variables marked as used: %d\n", used_count);
}


// Perform code compaction
// A more direct compaction function
static void perform_code_compaction(Quadruplet* quads, int *quad_count_ptr) {
    int original_count = *quad_count_ptr;
    
    // Print the first 10 quadruples to debug
    //printf("DEBUG: First 10 operators before compaction:\n");
    //for (int i = 0; i < 40 && i < original_count; i++) {
     //   printf("  [%d] op='%s'\n", i, quads[i].op);
    //}
    
    // Count NOPs first
    int nop_count = 0;
    for (int i = 0; i < original_count; i++) {
        if (strcmp(quads[i].op, "NOP") == 0) {
            nop_count++;
        }
    }
    
    //printf("DEBUG: Found %d NOPs out of %d quadruples\n", nop_count, original_count);
    
    // If no NOPs found, nothing to do
    if (nop_count == 0) {
        printf("No NOPs found, no compaction needed.\n");
        return;
    }
    
    int write_idx = 0;
    
    // Compact the array
    for (int read_idx = 0; read_idx < original_count; read_idx++) {
        if (strcmp(quads[read_idx].op, "NOP") != 0) {
            // Only copy if positions differ
            if (write_idx != read_idx) {
                // Copy the quadruple
                memcpy(&quads[write_idx], &quads[read_idx], sizeof(Quadruplet));
            }
            // Update the ID
            quads[write_idx].id = write_idx;
            write_idx++;
        }
    }
    
    printf("Code compaction removed %d NOPs. Quadruple count reduced from %d to %d.\n", 
           nop_count, original_count, write_idx);
    
    // Update the count
    *quad_count_ptr = write_idx;
    
    // Update statistics
    opt_stats.nops_eliminated = nop_count;
}


// Simple unused variable cleanup based on symbol table contents
// Updated unused variable cleanup function that works on final IR
static void perform_unused_variable_cleanup(Quadruplet* quads, int quad_count) {
    printf("Checking for unused variables in final IR...\n");
    
    // Get all symbols from the symbol table
    int symbol_count = symboltable_get_count();
    
    // First collect all variables used in the final IR
    char used_vars[1000][32] = {{0}};
    int used_count = 0;
    
    // Helper function to add to used list
    void add_to_used(const char* var) {
        if (!var || var[0] == '\0' || isdigit(var[0]) || var[0] == '-' || 
            (var[0] == 'L' && isdigit(var[1])) || strcmp(var, "$") == 0) {
            return;  // Skip literals, labels, etc.
        }
        
        // Check if already in list
        for (int i = 0; i < used_count; i++) {
            if (strcmp(used_vars[i], var) == 0) {
                return;  // Already in list
            }
        }
        
        // Add to list
        if (used_count < 1000) {
            strcpy(used_vars[used_count], var);
            used_count++;
        }
    }
    
    // Helper for array notation
    void check_array_notation(const char* expr) {
        if (!expr || !strchr(expr, '[')) return;
        
        char name[32] = {0};
        char index[32] = {0};
        
        sscanf(expr, "%[^[]", name);
        add_to_used(name);
        
        if (sscanf(expr, "%*[^[][%[^]]", index) == 1) {
            add_to_used(index);
        }
    }
    
    // Scan the FINAL IR for all variable references
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        
        // Handle array references
        if (strchr(q->arg1, '[')) check_array_notation(q->arg1);
        if (strchr(q->arg2, '[')) check_array_notation(q->arg2);
        if (strchr(q->result, '[')) check_array_notation(q->result);
        
        // Handle direct references
        if (strcmp(q->op, "AFFICHER") == 0 || strcmp(q->op, "LIRE") == 0) {
            // Skip arg1 for I/O operations
            add_to_used(q->arg2);
            add_to_used(q->result);
        } else {
            add_to_used(q->arg1);
            add_to_used(q->arg2);
            add_to_used(q->result);
        }
    }
    
    printf("Found %d variables used in final IR\n", used_count);
    
    // Now check each symbol to see if it's used
    int removed_count = 0;
    for (int i = 0; i < symbol_count; i++) {
        SymbolNode* symbol = symboltable_get_by_index(i);
        if (!symbol) continue;
        
        // Skip certain types (e.g., constants might not be directly referenced)
        if (symbol->category != CATEGORY_VARIABLE && 
            symbol->category != CATEGORY_TEMP) {
            continue;
        }
        
        // Check if this symbol is used in final IR
        int is_used = 0;
        for (int j = 0; j < used_count; j++) {
            if (strcmp(symbol->name, used_vars[j]) == 0) {
                is_used = 1;
                break;
            }
        }
        
        // If not used in final IR, remove it
        if (!is_used) {
            printf("  Removing unused symbol '%s'\n", symbol->name);
            if (symboltable_remove(symbol->name)) {
                removed_count++;
                i--;  // Adjust index since we removed an element
                symbol_count--;
            }
        }
    }
    
    printf("Removed %d unused symbols from symbol table\n", removed_count);
}

// Run a specific optimization pass
int optimizer_run_pass(OptimizationPass pass, const char* input_file, const char* output_file) {
    // Read quadruplets from input file
    if (!quadgen_read_from_file(input_file)) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Failed to read quadruplets from file '%s'", input_file);
        return 0;
    }
    
    // Get quadruplets array and count
    int quad_count = quadgen_get_count();
    Quadruplet* quads = (Quadruplet*) safe_malloc(quad_count * sizeof(Quadruplet));
    
    // Copy quadruplets to our local array
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = quadgen_get(i);
        if (q) {
            memcpy(&quads[i], q, sizeof(Quadruplet));
        }
    }
    
    // Apply the selected optimization pass
    switch (pass) {
        case OPT_PASS_COPY_PROPAGATION:
            printf("Running copy propagation pass...\n");
            perform_copy_propagation(quads, quad_count);
            break;
        
        case OPT_PASS_CONSTANT_FOLDING:
            printf("Running constant folding pass...\n");
            perform_constant_folding(quads, quad_count);
            break;
        
        case OPT_PASS_CONSTANT_PROPAGATION:
            printf("Running constant propagation pass...\n");
            perform_constant_propagation(quads, quad_count);
            break;
        
        case OPT_PASS_COMMON_SUBEXPR_ELIMINATION:
            printf("Running common subexpression elimination pass...\n");
            perform_common_subexpr_elimination(quads, quad_count);
            break;
        
        case OPT_PASS_DEAD_CODE_ELIMINATION:
            printf("Running dead code elimination pass...\n");
            perform_dead_code_elimination(quads, quad_count);
            break;
        
        case OPT_PASS_ALGEBRAIC_SIMPLIFICATION:
            printf("Running algebraic simplification pass...\n");
            perform_algebraic_simplifications(quads, quad_count);
            break;

        case OPT_PASS_CODE_COMPACTION:
            printf("Running code compaction pass...\n");
            perform_code_compaction(quads, &quad_count);
            break;
            
        case OPT_PASS_UNUSED_VAR_CLEANUP:
            printf("Running unused variable cleanup pass...\n");
            perform_unused_variable_cleanup(quads, quad_count);
            break;
    }
    
    // If we did code compaction, use a different approach to update the quadgen module
    if (pass == OPT_PASS_CODE_COMPACTION) {
        // Clear all existing quadruples in quadgen
        quadgen_cleanup();  // This resets quad_count to 0
        
        // Re-add only the non-NOP quadruples
        for (int i = 0; i < quad_count; i++) {
            quadgen_generate(quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
        }
    } else {
        // For other passes, just update each quadruple
        for (int i = 0; i < quad_count; i++) {
            quadgen_update(i, quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
        }
    }
    
    // Write optimized quadruplets to output file
    quadgen_write_to_file(output_file);
    
    // Clean up
    free(quads);
    return 1;
}
 
 // Run all optimization passes in sequence
 int optimizer_run_all_passes(const char* input_file, const char* output_file) {
    // Read quadruplets directly from input file
    if (!quadgen_read_from_file(input_file)) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Failed to read quadruplets from file '%s'", input_file);
        return 0;
    }
    
    // Get quadruplets array and count
    int quad_count = quadgen_get_count();
    Quadruplet* quads = (Quadruplet*) safe_malloc(quad_count * sizeof(Quadruplet));
    
    // Copy quadruplets to our local array
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = quadgen_get(i);
        if (q) {
            memcpy(&quads[i], q, sizeof(Quadruplet));
        }
    }
    
    // Run all optimization passes in memory
    printf("Running optimization pass 1/8: Copy Propagation\n");
    perform_copy_propagation(quads, quad_count);
    
    printf("Running optimization pass 2/8: Constant Folding\n");
    perform_constant_folding(quads, quad_count);
    
    printf("Running optimization pass 3/8: Constant Propagation\n");
    perform_constant_propagation(quads, quad_count);
    
    printf("Running optimization pass 4/8: Common Subexpression Elimination\n");
    perform_common_subexpr_elimination(quads, quad_count);
    
    printf("Running optimization pass 5/8: Algebraic Simplification\n");
    perform_algebraic_simplifications(quads, quad_count);
    
    printf("Running optimization pass 6/8: Dead Code Elimination\n");
    perform_dead_code_elimination(quads, quad_count);
    
    printf("Running optimization pass 7/8: Code Compaction\n");
    perform_code_compaction(quads, &quad_count);
    
    printf("Running optimization pass 8/8: Unused Variable Cleanup\n");
    perform_unused_variable_cleanup(quads, quad_count);
    
    // Clear quadgen and regenerate the compacted code
    quadgen_cleanup();
    
    // Add back only the non-NOP quadruples
    for (int i = 0; i < quad_count; i++) {
        quadgen_generate(quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }
    
    // Write the final optimized code
    quadgen_write_to_file(output_file);
    
    // Clean up
    free(quads);
    return 1;
}

 // Get optimization statistics
 OptimizationStats optimizer_get_stats() {
     return opt_stats;
 }
 
 // Display information about optimizations
 void optimizer_display_stats() {
     printf("\n********** OPTIMIZATION STATISTICS **********\n");
     printf("Copies propagated:                %d\n", opt_stats.copies_propagated);
     printf("Constants folded:                 %d\n", opt_stats.constants_folded);
     printf("Constants propagated:             %d\n", opt_stats.constants_propagated);
     printf("Common subexpressions eliminated: %d\n", opt_stats.common_subexprs_eliminated);
     printf("Dead code eliminated:             %d\n", opt_stats.dead_code_eliminated);
     printf("Algebraic simplifications:        %d\n", opt_stats.algebraic_simplifications);
     printf("NOPs eliminated (compacted):      %d\n", opt_stats.nops_eliminated);
     printf("Total optimizations:              %d\n", 
            opt_stats.copies_propagated + 
            opt_stats.constants_folded + 
            opt_stats.constants_propagated + 
            opt_stats.common_subexprs_eliminated +
            opt_stats.dead_code_eliminated +
            opt_stats.algebraic_simplifications +
            opt_stats.nops_eliminated);//
     printf("*******************************************\n\n");
 }
 
 // Clean up the optimizer
 void optimizer_cleanup() {
     // Reset optimization statistics
     memset(&opt_stats, 0, sizeof(opt_stats));
 }