/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "src/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"
#include "../include/lexer.h"
#include "../include/symboltable.h"
#include "../include/quadgen.h"
#include "../include/parser.h"
#include "../include/parser_defs.h"


SymbolType current_declaration_type = TYPE_UNKNOWN;


// Function declarations to avoid warnings
int yylex(void);
int yyerror(const char* s);




#line 94 "build/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DATA = 3,                       /* DATA  */
  YYSYMBOL_CODE = 4,                       /* CODE  */
  YYSYMBOL_END = 5,                        /* END  */
  YYSYMBOL_INTEGER = 6,                    /* INTEGER  */
  YYSYMBOL_FLOAT = 7,                      /* FLOAT  */
  YYSYMBOL_CHAR = 8,                       /* CHAR  */
  YYSYMBOL_STRING = 9,                     /* STRING  */
  YYSYMBOL_CONST = 10,                     /* CONST  */
  YYSYMBOL_VECTOR = 11,                    /* VECTOR  */
  YYSYMBOL_READ = 12,                      /* READ  */
  YYSYMBOL_DISPLAY = 13,                   /* DISPLAY  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_BREAK = 18,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 19,                  /* CONTINUE  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_OR = 21,                        /* OR  */
  YYSYMBOL_NOT = 22,                       /* NOT  */
  YYSYMBOL_GE = 23,                        /* GE  */
  YYSYMBOL_GT = 24,                        /* GT  */
  YYSYMBOL_LT = 25,                        /* LT  */
  YYSYMBOL_LE = 26,                        /* LE  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NE = 28,                        /* NE  */
  YYSYMBOL_PLUS = 29,                      /* PLUS  */
  YYSYMBOL_MINUS = 30,                     /* MINUS  */
  YYSYMBOL_MULT = 31,                      /* MULT  */
  YYSYMBOL_DIV = 32,                       /* DIV  */
  YYSYMBOL_MOD = 33,                       /* MOD  */
  YYSYMBOL_ASSIGN = 34,                    /* ASSIGN  */
  YYSYMBOL_COLON = 35,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 36,                 /* SEMICOLON  */
  YYSYMBOL_LBRACKET = 37,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 38,                  /* RBRACKET  */
  YYSYMBOL_COMMA = 39,                     /* COMMA  */
  YYSYMBOL_PIPE = 40,                      /* PIPE  */
  YYSYMBOL_LPAREN = 41,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 42,                    /* RPAREN  */
  YYSYMBOL_AT = 43,                        /* AT  */
  YYSYMBOL_IDENTIFIER = 44,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 45,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 46,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 47,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 48,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program = 50,                   /* program  */
  YYSYMBOL_51_1 = 51,                      /* $@1  */
  YYSYMBOL_data_section = 52,              /* data_section  */
  YYSYMBOL_declaration = 53,               /* declaration  */
  YYSYMBOL_var_declaration = 54,           /* var_declaration  */
  YYSYMBOL_55_2 = 55,                      /* $@2  */
  YYSYMBOL_type = 56,                      /* type  */
  YYSYMBOL_id_list = 57,                   /* id_list  */
  YYSYMBOL_const_declaration = 58,         /* const_declaration  */
  YYSYMBOL_array_declaration = 59,         /* array_declaration  */
  YYSYMBOL_code_section = 60,              /* code_section  */
  YYSYMBOL_statement_list = 61,            /* statement_list  */
  YYSYMBOL_statement = 62,                 /* statement  */
  YYSYMBOL_assignment = 63,                /* assignment  */
  YYSYMBOL_variable_left = 64,             /* variable_left  */
  YYSYMBOL_variable = 65,                  /* variable  */
  YYSYMBOL_expression = 66,                /* expression  */
  YYSYMBOL_term = 67,                      /* term  */
  YYSYMBOL_factor = 68,                    /* factor  */
  YYSYMBOL_condition = 69,                 /* condition  */
  YYSYMBOL_if_statement = 70,              /* if_statement  */
  YYSYMBOL_71_3 = 71,                      /* $@3  */
  YYSYMBOL_72_4 = 72,                      /* $@4  */
  YYSYMBOL_optional_else = 73,             /* optional_else  */
  YYSYMBOL_74_5 = 74,                      /* $@5  */
  YYSYMBOL_loop_statement = 75,            /* loop_statement  */
  YYSYMBOL_for_statement = 76,             /* for_statement  */
  YYSYMBOL_77_6 = 77,                      /* $@6  */
  YYSYMBOL_78_7 = 78,                      /* $@7  */
  YYSYMBOL_79_8 = 79,                      /* $@8  */
  YYSYMBOL_while_statement = 80,           /* while_statement  */
  YYSYMBOL_jump_statement = 81,            /* jump_statement  */
  YYSYMBOL_io_statement = 82,              /* io_statement  */
  YYSYMBOL_display_statement = 83,         /* display_statement  */
  YYSYMBOL_read_statement = 84             /* read_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   194

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  171

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    90,    90,    90,   104,   105,   109,   110,   111,   117,
     117,   127,   128,   129,   130,   135,   144,   156,   164,   172,
     180,   190,   203,   204,   208,   209,   213,   214,   215,   216,
     217,   218,   222,   242,   250,   261,   264,   271,   272,   276,
     283,   284,   288,   292,   300,   301,   306,   311,   316,   321,
     327,   330,   333,   336,   339,   342,   345,   348,   351,   354,
     360,   363,   360,   375,   378,   378,   388,   389,   393,   397,
     401,   393,   411,   418,   422,   429,   430,   434,   441
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DATA", "CODE", "END",
  "INTEGER", "FLOAT", "CHAR", "STRING", "CONST", "VECTOR", "READ",
  "DISPLAY", "IF", "ELSE", "FOR", "WHILE", "BREAK", "CONTINUE", "AND",
  "OR", "NOT", "GE", "GT", "LT", "LE", "EQ", "NE", "PLUS", "MINUS", "MULT",
  "DIV", "MOD", "ASSIGN", "COLON", "SEMICOLON", "LBRACKET", "RBRACKET",
  "COMMA", "PIPE", "LPAREN", "RPAREN", "AT", "IDENTIFIER", "INT_LITERAL",
  "FLOAT_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "$accept", "program",
  "$@1", "data_section", "declaration", "var_declaration", "$@2", "type",
  "id_list", "const_declaration", "array_declaration", "code_section",
  "statement_list", "statement", "assignment", "variable_left", "variable",
  "expression", "term", "factor", "condition", "if_statement", "$@3",
  "$@4", "optional_else", "$@5", "loop_statement", "for_statement", "$@6",
  "$@7", "$@8", "while_statement", "jump_statement", "io_statement",
  "display_statement", "read_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-101)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -34,  -101,    20,    19,  -101,  -101,   139,    29,  -101,  -101,
    -101,  -101,     3,     5,  -101,  -101,     9,  -101,  -101,    65,
      11,    43,  -101,    12,    49,    51,    62,    81,    25,    61,
    -101,    26,   124,    65,  -101,  -101,   125,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,   126,   127,   117,   114,   115,
     -20,   121,   -20,  -101,  -101,    80,   161,  -101,    80,   106,
     122,  -101,   -27,   133,   134,   -20,   -20,   135,  -101,  -101,
    -101,  -101,  -101,   113,    74,  -101,    -3,   136,    22,    80,
     -26,  -101,    64,   137,   138,   140,   141,   131,  -101,   142,
     132,    80,  -101,    88,    38,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,   -20,   -20,  -101,
      80,    65,   -23,  -101,  -101,  -101,  -101,  -101,  -101,   143,
    -101,   145,    56,  -101,  -101,    66,    46,    46,    46,    46,
      46,    46,    74,    74,  -101,  -101,  -101,  -101,   158,   144,
      46,    18,   146,   148,   147,  -101,  -101,   150,  -101,   149,
     151,  -101,    65,   -20,   153,  -101,    55,    68,   156,  -101,
     175,  -101,  -101,   159,  -101,    65,    65,    65,    65,   177,
    -101
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     2,     0,     0,     1,     4,     0,     0,    11,    12,
      13,    14,     0,     0,     5,     6,     0,     7,     8,    22,
       0,     0,     9,     0,     0,     0,     0,     0,     0,     0,
      31,    33,     0,    23,    24,    26,     0,    28,    29,    66,
      67,    30,    27,    75,    76,     0,     0,     0,     0,     0,
       0,     0,     0,    73,    74,     0,     0,    25,     0,     0,
       0,    15,     0,     0,     0,     0,     0,    35,    45,    46,
      47,    48,    44,     0,    37,    40,     0,     0,     0,     0,
       0,     3,     0,     0,     0,     0,     0,     0,    10,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,    34,    32,    17,    18,    19,    20,     0,
      16,     0,     0,    49,    56,     0,    51,    50,    52,    53,
      54,    55,    38,    39,    41,    42,    43,    57,    58,     0,
      68,     0,     0,     0,     0,    36,    61,     0,    72,     0,
       0,    77,     0,     0,     0,    78,    63,     0,     0,    64,
       0,    69,    21,     0,    62,     0,     0,    70,    65,     0,
      71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -101,  -101,  -101,  -101,  -101,  -101,  -101,    35,  -101,  -101,
    -101,  -101,  -100,   -33,  -101,  -101,    72,   -50,    17,    27,
     -51,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,  -101
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     6,    14,    15,    47,    16,    62,    17,
      18,    32,    33,    34,    35,    36,    72,    73,    74,    75,
      76,    37,   139,   152,   160,   163,    38,    39,   147,   165,
     169,    40,    41,    42,    43,    44
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    78,    65,   102,   103,    80,   102,   103,    82,    88,
       1,   141,   113,    89,    92,    94,    93,   107,   108,   123,
       4,    66,     5,   148,    67,    68,    69,    70,    71,   112,
      23,    24,    25,    19,    26,    27,    28,    29,    20,   109,
      21,   122,   107,   108,    22,   125,   126,   127,   128,   129,
     130,   131,   156,    48,    30,    45,   137,   138,   107,   108,
     140,    53,    31,    55,   111,   167,   168,    23,    24,    25,
     159,    26,    27,    28,    29,   102,   103,    23,    24,    25,
     124,    26,    27,    28,    29,   102,   103,    46,   107,   108,
      49,    30,    50,   102,   103,   102,   103,    54,   144,    31,
     114,    30,   157,    51,   145,   104,   105,   106,    57,    31,
     161,    96,    97,    98,    99,   100,   101,   102,   103,   132,
     133,    79,    52,    57,    67,    68,    69,    70,    71,    56,
     123,   134,   135,   136,    57,    57,    96,    97,    98,    99,
     100,   101,   102,   103,     7,     8,     9,    10,    11,    12,
      13,    83,    84,    85,    86,     8,     9,    10,    11,    58,
      59,    61,    63,    64,    60,    77,    81,    87,    90,    91,
     119,   110,    95,   115,   116,   121,   117,   118,   107,   146,
     164,   149,   170,   151,   154,   153,   120,   155,   142,    67,
     150,   158,   162,   143,   166
};

static const yytype_uint8 yycheck[] =
{
      33,    52,    22,    29,    30,    55,    29,    30,    58,    36,
      44,   111,    38,    40,    65,    66,    66,    20,    21,    42,
       0,    41,     3,     5,    44,    45,    46,    47,    48,    79,
      12,    13,    14,     4,    16,    17,    18,    19,    35,    42,
      35,    91,    20,    21,    35,    95,    96,    97,    98,    99,
     100,   101,   152,    41,    36,    44,   107,   108,    20,    21,
     110,    36,    44,    37,    42,   165,   166,    12,    13,    14,
      15,    16,    17,    18,    19,    29,    30,    12,    13,    14,
      42,    16,    17,    18,    19,    29,    30,    44,    20,    21,
      41,    36,    41,    29,    30,    29,    30,    36,    42,    44,
      36,    36,   153,    41,    38,    31,    32,    33,   141,    44,
      42,    23,    24,    25,    26,    27,    28,    29,    30,   102,
     103,    41,    41,   156,    44,    45,    46,    47,    48,     5,
      42,   104,   105,   106,   167,   168,    23,    24,    25,    26,
      27,    28,    29,    30,     5,     6,     7,     8,     9,    10,
      11,    45,    46,    47,    48,     6,     7,     8,     9,    34,
      34,    44,    48,    48,    37,    44,     5,    45,    35,    35,
      39,    35,    37,    36,    36,    43,    36,    36,    20,    35,
       5,    35,     5,    36,   149,    35,    44,    36,    45,    44,
      42,    38,    36,   121,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    44,    50,    51,     0,     3,    52,     5,     6,     7,
       8,     9,    10,    11,    53,    54,    56,    58,    59,     4,
      35,    35,    35,    12,    13,    14,    16,    17,    18,    19,
      36,    44,    60,    61,    62,    63,    64,    70,    75,    76,
      80,    81,    82,    83,    84,    44,    44,    55,    41,    41,
      41,    41,    41,    36,    36,    37,     5,    62,    34,    34,
      37,    44,    57,    48,    48,    22,    41,    44,    45,    46,
      47,    48,    65,    66,    67,    68,    69,    44,    69,    41,
      66,     5,    66,    45,    46,    47,    48,    45,    36,    40,
      35,    35,    69,    66,    69,    37,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    20,    21,    42,
      35,    42,    66,    38,    36,    36,    36,    36,    36,    39,
      44,    43,    66,    42,    42,    66,    66,    66,    66,    66,
      66,    66,    67,    67,    68,    68,    68,    69,    69,    71,
      66,    61,    45,    65,    42,    38,    35,    77,     5,    35,
      42,    36,    72,    35,    56,    36,    61,    69,    38,    15,
      73,    42,    36,    74,     5,    78,    35,    61,    61,    79,
       5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    51,    50,    52,    52,    53,    53,    53,    55,
      54,    56,    56,    56,    56,    57,    57,    58,    58,    58,
      58,    59,    60,    60,    61,    61,    62,    62,    62,    62,
      62,    62,    63,    64,    64,    65,    65,    66,    66,    66,
      67,    67,    67,    67,    68,    68,    68,    68,    68,    68,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      71,    72,    70,    73,    74,    73,    75,    75,    77,    78,
      79,    76,    80,    81,    81,    82,    82,    83,    84
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     9,     0,     2,     1,     1,     1,     0,
       5,     1,     1,     1,     1,     1,     3,     6,     6,     6,
       6,    11,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     4,     1,     4,     1,     4,     1,     3,     3,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       0,     0,    10,     0,     0,     4,     1,     1,     0,     0,
       0,    13,     6,     2,     2,     1,     1,     7,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 90 "src/parser.y"
               {
        strncpy(program_name, (yyvsp[0].str), sizeof(program_name) - 1);
        program_name[sizeof(program_name) - 1] = '\0';
    }
#line 1287 "build/parser.tab.c"
    break;

  case 3: /* program: IDENTIFIER $@1 DATA data_section END CODE code_section END END  */
