# PHYLOG Compiler Project

## Project Overview
This project involves the implementation of a mini-compiler for the PHYLOG language. The compilation process includes lexical analysis using FLEX, syntactico-semantic analysis using BISON, intermediate code generation, code optimization, and assembly code generation.

## 1. PHYLOG Language Specification

### 1.1 General Program Structure
```
nom_du_programme
DATA
………
END
CODE
………
END
END
```

### 1.2 Comments
Comments are ignored by the compiler and are preceded by the `§` symbol:
```
§ This is a comment
```

### 1.3 Data Declarations

#### 1.3.1 Simple Variables
```
TYPE: list_variables;
```
Where `list_variables` is a set of identifiers separated by a vertical bar (`|`).

Examples:
```
§ Declaration of a simple variable
type: nom_variable1;
§ Declaration of 2 variables
type: nom_variable2 | nom_variable3;
```

#### 1.3.2 Arrays
```
VECTOR: nom [borne_inf, taille: type];
```

#### 1.3.3 Data Types
- **INTEGER**: Whole numbers between -32768 and 32767. Signed values must be in parentheses.
- **FLOAT**: Real numbers with decimal points. Signed values must be in parentheses.
- **CHAR**: Single character values enclosed in single quotes.
- **STRING**: Character strings enclosed in double quotes.

#### 1.3.4 Constants
```
CONST: idf = value;
```

#### 1.3.5 Identifiers
An identifier is an alphanumeric string that starts with an uppercase letter followed by lowercase letters and numbers. It cannot contain more than 8 characters.

### 1.4 Operators

#### 1.4.1 Arithmetic Operators
`+`, `-`, `*`, `/`

#### 1.4.2 Logical Operators
- `(expression1. AND. expression2)`: Logical AND
- `(expression1. OR. expression2)`: Logical OR
- `(NOT expression)`: Logical negation

#### 1.4.3 Comparison Operators
- `(expression1.GE. expression2)`: Greater than
- `(expression1.L. expression2)`: Less than
- `(expression1.GE. expression2)`: Greater than or equal to
- `(expression1.LE. expression2)`: Less than or equal to
- `(expression1.EQ. expression2)`: Equal to
- `(expression1.DI. expression2)`: Not equal to

### 1.5 Instructions

#### 1.5.1 Assignment
```
Idf = expression;
```

#### 1.5.2 Input/Output
```
READ ("format_sign": @idf);
DISPLAY ("message format_sign": idf);
```

Format signs:
- `$`: For integers
- `%`: For floats
- `#`: For strings
- `&`: For chars

#### 1.5.3 Conditional Statement (IF)
```
IF(condition):
  instruction1
  instruction2
ELSE:
  instruction3
  instruction4
END
```

#### 1.5.4 Loop Statement (FOR)
```
FOR (idf: PAS: Condition_d'arrêt)
  instruction1
  instruction2
END
```

### 1.6 Operator Precedence (in ascending order)
1. Logical operators: OR (left associative), AND (left associative)
2. Comparison operators: > >= == != <= < (left associative)
3. Arithmetic operators: + - (left associative), * / (left associative)

## 2. Compiler Implementation Tasks

### 2.1 Lexical Analysis with FLEX
- Define lexical entities using regular expressions
- Generate the FLEX program for lexical analysis
- Implement token recognition

### 2.2 Syntactic and Semantic Analysis with BISON
- Define the grammar of the PHYLOG language
- Specify operator precedence rules
- Associate semantic routines with grammar rules
- Implement BISON parser

### 2.3 Symbol Table Management
- Create a symbol table during lexical analysis
- Store variables, constants, and their attributes
- Implement lookup and insertion procedures
- Use hash table for efficiency
- Include array type variables in the symbol table

### 2.4 Intermediate Code Generation
Generate intermediate code in quadruple format:
```
(Operator, Operand1, Operand2, Result)
```

Common quadruple operators:
- Arithmetic: +, -, *, /
- Assignment: :=
- Array operations: BOUNDS, ADEC
- Control flow: BR (unconditional branch), BZ (branch if zero), BNZ (branch if not zero)
- Comparison: BE, BNE, BL, BLE, BG, BGE

Examples of quadruples for different constructs:

#### Assignment
```
<Var> := <Expression>
```
Quadruple: `(:=, <Temp_Expression>, , <var>)`

#### Array Declaration
```
Array A[U1:L1, ..., Un:Ln]
```
Quadruples:
```
{Quadruples for U1 → T11}
{Quadruples for L1 → T12}
(BOUNDS, T11, T12, )
...
{Quadruples for Un → Tn1}
{Quadruples for Ln → Tn2}
(BOUNDS, Tn1, Tn2, )
(ADEC, A, , )
```

