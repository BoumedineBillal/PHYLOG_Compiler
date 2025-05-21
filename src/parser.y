/*
 * parser.y - Bison grammar specification for PHYLOG language with external function calls
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/symboltable.h"
#include "../include/quadgen.h"
#include "../include/parser.h"
#include "../include/parser_defs.h"


SymbolType current_declaration_type = TYPE_UNKNOWN;


// Function declarations to avoid warnings
int yylex(void);
int yyerror(const char* s);



%}

/* Semantic value union */
%union {
    char* str;           // String value
    int ival;            // Integer value
    float fval;          // Float value
    char cval;           // Character value
    SymbolType type;     // Data type
    SymbolNode* node;    // Symbol table node
    ConditionAttrs* cond; // Condition attributes
    PatchList* patch;    // Patch list for backpatching
    AssignmentTarget* target; // For left-side variables
}




/* Token declarations */
%token DATA CODE END
%token INTEGER FLOAT CHAR STRING
%token CONST VECTOR
%token READ DISPLAY
%token IF ELSE FOR WHILE BREAK CONTINUE
%token AND OR NOT
%token GE GT LT LE EQ NE
%token PLUS MINUS MULT DIV MOD
%token ASSIGN COLON SEMICOLON
%token LBRACKET RBRACKET COMMA PIPE
%token LPAREN RPAREN AT

%token <str> IDENTIFIER
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token <str> STRING_LITERAL

/* Non-terminal types */
%type <str> program data_section code_section
%type <type> type
%type <str> id_list variable expression term factor
%type <str> declaration var_declaration const_declaration array_declaration
%type <str> statement statement_list assignment
%type <cond> condition
%type <str> io_statement display_statement read_statement
%type <patch> if_statement 
%type <patch> loop_statement for_statement while_statement
%type <patch> jump_statement
%type <target> variable_left

/* Operator precedence and associativity */
%left OR
%left AND
%left EQ NE
%left GT GE LT LE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT
%left LBRACKET RBRACKET


%%

program:
    IDENTIFIER {
        strncpy(program_name, $1, sizeof(program_name) - 1);
        program_name[sizeof(program_name) - 1] = '\0';
    } 
    DATA data_section END 
    CODE code_section END 
    END
    {
        printf("Program '%s' successfully compiled.\n", program_name);
        $$ = "";
    }
    ;

data_section:
    /* empty */ { $$ = ""; }
    | data_section declaration { $$ = ""; }
    ;

declaration:
    var_declaration
    | const_declaration
    | array_declaration
    ;



var_declaration:
    type COLON {
        /* Store the type in a global variable for id_list to use */
        current_declaration_type = $1;
    } id_list SEMICOLON {
        $$ = "";
    }
    ;


type:
    INTEGER { $$ = TYPE_INTEGER; }
    | FLOAT { $$ = TYPE_FLOAT; }
    | CHAR  { $$ = TYPE_CHAR; }
    | STRING { $$ = TYPE_STRING; }
    ;


id_list:
    IDENTIFIER {
        /* Use the global type variable */
        SymbolNode* node = symboltable_insert_variable($1, current_declaration_type, current_line, current_column);
        if (node) {
            $$ = $1;
        } else {
            $$ = "";
        }
    }
    | id_list PIPE IDENTIFIER {
        /* Use the same global type variable */
        SymbolNode* node = symboltable_insert_variable($3, current_declaration_type, current_line, current_column);
        if (node) {
            $$ = $3;
        } else {
            $$ = "";
        }
    }
    ;