#line 97 "src/parser.y"
    {
        printf("Program '%s' successfully compiled.\n", program_name);
        (yyval.str) = "";
    }
#line 1296 "build/parser.tab.c"
    break;

  case 4: /* data_section: %empty  */
#line 104 "src/parser.y"
                { (yyval.str) = ""; }
#line 1302 "build/parser.tab.c"
    break;

  case 5: /* data_section: data_section declaration  */
#line 105 "src/parser.y"
                               { (yyval.str) = ""; }
#line 1308 "build/parser.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 117 "src/parser.y"
               {
        /* Store the type in a global variable for id_list to use */
        current_declaration_type = (yyvsp[-1].type);
    }
#line 1317 "build/parser.tab.c"
    break;

  case 10: /* var_declaration: type COLON $@2 id_list SEMICOLON  */
#line 120 "src/parser.y"
                        {
        (yyval.str) = "";
    }
#line 1325 "build/parser.tab.c"
    break;

  case 11: /* type: INTEGER  */
#line 127 "src/parser.y"
            { (yyval.type) = TYPE_INTEGER; }
#line 1331 "build/parser.tab.c"
    break;

  case 12: /* type: FLOAT  */
#line 128 "src/parser.y"
            { (yyval.type) = TYPE_FLOAT; }
#line 1337 "build/parser.tab.c"
    break;

  case 13: /* type: CHAR  */
