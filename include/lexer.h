/*
 * lexer.h - Lexical analyzer for the PHYLOG compiler
 */

#ifndef PHYLOG_LEXER_H
#define PHYLOG_LEXER_H

#include "common.h"

// Current line and column for error tracking
extern int current_line;
extern int current_column;

// Token value storage
extern char token_text[256];
extern int token_int_value;
extern float token_float_value;
extern char token_char_value;

// Initialize the lexical analyzer with an input file
int lexer_init(const char* filename);

// Report a lexical error
void lexer_error(const char* message);

// Close the input file and clean up
void lexer_cleanup();

#endif // PHYLOG_LEXER_H