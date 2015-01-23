/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "2paser.y"

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	
	#include <json/json.h>
	#include <iostream>
	#include <fstream>

	int yylex(void);
	void yyerror(char *);
	extern FILE* yyin; 
	extern FILE* yyout; 

	using namespace std;
	std::ofstream ofs;
	Json::Reader reader;
	Json::Value root;
	Json::Value& add_value = root["ArgumentElement"];

	std::ifstream iifs;
	Json::Reader sub_reader;
	Json::Value substitution;
	Json::Value& sub_value = substitution["ParameterElement"];
	
	Json::FastWriter writer;
	std::string json_append_file;

	//modification
	typedef struct Count
	{
		string name;
		int count;
	}nameCount;
	
	vector <nameCount> countList;
	
	vector <string> argumList;

	string code;

	int newMultiTempFlag=0; //flag for the template with mutlitple arguments.

	Json::Value append_value;


/* Line 371 of yacc.c  */
#line 115 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TEMP = 258,
     END = 259,
     INCLUDE = 260,
     DAL = 261,
     DAR = 262,
     VEC = 263,
     WELL = 264,
     ID = 265,
     RT = 266,
     INSTR = 267,
     OPERATOR = 268,
     SYMBOL = 269,
     SPKEYWORD = 270,
     ALR = 271,
     COMMA = 272,
     DECPOINT = 273,
     DIGIT = 274
   };
#endif
/* Tokens.  */
#define TEMP 258
#define END 259
#define INCLUDE 260
#define DAL 261
#define DAR 262
#define VEC 263
#define WELL 264
#define ID 265
#define RT 266
#define INSTR 267
#define OPERATOR 268
#define SYMBOL 269
#define SPKEYWORD 270
#define ALR 271
#define COMMA 272
#define DECPOINT 273
#define DIGIT 274



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 47 "2paser.y"

		char* str;
		int num;
	