#line 129 "src/parser.y"
            { (yyval.type) = TYPE_CHAR; }
#line 1343 "build/parser.tab.c"
    break;

  case 14: /* type: STRING  */
#line 130 "src/parser.y"
             { (yyval.type) = TYPE_STRING; }
#line 1349 "build/parser.tab.c"
    break;

  case 15: /* id_list: IDENTIFIER  */
#line 135 "src/parser.y"
               {
        /* Use the global type variable */
        SymbolNode* node = symboltable_insert_variable((yyvsp[0].str), current_declaration_type, current_line, current_column);
        if (node) {
            (yyval.str) = (yyvsp[0].str);
        } else {
            (yyval.str) = "";
        }
    }
#line 1363 "build/parser.tab.c"
    break;

  case 16: /* id_list: id_list PIPE IDENTIFIER  */
#line 144 "src/parser.y"
                              {
        /* Use the same global type variable */
        SymbolNode* node = symboltable_insert_variable((yyvsp[0].str), current_declaration_type, current_line, current_column);
        if (node) {
            (yyval.str) = (yyvsp[0].str);
        } else {
            (yyval.str) = "";
        }
    }
#line 1377 "build/parser.tab.c"
    break;

  case 17: /* const_declaration: CONST COLON IDENTIFIER ASSIGN INT_LITERAL SEMICOLON  */
