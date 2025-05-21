/*
 * parser_defs.h - Shared definitions for parser components
 */

#ifndef PHYLOG_PARSER_DEFS_H
#define PHYLOG_PARSER_DEFS_H

// parser_defs.h
typedef struct {
    char* name;     // Variable or array name
    char* index;    // Index expression (NULL for simple variables)
    int is_array;   // Flag to indicate array access
} AssignmentTarget;

/* FOR loop information structure */
typedef struct ForLoopContext {
    char* var_name;                  // Loop counter variable name
    int cond_label;                  // Label for condition check
    int body_label;                  // Label for loop body
    int end_label;                   // Label for loop end
    struct ForLoopContext* next;     // For stack implementation
} ForLoopContext;

// Patch list for backpatching
typedef struct PatchList {
    int quad_id;
    struct PatchList* next;
} PatchList;

// Structure for holding condition attributes
typedef struct {
    PatchList* true_list;   // List of jumps to patch for true condition
    PatchList* false_list;  // List of jumps to patch for false condition
} ConditionAttrs;

// Structure for holding loop control info
typedef struct {
    int start_label;        // Label at the start of the loop
    int end_label;          // Label at the end of the loop
    PatchList* break_list;  // List of break statements to patch
    PatchList* cont_list;   // List of continue statements to patch
} LoopControl;

// IF statement context structure
typedef struct IfContext {
    ConditionAttrs* condition;
    int true_label;
    int end_label;
    PatchList* end_jumps;
    struct IfContext* next;
} IfContext;


// IF statement handling functions
IfContext* begin_if_statement(ConditionAttrs* condition);
void begin_then_part(void);
void begin_else_part(void);
void end_if_statement(int has_else);

// Function declarations for backpatching
PatchList* create_patch_list(int quad_id);
PatchList* merge_patch_lists(PatchList* list1, PatchList* list2);
void backpatch(PatchList* list, int label);

// Loop control functions
void push_loop(int start_label, int end_label);
LoopControl* pop_loop();
LoopControl* current_loop();

#endif // PHYLOG_PARSER_DEFS_H