/* Line 387 of yacc.c  */
#line 202 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 230 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   103

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  20
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNRULES -- Number of states.  */
#define YYNSTATES  95

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   274

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      15,    16,    17,    18,    19
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,     9,    12,    15,    18,    20,
      22,    24,    26,    28,    30,    32,    34,    36,    38,    40,
      42,    44,    48,    53,    59,    62,    65,    67,    73,    79,
      83,    87,    90,    93,    96,    99,   102,   107,   112,   116,
     120,   123,   126,   128,   132,   135,   140,   143,   145,   148,
     151,   155,   159,   161,   164,   168,   172,   178,   182,   184,
     186,   187,   189
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      21,     0,    -1,    22,    21,    -1,    22,    -1,    -1,    23,
      22,    -1,    10,    16,    -1,    10,    17,    -1,    10,    -1,
      18,    -1,    17,    -1,    13,    -1,    14,    -1,    15,    -1,
       4,    -1,    26,    -1,    24,    -1,    19,    -1,    12,    -1,
       6,    -1,     7,    -1,     8,    16,    25,    -1,     8,    16,
      10,    16,    -1,     8,    16,    10,    16,    16,    -1,    33,
      34,    -1,     3,    28,    -1,    27,    -1,    10,    16,    10,
      16,    10,    -1,    10,    16,    19,    16,    10,    -1,    10,
      16,    10,    -1,    10,    16,    19,    -1,    17,    10,    -1,
      17,    19,    -1,    16,    10,    -1,    29,    28,    -1,    29,
      30,    -1,    16,    10,    10,    16,    -1,    16,    10,    10,
      17,    -1,    10,    10,    17,    -1,    10,    10,    16,    -1,
      10,    10,    -1,    31,    30,    -1,    31,    -1,    10,    10,
      16,    -1,    10,    16,    -1,    10,    16,    14,    14,    -1,
      10,    17,    -1,    32,    -1,    15,    14,    -1,    10,    14,
      -1,    10,    10,    17,    -1,    10,    10,    14,    -1,     5,
      -1,     9,    10,    -1,    16,    10,    16,    -1,    16,    13,
      16,    -1,    16,    10,    14,    13,    16,    -1,    16,     8,
      16,    -1,    12,    -1,    35,    -1,    -1,    10,    -1,     4,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    64,    64,    65,    66,    69,    70,    72,    74,    76,
      78,    80,    82,    84,    86,    91,    92,    93,    94,    95,
      96,   100,   101,   105,   110,   111,   112,   116,   216,   327,
     423,   531,   547,   569,   600,   601,   605,   606,   607,   608,
     612,   615,   616,   620,   623,   626,   628,   632,   636,   637,
     638,   639,   643,   644,   648,   649,   650,   651,   652,   653,
     654,   658,   659
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TEMP", "END", "INCLUDE", "DAL", "DAR",
  "VEC", "WELL", "ID", "RT", "INSTR", "OPERATOR", "SYMBOL", "SPKEYWORD",
  "ALR", "COMMA", "DECPOINT", "DIGIT", "$accept", "program", "express",
  "keyword", "vect1", "vect2", "template", "instance", "addition",
  "tmpparam", "classdec", "paramdec", "methods", "ind", "indparam",
  "otherdef", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    20,    21,    21,    21,    22,    22,    22,    22,    22,
      22,    22,    22,    22,    22,    23,    23,    23,    23,    23,
      23,    24,    24,    25,    26,    26,    26,    27,    27,    27,
      27,    27,    27,    27,    28,    28,    29,    29,    29,    29,
      30,    30,    30,    31,    31,    31,    31,    31,    32,    32,
      32,    32,    33,    33,    34,    34,    34,    34,    34,    34,
      34,    35,    35
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     0,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     5,     2,     2,     1,     5,     5,     3,
       3,     2,     2,     2,     2,     2,     4,     4,     3,     3,
       2,     2,     1,     3,     2,     4,     2,     1,     2,     2,
       3,     3,     1,     2,     3,     3,     5,     3,     1,     1,
       0,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    14,    52,    19,    20,     0,     0,     8,    18,
      11,    12,    13,     0,    10,     9,    17,     0,     3,     0,
      16,    15,    26,    60,     0,     0,    25,     0,     0,    53,
       6,     7,    33,    31,    32,     1,     2,     5,    62,    61,
      58,     0,    24,    59,     0,     0,     0,     0,    34,    35,
      42,    47,     0,     0,    21,    29,    30,     0,     0,     0,
      39,    38,     0,    40,    49,    44,    46,    48,     0,    41,
       0,    22,     0,     0,    57,     0,    54,    55,    36,    37,
      51,    43,    50,     0,    40,     0,    27,    28,     0,    45,
      43,    50,     0,    56,    23
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    17,    18,    19,    20,    54,    21,    22,    26,    27,
      49,    50,    51,    23,    42,    43
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -9
static const yytype_int8 yypact[] =
{
      -3,    34,    -9,    -9,    -9,    -9,    -8,    11,    29,    -9,
      -9,    -9,    -9,    14,     8,    -9,    -9,    54,    -3,    -3,
      -9,    -9,    -9,    13,    60,    61,    -9,    24,    12,    -9,
       9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,    -9,
      -9,    43,    -9,    -9,    50,    62,    16,    46,    -9,    -9,
      37,    -9,    48,    57,    -9,    58,    59,    63,    49,    64,
      -9,    -9,    52,    41,    -9,    67,    -9,    -9,    21,    -9,
      66,    -9,    68,    72,    -9,    70,    -9,    -9,    -9,    -9,
      -9,    26,    33,    71,    45,    73,    -9,    -9,    74,    -9,
      -9,    -9,    75,    -9,    -9
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
      -9,    69,    65,    -9,    -9,    -9,    -9,    -9,    76,    -9,
      27,    -9,    -9,    -9,    -9,    -9
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -40
static const yytype_int8 yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,    28,     9,
      10,    11,    12,    13,    14,    15,    16,    38,    33,    55,
      52,    29,    53,    39,    32,    40,    63,    34,    56,    41,
      64,    84,    65,    66,    46,    64,   -39,    65,    66,    47,
      25,   -39,   -39,   -38,    24,    30,    31,    68,   -38,   -38,
      25,    57,    47,    58,    35,    80,    59,    81,    82,    80,
      67,    90,    91,    75,    70,    76,    60,    61,    78,    79,
      44,    45,    62,    71,    72,    73,    85,    69,    86,    74,
      77,    83,    87,    88,    37,    89,     0,    36,     0,    92,
      93,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-9)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    16,    12,
      13,    14,    15,    16,    17,    18,    19,     4,    10,    10,
       8,    10,    10,    10,    10,    12,    10,    19,    19,    16,
      14,    10,    16,    17,    10,    14,    10,    16,    17,    15,
      16,    15,    16,    10,    10,    16,    17,    10,    15,    16,
      16,     8,    15,    10,     0,    14,    13,    16,    17,    14,
      14,    16,    17,    14,    16,    16,    16,    17,    16,    17,
      10,    10,    10,    16,    16,    16,    10,    50,    10,    16,
      16,    14,    10,    13,    19,    14,    -1,    18,    -1,    16,
      16,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    14,    15,    16,    17,    18,    19,    21,    22,    23,
      24,    26,    27,    33,    10,    16,    28,    29,    16,    10,
      16,    17,    10,    10,    19,     0,    21,    22,     4,    10,
      12,    16,    34,    35,    10,    10,    10,    15,    28,    30,
      31,    32,     8,    10,    25,    10,    19,     8,    10,    13,
      16,    17,    10,    10,    14,    16,    17,    14,    10,    30,
      16,    16,    16,    16,    16,    14,    16,    16,    16,    17,
      14,    16,    17,    14,    10,    10,    10,    10,    13,    14,
      16,    17,    16,    16,    16
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1787 of yacc.c  */
#line 64 "2paser.y"
    {}
    break;

  case 3:
