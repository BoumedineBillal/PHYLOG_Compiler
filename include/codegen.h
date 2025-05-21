/*
 * codegen.h - Header for assembly code generator
 */

 #ifndef CODEGEN_H
 #define CODEGEN_H
 
 // Initialize the code generator
 void codegen_init(void);
 
 // Generate assembly code from quadruple file
 int codegen_generate(const char* quad_file, const char* asm_file);
 
 // Clean up code generator resources
 void codegen_cleanup(void);
 
 #endif /* CODEGEN_H */