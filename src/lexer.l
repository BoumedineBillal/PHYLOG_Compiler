/*
 * lexer.l - Flex specification for PHYLOG language lexical analyzer
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/symboltable.h"
#include "../include/lexer.h"
#include "../include/parser_defs.h"
#include "parser.tab.h"

// Current line and column for error tracking
int current_line = 1;
int current_column = 1;

// Token value storage
char token_text[256];
int token_int_value;
float token_float_value;
char token_char_value;

// Update location information
static void update_location() {
    int i;
    for (i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            current_line++;
            current_column = 1;
        } else {
            current_column++;
        }
    }
}

// Report a lexical error
void lexer_error(const char* message) {
    report_error(ERROR_LEVEL_ERROR, current_line, current_column, "Lexical error: %s", message);
}
%}

%option noyywrap
%option noinput
%option nounput

/* Patterns for basic elements LETTER      [a-zA-Z] */
DIGIT       [0-9]
IDENTIFIER  [A-Za-z][a-zA-Z0-9_]{0,29}
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+
CHAR        \'[^\']\'
STRING      \"[^\"]*\"
COMMENT     §.*

%%

{COMMENT}           { update_location(); /* Ignore comments */ }

"DATA"              { update_location(); return DATA; }
"CODE"              { update_location(); return CODE; }
"END"               { update_location(); return END; }

"INTEGER"           { update_location(); return INTEGER; }
"FLOAT"             { update_location(); return FLOAT; }
"CHAR"              { update_location(); return CHAR; }
"STRING"            { update_location(); return STRING; }
"CONST"             { update_location(); return CONST; }
"VECTOR"            { update_location(); return VECTOR; }

"READ"              { update_location(); return READ; }
"DISPLAY"           { update_location(); return DISPLAY; }
"IF"                { update_location(); return IF; }
"ELSE"              { update_location(); return ELSE; }
"FOR"               { update_location(); return FOR; }
"WHILE"             { update_location(); return WHILE; }
"BREAK"             { update_location(); return BREAK; }
"CONTINUE"          { update_location(); return CONTINUE; }

".AND."             { update_location(); return AND; }
".OR."              { update_location(); return OR; }
"NOT"               { update_location(); return NOT; }

".GE."              { update_location(); return GE; }
".G."               { update_location(); return GT; }
".L."               { update_location(); return LT; }
".LE."              { update_location(); return LE; }
".EQ."              { update_location(); return EQ; }
".DI."              { update_location(); return NE; }

"+"                 { update_location(); return PLUS; }
"-"                 { update_location(); return MINUS; }
"*"                 { update_location(); return MULT; }
"/"                 { update_location(); return DIV; }
"%"                 { update_location(); return MOD; }
"="                 { update_location(); return ASSIGN; }
":"                 { update_location(); return COLON; }
";"                 { update_location(); return SEMICOLON; }
"["                 { update_location(); return LBRACKET; }
"]"                 { update_location(); return RBRACKET; }
","                 { update_location(); return COMMA; }
"|"                 { update_location(); return PIPE; }
"("                 { update_location(); return LPAREN; }
")"                 { update_location(); return RPAREN; }
"@"                 { update_location(); return AT; }

{IDENTIFIER}        { 
    update_location();
    if (yyleng <= 30) {
        strncpy(token_text, yytext, sizeof(token_text) - 1);
        token_text[sizeof(token_text) - 1] = '\0';
        yylval.str = safe_strdup(token_text);  // Make a COPY
        return IDENTIFIER;
    } else {
        lexer_error("Identifier too long");
    }
}

{INTEGER}           { 
    update_location();
    long val = strtol(yytext, NULL, 10);
    if (val >= -32768 && val <= 32767) {
        token_int_value = (int)val;
        yylval.ival = token_int_value;
        return INT_LITERAL;
    } else {
        lexer_error("Integer value out of range (-32768 to 32767)");
    }
}

{FLOAT}             { 
    update_location();
    token_float_value = atof(yytext);
    yylval.fval = token_float_value;
    return FLOAT_LITERAL;
}

{CHAR}              { 
    update_location();
    token_char_value = yytext[1];
    yylval.cval = token_char_value;
    return CHAR_LITERAL;
}

{STRING} { 
    update_location();
    int len = strlen(yytext) - 2;
    strncpy(token_text, yytext + 1, len);
    token_text[len] = '\0';
    yylval.str = safe_strdup(token_text);  // Make a copy
    return STRING_LITERAL;
}

[ \t]               { update_location(); /* Ignore whitespace */ }
\n                  { update_location(); /* Count line breaks */ }

.                   { 
    char message[100];
    sprintf(message, "Unknown character '%s'", yytext);
    lexer_error(message);
}

%%

// Initialize the lexical analyzer with an input file
int lexer_init(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        report_error(ERROR_LEVEL_FATAL, 0, 0, "Cannot open input file '%s'", filename);
        return 0;
    }
    
    yyin = file;
    current_line = 1;
    current_column = 1;
    return 1;
}

// Close the input file and clean up
void lexer_cleanup() {
    if (yyin) {
        fclose(yyin);
        yyin = NULL;
    }
}