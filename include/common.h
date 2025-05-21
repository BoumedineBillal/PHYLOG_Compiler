/*
 * common.h - Common utilities for the PHYLOG compiler
 */

#ifndef PHYLOG_COMMON_H
#define PHYLOG_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Error severity levels
typedef enum {
    ERROR_LEVEL_INFO,    // Informational message
    ERROR_LEVEL_WARNING, // Warning (compilation continues)
    ERROR_LEVEL_ERROR,   // Error (compilation may stop)
    ERROR_LEVEL_FATAL    // Fatal error (compilation stops)
} ErrorLevel;

// Report an error with file location
void report_error(ErrorLevel level, int line, int column, const char* format, ...);

// Safe memory allocation wrapper
void* safe_malloc(size_t size);

// Safe memory reallocation wrapper
void* safe_realloc(void* ptr, size_t size);

// Safe string duplication with error checking
char* safe_strdup(const char* str);

// Check if a string is a numeric literal
int is_integer_literal(const char* str);
int is_float_literal(const char* str);

// Convert a string literal to its corresponding value
int string_to_int(const char* str);
float string_to_float(const char* str);

#endif // PHYLOG_COMMON_H