#line 156 "src/parser.y"
                                                        {
        int value = (yyvsp[-1].ival);
        symboltable_insert_constant((yyvsp[-3].str), TYPE_INTEGER, &value, current_line, current_column);
        char value_str[32];
        sprintf(value_str, "%d", (yyvsp[-1].ival));
        quadgen_generate(":=", value_str, "", (yyvsp[-3].str));
        (yyval.str) = "";
    }
#line 1390 "build/parser.tab.c"
    break;

  case 18: /* const_declaration: CONST COLON IDENTIFIER ASSIGN FLOAT_LITERAL SEMICOLON  */
#line 164 "src/parser.y"
                                                            {
        float value = (yyvsp[-1].fval);
        symboltable_insert_constant((yyvsp[-3].str), TYPE_FLOAT, &value, current_line, current_column);
        char value_str[32];
        sprintf(value_str, "%f", (yyvsp[-1].fval));
        quadgen_generate(":=", value_str, "", (yyvsp[-3].str));
        (yyval.str) = "";
    }
#line 1403 "build/parser.tab.c"
    break;

  case 19: /* const_declaration: CONST COLON IDENTIFIER ASSIGN CHAR_LITERAL SEMICOLON  */
#line 172 "src/parser.y"
                                                           {
        char value = (yyvsp[-1].cval);
        symboltable_insert_constant((yyvsp[-3].str), TYPE_CHAR, &value, current_line, current_column);
        char value_str[8];
        sprintf(value_str, "'%c'", (yyvsp[-1].cval));
        quadgen_generate(":=", value_str, "", (yyvsp[-3].str));
        (yyval.str) = "";
    }