const_declaration:
    CONST COLON IDENTIFIER ASSIGN INT_LITERAL SEMICOLON {
        int value = $5;
        symboltable_insert_constant($3, TYPE_INTEGER, &value, current_line, current_column);
        char value_str[32];
        sprintf(value_str, "%d", $5);
        quadgen_generate(":=", value_str, "", $3);
        $$ = "";
    }
    | CONST COLON IDENTIFIER ASSIGN FLOAT_LITERAL SEMICOLON {
        float value = $5;
        symboltable_insert_constant($3, TYPE_FLOAT, &value, current_line, current_column);
        char value_str[32];
        sprintf(value_str, "%f", $5);
        quadgen_generate(":=", value_str, "", $3);
        $$ = "";
    }
    | CONST COLON IDENTIFIER ASSIGN CHAR_LITERAL SEMICOLON {
        char value = $5;
        symboltable_insert_constant($3, TYPE_CHAR, &value, current_line, current_column);
        char value_str[8];
        sprintf(value_str, "'%c'", $5);
        quadgen_generate(":=", value_str, "", $3);
        $$ = "";
    }
    | CONST COLON IDENTIFIER ASSIGN STRING_LITERAL SEMICOLON {
        symboltable_insert_constant($3, TYPE_STRING, $5, current_line, current_column);
        char value_str[256];
        sprintf(value_str, "\"%s\"", $5);
        quadgen_generate(":=", value_str, "", $3);
        $$ = "";
    }
    ;

array_declaration: 
    VECTOR COLON IDENTIFIER LBRACKET INT_LITERAL COMMA INT_LITERAL COLON type RBRACKET SEMICOLON {
        /* Process array declaration */
        int lower_bound = $5;
        int upper_bound = $7;
        int array_type = $9;  
        
        process_array_declaration($3, lower_bound, upper_bound, array_type);
        
        $$ = "";
    }
    ;

code_section:
    /* empty */ { $$ = ""; }
    | statement_list { $$ = $1; }
    ;

statement_list:
    statement { $$ = $1; }
    | statement_list statement { $$ = $1; }
    ;

statement:
    assignment { $$ = $1; }
    | io_statement { $$ = $1; }
    | if_statement { $$ = NULL; }
    | loop_statement { $$ = NULL; }
    | jump_statement { $$ = NULL; }
    | SEMICOLON  { $$ = ""; } /* Empty statement */
    ;

assignment:
    variable_left ASSIGN expression SEMICOLON {
        // $1 is info about left side, $3 is the expression result
        AssignmentTarget* target = (AssignmentTarget*)$1;
        
        if (target->is_array) {
            process_array_assignment(target->name, target->index, $3);
        } else {
            process_simple_assignment(target->name, $3);
        }
        
        free(target->name);
        if (target->index) free(target->index);
        free(target);
        
        $$ = "";
    }
    ;


variable_left:
    IDENTIFIER {
        // Create a structure to hold assignment target info
        AssignmentTarget* target = malloc(sizeof(AssignmentTarget));
        target->name = safe_strdup($1);
        target->index = NULL;
        target->is_array = 0;
        $$ = target;
    }
    | IDENTIFIER LBRACKET expression RBRACKET {
        // Create structure with array info
        AssignmentTarget* target = malloc(sizeof(AssignmentTarget));
        target->name = safe_strdup($1);
        target->index = safe_strdup($3);
        target->is_array = 1;
        $$ = target;
    }
    ;

variable:
    IDENTIFIER {
        $$ = process_variable($1);
    }
    | IDENTIFIER LBRACKET expression RBRACKET {
        $$ = process_array_access($1, $3);
    }
    ;


expression:
    term { $$ = $1; }
    | expression PLUS term {
        SymbolType result_type = check_arithmetic_types("+", $1, $3);
        $$ = gen_expr_quad("+", $1, $3, result_type);
    }
    | expression MINUS term {
        SymbolType result_type = check_arithmetic_types("-", $1, $3);
        $$ = gen_expr_quad("-", $1, $3, result_type);
    }
    ;

