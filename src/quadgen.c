/*
 * quadgen.c - Intermediate code generation (quadruplets) for the PHYLOG compiler
 */

#include "../include/quadgen.h"
#include "../include/symboltable.h"

#define MAX_QUADS 10000

// Quadruplet array
static Quadruplet quads[MAX_QUADS];
static int quad_count = 0;

// Counters for temporaries and labels
static int temp_counter = 0;
static int label_counter = 0;

// Buffer for temporary names
static char temp_name[32];

// Initialize the quadruplet generator
void quadgen_init() {
    quad_count = 0;
    temp_counter = 0;
    label_counter = 0;
}

// Generate a quadruplet and return its ID
int quadgen_generate(const char* op, const char* arg1, const char* arg2, const char* result) {
    if (quad_count >= MAX_QUADS) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Maximum number of quadruplets exceeded");
        return -1;
    }
    
    // Create the quadruplet
    Quadruplet* q = &quads[quad_count];
    q->id = quad_count;
    
    strncpy(q->op, op ? op : "", sizeof(q->op) - 1);
    q->op[sizeof(q->op) - 1] = '\0';
    
    // Special handling for array access operations
    if (op && strcmp(op, "[]") == 0) {
        // For array access, arg1 should be the array name, and arg2 should be the index
        // Make sure we have a valid array name
        if (!arg1 || arg1[0] == '\0') {
            arg1 = "NUMBERS";  // Default to NUMBERS if no array name specified
        }
        
        // Make sure we have a valid index expression
        if (!arg2 || arg2[0] == '\0') {
            arg2 = "0";  // Default to 0 index if none specified
        }
        
        // Make sure result is treated as INTEGER
        SymbolNode* temp = symboltable_lookup(result);
        if (temp && temp->type == TYPE_UNKNOWN) {
            temp->type = TYPE_INTEGER;
        }
    }
    
    strncpy(q->arg1, arg1 ? arg1 : "", sizeof(q->arg1) - 1);
    q->arg1[sizeof(q->arg1) - 1] = '\0';
    
    strncpy(q->arg2, arg2 ? arg2 : "", sizeof(q->arg2) - 1);
    q->arg2[sizeof(q->arg2) - 1] = '\0';
    
    strncpy(q->result, result ? result : "", sizeof(q->result) - 1);
    q->result[sizeof(q->result) - 1] = '\0';
    
    return quad_count++;
}

// Update a quadruplet (useful for backpatching)
void quadgen_update(int id, const char* op, const char* arg1, const char* arg2, const char* result) {
    if (id < 0 || id >= quad_count) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Invalid quadruplet ID: %d", id);
        return;
    }
    
    Quadruplet* q = &quads[id];
    
    if (op) {
        strncpy(q->op, op, sizeof(q->op) - 1);
        q->op[sizeof(q->op) - 1] = '\0';
    }
    
    if (arg1) {
        strncpy(q->arg1, arg1, sizeof(q->arg1) - 1);
        q->arg1[sizeof(q->arg1) - 1] = '\0';
    }
    
    if (arg2) {
        strncpy(q->arg2, arg2, sizeof(q->arg2) - 1);
        q->arg2[sizeof(q->arg2) - 1] = '\0';
    }
    
    if (result) {
        strncpy(q->result, result, sizeof(q->result) - 1);
        q->result[sizeof(q->result) - 1] = '\0';
    }
}

// Create a new temporary variable name
char* quadgen_new_temp() {
    sprintf(temp_name, "T%d", temp_counter++);
    return temp_name;
}

// Create a new label ID
int quadgen_new_label() {
    return label_counter++;
}

// Get the text representation of a label
char* quadgen_label_to_string(int label) {
    sprintf(temp_name, "L%d", label);
    return temp_name;
}

// Get the current quadruplet count
int quadgen_get_count() {
    return quad_count;
}

// Get a specific quadruplet by ID
Quadruplet* quadgen_get(int id) {
    if (id < 0 || id >= quad_count) {
        return NULL;
    }
    return &quads[id];
}

// Display all generated quadruplets
void quadgen_display() {
    printf("\n********** QUADRUPLETS **********\n");
    printf("%-4s | %-10s | %-15s | %-15s | %-15s\n", 
        "ID", "Operator", "Operand1", "Operand2", "Result");
    printf("----------------------------------------------------------------------\n");
    
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        printf("%-4d | %-10s | %-15s | %-15s | %-15s\n", 
               q->id, q->op, q->arg1, q->arg2, q->result);
    }
    
    printf("----------------------------------------------------------------------\n");
    printf("Total quadruplets: %d\n", quad_count);
    printf("********** END QUADRUPLETS **********\n\n");
}