#line 1416 "build/parser.tab.c"
    break;

  case 20: /* const_declaration: CONST COLON IDENTIFIER ASSIGN STRING_LITERAL SEMICOLON  */
#line 180 "src/parser.y"
                                                             {
        symboltable_insert_constant((yyvsp[-3].str), TYPE_STRING, (yyvsp[-1].str), current_line, current_column);
        char value_str[256];
        sprintf(value_str, "\"%s\"", (yyvsp[-1].str));
        quadgen_generate(":=", value_str, "", (yyvsp[-3].str));
        (yyval.str) = "";
    }
#line 1428 "build/parser.tab.c"
    break;

  case 21: /* array_declaration: VECTOR COLON IDENTIFIER LBRACKET INT_LITERAL COMMA INT_LITERAL COLON type RBRACKET SEMICOLON  */
#line 190 "src/parser.y"
                                                                                                 {
        /* Process array declaration */
        int lower_bound = (yyvsp[-6].ival);
        int upper_bound = (yyvsp[-4].ival);
        int array_type = (yyvsp[-2].type);  
        
        process_array_declaration((yyvsp[-8].str), lower_bound, upper_bound, array_type);
        
        (yyval.str) = "";
    }
#line 1443 "build/parser.tab.c"
    break;

  case 22: /* code_section: %empty  */
#line 203 "src/parser.y"
                { (yyval.str) = ""; }
#line 1449 "build/parser.tab.c"
    break;

  case 23: /* code_section: statement_list  */
#line 204 "src/parser.y"
                     { (yyval.str) = (yyvsp[0].str); }
#line 1455 "build/parser.tab.c"
    break;

  case 24: /* statement_list: statement  */
#line 208 "src/parser.y"
              { (yyval.str) = (yyvsp[0].str); }
#line 1461 "build/parser.tab.c"
    break;

  case 25: /* statement_list: statement_list statement  */
#line 209 "src/parser.y"
                               { (yyval.str) = (yyvsp[-1].str); }
#line 1467 "build/parser.tab.c"
    break;

  case 26: /* statement: assignment  */
#line 213 "src/parser.y"
               { (yyval.str) = (yyvsp[0].str); }
#line 1473 "build/parser.tab.c"
    break;

  case 27: /* statement: io_statement  */
#line 214 "src/parser.y"
                   { (yyval.str) = (yyvsp[0].str); }
#line 1479 "build/parser.tab.c"
    break;

  case 28: /* statement: if_statement  */
#line 215 "src/parser.y"
                   { (yyval.str) = NULL; }
#line 1485 "build/parser.tab.c"
    break;

  case 29: /* statement: loop_statement  */
#line 216 "src/parser.y"
                     { (yyval.str) = NULL; }
#line 1491 "build/parser.tab.c"
    break;

  case 30: /* statement: jump_statement  */
