/*
 * common.c - Common utilities implementation for the PHYLOG compiler
 */

 #include "../include/common.h"

 // Report an error with file location
 void report_error(ErrorLevel level, int line, int column, const char* format, ...) {
     va_list args;
     const char* level_str;
     
     // Select appropriate level string
     switch (level) {
         case ERROR_LEVEL_INFO:
             level_str = "Info";
             break;
         case ERROR_LEVEL_WARNING:
             level_str = "Warning";
             break;
         case ERROR_LEVEL_ERROR:
             level_str = "Error";
             break;
         case ERROR_LEVEL_FATAL:
             level_str = "Fatal Error";
             break;
         default:
             level_str = "Unknown";
     }
     
     // Print location information
     if (line > 0) {
         fprintf(stderr, "%s at line %d, column %d: ", level_str, line, column);
     } else {
         fprintf(stderr, "%s: ", level_str);
     }
     
     // Print formatted error message
     va_start(args, format);
     vfprintf(stderr, format, args);
     va_end(args);
     
     fprintf(stderr, "\n");
     
     // Exit on fatal errors
     if (level == ERROR_LEVEL_FATAL) {
         exit(EXIT_FAILURE);
     }
 }
 
 // Safe memory allocation wrapper
 void* safe_malloc(size_t size) {
     void* ptr = malloc(size);
     if (!ptr) {
         report_error(ERROR_LEVEL_FATAL, 0, 0, "Memory allocation failed");
     }
     return ptr;
 }
 
 // Safe memory reallocation wrapper
 void* safe_realloc(void* ptr, size_t size) {
     void* new_ptr = realloc(ptr, size);
     if (!new_ptr) {
         report_error(ERROR_LEVEL_FATAL, 0, 0, "Memory reallocation failed");
     }
     return new_ptr;
 }
 
 // Safe string duplication with safety checks
 char* safe_strdup(const char* str) {
     if (!str) return NULL;
     
     char* dup = strdup(str);
     if (!dup) {
         report_error(ERROR_LEVEL_FATAL, 0, 0, "Memory allocation failed in string duplication");
     }
     return dup;
 }
 
 // Check if a string is an integer literal
 int is_integer_literal(const char* str) {
     if (!str || !*str) return 0;
     
     // Check for optional minus sign
     int i = 0;
     if (str[0] == '-') i++;
     
     // Must have at least one digit
     if (!str[i]) return 0;
     
     // All remaining characters must be digits
     for (; str[i]; i++) {
         if (str[i] < '0' || str[i] > '9') return 0;
     }
     
     return 1;
 }
 
 // Check if a string is a floating-point literal
 int is_float_literal(const char* str) {
     if (!str || !*str) return 0;
     
     int i = 0;
     int seen_dot = 0;
     
     // Check for optional minus sign
     if (str[0] == '-') i++;
     
     // Must have at least one digit or a dot
     if (!str[i]) return 0;
     
     // All remaining characters must be digits or a single dot
     for (; str[i]; i++) {
         if (str[i] == '.') {
             if (seen_dot) return 0;  // Second dot found - not a valid float
             seen_dot = 1;
         } else if (str[i] < '0' || str[i] > '9') {
             return 0;  // Not a digit
         }
     }
     
     // Must have at least one digit and a dot to be a float
     return seen_dot;
 }
 
 // Convert a string literal to integer
 int string_to_int(const char* str) {
     if (!str) return 0;
     return atoi(str);
 }
 
 // Convert a string literal to float
 float string_to_float(const char* str) {
     if (!str) return 0.0f;
     return atof(str);
 }