// Write quadruplets to a file in a format that can be read back
void quadgen_write_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Cannot open file for writing: %s", filename);
        return;
    }
    
    // Write in CSV format for easier parsing
    fprintf(file, "ID,Operator,Arg1,Arg2,Result\n");
    
    // Write quadruplets
    for (int i = 0; i < quad_count; i++) {
        Quadruplet* q = &quads[i];
        fprintf(file, "%d,%s,%s,%s,%s\n", 
                q->id, q->op, q->arg1, q->arg2, q->result);
    }
    
    fclose(file);
}

// Read quadruplets from a file
// Read quadruplets from a file
int quadgen_read_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Cannot open file for reading: %s", filename);
        return 0;
    }
    
    // Reset state
    quadgen_init();
    
    char line[256];
    int line_num = 0;
    
    // Read header line
    if (!fgets(line, sizeof(line), file)) {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Empty or invalid quadruplet file");
        fclose(file);
        return 0;
    }
    line_num++;
    
    // Process data lines
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        
        // Direct approach - manually parse by position
        int id;
        char op[16] = {0};
        char arg1[32] = {0};
        char arg2[32] = {0};
        char result[32] = {0};
        
        // Debug print
        //printf("Parsing line: '%s'\n", line);
        
        char *p = line;
        char *end;
        
        // Parse ID (first number until comma)
        id = strtol(p, &end, 10);
        if (p == end) {
            report_error(ERROR_LEVEL_WARNING, 0, 0, 
                        "Invalid ID in quadruplet at line %d: %s", line_num, line);
            continue;
        }
        
        // Move past the comma
        p = strchr(p, ',');
        if (!p) {
            report_error(ERROR_LEVEL_WARNING, 0, 0, 
                        "Missing comma after ID at line %d: %s", line_num, line);
            continue;
        }
        p++; // Skip the comma
        
        // Parse operator (next field until comma)
        end = strchr(p, ',');
        if (!end) {
            report_error(ERROR_LEVEL_WARNING, 0, 0, 
                        "Missing comma after operator at line %d: %s", line_num, line);
            continue;
        }
        
        int len = end - p;
        if (len > 0 && len < sizeof(op)) {
            strncpy(op, p, len);
            op[len] = '\0';
        }
        
        // Move to arg1
        p = end + 1;
        
        // Parse arg1 (next field until comma)
        end = strchr(p, ',');
        if (!end) {
            report_error(ERROR_LEVEL_WARNING, 0, 0, 
                        "Missing comma after arg1 at line %d: %s", line_num, line);
            continue;
        }
        
        len = end - p;
        if (len > 0 && len < sizeof(arg1)) {
            strncpy(arg1, p, len);
            arg1[len] = '\0';
        }
        
        // Move to arg2
        p = end + 1;
        
        // Parse arg2 (next field until comma)
        end = strchr(p, ',');
        if (!end) {
            report_error(ERROR_LEVEL_WARNING, 0, 0, 
                        "Missing comma after arg2 at line %d: %s", line_num, line);
            continue;
        }
        
        len = end - p;
        if (len > 0 && len < sizeof(arg2)) {
            strncpy(arg2, p, len);
            arg2[len] = '\0';
        }
        
        // Move to result
        p = end + 1;
        
        // Parse result (remainder of the line)
        // Remove trailing newline if present
        end = strchr(p, '\n');
        if (end) {
            len = end - p;
        } else {
            len = strlen(p);
        }
        
        if (len > 0 && len < sizeof(result)) {
            strncpy(result, p, len);
            result[len] = '\0';
        }
        
        // Debug print successful parsing
        //printf("Parsed: ID=%d, OP='%s', ARG1='%s', ARG2='%s', RESULT='%s'\n",
        //       id, op, arg1, arg2, result);
        
        // Add the quadruplet
        if (quad_count < MAX_QUADS) {
            Quadruplet* q = &quads[quad_count];
            q->id = quad_count;
            strncpy(q->op, op, sizeof(q->op) - 1);
            q->op[sizeof(q->op) - 1] = '\0';
            strncpy(q->arg1, arg1, sizeof(q->arg1) - 1);
            q->arg1[sizeof(q->arg1) - 1] = '\0';
            strncpy(q->arg2, arg2, sizeof(q->arg2) - 1);
            q->arg2[sizeof(q->arg2) - 1] = '\0';
            strncpy(q->result, result, sizeof(q->result) - 1);
            q->result[sizeof(q->result) - 1] = '\0';
            quad_count++;
        } else {
            report_error(ERROR_LEVEL_ERROR, 0, 0, "Maximum number of quadruplets exceeded");
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

// Clean up the quadruplet generator
void quadgen_cleanup() {
    // Nothing to do in this implementation
    // If we were dynamically allocating memory, we would free it here
    quad_count = 0;
    temp_counter = 0;
    label_counter = 0;
}


// Set the current quadruplet count (needed for code compaction)
void quadgen_set_count(int count) {
    if (count >= 0 && count <= MAX_QUADS) {
        quad_count = count;
    } else {
        report_error(ERROR_LEVEL_ERROR, 0, 0, "Invalid quadruplet count: %d", count);
    }
}