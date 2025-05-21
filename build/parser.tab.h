/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DATA = 258,                    /* DATA  */
    CODE = 259,                    /* CODE  */
    END = 260,                     /* END  */
    INTEGER = 261,                 /* INTEGER  */
    FLOAT = 262,                   /* FLOAT  */
    CHAR = 263,                    /* CHAR  */
    STRING = 264,                  /* STRING  */
    CONST = 265,                   /* CONST  */
    VECTOR = 266,                  /* VECTOR  */
    READ = 267,                    /* READ  */
    DISPLAY = 268,                 /* DISPLAY  */
    IF = 269,                      /* IF  */
    ELSE = 270,                    /* ELSE  */
    FOR = 271,                     /* FOR  */
    WHILE = 272,                   /* WHILE  */
    BREAK = 273,                   /* BREAK  */
    CONTINUE = 274,                /* CONTINUE  */
    AND = 275,                     /* AND  */
    OR = 276,                      /* OR  */
    NOT = 277,                     /* NOT  */
    GE = 278,                      /* GE  */
    GT = 279,                      /* GT  */
    LT = 280,                      /* LT  */
    LE = 281,                      /* LE  */
    EQ = 282,                      /* EQ  */
    NE = 283,                      /* NE  */
    PLUS = 284,                    /* PLUS  */
    MINUS = 285,                   /* MINUS  */
    MULT = 286,                    /* MULT  */
    DIV = 287,                     /* DIV  */
    MOD = 288,                     /* MOD  */
    ASSIGN = 289,                  /* ASSIGN  */
    COLON = 290,                   /* COLON  */
    SEMICOLON = 291,               /* SEMICOLON  */
    LBRACKET = 292,                /* LBRACKET  */
    RBRACKET = 293,                /* RBRACKET  */
    COMMA = 294,                   /* COMMA  */
    PIPE = 295,                    /* PIPE  */
    LPAREN = 296,                  /* LPAREN  */
    RPAREN = 297,                  /* RPAREN  */
    AT = 298,                      /* AT  */
    IDENTIFIER = 299,              /* IDENTIFIER  */
    INT_LITERAL = 300,             /* INT_LITERAL  */
    FLOAT_LITERAL = 301,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 302,            /* CHAR_LITERAL  */
    STRING_LITERAL = 303           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 29 "src/parser.y"

    char* str;           // String value
    int ival;            // Integer value
    float fval;          // Float value
    char cval;           // Character value
    SymbolType type;     // Data type
    SymbolNode* node;    // Symbol table node
    ConditionAttrs* cond; // Condition attributes
    PatchList* patch;    // Patch list for backpatching
    AssignmentTarget* target; // For left-side variables

#line 124 "build/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