#line 217 "src/parser.y"
                     { (yyval.str) = NULL; }
#line 1497 "build/parser.tab.c"
    break;

  case 31: /* statement: SEMICOLON  */
#line 218 "src/parser.y"
                 { (yyval.str) = ""; }
#line 1503 "build/parser.tab.c"
    break;

  case 32: /* assignment: variable_left ASSIGN expression SEMICOLON  */
#line 222 "src/parser.y"
                                              {
        // $1 is info about left side, $3 is the expression result
        AssignmentTarget* target = (AssignmentTarget*)(yyvsp[-3].target);
        
        if (target->is_array) {
            process_array_assignment(target->name, target->index, (yyvsp[-1].str));
        } else {
            process_simple_assignment(target->name, (yyvsp[-1].str));
        }
        
        free(target->name);
        if (target->index) free(target->index);
        free(target);
        
        (yyval.str) = "";
    }
#line 1524 "build/parser.tab.c"
    break;

  case 33: /* variable_left: IDENTIFIER  */
#line 242 "src/parser.y"
               {
        // Create a structure to hold assignment target info
        AssignmentTarget* target = malloc(sizeof(AssignmentTarget));
        target->name = safe_strdup((yyvsp[0].str));
        target->index = NULL;
        target->is_array = 0;
        (yyval.target) = target;
    }
#line 1537 "build/parser.tab.c"
    break;

  case 34: /* variable_left: IDENTIFIER LBRACKET expression RBRACKET  */
#line 250 "src/parser.y"
                                              {
        // Create structure with array info
        AssignmentTarget* target = malloc(sizeof(AssignmentTarget));
        target->name = safe_strdup((yyvsp[-3].str));
        target->index = safe_strdup((yyvsp[-1].str));
        target->is_array = 1;
        (yyval.target) = target;
    }
#line 1550 "build/parser.tab.c"
    break;

  case 35: /* variable: IDENTIFIER  */
#line 261 "src/parser.y"
               {
        (yyval.str) = process_variable((yyvsp[0].str));
    }
#line 1558 "build/parser.tab.c"
    break;

  case 36: /* variable: IDENTIFIER LBRACKET expression RBRACKET  */
#line 264 "src/parser.y"
                                              {
        (yyval.str) = process_array_access((yyvsp[-3].str), (yyvsp[-1].str));
    }
#line 1566 "build/parser.tab.c"
    break;

  case 37: /* expression: term  */
#line 271 "src/parser.y"
         { (yyval.str) = (yyvsp[0].str); }
#line 1572 "build/parser.tab.c"
    break;

  case 38: /* expression: expression PLUS term  */
#line 272 "src/parser.y"
                           {
        SymbolType result_type = check_arithmetic_types("+", (yyvsp[-2].str), (yyvsp[0].str));
        (yyval.str) = gen_expr_quad("+", (yyvsp[-2].str), (yyvsp[0].str), result_type);
    }
#line 1581 "build/parser.tab.c"
    break;

  case 39: /* expression: expression MINUS term  */
#line 276 "src/parser.y"
                            {
        SymbolType result_type = check_arithmetic_types("-", (yyvsp[-2].str), (yyvsp[0].str));
        (yyval.str) = gen_expr_quad("-", (yyvsp[-2].str), (yyvsp[0].str), result_type);
    }
#line 1590 "build/parser.tab.c"
    break;

  case 40: /* term: factor  */
#line 283 "src/parser.y"
           { (yyval.str) = (yyvsp[0].str); }
#line 1596 "build/parser.tab.c"
    break;

  case 41: /* term: term MULT factor  */
#line 284 "src/parser.y"
                       {
        SymbolType result_type = check_arithmetic_types("*", (yyvsp[-2].str), (yyvsp[0].str));
        (yyval.str) = gen_expr_quad("*", (yyvsp[-2].str), (yyvsp[0].str), result_type);
    }
#line 1605 "build/parser.tab.c"
    break;

  case 42: /* term: term DIV factor  */
#line 288 "src/parser.y"
                      {
        SymbolType result_type = check_arithmetic_types("/", (yyvsp[-2].str), (yyvsp[0].str));
        (yyval.str) = gen_expr_quad("/", (yyvsp[-2].str), (yyvsp[0].str), result_type);
    }
#line 1614 "build/parser.tab.c"
    break;

  case 43: /* term: term MOD factor  */
#line 292 "src/parser.y"
                      {
        // Modulo only works on integers
        SymbolType result_type = check_arithmetic_types("%", (yyvsp[-2].str), (yyvsp[0].str));
        (yyval.str) = gen_expr_quad("%", (yyvsp[-2].str), (yyvsp[0].str), result_type);
    }
