/*
 * parser.h - Syntactic and semantic analyzer for the PHYLOG compiler
 */

#ifndef PHYLOG_PARSER_H
#define PHYLOG_PARSER_H

#include "common.h"
#include "symboltable.h"
#include "quadgen.h"
#include "parser_defs.h"

// Program name storage
extern char program_name[32];

/* Only declare the variable - NO initialization */
extern SymbolType current_declaration_type;

// Global if-statement context stack
extern IfContext* if_context_stack;

// Start syntactic analysis of the input file
int parser_parse();

// Report a syntax error
void parser_syntax_error(const char* message);

// Report a semantic error
void parser_semantic_error(const char* message);

// Helper function to convert string type to enum
SymbolType get_type_from_string(const char* type_str);

// Helper functions for expressions
char* process_literal(const char* literal_str, SymbolType type);
char* gen_expr_quad(const char* op, const char* arg1, const char* arg2, SymbolType result_type);

// Helper functions for condition processing
ConditionAttrs* process_condition(const char* op, const char* arg1, const char* arg2);
ConditionAttrs* process_logical_operator(const char* op, ConditionAttrs* cond1, ConditionAttrs* cond2);

// Variable and array management
char* process_variable(const char* name);
char* process_array_access(const char* array_name, const char* index);

// Statement processing
void process_assignment(const char* var_name, const char* expr);
IfContext* begin_if_statement(ConditionAttrs* condition);
void begin_then_part(void);
void begin_else_part(void);
void end_if_statement(int has_else);
void begin_for_loop_init(const char* var_name, const char* initial_value);
void begin_for_loop_condition(ConditionAttrs* condition);
void end_for_loop();
void process_while_loop(ConditionAttrs* condition);
void process_jump_statement(const char* jump_type);
void process_io_statement(const char* io_type, const char* format, const char* target);
void process_array_declaration(const char* array_name, int lower_bound, int upper_bound, SymbolType type);

// Helper function for type checking
SymbolType check_arithmetic_types(const char* op, const char* arg1, const char* arg2);

// Evaluation helper functions
int eval_int_expr(const char* expr);
float eval_float_expr(const char* expr);

// Helper function to determine the type of an expression
SymbolType get_expression_type(const char* expr);

// Function to handle simple variable assignment
void process_simple_assignment(const char* var_name, const char* expr_value);

// Function to handle array element assignment
void process_array_assignment(const char* array_name, const char* index, const char* expr_value);

// Utility function to get type name as string (for debugging)
const char* get_type_name(SymbolType type);

#endif // PHYLOG_PARSER_H