#### Conditional (IF-THEN-ELSE)
```
IF (<cond>) THEN <Inst1> ELSE <Inst2>
```
Quadruples:
```
{Quadruples for <cond> → Temp}
(BZ, else, Temp, )
{Quadruples for <Inst1>}
(BR, fin, , )
else:{Quadruples for <Inst2>}
fin:...
```

#### While Loop
```
WHILE (<cond>) DO <Inst>;
```
Quadruples:
```
debut:{Quadruples for <cond> → Temp}
(BZ, fin, Temp, )
{Quadruples for <Inst>}
(BR, debut, , )
fin:...
```

#### Do-While Loop
```
DO <Inst>; WHILE (<cond>);
```
Quadruples:
```
debut:{Quadruples for <Inst>}
{Quadruples for <cond> → Temp}
(BNZ, debut, Temp, )
fin:...
```

#### Repeat-Until Loop
```
REPEAT <Inst>; UNTIL (<cond>);
```
Quadruples:
```
debut:{Quadruples for <Inst>}
{Quadruples for <cond> → Temp}
(BZ, debut, Temp, )
fin:...
```

### 2.5 Code Optimization Techniques

#### 2.5.1 Copy Propagation
Replace variables with equivalent ones to eliminate redundant variables.

Example:
```
t1=t2;
t3=4*t2;
```
Optimized to:
```
t1=t2;
t3=4*t1;
```

#### 2.5.2 Expression Propagation
Propagate expressions to eliminate variables used only once.

Example:
```
t1=expr;
t3=4*t1;
```
Optimized to:
```
t1=expr;
t3=4*expr;
```

#### 2.5.3 Elimination of Redundant Expressions
Reuse previously calculated expressions when they haven't changed.

Example:
```
t6=4*j;
t12=4*j;
```
Optimized to:
```
t6=4*j;
t12=t6;
```

#### 2.5.4 Algebraic Simplification
1. Replace multiplication by additions when possible:
   ```
   X=X*2; => X=X+X;
   ```
2. Eliminate unnecessary calculations:
   ```
   X=Y+1-1; => X=Y;
   ```

#### 2.5.5 Dead Code Elimination
Remove calculations whose results are never used.

Example:
```
X=t3;
a[t2]=t5;
a[t4]=t3;
```
Optimized to:
```
a[t2]=t5;
a[t4]=t3;
```

### 2.6 Assembly Code Generation (8086)

#### 2.6.1 Assembly Program Structure
```
TITLE program_name
PILE SEGMENT STACK
...
PILE ENDS
DONNEE SEGMENT
...
DONNEE ENDS
LECODE SEGMENT
Debut:
...
LECODE ENDS
END Debut
```

#### 2.6.2 Important Registers
- **AX**: Accumulator, implicit operand in many operations
- **BX**: Base register
- **CX**: Counter, used for loops
- **DX**: Data register, used for multiplication/division
- **BP**: Base pointer for stack access
- **SP**: Stack pointer
- **DS**: Data segment pointer
- **CS**: Code segment pointer
- **SS**: Stack segment pointer

#### 2.6.3 Common Instructions
- **MOV**: Data transfer
- **ADD/SUB**: Addition/subtraction
- **MUL/DIV**: Multiplication/division
- **INC/DEC**: Increment/decrement
- **CMP**: Compare
- **JMP**: Unconditional jump
- **Conditional jumps**: JE, JNE, JG, JL, JGE, JLE
- **PUSH/POP**: Stack operations

#### 2.6.4 Mapping High-Level Constructs to Assembly

##### IF-THEN-ELSE
```
if:    CMP AX, 1
       JNZ Else
Then:  MOV BX,10
       JMP endif
Else:  MOV BX,0
       MOV CX,10
endif: ...
```

##### FOR Loop
```
       MOV BX,0
       MOV CX,0
For:   CMP CX,10
       JA Endfor
       ADD BX,CX
       INC CX
       JMP For
Endfor:
```

##### Variable and Array Access
```
; Access array element t[i]
mov SI, i
ADD SI, SI  ; SI = SI * 2 (size of an element)
MOV AX, t[SI]
```

### 2.7 Error Handling
Display appropriate error messages at each stage of the compilation process using the format:
```
Type_of_error, line X, column Y: entity that generated the error.
```

## 3. Implementation Guidelines

### 3.1 Project Structure
- Implement modular code with clear separation of concerns
- Organize source files based on compilation phases
- Document code thoroughly

### 3.2 Development Tools
- Use FLEX for lexical analysis
- Use BISON for syntactic analysis
- Implement the compiler in a suitable programming language (C/C++ recommended)

### 3.3 Testing Strategy
- Create test cases covering all language features
- Test error detection and reporting
- Validate generated code

### 3.4 Deliverables
1. Source code (FLEX specifications, BISON grammar, implementation files)
2. Documentation (language specification, compiler design, user guide)
3. Test cases with expected results
4. Sample PHYLOG programs