#line 1624 "build/parser.tab.c"
    break;

  case 44: /* factor: variable  */
#line 300 "src/parser.y"
             { (yyval.str) = (yyvsp[0].str); }
#line 1630 "build/parser.tab.c"
    break;

  case 45: /* factor: INT_LITERAL  */
#line 301 "src/parser.y"
                  {
        char temp[32];
        sprintf(temp, "%d", (yyvsp[0].ival));
        (yyval.str) = process_literal(temp, TYPE_INTEGER);
    }
#line 1640 "build/parser.tab.c"
    break;

  case 46: /* factor: FLOAT_LITERAL  */
#line 306 "src/parser.y"
                    {
        char temp[32];
        sprintf(temp, "%f", (yyvsp[0].fval));
        (yyval.str) = process_literal(temp, TYPE_FLOAT);
    }
#line 1650 "build/parser.tab.c"
    break;

  case 47: /* factor: CHAR_LITERAL  */
#line 311 "src/parser.y"
                   {
        char temp[4];
        sprintf(temp, "'%c'", (yyvsp[0].cval));
        (yyval.str) = process_literal(temp, TYPE_CHAR);
    }
#line 1660 "build/parser.tab.c"
    break;

  case 48: /* factor: STRING_LITERAL  */
#line 316 "src/parser.y"
                     {
        char temp[256];
        sprintf(temp, "\"%s\"", (yyvsp[0].str));
        (yyval.str) = process_literal(temp, TYPE_STRING);
    }
#line 1670 "build/parser.tab.c"
    break;

  case 49: /* factor: LPAREN expression RPAREN  */
#line 321 "src/parser.y"
                               {
        (yyval.str) = (yyvsp[-1].str);
    }
#line 1678 "build/parser.tab.c"
    break;

  case 50: /* condition: expression GT expression  */