/* Line 1787 of yacc.c  */
#line 65 "2paser.y"
    {}
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 66 "2paser.y"
    {}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 69 "2paser.y"
    {}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 70 "2paser.y"
    {fprintf(yyout," %s%s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
					printf(" id arl: %s %s\n", (yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 72 "2paser.y"
    {fprintf(yyout," %s%s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
					printf("id comma: %s %s\n", (yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));}
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 74 "2paser.y"
    {fprintf(yyout," %s",(yyvsp[(1) - (1)].str));
					printf("id: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 76 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));
					printf("decimal point: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 78 "2paser.y"
    {fprintf(yyout," %s",(yyvsp[(1) - (1)].str));
					printf("comma: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 80 "2paser.y"
    {fprintf(yyout," %s",(yyvsp[(1) - (1)].str));
					printf("operator: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 82 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));
					printf("symbol: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 84 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));
					printf("keyword: %s \n", (yyvsp[(1) - (1)].str));}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 86 "2paser.y"
    {fprintf(yyout,"\n");}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 91 "2paser.y"
    {}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 92 "2paser.y"
    {}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 93 "2paser.y"
    {fprintf(yyout," %d", (yyvsp[(1) - (1)].num));}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 94 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));}
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 95 "2paser.y"
    {fprintf(yyout,"<<");}
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 96 "2paser.y"
    {fprintf(yyout,">>");}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 100 "2paser.y"
    {}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 101 "2paser.y"
    {fprintf(yyout," vector %s%s%s",(yyvsp[(2) - (4)].str),(yyvsp[(3) - (4)].str),(yyvsp[(4) - (4)].str));}
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 105 "2paser.y"
    {fprintf(yyout," vector < vector %s>",(yyvsp[(3) - (5)].str));}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 110 "2paser.y"
    {}
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 111 "2paser.y"
    {}
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 112 "2paser.y"
    {}
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 116 "2paser.y"
    {
						string curName=(yyvsp[(1) - (5)].str);
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=(yyvsp[(1) - (5)].str);
									string currentArgum=(yyvsp[(3) - (5)].str);
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==(yyvsp[(1) - (5)].str))		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=(yyvsp[(3) - (5)].str))
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

										
									append_value["orName"] = (yyvsp[(1) - (5)].str);
									append_value["newName"] = curName;
									
									Json::Value& arlist=append_value["arguementList"];
									string argument=(yyvsp[(3) - (5)].str);
									Json::Value tmp=argument;
									arlist.append(tmp);								
						
									add_value.append(append_value);
									printf("No Name and argument matched!\n");
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							fprintf(yyout," %s %s",curName.c_str(),(yyvsp[(5) - (5)].str));
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %s %s %s",(yyvsp[(1) - (5)].str),(yyvsp[(2) - (5)].str),(yyvsp[(3) - (5)].str),(yyvsp[(4) - (5)].str),(yyvsp[(5) - (5)].str));	
						}
					}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 216 "2paser.y"
    {
						string curName=(yyvsp[(1) - (5)].str);
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=(yyvsp[(1) - (5)].str);
									//string currentArgum=$3;
									int tt=(yyvsp[(3) - (5)].num);
									char dig[4];
									sprintf(dig, "%d", tt);
									string currentArgum=dig;
									//
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==(yyvsp[(1) - (5)].str))		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=currentArgum)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

										
									append_value["orName"] = (yyvsp[(1) - (5)].str);
									append_value["newName"] = curName;
									
									Json::Value& arlist=append_value["arguementList"];
									//string argument=$3;

									int tt=(yyvsp[(3) - (5)].num);
									char dig[4];
									sprintf(dig, "%d", tt);
									string argument=dig;
									//
									Json::Value tmp=argument;
									arlist.append(tmp);								
						
									add_value.append(append_value);
									printf("No Name and argument matched!\n");
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							fprintf(yyout," %s %s",curName.c_str(),(yyvsp[(5) - (5)].str));
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %d %s %s",(yyvsp[(1) - (5)].str),(yyvsp[(2) - (5)].str),(yyvsp[(3) - (5)].num),(yyvsp[(4) - (5)].str),(yyvsp[(5) - (5)].str));	
						}
					}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 327 "2paser.y"
    {
						string curName=(yyvsp[(1) - (3)].str);
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=(yyvsp[(1) - (3)].str);
									string currentArgum=(yyvsp[(3) - (3)].str);
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==(yyvsp[(1) - (3)].str))		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=(yyvsp[(3) - (3)].str))
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

									append_value["orName"] = (yyvsp[(1) - (3)].str);
									append_value["newName"] = curName;
									
									argumList.clear();
									string argument=(yyvsp[(3) - (3)].str);
									argumList.push_back(argument);								
						
									printf("No Name and argument matched!\n");
									fprintf(yyout," %s",curName.c_str());
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							newMultiTempFlag=1; // in the multi-arguments template declaration 
							printf("multi-argument flag=%d\n",newMultiTempFlag);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %s",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));	
						}							
					}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 423 "2paser.y"
    {
						string curName=(yyvsp[(1) - (3)].str);
						//check curName is the parclass template or not
						int tmpFlag=0;
						for (int i = 0; i < sub_value.size(); ++i)
						{
							Json::Value temp_value = sub_value[i];
							if(temp_value["orName"]==curName) 	//ture, it is a parclass template
							{
								printf("locate curName: %s\n", curName.c_str());
								tmpFlag=1;
								
								int Match=1;
								append_value.clear(); //reset global appending value
								printf("argumentList number: %d\n", add_value.size());
								for (int i=0; i < add_value.size(); ++i) //check existed name and arguments
								{
									Json::Value tem= add_value[i];
									string currentName=(yyvsp[(1) - (3)].str);
									//string currentArgum=$3;
									int tt=(yyvsp[(3) - (3)].num);
									char dig[4];
									sprintf(dig, "%d", tt);
									string currentArgum=dig;
									//
									Json::Value& array=tem["arguementList"];
									if(tem["orName"]==(yyvsp[(1) - (3)].str))		//name match with info.json
									{
										printf("check arguments!\n");
										int Flag=0;
										for (int k=0;k<array.size();++k)
										{
											string curArg=array[k].asString();
											printf("string :%s!\n",curArg.c_str());
											if(curArg!=currentArgum)
											{
												Flag=1;
												printf("argument not matched!!\n");
												break;
											}
										}	
										if(Flag==0)
										{
											Match=0; //name fully matched, do nothing
											printf("Name and argument matched! Keep existed name\n");
											curName.clear();
											curName=tem["newName"].asString();
										}
										else 	{Match=1;}  //no name matched	
									}
								}
								if (Match==1)//no name and argument fully matched, add new template information
								{
									curName.append("_");
									int nameIndex=0;
									int mark=0; //found mark
									if (!countList.empty())		//check the original name is existed or not
									{
										for(int t=0;t<countList.size();++t)
										{
											if (countList[t].name==curName)
											{	countList[t].count++;
												nameIndex=countList[t].count;
												mark=1;
											}
										}
									}
									if (mark==0)
									{
										nameCount toto;
										toto.name=curName;
										toto.count=0;
										countList.push_back(toto);
									}
									char num[4];
									sprintf(num,"%d",nameIndex);
									curName.append(num);
									printf("newname: %s\n", curName.c_str()); //generate new name	

									append_value["orName"] = (yyvsp[(1) - (3)].str);
									append_value["newName"] = curName;
									
									argumList.clear();
									//string argument=$3;
									int tt=(yyvsp[(3) - (3)].num);
									char dig[4];
									sprintf(dig, "%d", tt);
									string argument=dig;					
									//
									argumList.push_back(argument);								
						
									printf("No Name and argument matched!\n");
									fprintf(yyout," %s",curName.c_str());
								}	
							}
						}
						if (tmpFlag==1)		//parclass template
						{
							newMultiTempFlag=1; // in the multi-arguments template declaration 
							printf("multi-argument flag=%d\n",newMultiTempFlag);
						}
						else			//normal template
						{
							fprintf(yyout," %s %s %d",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].num));	
						}							
					}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 531 "2paser.y"
    {	
						if(newMultiTempFlag==1)
						{
							printf("multi-argument flag=%d\n",newMultiTempFlag);
							
							string argument=(yyvsp[(2) - (2)].str);		
							argumList.push_back(argument);
																			
							
							printf("New Arguments added to list. %s \n",argument.c_str());
						}
						else 
						{	printf("Arguments ID : %s \n",(yyvsp[(2) - (2)].str));
							fprintf(yyout," %s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
						}
					}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 547 "2paser.y"
    {	
						if(newMultiTempFlag==1)
						{
							printf("multi-argument flag=%d\n",newMultiTempFlag);
							
							//string argument=$2;	

							int tt=(yyvsp[(2) - (2)].num);
							char dig[4];
							sprintf(dig, "%d", tt);
							string argument=dig;
							//	
							argumList.push_back(argument);
																			
							
							printf("New Arguments added to list. %s \n",argument.c_str());
						}
						else 
						{	printf("Arguments ID : %d \n",(yyvsp[(2) - (2)].num));
							fprintf(yyout," %s %d",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].num));
						}
					}
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 569 "2paser.y"
    {
						if (newMultiTempFlag==1)	//end of template
						{
							newMultiTempFlag=0; //end appending operation
							printf("multi-argument flag=%d\n",newMultiTempFlag);								
							printf("End list appending. \n");
							
							Json::Value& arlist=append_value["arguementList"];
							for (int k=0;k<argumList.size();++k)
							{							
								Json::Value tmp=argumList[k];
								arlist.append(tmp);
							}								
						
							add_value.append(append_value);
							

							json_append_file.clear();
							json_append_file = writer.write(root);	
							printf(" %s\n", json_append_file.c_str());
							
							fprintf(yyout," %s",(yyvsp[(2) - (2)].str));
						}
						else 
						{	printf("end Arguments : %s \n",(yyvsp[(2) - (2)].str));
							fprintf(yyout," %s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
						}
					}
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 600 "2paser.y"
    {}
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 601 "2paser.y"
    {}
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 605 "2paser.y"
    {fprintf(yyout,"template %s %s %s %s ",(yyvsp[(1) - (4)].str),(yyvsp[(2) - (4)].str),(yyvsp[(3) - (4)].str),(yyvsp[(4) - (4)].str));}
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 606 "2paser.y"
    {fprintf(yyout,"template %s %s %s, ",(yyvsp[(1) - (4)].str),(yyvsp[(2) - (4)].str),(yyvsp[(3) - (4)].str));}
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 607 "2paser.y"
    {fprintf(yyout,"%s %s, ",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str));}
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 608 "2paser.y"
    {fprintf(yyout,"%s %s %s ",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));}
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 612 "2paser.y"
    {	printf("class name:%s\n",(yyvsp[(1) - (2)].str));	
						fprintf(yyout,"%s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
					}
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 615 "2paser.y"
    {}
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 616 "2paser.y"
    {}
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 620 "2paser.y"
    {	printf("class name:%s\n",(yyvsp[(2) - (3)].str));	
						fprintf(yyout,"%s %s %s",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));
					}
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 623 "2paser.y"
    {	printf("class name:%s\n",(yyvsp[(1) - (2)].str));	
						fprintf(yyout,"%s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));				
					}
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 626 "2paser.y"
    {	printf("end of parameters %s\n",(yyvsp[(1) - (4)].str));
						fprintf(yyout," %s %s%s%s",(yyvsp[(1) - (4)].str),(yyvsp[(2) - (4)].str),(yyvsp[(3) - (4)].str),(yyvsp[(4) - (4)].str));}
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 628 "2paser.y"
    {
						printf("appendition of parameters: %s\n",(yyvsp[(1) - (2)].str));
						fprintf(yyout," %s,",(yyvsp[(1) - (2)].str));
					}
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 632 "2paser.y"
    {printf("method in\n");}
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 636 "2paser.y"
    {fprintf(yyout," %s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));}
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 637 "2paser.y"
    {fprintf(yyout," %s %s",(yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));}
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 638 "2paser.y"
    {fprintf(yyout," %s %s %s",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));}
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 639 "2paser.y"
    {fprintf(yyout," %s %s %s",(yyvsp[(1) - (3)].str),(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));}
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 643 "2paser.y"
    {fprintf(yyout,"#include ");}
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 644 "2paser.y"
    {fprintf(yyout,"#%s ",(yyvsp[(2) - (2)].str));}
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 648 "2paser.y"
    {fprintf(yyout,"<%s>",(yyvsp[(2) - (3)].str));}
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 649 "2paser.y"
    {fprintf(yyout,"<%s>",(yyvsp[(2) - (3)].str));}
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 650 "2paser.y"
    {fprintf(yyout,"<%s%s%s>",(yyvsp[(2) - (5)].str),(yyvsp[(3) - (5)].str),(yyvsp[(4) - (5)].str));}
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 651 "2paser.y"
    {fprintf(yyout,"<vector>");}
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 652 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));}
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 653 "2paser.y"
    {}
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 654 "2paser.y"
    {}
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 658 "2paser.y"
    {fprintf(yyout,"%s",(yyvsp[(1) - (1)].str));}
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 659 "2paser.y"
    {fprintf(yyout,"\n");}
    break;


