/*
 * codegen.c - Assembly code generator for PHYLOG compiler
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "../include/codegen.h"
 #include "../include/common.h"
 #include "../include/symboltable.h"
 #include "../include/quadgen.h"
 
 // Maximum line length for reading quadruples
 #define MAX_LINE_LENGTH 256
 
 // Output file handle
 static FILE* output_file = NULL;
 
 // Counter for generating unique labels
 static int label_counter = 0;
 
 // Message tracking for display strings
 #define MAX_MESSAGES 100
 static struct {
     char* label;
     char* text;
 } message_strings[MAX_MESSAGES];
 static int message_count = 0;
 static int message_counter = 0;
 
 // Helper function to load a value into AX
 static void load_value_to_ax(const char* value) {
     // Check if value is a number
     if ((value[0] >= '0' && value[0] <= '9') || value[0] == '-') {
         fprintf(output_file, "    mov ax, %s\n", value);
     }
     // Check if it's an array access
     else if (strchr(value, '[') && strchr(value, ']')) {
         char array_name[64] = {0};
         char index[64] = {0};
         
         // Parse array[index]
         sscanf(value, "%[^[][%[^]]", array_name, index);
         
         // Load index to bx
         fprintf(output_file, "    ; Array access %s[%s]\n", array_name, index);
         fprintf(output_file, "    mov bx, [%s]\n", index);
         fprintf(output_file, "    shl bx, 1     ; Multiply by 2 for word size\n");
         fprintf(output_file, "    add bx, %s    ; Add array base address\n", array_name);
         fprintf(output_file, "    mov ax, [bx]  ; Load value from array\n");
     }
     // Regular variable
     else {
         fprintf(output_file, "    mov ax, [%s]\n", value);
     }
 }
 
 // Helper function to store AX to a variable or array element
 static void store_ax_to_result(const char* result) {
     // Check if it's an array access
     if (strchr(result, '[') && strchr(result, ']')) {
         char array_name[64] = {0};
         char index[64] = {0};
         
         // Parse array[index]
         sscanf(result, "%[^[][%[^]]", array_name, index);
         
         // Calculate effective address and store
         fprintf(output_file, "    ; Store to array %s[%s]\n", array_name, index);
         fprintf(output_file, "    push ax       ; Save value temporarily\n");
         fprintf(output_file, "    mov bx, [%s]  ; Load index\n", index);
         fprintf(output_file, "    shl bx, 1     ; Multiply by 2 for word size\n");
         fprintf(output_file, "    add bx, %s    ; Add array base address\n", array_name);
         fprintf(output_file, "    pop ax        ; Restore value\n");
         fprintf(output_file, "    mov [bx], ax  ; Store at calculated address\n");
     }
     // Regular variable
     else {
         fprintf(output_file, "    mov [%s], ax\n", result);
     }
 }
 
 // Forward declarations of helper functions
 static void emit_file_header(void);
 static void emit_data_segment(void);
 static void emit_code_segment(void);
 static void emit_io_hooks(void);
 static void emit_file_footer(void);
 static void process_quadruple(int id, const char* op, const char* arg1, const char* arg2, const char* result);
 
 // Generate a unique label
 static char* generate_label(void) {
     static char label[16];
     sprintf(label, "L%d", label_counter++);
     return label;
 }
 
 // Initialize the code generator
 void codegen_init(void) {
     // Reset state
     label_counter = 0;
     message_counter = 0;
     message_count = 0;
     
     // Clear message string array
     for (int i = 0; i < MAX_MESSAGES; i++) {
         message_strings[i].label = NULL;
         message_strings[i].text = NULL;
     }
 }
 
 // Main code generation function
 int codegen_generate(const char* quad_file, const char* asm_file) {
     FILE* input = fopen(quad_file, "r");
     if (!input) {
         report_error(ERROR_LEVEL_ERROR, 0, 0, "Cannot open quadruple file '%s'", quad_file);
         return 0;
     }
     
     output_file = fopen(asm_file, "w");
     if (!output_file) {
         fclose(input);
         report_error(ERROR_LEVEL_ERROR, 0, 0, "Cannot open output file '%s'", asm_file);
         return 0;
     }
     
     // Process header line (skip it)
     char line[MAX_LINE_LENGTH];
     fgets(line, MAX_LINE_LENGTH, input);
     
     // First pass: Collect all messages from quadruples
     while (fgets(line, MAX_LINE_LENGTH, input)) {
         // Parse the quadruple
         int id;
         char op[32] = {0};
         char arg1[64] = {0};
         
         // Remove newline
         char* newline = strchr(line, '\n');
         if (newline) *newline = '\0';
         
         // Parse CSV format
         char* p = line;
         char* end;
         
         // Parse ID
         id = strtol(p, &end, 10);
         if (p == end) continue;
         
         // Move to op
         p = strchr(p, ',');
         if (!p) continue;
         p++;
         
         // Parse op
         end = strchr(p, ',');
         if (!end) continue;
         
         int len = end - p;
         if (len > 0 && len < sizeof(op)) {
             strncpy(op, p, len);
             op[len] = '\0';
         }
         
         // Move to arg1
         p = end + 1;
         
         // Parse arg1
         end = strchr(p, ',');
         if (!end) continue;
         
         len = end - p;
         if (len > 0 && len < sizeof(arg1)) {
             strncpy(arg1, p, len);
             arg1[len] = '\0';
         }
         
         // If this is a display operation, save the message
         if (strcmp(op, "AFFICHER") == 0) {
             char msg_label[32];
             sprintf(msg_label, "msg%d", message_counter++);
             
             if (message_count < MAX_MESSAGES) {
                 message_strings[message_count].label = strdup(msg_label);
                 message_strings[message_count].text = strdup(arg1);
                 message_count++;
             }
         }
     }
     
     // Rewind the file to process all quadruples
     rewind(input);
     fgets(line, MAX_LINE_LENGTH, input); // Skip header again
     
     // Generate file structure
     emit_file_header();
     emit_data_segment();
     emit_code_segment();
     
     // Second pass: Process each quadruple
     while (fgets(line, MAX_LINE_LENGTH, input)) {
         // Parse the quadruple
         int id;
         char op[32] = {0};
         char arg1[64] = {0};
         char arg2[64] = {0};
         char result[64] = {0};
         
         // Remove newline
         char* newline = strchr(line, '\n');
         if (newline) *newline = '\0';
         
         // Parse CSV format
         char* p = line;
         char* end;
         
         // Parse ID
         id = strtol(p, &end, 10);
         if (p == end) continue;
         
         // Move to op
         p = strchr(p, ',');
         if (!p) continue;
         p++;
         
         // Parse op
         end = strchr(p, ',');
         if (!end) continue;
         
         int len = end - p;
         if (len > 0 && len < sizeof(op)) {
             strncpy(op, p, len);
             op[len] = '\0';
         }
         
         // Move to arg1
         p = end + 1;
         
         // Parse arg1
         end = strchr(p, ',');
         if (!end) continue;
         
         len = end - p;
         if (len > 0 && len < sizeof(arg1)) {
             strncpy(arg1, p, len);
             arg1[len] = '\0';
         }
         
         // Move to arg2
         p = end + 1;
         
         // Parse arg2
         end = strchr(p, ',');
         if (!end) continue;
         
         len = end - p;
         if (len > 0 && len < sizeof(arg2)) {
             strncpy(arg2, p, len);
             arg2[len] = '\0';
         }
         
         // Move to result
         p = end + 1;
         
         // Parse result (rest of line)
         if (strlen(p) > 0 && strlen(p) < sizeof(result)) {
             strcpy(result, p);
         }
         
         // Process the quadruple
         process_quadruple(id, op, arg1, arg2, result);
     }
     
     emit_io_hooks();
     emit_file_footer();
     
     fclose(input);
     fclose(output_file);
     output_file = NULL;
     
     printf("Assembly code generated to '%s'\n", asm_file);
     
     // Free memory used for message strings
     for (int i = 0; i < message_count; i++) {
         if (message_strings[i].label) free(message_strings[i].label);
         if (message_strings[i].text) free(message_strings[i].text);
     }
     
     return 1;
 }
 
 // Clean up code generator resources
 void codegen_cleanup(void) {
     if (output_file) {
         fclose(output_file);
         output_file = NULL;
     }
     
     // Free any allocated memory for message strings
     for (int i = 0; i < message_count; i++) {
         if (message_strings[i].label) free(message_strings[i].label);
         if (message_strings[i].text) free(message_strings[i].text);
         message_strings[i].label = NULL;
         message_strings[i].text = NULL;
     }
     message_count = 0;
 }
 
 // Emit file header with basic setup
 static void emit_file_header(void) {
     fprintf(output_file, "; Generated by PHYLOG Compiler\n");
     fprintf(output_file, "bits 16\n");
     fprintf(output_file, "global _start\n\n");
 }
 
 // Emit data segment with variables
 static void emit_data_segment(void) {
     fprintf(output_file, "section .data\n");
     
     // Define message for hooks
     fprintf(output_file, "    hook_input db 'INPUT', 0\n");
     fprintf(output_file, "    hook_output db 'OUTPUT', 0\n");
     
     // Output all message strings collected during first pass
     fprintf(output_file, "    ; Message strings\n");
     for (int i = 0; i < message_count; i++) {
         fprintf(output_file, "    %s db '%s', 0\n", 
                 message_strings[i].label, message_strings[i].text);
     }
     
     // Output all variables from symbol table
     int symbol_count = symboltable_get_count();
     
     // First pass: Regular variables and constants
     for (int i = 0; i < symbol_count; i++) {
         SymbolNode* symbol = symboltable_get_by_index(i);
         if (!symbol) continue;
         
         // Skip temporaries for now, we'll handle them in the second pass
         if (symbol->name[0] == 'T' && symbol->name[1] >= '0' && symbol->name[1] <= '9') {
             continue;
         }
         
         switch (symbol->category) {
             case CATEGORY_VARIABLE:
                 // Regular variable - define as word (2 bytes)
                 fprintf(output_file, "    %s dw 0\n", symbol->name);
                 break;
                 
             case CATEGORY_CONSTANT:
                 // Constant - define with its value
                 if (symbol->type == TYPE_INTEGER) {
                     fprintf(output_file, "    %s dw %d\n", symbol->name, symbol->value.int_value);
                 }
                 break;
                 
             case CATEGORY_ARRAY:
                 // Array - reserve space
                 if (symbol->type == TYPE_INTEGER) {
                     fprintf(output_file, "    %s times %d dw 0\n", 
                             symbol->name, symbol->array_size);
                 }
                 break;
                 
             default:
                 break;
         }
     }
     
     // Second pass: Only include temporary variables that are in the symbol table
     fprintf(output_file, "    ; Temporary variables\n");
     for (int i = 0; i < symbol_count; i++) {
         SymbolNode* symbol = symboltable_get_by_index(i);
         if (symbol && symbol->name[0] == 'T' && symbol->name[1] >= '0' && symbol->name[1] <= '9') {
             fprintf(output_file, "    %s dw 0\n", symbol->name);
         }
     }
     
     fprintf(output_file, "\n");
 }
 
 // Emit beginning of code segment
 static void emit_code_segment(void) {
     fprintf(output_file, "section .text\n");
     fprintf(output_file, "_start:\n");
     fprintf(output_file, "    ; Program initialization\n");
     fprintf(output_file, "    mov ax, data\n");
     fprintf(output_file, "    mov ds, ax\n\n");
 }
 
 // Emit I/O hook routines
 static void emit_io_hooks(void) {
     fprintf(output_file, "\n; I/O Hooks for Python emulator\n");
     
     // Input hook routine
     fprintf(output_file, "read_int:\n");
     fprintf(output_file, "    ; Input hook - Python will intercept\n");
     fprintf(output_file, "    mov ax, 0FFFFh  ; Input hook identifier\n");
     fprintf(output_file, "    int 80h         ; Custom interrupt\n");
     fprintf(output_file, "    ret\n\n");
     
     // Output hook routine
     fprintf(output_file, "print_int:\n");
     fprintf(output_file, "    ; Output hook - Python will intercept\n");
     fprintf(output_file, "    mov ax, 0FFFEh  ; Output hook identifier\n");
     fprintf(output_file, "    int 80h         ; Custom interrupt\n");
     fprintf(output_file, "    ret\n\n");
 }
 
 // Emit file footer with program termination
 static void emit_file_footer(void) {
     fprintf(output_file, "\n    ; Program termination\n");
     fprintf(output_file, "    mov ax, 4C00h\n");
     fprintf(output_file, "    int 21h\n");
 }
 
 // Process a single quadruple
 static void process_quadruple(int id, const char* op, const char* arg1, const char* arg2, const char* result) {
     fprintf(output_file, "\n    ; Quad %d: %s %s %s %s\n", id, op, arg1, arg2, result);
     
     // Assignment operation
     if (strcmp(op, ":=") == 0) {
         // Load value to AX
         load_value_to_ax(arg1);
         
         // Store to result
         store_ax_to_result(result);
     }
     // Arithmetic operations
     else if (strcmp(op, "+") == 0) {
         // Load first operand to AX
         load_value_to_ax(arg1);
         
         // Add second operand
         fprintf(output_file, "    mov cx, ax    ; Save first operand\n");
         load_value_to_ax(arg2);
         fprintf(output_file, "    add ax, cx    ; Add operands\n");
         
         // Store result
         store_ax_to_result(result);
     }
     else if (strcmp(op, "-") == 0) {
         // Load first operand to AX
         load_value_to_ax(arg1);
         
         // Subtract second operand
         fprintf(output_file, "    mov cx, ax    ; Save first operand\n");
         load_value_to_ax(arg2);
         fprintf(output_file, "    mov dx, ax    ; Save second operand\n");
         fprintf(output_file, "    mov ax, cx    ; Restore first operand\n");
         fprintf(output_file, "    sub ax, dx    ; Subtract second operand\n");
         
         // Store result
         store_ax_to_result(result);
     }
     else if (strcmp(op, "*") == 0) {
         // Load first operand to AX
         load_value_to_ax(arg1);
         
         // Multiply by second operand
         fprintf(output_file, "    mov cx, ax    ; Save first operand\n");
         load_value_to_ax(arg2);
         fprintf(output_file, "    imul cx       ; Multiply (result in DX:AX)\n");
         
         // Store result (only using AX part for simplicity)
         store_ax_to_result(result);
     }
     else if (strcmp(op, "/") == 0) {
         // Load first operand to AX
         load_value_to_ax(arg1);
         
         // Divide by second operand
         fprintf(output_file, "    mov cx, ax    ; Save dividend\n");
         load_value_to_ax(arg2);
         fprintf(output_file, "    mov bx, ax    ; Move divisor to BX\n");
         fprintf(output_file, "    mov ax, cx    ; Move dividend to AX\n");
         fprintf(output_file, "    cwd           ; Sign extend to DX:AX\n");
         fprintf(output_file, "    idiv bx       ; Divide, quotient in AX\n");
         
         // Store result
         store_ax_to_result(result);
     }
     // Branches and jumps
     else if (strcmp(op, "SAUT") == 0) {
         // Unconditional jump
         fprintf(output_file, "    jmp %s\n", arg1);
     }
     else if (strcmp(op, "BG") == 0) {
         // Branch if greater
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    jg %s\n", arg1);
     }
     else if (strcmp(op, "BGE") == 0) {
         // Branch if greater or equal
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    jge %s\n", arg1);
     }
     else if (strcmp(op, "BL") == 0) {
         // Branch if less
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    jl %s\n", arg1);
     }
     else if (strcmp(op, "BLE") == 0) {
         // Branch if less or equal
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    jle %s\n", arg1);
     }
     else if (strcmp(op, "BE") == 0) {
         // Branch if equal
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    je %s\n", arg1);
     }
     else if (strcmp(op, "BNE") == 0) {
         // Branch if not equal
         fprintf(output_file, "    mov ax, [%s]\n", arg2);
         fprintf(output_file, "    cmp ax, [%s]\n", result);
         fprintf(output_file, "    jne %s\n", arg1);
     }
     // Labels
     else if (strcmp(op, "ETIQ") == 0) {
         // Label definition
         fprintf(output_file, "%s:\n", arg1);
     }
     // I/O Operations
     else if (strcmp(op, "LIRE") == 0) {
         // Input operation
         fprintf(output_file, "    ; Input operation\n");
         fprintf(output_file, "    lea bx, [%s]  ; Get address of target variable\n", result);
         fprintf(output_file, "    mov ax, 0FFFFh  ; Input hook identifier\n");
         fprintf(output_file, "    int 80h         ; Call hook\n");
     }
     else if (strcmp(op, "AFFICHER") == 0) {
         // Find the message label for this display string
         char* msg_label = NULL;
         for (int i = 0; i < message_count; i++) {
             if (strcmp(message_strings[i].text, arg1) == 0) {
                 msg_label = message_strings[i].label;
                 break;
             }
         }
         
         fprintf(output_file, "    ; Output operation\n");
         
         if (msg_label) {
             fprintf(output_file, "    mov dx, %s  ; String message\n", msg_label);
         } else {
             fprintf(output_file, "    mov dx, offset hook_output  ; Message address\n");
         }
         
         // If there's a value to output
         if (arg2[0] != '\0' && strcmp(arg2, "0") != 0 && strcmp(arg2, "ZERO") != 0) {
             // Load value to display
             load_value_to_ax(arg2);
             fprintf(output_file, "    mov bx, ax  ; Value to output\n");
         } else {
             fprintf(output_file, "    mov bx, 0     ; No value to output\n");
         }
         
         fprintf(output_file, "    mov ax, 0FFFEh  ; Output hook identifier\n");
         fprintf(output_file, "    int 80h         ; Call hook\n");
     }
     // Array operations
     else if (strcmp(op, "BOUNDS") == 0) {
         // Array bounds checking
         fprintf(output_file, "    ; Array bounds checking\n");
         // No actual code generated for bounds checking in this simple implementation
     }
     else if (strcmp(op, "ADEC") == 0) {
         // Array declaration
         fprintf(output_file, "    ; Array declaration: %s\n", arg1);
         // No code needed, as array declaration is handled in data segment
     }
     // Unsupported operations
     else {
         fprintf(output_file, "    ; Unsupported operation: %s\n", op);
     }
 }