#line 327 "src/parser.y"
                             {
        (yyval.cond) = process_condition("GT", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1686 "build/parser.tab.c"
    break;

  case 51: /* condition: expression GE expression  */
#line 330 "src/parser.y"
                               {
        (yyval.cond) = process_condition("GE", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1694 "build/parser.tab.c"
    break;

  case 52: /* condition: expression LT expression  */
#line 333 "src/parser.y"
                               {
        (yyval.cond) = process_condition("LT", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1702 "build/parser.tab.c"
    break;

  case 53: /* condition: expression LE expression  */
#line 336 "src/parser.y"
                               {
        (yyval.cond) = process_condition("LE", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1710 "build/parser.tab.c"
    break;

  case 54: /* condition: expression EQ expression  */
#line 339 "src/parser.y"
                               {
        (yyval.cond) = process_condition("EQ", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1718 "build/parser.tab.c"
    break;

  case 55: /* condition: expression NE expression  */
#line 342 "src/parser.y"
                               {
        (yyval.cond) = process_condition("NE", (yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1726 "build/parser.tab.c"
    break;

  case 56: /* condition: LPAREN condition RPAREN  */
#line 345 "src/parser.y"
                              {
        (yyval.cond) = (yyvsp[-1].cond);
    }
#line 1734 "build/parser.tab.c"
    break;

  case 57: /* condition: condition AND condition  */
#line 348 "src/parser.y"
                              {
        (yyval.cond) = process_logical_operator("AND", (yyvsp[-2].cond), (yyvsp[0].cond));
    }
#line 1742 "build/parser.tab.c"
    break;

  case 58: /* condition: condition OR condition  */
#line 351 "src/parser.y"
                             {
        (yyval.cond) = process_logical_operator("OR", (yyvsp[-2].cond), (yyvsp[0].cond));
    }
#line 1750 "build/parser.tab.c"
    break;

  case 59: /* condition: NOT condition  */
#line 354 "src/parser.y"
                    {
        (yyval.cond) = process_logical_operator("NOT", (yyvsp[0].cond), NULL);
    }
#line 1758 "build/parser.tab.c"
    break;

  case 60: /* $@3: %empty  */
#line 360 "src/parser.y"
                               {
        begin_if_statement((yyvsp[-1].cond));
    }
#line 1766 "build/parser.tab.c"
    break;

  case 61: /* $@4: %empty  */
#line 363 "src/parser.y"
          {
        begin_then_part();
    }
#line 1774 "build/parser.tab.c"
    break;

  case 62: /* if_statement: IF LPAREN condition RPAREN $@3 COLON $@4 statement_list optional_else END  */
#line 368 "src/parser.y"
        {
        /* end_if_statement will be called in optional_else */
        (yyval.patch) = NULL;
    }
#line 1783 "build/parser.tab.c"
    break;

  case 63: /* optional_else: %empty  */
#line 375 "src/parser.y"
                {
        end_if_statement(0); // No else part
    }
#line 1791 "build/parser.tab.c"
    break;

  case 64: /* $@5: %empty  */
#line 378 "src/parser.y"
           {
        begin_else_part();
    }
#line 1799 "build/parser.tab.c"
    break;

  case 65: /* optional_else: ELSE $@5 COLON statement_list  */
#line 382 "src/parser.y"
                   {
        end_if_statement(1); // Has else part
    }
#line 1807 "build/parser.tab.c"
    break;

  case 66: /* loop_statement: for_statement  */
#line 388 "src/parser.y"
                  { (yyval.patch) = (yyvsp[0].patch); }
#line 1813 "build/parser.tab.c"
    break;

  case 67: /* loop_statement: while_statement  */
#line 389 "src/parser.y"
                      { (yyval.patch) = (yyvsp[0].patch); }
#line 1819 "build/parser.tab.c"
    break;

  case 68: /* $@6: %empty  */
#line 393 "src/parser.y"
                                           {
        // Initialize counter and set up condition label
        begin_for_loop_init((yyvsp[-2].str), (yyvsp[0].str));
    }
#line 1828 "build/parser.tab.c"
    break;

  case 69: /* $@7: %empty  */
#line 397 "src/parser.y"
                           {
        // Generate condition checking code
        begin_for_loop_condition((yyvsp[-1].cond));
    }
#line 1837 "build/parser.tab.c"
    break;

  case 70: /* $@8: %empty  */
#line 401 "src/parser.y"
                   {
        // Generate increment, jump back and end label
        end_for_loop();
    }
#line 1846 "build/parser.tab.c"
    break;

  case 71: /* for_statement: FOR LPAREN IDENTIFIER COLON expression $@6 COLON condition RPAREN $@7 statement_list $@8 END  */
#line 405 "src/parser.y"
        {
        (yyval.patch) = NULL;
    }
#line 1854 "build/parser.tab.c"
    break;

  case 72: /* while_statement: WHILE LPAREN condition RPAREN statement_list END  */
#line 411 "src/parser.y"
                                                     {
        process_while_loop((yyvsp[-3].cond));
        (yyval.patch) = NULL;
    }
#line 1863 "build/parser.tab.c"
    break;

  case 73: /* jump_statement: BREAK SEMICOLON  */
#line 418 "src/parser.y"
                    {
        process_jump_statement("BREAK");
        (yyval.patch) = NULL;
    }
#line 1872 "build/parser.tab.c"
    break;

  case 74: /* jump_statement: CONTINUE SEMICOLON  */
#line 422 "src/parser.y"
                         {
        process_jump_statement("CONTINUE");
        (yyval.patch) = NULL;
    }
#line 1881 "build/parser.tab.c"
    break;

  case 75: /* io_statement: display_statement  */
#line 429 "src/parser.y"
                      { (yyval.str) = (yyvsp[0].str); }
#line 1887 "build/parser.tab.c"
    break;

  case 76: /* io_statement: read_statement  */
#line 430 "src/parser.y"
                     { (yyval.str) = (yyvsp[0].str); }
#line 1893 "build/parser.tab.c"
    break;

  case 77: /* display_statement: DISPLAY LPAREN STRING_LITERAL COLON expression RPAREN SEMICOLON  */
#line 434 "src/parser.y"
                                                                    {
        process_io_statement("DISPLAY", (yyvsp[-4].str), (yyvsp[-2].str));
        (yyval.str) = "";
    }
#line 1902 "build/parser.tab.c"
    break;

  case 78: /* read_statement: READ LPAREN STRING_LITERAL COLON AT variable RPAREN SEMICOLON  */
#line 441 "src/parser.y"
                                                                  {
        process_io_statement("READ", (yyvsp[-5].str), (yyvsp[-2].str));
        (yyval.str) = "";
    }
#line 1911 "build/parser.tab.c"
    break;


#line 1915 "build/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 447 "src/parser.y"


// Start syntactic analysis
int parser_parse() {
    return yyparse();
}

// Bison error handler
int yyerror(const char* s) {
    parser_syntax_error(s);
    return 0;
}