/* Line 1787 of yacc.c  */
#line 2360 "y.tab.c"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 663 "2paser.y"

	int yywrap()
	{
    return 1;
	}	
	void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	}
	int main(int argv, char** argc) {
	
	const char* sFile=argc[1];
	FILE* infp=fopen(sFile, "r"); 
	
	if(infp==NULL)
	{
		printf("can not open %s \n",sFile);
		return -1;
	}
	

	yyin=infp;
	
	string fileName=argc[1];
	int index1=fileName.find('_',0);
	printf("index1 : %d\n",index1);
	string front=fileName.substr(0,index1);
	int index2=fileName.find('.',0);
	string behind=fileName.substr(index2,fileName.size());
	char num='2';
	string whole=front+"_"+num+behind;

	FILE* outfp=fopen(whole.c_str(), "w"); 
	
	if(outfp==NULL)
	{
		printf("can not open outfile \n");
		return -1;
	}
	
	  
	yyout=outfp;
	
	ofs.open("info.json",ios::app);
	//oofs.open("substitution.json",ios::app);
	

	iifs.open("substitution.json");
	if (!sub_reader.parse(iifs, substitution, false))
    	{ return -1; }

	
	

	
	yyparse();
	ofs << json_append_file;
	//oofs << sub_append_file;
	
	fclose(infp);
	fclose(outfp);
	ofs.close();
	iifs.close();

	return 0;
	}