term:
    factor { $$ = $1; }
    | term MULT factor {
        SymbolType result_type = check_arithmetic_types("*", $1, $3);
        $$ = gen_expr_quad("*", $1, $3, result_type);
    }
    | term DIV factor {
        SymbolType result_type = check_arithmetic_types("/", $1, $3);
        $$ = gen_expr_quad("/", $1, $3, result_type);
    }
    | term MOD factor {
        // Modulo only works on integers
        SymbolType result_type = check_arithmetic_types("%", $1, $3);
        $$ = gen_expr_quad("%", $1, $3, result_type);
    }
    ;

factor:
    variable { $$ = $1; }
    | INT_LITERAL {
        char temp[32];
        sprintf(temp, "%d", $1);
        $$ = process_literal(temp, TYPE_INTEGER);
    }
    | FLOAT_LITERAL {
        char temp[32];
        sprintf(temp, "%f", $1);
        $$ = process_literal(temp, TYPE_FLOAT);
    }
    | CHAR_LITERAL {
        char temp[4];
        sprintf(temp, "'%c'", $1);
        $$ = process_literal(temp, TYPE_CHAR);
    }
    | STRING_LITERAL {
        char temp[256];
        sprintf(temp, "\"%s\"", $1);
        $$ = process_literal(temp, TYPE_STRING);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

condition:
    expression GT expression {
        $$ = process_condition("GT", $1, $3);
    }
    | expression GE expression {
        $$ = process_condition("GE", $1, $3);
    }
    | expression LT expression {
        $$ = process_condition("LT", $1, $3);
    }
    | expression LE expression {
        $$ = process_condition("LE", $1, $3);
    }
    | expression EQ expression {
        $$ = process_condition("EQ", $1, $3);
    }
    | expression NE expression {
        $$ = process_condition("NE", $1, $3);
    }
    | LPAREN condition RPAREN {
        $$ = $2;
    }
    | condition AND condition {
        $$ = process_logical_operator("AND", $1, $3);
    }
    | condition OR condition {
        $$ = process_logical_operator("OR", $1, $3);
    }
    | NOT condition {
        $$ = process_logical_operator("NOT", $2, NULL);
    }
    ;

if_statement:
    IF LPAREN condition RPAREN {
        begin_if_statement($3);
    }
    COLON {
        begin_then_part();
    }
    statement_list 
    optional_else
    END {
        /* end_if_statement will be called in optional_else */
        $$ = NULL;
    }
    ;

optional_else:
    /* empty */ {
        end_if_statement(0); // No else part
    }
    | ELSE {
        begin_else_part();
    }
    COLON 
    statement_list {
        end_if_statement(1); // Has else part
    }
    ;

loop_statement:
    for_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    ;

for_statement:
    FOR LPAREN IDENTIFIER COLON expression {
        // Initialize counter and set up condition label
        begin_for_loop_init($3, $5);
    }
    COLON condition RPAREN {
        // Generate condition checking code
        begin_for_loop_condition($8);
    }
    statement_list {
        // Generate increment, jump back and end label
        end_for_loop();
    }
    END {
        $$ = NULL;
    }
    ;

while_statement:
    WHILE LPAREN condition RPAREN statement_list END {
        process_while_loop($3);
        $$ = NULL;
    }
    ;

jump_statement:
    BREAK SEMICOLON {
        process_jump_statement("BREAK");
        $$ = NULL;
    }
    | CONTINUE SEMICOLON {
        process_jump_statement("CONTINUE");
        $$ = NULL;
    }
    ;

io_statement:
    display_statement { $$ = $1; }
    | read_statement { $$ = $1; }
    ;

display_statement:
    DISPLAY LPAREN STRING_LITERAL COLON expression RPAREN SEMICOLON {
        process_io_statement("DISPLAY", $3, $5);
        $$ = "";
    }
    ;

read_statement:
    READ LPAREN STRING_LITERAL COLON AT variable RPAREN SEMICOLON {
        process_io_statement("READ", $3, $6);
        $$ = "";
    }
    ;

%%

// Start syntactic analysis
int parser_parse() {
    return yyparse();
}

// Bison error handler
int yyerror(const char* s) {
    parser_syntax_error(s);
    return 0;
}