/*
 * main.c - Main driver for the PHYLOG compiler
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "../include/common.h"
 #include "../include/lexer.h"
 #include "../include/symboltable.h"
 #include "../include/quadgen.h"
 #include "../include/parser.h"
 #include "../include/optimizer.h"
 #include "../include/codegen.h"
 
 // Print usage information
 void print_usage(const char* program_name) {
     printf("Usage: %s <input_file>\n", program_name);
     printf("  Compiles a PHYLOG source file to intermediate representation\n");
     printf("\n");
     printf("Options:\n");
     printf("  -o <output_prefix>  Output file prefix (default: 'output')\n");
     printf("  -O[level]           Enable optimization with optional level (1-3, default: 1)\n");
     printf("                      Level 1: Basic optimizations\n");
     printf("                      Level 2: Intermediate optimizations\n");
     printf("                      Level 3: All optimizations\n");
     printf("  -q                  Quiet mode - don't display assembly code in console\n");
 }
 
 // Function to display file contents
 void display_file_contents(const char* filename) {
     FILE* file = fopen(filename, "r");
     if (!file) {
         printf("Warning: Could not open %s for display\n", filename);
         return;
     }
     
     printf("\n--- Generated Assembly Code ---\n");
     printf("=================================\n");
     
     char line[256];
     while (fgets(line, sizeof(line), file) != NULL) {
         printf("%s", line);
     }
     
     printf("=================================\n");
     fclose(file);
 }
 
 int main(int argc, char* argv[]) {
     // Default values
     const char* input_file = NULL;
     const char* output_prefix = "output";
     int optimize = 1;
     int opt_level = 3;
     int display_asm = 1;  // Default: display assembly code
     
     // Parse command line arguments
     for (int i = 1; i < argc; i++) {
         if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
             output_prefix = argv[++i];
         } else if (strncmp(argv[i], "-O", 2) == 0) {
             optimize = 1;
             // Check if optimization level is specified
             if (argv[i][2] >= '1' && argv[i][2] <= '3') {
                 opt_level = argv[i][2] - '0';
             }
         } else if (strcmp(argv[i], "-q") == 0) {
             display_asm = 0;  // Quiet mode - don't display assembly
         } else if (argv[i][0] == '-') {
             printf("Unknown option: %s\n", argv[i]);
             print_usage(argv[0]);
             return EXIT_FAILURE;
         } else {
             if (input_file == NULL) {
                 input_file = argv[i];
             } else {
                 printf("Multiple input files not supported\n");
                 print_usage(argv[0]);
                 return EXIT_FAILURE;
             }
         }
     }
     
     // Check for input file
     if (input_file == NULL) {
         printf("No input file specified\n");
         print_usage(argv[0]);
         return EXIT_FAILURE;
     }
     
     // Initialize components
     if (!lexer_init(input_file)) {
         report_error(ERROR_LEVEL_FATAL, 0, 0, "Failed to initialize lexer");
         return EXIT_FAILURE;
     }
     
     symboltable_init();
     quadgen_init();
     if (optimize) {
         optimizer_init();
     }
     
     // Parse the input file
     printf("Compiling %s...\n", input_file);
     
     int parse_result = parser_parse();
     
     if (parse_result) {
         printf("Parse successful!\n");
     } else {
         printf("Parse completed with errors, but output will still be generated.\n");
     }
     
     // Generate output file names
     char symtab_file[256];
     char ir_file[256];
     char opt_ir_file[256];
     char asm_file[256];
     
     sprintf(symtab_file, "%s.symtab.csv", output_prefix);
     sprintf(ir_file, "%s.ir.csv", output_prefix);
     sprintf(opt_ir_file, "%s.opt.ir.csv", output_prefix);
     sprintf(asm_file, "%s.asm", output_prefix);  // Assembly file follows same naming convention
     
     // Write the symbol table and IR to files even if there were errors
     symboltable_write_to_file(symtab_file);
     quadgen_write_to_file(ir_file);
     
     printf("Symbol table written to %s\n", symtab_file);
     printf("IR code written to %s\n", ir_file);
     
     // Display summary
     printf("\nCompilation Summary:\n");
     printf("\n--- Original IR ---\n");
     symboltable_display();
     quadgen_display();  // This displays the original IR
     
     // Run optimizer if requested
     if (optimize) {
         printf("\nRunning code optimization at level %d...\n", opt_level);
         
         int opt_result = 0;
         
         // Apply optimization based on level
         switch (opt_level) {
             case 1:
                 // Basic optimization - just constant folding
                 printf("Performing constant folding...\n");
                 opt_result = optimizer_run_pass(OPT_PASS_CONSTANT_FOLDING, ir_file, opt_ir_file);
                 break;
             
             case 2:
                 // Medium optimization - selected passes
                 {
                     char temp_file[256];
                     sprintf(temp_file, "%s.tmp.csv", output_prefix);
                     
                     printf("Performing constant folding and copy propagation...\n");
                     opt_result = optimizer_run_pass(OPT_PASS_CONSTANT_FOLDING, ir_file, temp_file);
                     if (opt_result) {
                         opt_result = optimizer_run_pass(OPT_PASS_COPY_PROPAGATION, temp_file, opt_ir_file);
                     }
                     
                     // Cleanup temp file
                     remove(temp_file);
                 }
                 break;
             
             case 3:
                 // Full optimization - all passes
                 printf("Performing all optimization passes...\n");
                 opt_result = optimizer_run_all_passes(ir_file, opt_ir_file);
                 break;
         }
         
         if (opt_result) {
             printf("Optimized IR code written to %s\n", opt_ir_file);
             
             // Display optimization statistics
             optimizer_display_stats();
             
             // Load and display the optimized code
             printf("\n--- Optimized IR ---\n");
             symboltable_display();
             symboltable_write_to_file(symtab_file);  // save the updated symbol table
             quadgen_init();  // Reset quadgen to clear original IR
             quadgen_read_from_file(opt_ir_file);
             quadgen_display();  // Now display the optimized IR
         } else {
             printf("Optimization failed, using unoptimized code.\n");
         }
     }
     
     // Generate assembly code
     if (parse_result || 1) { // Continue even if parsing had errors
         printf("\nGenerating 8086 assembly code...\n");
         codegen_init();
         if (optimize) {
             codegen_generate(opt_ir_file, asm_file);
         } else {
             codegen_generate(ir_file, asm_file);
         }
         codegen_cleanup();
         printf("Assembly code written to %s\n", asm_file);
         
         // Display the generated assembly code
         if (display_asm) {
             display_file_contents(asm_file);
         }
     }
     
     // Clean up
     lexer_cleanup();
     symboltable_cleanup();
     quadgen_cleanup();
     if (optimize) {
         optimizer_cleanup();
     }
     
     // Exit with appropriate status code
     return parse_result ? EXIT_SUCCESS : EXIT_FAILURE;
 }