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
#line 1 "parser.y"

#include <strings.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <list>
#include <set>

#include "json/json.h"

#include "parser.h"
#include "paroc_utils.h"
#include "debug.h"


 //Declaration in parser.lex:
  
 int PutToken(char *str);
 int PutToken(char chr);
 char *GetToken(int yyval);

 void errormsg(const  char *s);
 void setReturnParam(int pointer, int ref, int const_virtual); // Methode to group code that set C++ specific C++ Param attributes
 void setPOPCMethodeModifier(int settings); // mehtode to group code that set/controlle Methode attributes (sync, conc, ...)
 void errorGlobalMehtode(bool isGlobal);
  
extern "C" {
	int yywrap();
}

void yyerror(const  char *s);
int yylex();

extern int linenumber;
extern char filename[1024];

CArrayCharPtr incl[1000];
CArrayCharPtr sources;
CArrayCharPtr searchpath; 

int indexsource=0;  //the index of source file and the include directive


 extern CArrayChar othercodes;
 extern bool insideClass;
 extern int startPos;


	CodeFile *thisCodeFile;
	Class *currentClass;
	DataType *currenttype;
	DataType *returntype;
 
	std::vector<bool> constPointerPositions; // counter variables who contains order of conts possition in pointers

	TypeClassStruct *seqclass;

	Param *currentparam;

	PackObject *currentPack;
	Structure *structContainer;

	TypeDefinition *typeDefContainer;
	Method *method; // a method
	// Param *param;

 int n,t;
 bool isNamespace = false;
 
 bool isInStruct = false;
 
 bool isParclassDeclared = false;
 
 bool hadParclass = false; 
 
 bool isWarningEnable;
 bool isImplicitPackEnable;
 bool isPOPCPPCompilation; 
 bool isAsyncAllocationDisable;
 char holdnamespace[500];
 char tmp[10240]; /* what is the content of tmp */
 char typetmp[100];
 char tmpProc[10240];
 char tmpSize[10240];
 int parsingclass=0;

 void UpdateMarshalParam(int flags, Param *t);

 int ParseFile(char *infile, char *outfile, bool client, bool broker, bool isWarningEnable, bool isImplicitPackEnable, bool isPOPCPPCompilation, bool isAsyncAllocationDisable);
 char *CheckAndCreateDir(char *fname,char *name);
 bool CheckAndInsert(CArrayCharPtr &source, CArrayCharPtr &searchpath, char *fname);

 int AfterParsing(int status, char *classname, char *stubproc, bool gen_stub);


 paroc_list<TypeClassStruct *>  typestack;
 TypeClassStruct *currentstruct;
 void CleanStack();
 void Push(TypeClassStruct *x);
 TypeClassStruct *Pop();
 TypeClassStruct *Peek();

 int counter=0;
 int errorcode=0;

 AccessType  accessmodifier=PUBLIC;


struct TemplateArgument
{
  DataType *type;
  bool isRef;
};



#define YYMAXDEPTH 100000
#define YYINITDEPTH 2000

/* Line 371 of yacc.c  */
#line 196 "y.tab.c"

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
# define YYERROR_VERBOSE 1
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
     PARCLASS_KEYWORD = 258,
     CLASS_KEYWORD = 259,
     CLASSID = 260,
     ID = 261,
     ID1 = 262,
     INTEGER = 263,
     REAL = 264,
     STRING = 265,
     PUBLIC_KEYWORD = 266,
     PROTECTED_KEYWORD = 267,
     PRIVATE_KEYWORD = 268,
     VIRTUAL_KEYWORD = 269,
     CONST_KEYWORD = 270,
     STRUCT_KEYWORD = 271,
     TYPEDEF_KEYWORD = 272,
     SYNC_INVOKE = 273,
     ASYNC_INVOKE = 274,
     INPUT = 275,
     OUTPUT = 276,
     CONCURRENT = 277,
     SEQUENTIAL = 278,
     MUTEX = 279,
     HIDDEN = 280,
     PROC = 281,
     SIZE = 282,
     THIS_KEYWORD = 283,
     INCLUDE = 284,
     DIRECTIVE = 285,
     OD = 286,
     AUTO_KEYWORD = 287,
     REGISTER_KEYWORD = 288,
     VOLATILE_KEYWORD = 289,
     PACK_KEYWORD = 290,
     AND_OP = 291,
     OR_OP = 292,
     EQUAL_OP = 293,
     NOTEQUAL_OP = 294,
     GREATEREQUAL_OP = 295,
     LESSEQUAL_OP = 296,
     NONSTRICT_OD_OP = 297,
     EOFCODE = 298,
     SCOPE = 299,
     ENUM = 300,
     CLASS = 301,
     NAMESPACE = 302,
     STATIC_KEYWORD = 303,
     UMINUS = 304
   };
#endif
/* Tokens.  */
#define PARCLASS_KEYWORD 258
#define CLASS_KEYWORD 259
#define CLASSID 260
#define ID 261
#define ID1 262
#define INTEGER 263
#define REAL 264
#define STRING 265
#define PUBLIC_KEYWORD 266
#define PROTECTED_KEYWORD 267
#define PRIVATE_KEYWORD 268
#define VIRTUAL_KEYWORD 269
#define CONST_KEYWORD 270
#define STRUCT_KEYWORD 271
#define TYPEDEF_KEYWORD 272
#define SYNC_INVOKE 273
#define ASYNC_INVOKE 274
#define INPUT 275
#define OUTPUT 276
#define CONCURRENT 277
#define SEQUENTIAL 278
#define MUTEX 279
#define HIDDEN 280
#define PROC 281
#define SIZE 282
#define THIS_KEYWORD 283
#define INCLUDE 284
#define DIRECTIVE 285
#define OD 286
#define AUTO_KEYWORD 287
#define REGISTER_KEYWORD 288
#define VOLATILE_KEYWORD 289
#define PACK_KEYWORD 290
#define AND_OP 291
#define OR_OP 292
#define EQUAL_OP 293
#define NOTEQUAL_OP 294
#define GREATEREQUAL_OP 295
#define LESSEQUAL_OP 296
#define NONSTRICT_OD_OP 297
#define EOFCODE 298
#define SCOPE 299
#define ENUM 300
#define CLASS 301
#define NAMESPACE 302
#define STATIC_KEYWORD 303
#define UMINUS 304



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
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
#line 360 "y.tab.c"

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
#define YYFINAL  40
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   701

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  84
/* YYNRULES -- Number of rules.  */
#define YYNRULES  216
/* YYNRULES -- Number of states.  */
#define YYNSTATES  451

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,     2,     2,     2,    53,    54,     2,
      65,    68,    51,    49,    70,    50,    74,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    62,    66,
      61,    59,    60,    63,    73,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,    55,    67,    57,     2,     2,     2,
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
      45,    46,    47,    48,    64
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    10,    14,    17,    20,
      23,    26,    29,    31,    33,    35,    38,    41,    44,    49,
      51,    54,    55,    62,    63,    66,    68,    70,    72,    74,
      76,    81,    84,    87,    89,    90,    94,    97,   102,   103,
     106,   112,   119,   126,   132,   138,   141,   144,   145,   148,
     150,   154,   156,   159,   165,   169,   173,   176,   182,   186,
     189,   190,   193,   195,   199,   201,   204,   208,   212,   214,
     216,   218,   219,   222,   223,   226,   227,   232,   235,   238,
     241,   244,   247,   250,   253,   255,   261,   267,   269,   273,
     275,   279,   282,   284,   288,   293,   294,   297,   302,   305,
     307,   309,   311,   313,   315,   320,   322,   325,   327,   331,
     336,   337,   341,   342,   344,   345,   347,   349,   351,   354,
     355,   358,   364,   366,   371,   377,   379,   388,   398,   408,
     419,   431,   432,   447,   448,   464,   466,   469,   471,   473,
     475,   477,   479,   481,   483,   485,   488,   491,   492,   494,
     496,   499,   502,   504,   505,   510,   511,   524,   535,   544,
     551,   552,   555,   556,   558,   560,   564,   574,   575,   579,
     581,   585,   587,   589,   593,   597,   598,   600,   602,   603,
     605,   606,   609,   612,   614,   616,   620,   624,   628,   632,
     636,   640,   644,   648,   652,   656,   660,   664,   670,   674,
     678,   682,   686,   690,   693,   697,   701,   706,   709,   712,
     715,   718,   720,   722,   724,   728,   730
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      76,     0,    -1,    79,    76,    -1,    -1,    84,    -1,    -1,
     103,    77,    76,    -1,   102,    76,    -1,    30,    76,    -1,
      81,    76,    -1,    87,    76,    -1,    80,    76,    -1,    78,
      -1,    43,    -1,    28,    -1,     1,    66,    -1,     1,    67,
      -1,     1,    43,    -1,    82,    65,    83,    68,    -1,    35,
      -1,     6,    86,    -1,    -1,    47,     6,    85,    69,    76,
      67,    -1,    -1,    70,    83,    -1,    90,    -1,    96,    -1,
      97,    -1,     4,    -1,    48,    -1,    91,    69,    92,    67,
      -1,    91,    80,    -1,    16,     6,    -1,    16,    -1,    -1,
      93,    66,    92,    -1,   121,    94,    -1,   126,     6,   120,
      95,    -1,    -1,    70,    94,    -1,    17,     6,   126,     6,
     120,    -1,    17,    16,     6,   126,     6,   120,    -1,    17,
       4,     6,   126,     6,   120,    -1,    17,    90,   126,     6,
     120,    -1,    98,    99,    69,     1,    67,    -1,    98,    66,
      -1,     4,     6,    -1,    -1,    62,   100,    -1,   101,    -1,
     100,    70,   101,    -1,     6,    -1,   109,     6,    -1,   109,
       6,    61,     6,    60,    -1,    14,   109,     6,    -1,   109,
      14,     6,    -1,   105,    66,    -1,   104,    69,   111,    67,
      66,    -1,   105,   110,   106,    -1,     3,     6,    -1,    -1,
      62,   107,    -1,   108,    -1,   107,    70,   108,    -1,     6,
      -1,   109,     6,    -1,    14,   109,     6,    -1,   109,    14,
       6,    -1,    13,    -1,    11,    -1,    12,    -1,    -1,    59,
       8,    -1,    -1,   113,   111,    -1,    -1,   109,   112,    62,
     111,    -1,   114,    66,    -1,    90,    66,    -1,   129,    66,
      -1,    88,    66,    -1,    89,    66,    -1,    96,    66,    -1,
     117,    66,    -1,    30,    -1,     5,    65,     8,    68,    66,
      -1,    45,     6,    69,   115,    67,    -1,   116,    -1,   116,
      70,   115,    -1,     6,    -1,     6,    59,     8,    -1,   121,
     118,    -1,   119,    -1,   119,    70,   118,    -1,   126,   128,
       6,   120,    -1,    -1,    71,    72,    -1,    71,   155,    72,
     120,    -1,   122,   123,    -1,   123,    -1,    32,    -1,    33,
      -1,     6,    -1,     7,    -1,     6,    61,   124,    60,    -1,
      90,    -1,    16,     6,    -1,   125,    -1,   125,    70,   124,
      -1,   123,   126,   120,   128,    -1,    -1,    51,   127,   126,
      -1,    -1,    15,    -1,    -1,    54,    -1,   131,    -1,   133,
      -1,   135,   130,    -1,    -1,    59,     8,    -1,   132,    65,
     146,    68,   143,    -1,     6,    -1,    57,   134,    65,    68,
      -1,    14,    57,   134,    65,    68,    -1,     6,    -1,   121,
     126,   128,   142,    65,   146,    68,   127,    -1,   140,   121,
     126,   128,   142,    65,   146,    68,   127,    -1,   121,   140,
     126,   128,   142,    65,   146,    68,   127,    -1,   140,   121,
     140,   126,   128,   142,    65,   146,    68,   127,    -1,   138,
     141,   121,   141,   126,   128,   142,    65,   146,    68,   127,
      -1,    -1,    71,   150,    72,   141,   121,   141,   126,   128,
     142,   136,    65,   146,    68,   127,    -1,    -1,   138,    71,
     150,    72,   141,   121,   141,   126,   128,   142,   137,    65,
     146,    68,   127,    -1,   139,    -1,   138,   139,    -1,    18,
      -1,    19,    -1,    22,    -1,    23,    -1,    24,    -1,    25,
      -1,    14,    -1,    15,    -1,    15,    14,    -1,    14,    15,
      -1,    -1,    14,    -1,    15,    -1,    15,    14,    -1,    14,
      15,    -1,     6,    -1,    -1,    73,    69,   144,    67,    -1,
      -1,     6,    74,     6,    65,   155,    70,   155,    70,   155,
      68,    66,   144,    -1,     6,    74,     6,    65,   155,    70,
     155,    68,    66,   144,    -1,     6,    74,     6,    65,   155,
      68,    66,   144,    -1,     6,    59,   155,   145,    66,   144,
      -1,    -1,    42,   155,    -1,    -1,   147,    -1,   148,    -1,
     148,    70,   147,    -1,   149,   152,   121,   152,   126,   128,
     153,   120,   154,    -1,    -1,    71,   150,    72,    -1,   151,
      -1,   151,    70,   150,    -1,    20,    -1,    21,    -1,    27,
      59,   155,    -1,    26,    59,     6,    -1,    -1,    15,    -1,
      34,    -1,    -1,     6,    -1,    -1,    59,   155,    -1,   157,
     120,    -1,   156,    -1,    10,    -1,   155,    49,   155,    -1,
     155,    50,   155,    -1,   155,    51,   155,    -1,   155,    52,
     155,    -1,   155,    53,   155,    -1,   155,    54,   155,    -1,
     155,    55,   155,    -1,   155,    56,   155,    -1,   155,    36,
     155,    -1,   155,    37,   155,    -1,   155,    38,   155,    -1,
     155,    59,   155,    -1,   155,    63,   155,    62,   155,    -1,
     155,    39,   155,    -1,   155,    60,   155,    -1,   155,    61,
     155,    -1,   155,    40,   155,    -1,   155,    41,   155,    -1,
      50,   155,    -1,    65,   155,    68,    -1,   157,    65,    68,
      -1,   157,    65,   158,    68,    -1,    51,   155,    -1,    54,
     155,    -1,    58,   155,    -1,    57,   155,    -1,     8,    -1,
       9,    -1,     6,    -1,     6,    44,     6,    -1,   155,    -1,
     158,    70,   155,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   147,   147,   149,   159,   161,   160,   166,   167,   168,
     169,   170,   174,   177,   375,   382,   395,   408,   424,   434,
     451,   463,   461,   487,   489,   497,   501,   502,   513,   525,
     538,   546,   552,   589,   598,   599,   602,   605,   633,   634,
     643,   685,   715,   739,   770,   775,   777,   794,   795,   799,
     800,   803,   815,   827,   839,   851,   868,   889,   897,   906,
     945,   946,   949,   950,   953,   967,   982,   999,  1018,  1022,
    1026,  1034,  1037,  1046,  1047,  1048,  1048,  1052,  1053,  1054,
    1055,  1055,  1055,  1072,  1081,  1086,  1097,  1108,  1112,  1119,
    1123,  1134,  1136,  1137,  1140,  1166,  1169,  1173,  1185,  1189,
    1197,  1201,  1207,  1218,  1234,  1253,  1259,  1272,  1279,  1288,
    1361,  1365,  1374,  1377,  1384,  1387,  1397,  1398,  1399,  1402,
    1403,  1409,  1425,  1434,  1443,  1456,  1586,  1593,  1600,  1607,
    1614,  1622,  1622,  1629,  1629,  1639,  1643,  1689,  1693,  1697,
    1701,  1705,  1709,  1716,  1720,  1724,  1728,  1736,  1739,  1743,
    1747,  1751,  1757,  1769,  1772,  1779,  1783,  1800,  1816,  1852,
    1945,  1948,  1956,  1957,  1959,  1960,  1963,  2027,  2030,  2036,
    2040,  2046,  2050,  2054,  2059,  2068,  2071,  2075,  2082,  2085,
    2092,  2095,  2108,  2117,  2121,  2125,  2130,  2135,  2140,  2145,
    2150,  2155,  2160,  2165,  2170,  2175,  2180,  2185,  2190,  2195,
    2200,  2205,  2210,  2215,  2220,  2225,  2230,  2236,  2241,  2246,
    2251,  2258,  2262,  2268,  2272,  2280,  2284
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PARCLASS_KEYWORD", "CLASS_KEYWORD",
  "CLASSID", "ID", "ID1", "INTEGER", "REAL", "STRING", "PUBLIC_KEYWORD",
  "PROTECTED_KEYWORD", "PRIVATE_KEYWORD", "VIRTUAL_KEYWORD",
  "CONST_KEYWORD", "STRUCT_KEYWORD", "TYPEDEF_KEYWORD", "SYNC_INVOKE",
  "ASYNC_INVOKE", "INPUT", "OUTPUT", "CONCURRENT", "SEQUENTIAL", "MUTEX",
  "HIDDEN", "PROC", "SIZE", "THIS_KEYWORD", "INCLUDE", "DIRECTIVE", "OD",
  "AUTO_KEYWORD", "REGISTER_KEYWORD", "VOLATILE_KEYWORD", "PACK_KEYWORD",
  "AND_OP", "OR_OP", "EQUAL_OP", "NOTEQUAL_OP", "GREATEREQUAL_OP",
  "LESSEQUAL_OP", "NONSTRICT_OD_OP", "EOFCODE", "SCOPE", "ENUM", "CLASS",
  "NAMESPACE", "STATIC_KEYWORD", "'+'", "'-'", "'*'", "'/'", "'%'", "'&'",
  "'|'", "'^'", "'~'", "'!'", "'='", "'>'", "'<'", "':'", "'?'", "UMINUS",
  "'('", "';'", "'}'", "')'", "'{'", "','", "'['", "']'", "'@'", "'.'",
  "$accept", "startlist", "$@1", "handle_eof", "handle_this",
  "not_care_code", "pack_directive", "pack_header", "object_list",
  "namespace_declaration", "$@2", "rest_object_list", "type_definition",
  "innerclass_definition", "innerclass_static", "struct_definition",
  "struct_head", "struct_body", "struct_element", "struct_elname_list",
  "struct_elname_other", "typedef_definition", "seqclass_definition",
  "seqclass_header", "seqbase_spec", "seqbase_list", "seqbase_specifier",
  "class_prototype", "class_declaration", "class_head", "class_key",
  "base_spec", "base_list", "base_specifier", "access_specifier",
  "pure_class_decl", "member_list", "$@3", "member_declaration",
  "enum_declaration", "enum_members", "enum_member",
  "attribute_definition", "attribute_name_list", "attribute_name",
  "array_declarator", "decl_specifier", "storage_class_specifier",
  "type_specifier", "template_arguments", "template_arg",
  "pointer_specifier", "const_specifier", "ref_specifier",
  "function_definition", "pure_virtual_decl", "constructor_definition",
  "constructor_name", "destructor_definition", "destructor_name",
  "method_definition", "$@4", "$@5", "fct_specifier", "fct_spec",
  "const_virutal_specifier", "const_virutal_empty_specifier",
  "function_name", "od_specifier", "od_exprlist", "od_expr_nonstrict",
  "argument_declaration", "argument_list", "arg_declaration",
  "marshal_decl", "marshal_opt_list", "marshal_opt", "cv_qualifier",
  "argument_name", "arg_default_value", "expr_decl", "number", "expr_name",
  "expr_list", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    43,
      45,    42,    47,    37,    38,   124,    94,   126,    33,    61,
      62,    60,    58,    63,   304,    40,    59,   125,    41,   123,
      44,    91,    93,    64,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    76,    76,    77,    76,    76,    76,    76,
      76,    76,    76,    78,    79,    80,    80,    80,    81,    82,
      83,    85,    84,    86,    86,    87,    87,    87,    88,    89,
      90,    90,    91,    91,    92,    92,    93,    94,    95,    95,
      96,    96,    96,    96,    97,    97,    98,    99,    99,   100,
     100,   101,   101,   101,   101,   101,   102,   103,   104,   105,
     106,   106,   107,   107,   108,   108,   108,   108,   109,   109,
     109,   110,   110,   111,   111,   112,   111,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   114,   115,   115,   116,
     116,   117,   118,   118,   119,   120,   120,   120,   121,   121,
     122,   122,   123,   123,   123,   123,   123,   124,   124,   125,
     126,   126,   127,   127,   128,   128,   129,   129,   129,   130,
     130,   131,   132,   133,   133,   134,   135,   135,   135,   135,
     135,   136,   135,   137,   135,   138,   138,   139,   139,   139,
     139,   139,   139,   140,   140,   140,   140,   141,   141,   141,
     141,   141,   142,   143,   143,   144,   144,   144,   144,   144,
     145,   145,   146,   146,   147,   147,   148,   149,   149,   150,
     150,   151,   151,   151,   151,   152,   152,   152,   153,   153,
     154,   154,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   157,   158,   158
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     0,     3,     2,     2,     2,
       2,     2,     1,     1,     1,     2,     2,     2,     4,     1,
       2,     0,     6,     0,     2,     1,     1,     1,     1,     1,
       4,     2,     2,     1,     0,     3,     2,     4,     0,     2,
       5,     6,     6,     5,     5,     2,     2,     0,     2,     1,
       3,     1,     2,     5,     3,     3,     2,     5,     3,     2,
       0,     2,     1,     3,     1,     2,     3,     3,     1,     1,
       1,     0,     2,     0,     2,     0,     4,     2,     2,     2,
       2,     2,     2,     2,     1,     5,     5,     1,     3,     1,
       3,     2,     1,     3,     4,     0,     2,     4,     2,     1,
       1,     1,     1,     1,     4,     1,     2,     1,     3,     4,
       0,     3,     0,     1,     0,     1,     1,     1,     2,     0,
       2,     5,     1,     4,     5,     1,     8,     9,     9,    10,
      11,     0,    14,     0,    15,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     1,     1,
       2,     2,     1,     0,     4,     0,    12,    10,     8,     6,
       0,     2,     0,     1,     1,     3,     9,     0,     3,     1,
       3,     1,     1,     3,     3,     0,     1,     1,     0,     1,
       0,     2,     2,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     3,     3,
       3,     3,     3,     2,     3,     3,     4,     2,     2,     2,
       2,     1,     1,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    33,     0,    14,     0,    19,    13,
       0,     0,    12,     0,     0,     0,     0,     4,     0,    25,
       0,    26,    27,    47,     0,     5,     0,    71,    17,    15,
      16,    59,    46,    32,     0,   110,    33,   110,     8,    21,
       1,     2,    11,     9,     0,    10,    34,    31,     0,    45,
       0,     7,     0,    73,     0,    56,    60,   110,   112,     0,
      32,     0,     0,    23,     0,   102,   103,    33,   100,   101,
     105,     0,     0,   110,     0,    99,    51,    69,    70,    68,
       0,    48,    49,     0,     0,     6,    28,     0,   102,   143,
     144,   137,   138,   139,   140,   141,   142,    84,     0,    29,
       0,     0,     0,     0,   105,     0,    75,     0,    73,     0,
       0,   110,     0,   116,     0,   117,   119,   147,   135,     0,
      72,     0,    58,     0,   113,   110,    95,     0,    95,     0,
       0,    20,    18,     0,   106,    30,    34,    36,     0,    98,
       0,     0,    52,     0,     0,     0,   146,     0,   145,     0,
     125,     0,   171,   172,     0,     0,     0,   169,    80,    81,
      78,    82,     0,     0,    74,    77,    83,   143,    91,    92,
     114,   110,    79,   167,     0,   118,   148,   149,     0,   136,
       0,   110,    64,     0,    61,    62,     0,    95,   111,     0,
      40,    95,    43,     0,    24,   110,     0,   107,    35,    95,
      54,    50,     0,    55,    44,     0,     0,     0,     0,     0,
       0,   147,     0,    73,    57,   110,   115,     0,   114,     0,
       0,   163,   164,   175,   120,   151,   150,     0,   147,   114,
     110,     0,     0,    65,     0,    42,   213,   211,   212,   184,
       0,     0,     0,     0,     0,     0,    96,     0,   183,    95,
      41,    22,    95,   104,     0,    38,     0,     0,     0,    89,
       0,    87,   123,   174,   173,     0,   170,    76,    93,   114,
      95,     0,     0,     0,   153,   167,   176,   177,     0,   147,
     110,     0,   114,    66,    63,    67,     0,   203,   207,   208,
     210,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,   182,   114,   108,   110,    37,    53,    85,
     124,     0,    86,     0,   147,     0,    94,   167,   152,     0,
     168,     0,   121,   165,   175,     0,   114,     0,     0,   214,
     204,   193,   194,   195,   198,   201,   202,   185,   186,   187,
     188,   189,   190,   191,   192,   196,   199,   200,     0,    97,
     205,   215,     0,   109,    39,    90,    88,   110,    95,     0,
     167,   155,   110,   147,     0,   167,     0,     0,   206,     0,
     114,   112,     0,     0,     0,   114,   110,     0,     0,   167,
     197,   216,     0,   126,   112,     0,     0,   154,   178,   114,
     167,   112,     0,   131,   128,   160,     0,   179,    95,     0,
       0,   127,   112,     0,     0,     0,     0,   180,   133,   112,
     129,   167,   161,   155,     0,     0,   166,     0,   130,     0,
     159,     0,     0,   181,   167,   112,   155,     0,     0,   132,
     158,     0,     0,   112,   155,     0,   134,   157,     0,   155,
     156
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    11,    52,    12,    13,    14,    15,    16,    64,    17,
      62,   131,    18,   102,   103,    70,    20,    71,    72,   137,
     317,    21,    22,    23,    50,    81,    82,    24,    25,    26,
      27,   122,   184,   185,   106,    56,   107,   162,   108,   109,
     260,   261,   110,   168,   169,   326,   111,    74,    75,   196,
     197,   138,   125,   217,   112,   175,   113,   114,   115,   151,
     116,   413,   427,   117,   118,   119,   180,   271,   332,   384,
     415,   220,   221,   222,   223,   156,   157,   278,   408,   426,
     247,   248,   249,   362
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -286
static const yytype_int16 yypact[] =
{
     389,   -16,    30,    80,   108,    55,  -286,   149,  -286,  -286,
     115,    17,  -286,   149,   149,   149,    67,  -286,   149,  -286,
      13,  -286,  -286,   131,   149,  -286,    78,    59,  -286,  -286,
    -286,  -286,  -286,  -286,   165,   121,   179,   121,  -286,  -286,
    -286,  -286,  -286,  -286,   181,  -286,    31,  -286,   156,  -286,
     134,  -286,   149,   249,   182,  -286,   138,   121,   184,   200,
       2,   208,   148,   153,   139,   164,  -286,   223,  -286,  -286,
    -286,   167,   166,   121,   110,  -286,  -286,  -286,  -286,  -286,
      43,   168,  -286,    66,   234,  -286,  -286,   171,   137,    11,
     225,  -286,  -286,  -286,  -286,  -286,  -286,  -286,   231,  -286,
     237,   201,   174,   178,   180,   183,  -286,   185,   249,   191,
     192,    34,   203,  -286,   186,  -286,   188,   363,  -286,    31,
    -286,   198,  -286,   242,  -286,   121,   199,   269,   199,   288,
     181,  -286,  -286,   110,    15,  -286,    31,  -286,   270,  -286,
     271,   156,   217,   274,   216,   276,  -286,   237,  -286,   218,
    -286,   220,  -286,  -286,   229,   236,   221,   226,  -286,  -286,
    -286,  -286,   241,   233,  -286,  -286,  -286,   294,  -286,   243,
     253,   121,  -286,   -37,   306,  -286,   300,   303,   201,  -286,
      31,    34,  -286,    43,   251,  -286,    75,   199,  -286,   292,
    -286,   199,  -286,   252,  -286,   121,   262,   255,  -286,   199,
    -286,  -286,   323,  -286,  -286,   265,   273,   324,   266,   330,
     318,   141,   201,   249,  -286,   121,  -286,   333,   253,   201,
     272,  -286,   275,    18,  -286,  -286,  -286,   282,   141,   253,
     121,   335,   198,  -286,   338,  -286,   304,  -286,  -286,  -286,
     318,   318,   318,   318,   318,   318,  -286,   390,  -286,    57,
    -286,  -286,   199,  -286,   110,   277,   298,   295,   297,   307,
     296,   301,  -286,  -286,   610,    31,  -286,  -286,  -286,   253,
      92,   308,   356,   312,   321,   309,  -286,  -286,    31,   141,
     121,   356,   253,  -286,  -286,  -286,   368,  -286,   638,   360,
     360,   360,   488,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   199,   302,  -286,   253,  -286,   121,  -286,  -286,  -286,
    -286,   371,  -286,   324,   141,   385,  -286,   -37,  -286,   337,
    -286,   326,  -286,  -286,    18,    31,   253,   339,   356,  -286,
    -286,   340,   340,   340,   340,   340,   340,   638,   638,   638,
     638,   638,   360,   360,   360,   360,   340,   340,   554,  -286,
    -286,   610,    63,  -286,  -286,  -286,  -286,   121,   199,   341,
     -37,   401,   121,   141,   356,   -37,   343,   318,  -286,   318,
     253,   184,   342,   -14,   344,   253,   121,   347,   345,   -37,
    -286,   610,   356,  -286,   184,   318,   408,  -286,   409,   253,
     -37,   184,   348,  -286,  -286,   582,   353,  -286,   199,   356,
     354,  -286,   184,   370,   318,   359,   318,   374,  -286,   184,
    -286,   -37,   610,   401,   418,   318,  -286,   372,  -286,   379,
    -286,   382,   318,   610,   -37,   184,   401,   453,   384,  -286,
    -286,   394,   318,   184,   401,   521,  -286,  -286,   395,   401,
    -286
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -286,   130,  -286,  -286,  -286,   443,  -286,  -286,   334,  -286,
    -286,  -286,  -286,  -286,  -286,     5,  -286,   329,  -286,   122,
    -286,   -38,  -286,  -286,  -286,  -286,   325,  -286,  -286,  -286,
    -286,  -286,  -286,   244,    -2,  -286,  -101,  -286,  -286,  -286,
     152,  -286,  -286,   267,  -286,  -122,   -45,  -286,   -71,   230,
    -286,   -35,  -193,  -194,  -286,  -286,  -286,  -286,  -286,   336,
    -286,  -286,  -286,  -286,   378,   -98,  -200,  -251,  -286,  -285,
    -286,  -260,   205,  -286,  -286,  -146,  -286,   151,  -286,  -286,
    -201,  -286,  -286,  -286
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -163
static const yytype_int16 yytable[] =
{
      59,    73,    61,   139,   190,    19,   192,   164,  -110,   264,
      37,   265,    19,   171,     1,   105,   -32,    40,    19,    19,
      19,   329,   123,    19,   272,   127,   146,    28,   280,    19,
     337,  -162,   227,   276,   219,   281,    31,    65,    66,   287,
     288,   289,   290,   291,   292,   395,    83,    67,   167,    90,
      29,    30,   277,    58,    77,    78,    79,    19,   104,    34,
     396,    35,   195,    68,    69,   235,   266,   369,   147,   250,
     105,    36,   142,   273,   181,   325,   170,   255,   140,   335,
     143,   233,    46,   230,   -32,    58,    32,   376,   338,   234,
     188,    73,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     382,   361,   267,   104,    33,   388,    65,    66,    54,   186,
     363,    39,   312,   387,   367,    55,    67,   313,   189,   402,
     314,   378,    44,   379,    19,   228,   218,    38,   430,    83,
     410,   403,   374,    41,    42,    43,   229,    53,    45,    -3,
       1,   440,     2,     3,    51,   176,   177,  -152,   418,   447,
     252,   429,    76,   189,   450,     4,     5,    77,    78,    79,
      80,    57,    58,   386,   438,   105,   390,     6,   391,     7,
     269,   231,    85,   195,     8,    60,   392,    63,   393,   359,
     120,   398,     9,    48,   405,   282,    10,    49,   133,   124,
     121,   404,  -122,    84,   182,   409,   126,   132,   411,    77,
      78,    79,   183,   422,   128,   424,    -3,   129,   104,   420,
     324,   152,   153,   130,   433,   133,   428,   154,   155,   134,
     186,   437,   136,   334,   135,   144,   145,   149,   141,   148,
     158,   445,   439,   150,   159,   336,   160,   174,   187,   161,
     446,   173,   163,    86,    87,    88,    66,   165,   166,   193,
      77,    78,    79,    89,    90,    67,     5,    91,    92,   172,
     189,    93,    94,    95,    96,   191,   199,   200,   202,    97,
     203,    68,    69,   204,   205,   208,   417,   207,   209,     1,
     373,     2,     3,   211,    98,   210,   212,    99,   236,   214,
     237,   238,   239,   213,     4,     5,   100,   216,   236,   146,
     237,   238,   239,   215,   224,   225,     6,   226,     7,   251,
     101,   232,   253,     8,   236,   254,   237,   238,   239,   256,
     259,     9,   380,   257,   262,    10,   263,   385,   258,   270,
     274,   283,   240,   241,   285,   275,   242,   316,   286,   243,
     244,   399,   240,   241,   279,    -3,   242,   245,   318,   243,
     244,   319,   328,   322,   246,   320,   321,   245,   240,   241,
     360,   323,   242,   327,   339,   243,   244,   176,   177,   365,
     219,    91,    92,   245,   330,    93,    94,    95,    96,    -3,
       1,   368,     2,     3,   331,   371,   293,   294,   295,   296,
     297,   298,   370,   310,   375,     4,     5,   383,   389,   381,
     394,   397,   400,   401,   406,   407,   412,     6,   416,     7,
     308,   309,   419,   310,     8,   423,   293,   294,   295,   296,
     297,   298,     9,   425,   178,   421,    10,   434,   364,   299,
     300,   301,   302,   303,   304,   305,   306,   435,   436,   307,
     308,   309,   443,   310,   293,   294,   295,   296,   297,   298,
     444,   449,   311,    47,   194,   198,   201,   299,   300,   301,
     302,   303,   304,   305,   306,   366,   284,   307,   308,   309,
     333,   310,   268,   206,   315,   372,   431,     0,   432,   293,
     294,   295,   296,   297,   298,   179,     0,     0,     0,     0,
       0,     0,   299,   300,   301,   302,   303,   304,   305,   306,
       0,     0,   307,   308,   309,     0,   310,     0,     0,     0,
       0,   441,     0,   442,   293,   294,   295,   296,   297,   298,
       0,     0,     0,     0,     0,     0,     0,   299,   300,   301,
     302,   303,   304,   305,   306,     0,     0,   307,   308,   309,
       0,   310,     0,     0,     0,     0,   340,   293,   294,   295,
     296,   297,   298,     0,     0,     0,     0,     0,     0,     0,
     299,   300,   301,   302,   303,   304,   305,   306,     0,     0,
     307,   308,   309,     0,   310,     0,     0,     0,     0,   448,
     293,   294,   295,   296,   297,   298,     0,     0,     0,     0,
       0,     0,     0,   299,   300,   301,   302,   303,   304,   305,
     306,     0,     0,   307,   308,   309,   377,   310,   293,   294,
     295,   296,   297,   298,   414,     0,     0,     0,     0,     0,
       0,   299,   300,   301,   302,   303,   304,   305,   306,     0,
       0,   307,   308,   309,     0,   310,   293,   294,   295,   296,
     297,   298,     0,     0,     0,     0,     0,     0,     0,   299,
     300,   301,   302,   303,   304,   305,   306,     0,     0,   307,
     308,   309,     0,   310,   293,   294,   295,   296,   297,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,   305,   306,     0,     0,   307,   308,   309,
       0,   310
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-286)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      35,    46,    37,    74,   126,     0,   128,   108,     6,   210,
       5,   211,     7,   111,     1,    53,     1,     0,    13,    14,
      15,   272,    57,    18,   218,    60,    15,    43,   228,    24,
     281,    68,   178,    15,    71,   229,     6,     6,     7,   240,
     241,   242,   243,   244,   245,    59,    48,    16,    14,    15,
      66,    67,    34,    51,    11,    12,    13,    52,    53,     4,
      74,     6,   133,    32,    33,   187,   212,   327,    57,   191,
     108,    16,     6,   219,   119,   269,   111,   199,    80,   279,
      14,     6,    69,   181,    69,    51,     6,   338,   282,    14,
     125,   136,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     370,   312,   213,   108,     6,   375,     6,     7,    59,   121,
     314,     6,    65,   374,   324,    66,    16,   249,    71,   389,
     252,    68,    65,    70,   129,   180,   171,     7,   423,   141,
     400,   392,   336,    13,    14,    15,   181,    69,    18,     0,
       1,   436,     3,     4,    24,    14,    15,    65,   409,   444,
     195,   421,     6,    71,   449,    16,    17,    11,    12,    13,
      14,     6,    51,   373,   434,   213,   377,    28,   379,    30,
     215,   183,    52,   254,    35,     6,   380,     6,   381,   311,
       8,   385,    43,    62,   395,   230,    47,    66,    61,    15,
      62,   394,    65,    69,     6,   399,     6,    68,   401,    11,
      12,    13,    14,   414,     6,   416,    67,    69,   213,   412,
     265,    20,    21,    70,   425,    61,   419,    26,    27,     6,
     232,   432,    66,   278,    67,     1,    65,     6,    70,    14,
      66,   442,   435,     6,    66,   280,    66,    59,     6,    66,
     443,    65,    67,     4,     5,     6,     7,    66,    66,   129,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    66,
      71,    22,    23,    24,    25,     6,     6,     6,    61,    30,
       6,    32,    33,    67,     8,    65,   408,    69,    59,     1,
     335,     3,     4,    72,    45,    59,    70,    48,     6,    66,
       8,     9,    10,    62,    16,    17,    57,    54,     6,    15,
       8,     9,    10,    70,     8,    15,    28,    14,    30,    67,
      71,    70,    60,    35,     6,    70,     8,     9,    10,     6,
       6,    43,   367,    68,    68,    47,     6,   372,    65,     6,
      68,     6,    50,    51,     6,    70,    54,    70,    44,    57,
      58,   386,    50,    51,    72,    67,    54,    65,    60,    57,
      58,    66,     6,    67,    72,    68,    59,    65,    50,    51,
      68,    70,    54,    65,     6,    57,    58,    14,    15,     8,
      71,    18,    19,    65,    72,    22,    23,    24,    25,     0,
       1,     6,     3,     4,    73,    69,    36,    37,    38,    39,
      40,    41,    65,    63,    65,    16,    17,     6,    65,    68,
      68,    67,    65,    68,     6,     6,    68,    28,    65,    30,
      60,    61,    68,    63,    35,    66,    36,    37,    38,    39,
      40,    41,    43,    59,    71,    65,    47,    65,   316,    49,
      50,    51,    52,    53,    54,    55,    56,    68,    66,    59,
      60,    61,    68,    63,    36,    37,    38,    39,    40,    41,
      66,    66,    72,    20,   130,   136,   141,    49,    50,    51,
      52,    53,    54,    55,    56,   323,   232,    59,    60,    61,
     275,    63,   215,   147,   254,   334,    68,    -1,    70,    36,
      37,    38,    39,    40,    41,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    -1,    59,    60,    61,    -1,    63,    -1,    -1,    -1,
      -1,    68,    -1,    70,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    59,    60,    61,
      -1,    63,    -1,    -1,    -1,    -1,    68,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    -1,
      59,    60,    61,    -1,    63,    -1,    -1,    -1,    -1,    68,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    59,    60,    61,    -1,    63,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    -1,    63,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    -1,    -1,    59,    60,    61,
      -1,    63
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,    16,    17,    28,    30,    35,    43,
      47,    76,    78,    79,    80,    81,    82,    84,    87,    90,
      91,    96,    97,    98,   102,   103,   104,   105,    43,    66,
      67,     6,     6,     6,     4,     6,    16,    90,    76,     6,
       0,    76,    76,    76,    65,    76,    69,    80,    62,    66,
      99,    76,    77,    69,    59,    66,   110,     6,    51,   126,
       6,   126,    85,     6,    83,     6,     7,    16,    32,    33,
      90,    92,    93,   121,   122,   123,     6,    11,    12,    13,
      14,   100,   101,   109,    69,    76,     4,     5,     6,    14,
      15,    18,    19,    22,    23,    24,    25,    30,    45,    48,
      57,    71,    88,    89,    90,    96,   109,   111,   113,   114,
     117,   121,   129,   131,   132,   133,   135,   138,   139,   140,
       8,    62,   106,   126,    15,   127,     6,   126,     6,    69,
      70,    86,    68,    61,     6,    67,    66,    94,   126,   123,
     109,    70,     6,    14,     1,    65,    15,    57,    14,     6,
       6,   134,    20,    21,    26,    27,   150,   151,    66,    66,
      66,    66,   112,    67,   111,    66,    66,    14,   118,   119,
     126,   140,    66,    65,    59,   130,    14,    15,    71,   139,
     141,   121,     6,    14,   107,   108,   109,     6,   126,    71,
     120,     6,   120,    76,    83,   123,   124,   125,    92,     6,
       6,   101,    61,     6,    67,     8,   134,    69,    65,    59,
      59,    72,    70,    62,    66,    70,    54,   128,   126,    71,
     146,   147,   148,   149,     8,    15,    14,   150,   121,   126,
     140,   109,    70,     6,    14,   120,     6,     8,     9,    10,
      50,    51,    54,    57,    58,    65,    72,   155,   156,   157,
     120,    67,   126,    60,    70,   120,     6,    68,    65,     6,
     115,   116,    68,     6,   155,   141,   150,   111,   118,   126,
       6,   142,   128,   150,    68,    70,    15,    34,   152,    72,
     141,   128,   126,     6,   108,     6,    44,   155,   155,   155,
     155,   155,   155,    36,    37,    38,    39,    40,    41,    49,
      50,    51,    52,    53,    54,    55,    56,    59,    60,    61,
      63,    72,    65,   120,   120,   124,    70,    95,    60,    66,
      68,    59,    67,    70,   121,   128,   120,    65,     6,   142,
      72,    73,   143,   147,   121,   141,   126,   142,   128,     6,
      68,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   120,
      68,   155,   158,   128,    94,     8,   115,   141,     6,   146,
      65,    69,   152,   121,   128,    65,   142,    62,    68,    70,
     126,    68,   146,     6,   144,   126,   141,   142,   146,    65,
     155,   155,   128,   127,    68,    59,    74,    67,   128,   126,
      65,    68,   146,   142,   127,   155,     6,     6,   153,   128,
     146,   127,    68,   136,    42,   145,    65,   120,   142,    68,
     127,    65,   155,    66,   155,    59,   154,   137,   127,   146,
     144,    68,    70,   155,    65,    68,    66,   155,   146,   127,
     144,    68,    70,    68,    66,   155,   127,   144,    68,    66,
     144
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
        case 3:
/* Line 1787 of yacc.c  */
#line 149 "parser.y"
    {
	if (othercodes.GetSize()) {
		assert(thisCodeFile!=NULL);
		OtherCode *dat=new OtherCode(thisCodeFile);
		dat->AddCode(othercodes);
		thisCodeFile->AddCodeData(dat);
		othercodes.SetSize(0);
	}
}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 161 "parser.y"
    { 
	insideClass=false; 
	othercodes.SetSize(0); 
	startPos=-1;
}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 171 "parser.y"
    {
	CleanStack(); 
}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 178 "parser.y"
    {
	


	if(isImplicitPackEnable){     
	 
		/** 
		 * The following code handles the implicit @pack directive if it's not specified by the developer 
		 * Algo for implicit pack
		 * 1. Look for all included .ph file in the current file
		 * 2. For all .ph file find the parclass(s) included in it
		 * 3. Match the found parclass(s) with the code in the current file (ParClassName::). If match, put in pack directive
		 */
		 
		
		/** 
		 * Perform this task only on file with implementation (.C, .cc, .cpp, .cp, .cxx, .c++, .CPP)
		 * Find the extension of the file in the current C++ extension 
		 */
		std::string fname(filename);
				
		std::size_t cc = fname.find(".cc");
		std::size_t cpp = fname.find(".cpp");
		std::size_t cp = fname.find(".cp");		
		std::size_t cxx = fname.find(".cxx");	
		std::size_t cplusplus = fname.find(".c++");	
		std::size_t CPP = fname.find(".CPP");	
		std::size_t C = fname.find(".C");	
			
		if((cc != std::string::npos || cpp != std::string::npos || cxx != std::string::npos || cp != std::string::npos || cplusplus != std::string::npos 
			|| CPP != std::string::npos || C != std::string::npos) && !isParclassDeclared){	
				
			/**
			 * Find all .ph files included in the current file
			 */
			 
			// Create a list to store all .ph files' name
			std::list<std::string> ph_files; 
			 
			std::fstream cc_real_file; 
			cc_real_file.open(fname.c_str(), std::ios_base::out | std::ios_base::in);

			if (cc_real_file.is_open()){
				// Find name of the parclass
				char line[512]; 
				bool notEOF = true;
				while (notEOF){
					cc_real_file.getline(line, 512);
					if(cc_real_file.eof()){
						notEOF = false;
						cc_real_file.close();
					} else {
						// Get the next line
						std::string str_line(line);
						// Find a .ph include in the current file
						std::size_t ph_extension_pos = str_line.find(".ph"); 
						if(ph_extension_pos != std::string::npos){
							std::size_t ph_file_name_start = str_line.rfind('"', ph_extension_pos);
							std::string ph_file_name = str_line.substr(ph_file_name_start+1, (ph_extension_pos-ph_file_name_start)+2);
							// Save the .ph file found
							ph_files.push_back(ph_file_name);
						}
					}
				}
			}	
			
			
			if(!ph_files.empty()){
				/**
				 * Find all the parclass declared in the .ph files found before. 
				 */
				std::list<std::string> parclass_names;
				std::list<std::string>::iterator it;
				for(it=ph_files.begin(); it != ph_files.end(); it++) {
					std::fstream ph_real_file;			
					ph_real_file.open((*it).c_str(), std::ios_base::out | std::ios_base::in);				
					if (ph_real_file.is_open()){
						// Find name of the parclass
						char line[512]; 
						bool notEOF = true;
						while (notEOF){
							ph_real_file.getline(line, 512);
							if(ph_real_file.eof()){
								notEOF = false;
								ph_real_file.close();
							} else {
								std::string str_line(line);
								std::size_t parclass_pos = str_line.find("parclass"); 
								if(parclass_pos != std::string::npos){
									std::size_t parclass_name_start = str_line.find(" ", parclass_pos); 
									std::size_t parclass_name_stop = str_line.find(" ", parclass_name_start+1);
									if(parclass_name_stop == std::string::npos){
										parclass_name_stop = str_line.find(";", parclass_name_start+1); 
									}
									if(parclass_name_stop == std::string::npos){
										parclass_name_stop = str_line.find("{", parclass_name_start+1); 																	
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\n", parclass_name_start+1); 		
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\r", parclass_name_start+1); 		
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.find("\r\n", parclass_name_start+1); 						
									}
									if(parclass_name_stop == std::string::npos){			
										parclass_name_stop = str_line.size();
									}
									if(parclass_name_stop != std::string::npos && parclass_name_start != std::string::npos){
										std::string parclass_name = str_line.substr(parclass_name_start, (parclass_name_stop - parclass_name_start));
										// Removed possible withespaces from the parclass name
										for(int i=0; i < parclass_name.length(); i++){
											if(parclass_name[i] == ' '){
												parclass_name.erase(i, 1);
												i--;
											}
										}
										// Save the parclass's name found
										parclass_names.push_back(parclass_name);
									}
								}
							}	
						}			
					}
				}	
				/**
				 * Find implementation of the found parclass in the current file
				 */
				std::set<std::string> matched_parclasses;			
				cc_real_file.open(fname.c_str(), std::ios_base::out | std::ios_base::in);
				if (cc_real_file.is_open()){
					// Find name of the parclass
					char line[512]; 
					bool notEOF = true;
					while (notEOF){
						cc_real_file.getline(line, 512);
						if(cc_real_file.eof()){
							notEOF = false;
							cc_real_file.close();
						} else {
							// Get the next line
							std::string str_line(line);
							std::list<std::string>::iterator it;
							for(it=parclass_names.begin(); it != parclass_names.end(); it++) {
								std::string prefix = (*it);
								prefix.append("::");
								std::size_t pos = str_line.find(prefix);
								if(pos != std::string::npos){
									// Saved the matched parclass in a set
									matched_parclasses.insert((*it));
								}
							}
						}
					}
				}	
			
				if(!matched_parclasses.empty()){
					// Implict add the pack directive
					if (othercodes.GetSize() && startPos>0) {
						assert(thisCodeFile!=NULL);
						OtherCode *dat=new OtherCode(thisCodeFile);
						dat->AddCode((char *)othercodes,startPos);
						thisCodeFile->AddCodeData(dat);
						othercodes.SetSize(0);
					}  
					startPos=-1;
					currentPack=new PackObject(thisCodeFile);
					currentPack->SetStartLine(linenumber-1);
					thisCodeFile->AddCodeData(currentPack); 
					std::set<std::string>::iterator it;
					for(it=matched_parclasses.begin() ; it != matched_parclasses.end(); it++ ){
						if (currentPack!=NULL) {

							char * objname = new char[(*it).size() + 1];
							std::copy((*it).begin(), (*it).end(), objname);
							objname[(*it).size()] = '\0';
							currentPack->AddObject(objname);
							// Ok file exists, so we will find the parclass name and add the pack directive with it
							if(isWarningEnable){
								std::cout << filename << ":" << linenumber << ": Warning: No @pack directive for class: " << objname;
								std::cout << ", @pack(" << objname << ") is assumed..." << std::endl;
							}							
							delete [] objname;
						}
					}
					isParclassDeclared = true; 
					currentPack->SetEndLine(linenumber-1);
					currentPack=NULL;
					othercodes.SetSize(0);
					startPos=-1;
				}
			}
		}
	}	
}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 376 "parser.y"
    {
	othercodes.InsertAt(-1,"\n",strlen("\n"));
}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 383 "parser.y"
    {
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    }  
}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 396 "parser.y"
    {
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    }  
}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 409 "parser.y"
    {
  startPos=-1;
  insideClass=false;
  if (othercodes.GetSize())
    {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,othercodes.GetSize());
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
    } 
  YYACCEPT;
}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 425 "parser.y"
    {
	
	isParclassDeclared = true; 
	currentPack->SetEndLine(linenumber-1);
  	currentPack=NULL;
  	othercodes.SetSize(0);
  	startPos=-1;
}
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 435 "parser.y"
    {
  	if (othercodes.GetSize() && startPos>0) {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,startPos);
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
	}  
	startPos=-1;
	currentPack=new PackObject(thisCodeFile);
	currentPack->SetStartLine(linenumber-1);
	thisCodeFile->AddCodeData(currentPack);  
}
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 452 "parser.y"
    {
  		if (currentPack!=NULL) {
			currentPack->AddObject(GetToken((yyvsp[(1) - (2)])));
		}
}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 463 "parser.y"
    {
	
	// Avoid handling of standard namespace used in POP-C++
	
	if(strcmp("__gnu_cxx", GetToken((yyvsp[(2) - (2)]))) != 0 &&  strcmp("std", GetToken((yyvsp[(2) - (2)]))) != 0 && strcmp("__gnu_debug", GetToken((yyvsp[(2) - (2)]))) != 0 && strcmp("rel_ops", GetToken((yyvsp[(2) - (2)]))) != 0  && strcmp("__debug", GetToken((yyvsp[(2) - (2)]))) != 0){
			
		isNamespace = true;
			
		sprintf(holdnamespace, "%s", GetToken((yyvsp[(2) - (2)])));
	
	}

}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 478 "parser.y"
    {
	
	isNamespace = false;

}
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 487 "parser.y"
    {
}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 490 "parser.y"
    {
}
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 498 "parser.y"
    {
    CleanStack();
}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 514 "parser.y"
    {
	
	sprintf(tmp,"Class declaration inside parclass are not currently supported in POP-C++ !\n");
	
	errormsg(tmp);
	exit(1);

}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 525 "parser.y"
    {
	sprintf(tmp,"Use of static member inside parclass is not currently supported in POP-C++!\n");
	
	errormsg(tmp);
	exit(1);

}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 539 "parser.y"
    {
  	currentstruct=Pop();
  	if (currentstruct!=NULL) currentstruct->SetTypeClass(false);
  	(yyval)=(yyvsp[(1) - (4)]);
  	structContainer = NULL;
  	isInStruct = false;
}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 547 "parser.y"
    {
  currentstruct=Pop();
  (yyval)=(yyvsp[(1) - (2)]);
}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 553 "parser.y"
    {
	
	if(currentClass!=NULL){
		
		isInStruct = true;
		
		structContainer = new Structure(currentClass, accessmodifier);
		
		structContainer->SetLineInfo(linenumber-1);
		
		currentClass->AddMember(structContainer);
      
		structContainer->setName(GetToken((yyvsp[(2) - (2)])));	
	
	}
  	char *tname=GetToken((yyvsp[(2) - (2)]));
	DataType *type=thisCodeFile->FindDataType(tname);
  	TypeClassStruct *t;
  	if (type!=NULL){
		t=dynamic_cast<TypeClassStruct *>(type);
		if (t==NULL) {
	  //	  	  thisCodeFile->RemoveDataType(type);
	  //	  	  delete type;
	  t=new TypeClassStruct(tname, false);
	  thisCodeFile->AddDataType(t);
	  //	  sprintf(tmp,"data type \"%s\" has been redefined!\n",tname);
	  //	  errormsg(tmp);
	  //	  exit(1);
		}
	} else { 
      t=new TypeClassStruct(tname,false);
      thisCodeFile->AddDataType(t);	
   }
	Push(t);
	(yyval)=(yyvsp[(2) - (2)]);
}
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 590 "parser.y"
    {
  TypeClassStruct *t=new TypeClassStruct(NULL,false);
  thisCodeFile->AddDataType(t);
  Push(t);
  (yyval)=-1;
}
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 606 "parser.y"
    {
	// Save the attribute in the Strcuture object to be able to produce code
	if(structContainer!=NULL) {
		std::string attribute;
		attribute.append(typetmp);
		attribute.append(" ");
		attribute.append(GetToken((yyvsp[(2) - (4)])));
		attribute.append(";");	
		structContainer->setInnerDecl(attribute);			
	}

	DataType *type1=currenttype;
	if ((yyvsp[(1) - (4)])>0) {
		//type1=new TypePtr(NULL,$1, type1);
      type1=new TypePtr(NULL, (yyvsp[(1) - (4)]), type1, constPointerPositions);
      thisCodeFile->AddDataType(type1);
   }
  	if ((yyvsp[(3) - (4)])!=-1){
      type1=new TypeArray(NULL,GetToken((yyvsp[(3) - (4)])), type1);
      thisCodeFile->AddDataType(type1);     
   }  
	TypeClassStruct *t=Peek();
	assert(t!=NULL);
	t->Add(type1, GetToken((yyvsp[(2) - (4)])));
}
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 644 "parser.y"
    {
	if(currentClass!=NULL){
			
		typeDefContainer = new TypeDefinition(currentClass, accessmodifier);
		
		typeDefContainer->SetLineInfo(linenumber-1);
		
		currentClass->AddMember(typeDefContainer);
      
		typeDefContainer->setName(GetToken((yyvsp[(2) - (5)])));	
	
		typeDefContainer->setBaseType(GetToken((yyvsp[(4) - (5)])));	
	
		if ((yyvsp[(3) - (5)])>0) {
			typeDefContainer->setAsPtr();	

		}
		if ((yyvsp[(5) - (5)])!=-1) {
			typeDefContainer->setAsArray();	

    	}
	}
	char *orgtype=GetToken((yyvsp[(2) - (5)]));
	DataType *type=thisCodeFile->FindDataType(orgtype);
	if (type==NULL) {
		type=new DataType(orgtype);
      thisCodeFile->AddDataType(type);
	}
	if ((yyvsp[(3) - (5)])>0) {
      //type=new TypePtr(NULL,$3, type);
      type=new TypePtr(NULL,(yyvsp[(3) - (5)]), type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}
  if ((yyvsp[(5) - (5)])!=-1)
    {
      type=new TypeArray(NULL,GetToken((yyvsp[(5) - (5)])), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken((yyvsp[(4) - (5)])),type);
  thisCodeFile->AddDataType(type); 
}
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 686 "parser.y"
    {
	if(insideClass){
		sprintf(tmp,"typedef definition with structure inside a parclass is not currently supported !\n");
	
		errormsg(tmp);
		exit(1);
	}
  char *orgtype=GetToken((yyvsp[(3) - (6)]));
  DataType *type=thisCodeFile->FindDataType(orgtype);
  if (type==NULL)
    {
      type=new TypeClassStruct(orgtype,false);
      thisCodeFile->AddDataType(type);
    }
  if ((yyvsp[(4) - (6)])>0)
    {
      //type=new TypePtr(NULL,$4, type);
      type=new TypePtr(NULL, (yyvsp[(4) - (6)]), type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ((yyvsp[(6) - (6)])!=-1)
    {
      type=new TypeArray(NULL,GetToken((yyvsp[(6) - (6)])), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken((yyvsp[(5) - (6)])),type);
  thisCodeFile->AddDataType(type);   
}
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 716 "parser.y"
    {
  char *orgtype=GetToken((yyvsp[(3) - (6)]));
  DataType *type=thisCodeFile->FindDataType(orgtype);
  if (type==NULL)
    {
      type=new TypeClassStruct(orgtype, true);
      thisCodeFile->AddDataType(type);
    }
  if ((yyvsp[(4) - (6)])>0)
    {
      //type=new TypePtr(NULL,$4, type);
      type=new TypePtr(NULL, (yyvsp[(4) - (6)]), type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ((yyvsp[(6) - (6)])!=-1)
    {
      type=new TypeArray(NULL,GetToken((yyvsp[(6) - (6)])), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken((yyvsp[(5) - (6)])),type);
  thisCodeFile->AddDataType(type);   
}
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 740 "parser.y"
    {
	if(insideClass){
		sprintf(tmp,"typedef definition with structure inside a parclass is not currently supported !\n");
	
		errormsg(tmp);
		exit(1);
	}
  DataType *type=currentstruct;
  assert(type!=NULL);

  if ((yyvsp[(3) - (5)])>0)
    {
      //type=new TypePtr(NULL,$3, type);
      type=new TypePtr(NULL, (yyvsp[(3) - (5)]), type, constPointerPositions);
      
      thisCodeFile->AddDataType(type);
    }
  if ((yyvsp[(5) - (5)])!=-1)
    {
      type=new TypeArray(NULL,GetToken((yyvsp[(5) - (5)])), type);
      thisCodeFile->AddDataType(type); 
    }
  type=new TypeEqual(GetToken((yyvsp[(4) - (5)])),type);
  thisCodeFile->AddDataType(type); 
}
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 771 "parser.y"
    {
	if (seqclass!=NULL) 
		seqclass->SetTypeClass(true);
}
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 778 "parser.y"
    {
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(2) - (2)])));
	if (t==NULL) {
		seqclass=new TypeClassStruct(GetToken((yyvsp[(2) - (2)])),true);
		thisCodeFile->AddDataType(seqclass);
	} else if (!t->IsStandardType()) {
		seqclass=dynamic_cast<TypeClassStruct *>(t);
      if (seqclass==NULL) {
			char tmp[256];
			sprintf(tmp,"%s has been declared as non-class data type", GetToken((yyvsp[(2) - (2)])));
			errormsg(tmp);
			exit(1);
		}
	}
}
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 804 "parser.y"
    {
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(1) - (1)])));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken((yyvsp[(1) - (1)])), true);
		seqclass->AddBase(seqbase);  
	} else if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
}
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 816 "parser.y"
    {
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(2) - (2)])));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken((yyvsp[(2) - (2)])), true);
      seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
		seqclass->AddBase(t);
	}
}
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 828 "parser.y"
    {
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(2) - (5)])));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken((yyvsp[(2) - (5)])), true);
      seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
		seqclass->AddBase(t);
	}
}
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 840 "parser.y"
    {
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(3) - (3)])));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken((yyvsp[(3) - (3)])), true);
		seqclass->AddBase(seqbase);  
	} else  if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
}
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 852 "parser.y"
    {
	assert(seqclass!=NULL);
	TypeClassStruct *seqbase;
	DataType *t=thisCodeFile->FindDataType(GetToken((yyvsp[(3) - (3)])));
	if (t==NULL) {
		seqbase=new TypeClassStruct(GetToken((yyvsp[(3) - (3)])), true);
		seqclass->AddBase(seqbase);
	} else  if (!t->IsStandardType()) {
      seqclass->AddBase(t);
	}
}
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 869 "parser.y"
    {
  assert(currentClass!=NULL);
  sprintf(tmp,"class %s;",currentClass->GetName());

  assert(thisCodeFile!=NULL);
  OtherCode *dat=new OtherCode(thisCodeFile);
  dat->AddCode(tmp);
  thisCodeFile->AddCodeData(dat);

  othercodes.SetSize(0);

  currentClass=NULL;
  insideClass=false;
}
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 890 "parser.y"
    {
  currentClass->SetEndLine(linenumber);
  currentClass=NULL;
  insideClass=false;
}
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 898 "parser.y"
    {
  accessmodifier=PUBLIC;
  if ((yyvsp[(2) - (3)])) currentClass->SetPureVirtual(true);

  thisCodeFile->AddCodeData(currentClass);
}
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 907 "parser.y"
    {
	hadParclass = true; 
	if (othercodes.GetSize() && startPos>0) {
      assert(thisCodeFile!=NULL);
      OtherCode *dat=new OtherCode(thisCodeFile);
      dat->AddCode((char *)othercodes,startPos);
      thisCodeFile->AddCodeData(dat);
      othercodes.SetSize(0);
	}
	insideClass=true;
	char *clname=GetToken((yyvsp[(2) - (2)]));
  
	Class *t;
	if ((t=thisCodeFile->FindClass(clname))==NULL) {
      t=new Class(clname, thisCodeFile);
      thisCodeFile->AddDataType(t);
      
      // Pass compilation options to classes
      if(thisCodeFile->IsCoreCompilation())
      	t->SetAsCoreCompilation();
		if(thisCodeFile->IsAsyncAllocationDisable()) {
			t->DisableAsyncAllocation();
		}
      if(isWarningEnable)
      	t->EnableWarning();  
	}
	t->SetFileInfo(filename);
	t->SetStartLine(linenumber);
	currentClass=t;
	
	if(isNamespace) {
		
		t->SetNamespace(holdnamespace);
	
	}

}
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 954 "parser.y"
    {
	assert(currentClass!=NULL);
	Class *cl=thisCodeFile->FindClass(GetToken((yyvsp[(1) - (1)])));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken((yyvsp[(1) - (1)])));
	    errormsg(str);
	    exit(1);
	  }
	BaseClass *t=new BaseClass(cl, PUBLIC, false);
	currentClass->baseClass.InsertAt(-1,t);
}
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 968 "parser.y"
    {
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)(yyvsp[(1) - (2)]);
	Class *cl=thisCodeFile->FindClass(GetToken((yyvsp[(2) - (2)])));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken((yyvsp[(2) - (2)])));
	    errormsg(str);
	    exit(1);
	  }
	BaseClass *t=new BaseClass(cl,accessmode,false);
	currentClass->baseClass.InsertAt(-1,t);
}
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 983 "parser.y"
    {
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)(yyvsp[(2) - (3)]);

	Class *cl=thisCodeFile->FindClass(GetToken((yyvsp[(3) - (3)])));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken((yyvsp[(3) - (3)])));
	    errormsg(str);
	    exit(1);
	  }

	BaseClass *t=new BaseClass(cl,accessmode,true);
	currentClass->baseClass.InsertAt(-1,t);
}
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 1000 "parser.y"
    {
	assert(currentClass!=NULL);
	AccessType accessmode=(AccessType)(yyvsp[(1) - (3)]);

	Class *cl=thisCodeFile->FindClass(GetToken((yyvsp[(3) - (3)])));
	if (cl==NULL) 
	  {
	    char str[1024];
	    sprintf(str,"base class %s not declared",GetToken((yyvsp[(3) - (3)])));
	    errormsg(str);
	    exit(1);
	  }

	BaseClass *t=new BaseClass(cl,accessmode,true);
	currentClass->baseClass.InsertAt(-1,t);
}
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 1019 "parser.y"
    {
	(yyval)=PRIVATE;
}
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 1023 "parser.y"
    {
	(yyval)=PUBLIC;
}
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 1027 "parser.y"
    {
	(yyval)=PROTECTED;
}
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 1034 "parser.y"
    {
  (yyval)=0;
}
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 1038 "parser.y"
    {
  (yyval)= (paroc_utils::isEqual(GetToken((yyvsp[(2) - (2)])), "0")) ? 1 : 0;
}
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 1048 "parser.y"
    { accessmodifier=(AccessType)(yyvsp[(1) - (1)]); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 1056 "parser.y"
    {
	assert(method!=NULL);
	int t=method->CheckMarshal();
	if (t!=0) {      
		if (t==-1) {
			sprintf(tmp,"In method %s::%s: unable to marshal the return argument.\n", currentClass->GetName(), method->name);
      } else {
      	
			sprintf(tmp,"In method %s::%s: unable to marshal argument %d.\n", currentClass->GetName(), method->name, t);
		}
      errormsg(tmp);
		exit(1);
	}
	currenttype=returntype=NULL;
}
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 1073 "parser.y"
    {
	if (accessmodifier==PUBLIC) {
      sprintf(tmp,"%s:%d: attributes of a parallel class must be private or protected.\n",thisCodeFile->GetFileName(), linenumber);
      errormsg(tmp);
      exit(1);
	}
	currenttype=returntype=NULL;  
}
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 1082 "parser.y"
    {
  //  Directive *t=new Directive(currentClass, GetToken($1));
  //  currentClass->AddMember(t);
}
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 1087 "parser.y"
    {
  currentClass->SetClassID(GetToken((yyvsp[(3) - (5)])));
}
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 1098 "parser.y"
    {
	assert(currentClass!=NULL);
	Enumeration *t = new Enumeration(currentClass, accessmodifier);
	t->SetLineInfo(linenumber-1);
	currentClass->AddMember(t);
	t->setName(GetToken((yyvsp[(2) - (5)])));
	t->setArgs(GetToken((yyvsp[(4) - (5)])));
}
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 1109 "parser.y"
    {
	(yyval) = (yyvsp[(1) - (1)]);	
}
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 1113 "parser.y"
    {
	sprintf(tmp,"%s , %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
	(yyval) = PutToken(tmp);
}
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 1120 "parser.y"
    {
	(yyval) = (yyvsp[(1) - (1)]);
}
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 1124 "parser.y"
    {      
	sprintf(tmp,"%s = %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
   (yyval)=PutToken(tmp);
}
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 1141 "parser.y"
    {
	assert(currentClass!=NULL);
	Attribute *t=new Attribute(currentClass,accessmodifier);
	t->SetLineInfo(linenumber-1);
	currentClass->AddMember(t);
	Param *tparam = t->NewAttribute();
	DataType *type;
	strcpy(tparam->name,GetToken((yyvsp[(3) - (4)])));
	
	if ((yyvsp[(1) - (4)])==0) {
		type=currenttype;
	} else {
      type=new TypePtr(NULL, (yyvsp[(1) - (4)]) , currenttype, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}

	if ((yyvsp[(4) - (4)])!=-1) {
		type=new TypeArray(NULL, GetToken((yyvsp[(4) - (4)])) , type);
		thisCodeFile->AddDataType(type);
	}
	tparam->isRef=(yyvsp[(2) - (4)]);
	tparam->SetType(type);
}
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 1166 "parser.y"
    {
  (yyval)=-1;
}
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 1170 "parser.y"
    {
	(yyval)=0;
}
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 1174 "parser.y"
    {
	if ((yyvsp[(4) - (4)])==-1) {
		sprintf(tmp,"[%s]",GetToken((yyvsp[(2) - (4)])));
		(yyval)=PutToken(tmp);
	} else {
      sprintf(tmp,"[%s]%s",GetToken((yyvsp[(2) - (4)])),GetToken((yyvsp[(4) - (4)])));
      (yyval)=PutToken(tmp);
	}
}
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 1186 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (2)]);
}
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 1190 "parser.y"
    {
	
	
	(yyval)=0;
}
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 1198 "parser.y"
    {
  (yyval)=1;  
}
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 1202 "parser.y"
    {
  (yyval)=2;
}
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 1208 "parser.y"
    {
	if(isInStruct)
		sprintf(typetmp, "%s", GetToken((yyvsp[(1) - (1)]))); // Save the type specifier for struct attribute
	currenttype=thisCodeFile->FindDataType(GetToken((yyvsp[(1) - (1)])));
	if (currenttype==NULL) {
	   currenttype=new DataType(GetToken((yyvsp[(1) - (1)])));
      thisCodeFile->AddDataType(currenttype);
   }
	(yyval)=(YYSTYPE)currenttype;
}
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 1219 "parser.y"
    {
  currenttype=thisCodeFile->FindDataType(GetToken((yyvsp[(1) - (1)])));
  if (currenttype==NULL)
    {
      currenttype=new DataType(GetToken((yyvsp[(1) - (1)])));
      thisCodeFile->AddDataType(currenttype);

      /*
	sprintf(tmp,"Undeclared type \"%s\"\n",GetToken($1)); 
	errormsg(tmp);
	exit(1);
      */
    }
  (yyval)=(YYSTYPE)currenttype;
}
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 1235 "parser.y"
    {

  TypeTemplate *type=new TypeTemplate(GetToken((yyvsp[(1) - (4)])));
  paroc_list<TemplateArgument *> *list=(paroc_list<TemplateArgument *> *)(yyvsp[(3) - (4)]);
  POSITION pos=list->GetHeadPosition();
  while (pos!=NULL)
    {
      TemplateArgument *el=list->GetNext(pos);
      type->AddTemplate(el->type, el->isRef);
      delete el;
    }
  delete list;
 
  thisCodeFile->AddDataType(type);
  currenttype=type;
  (yyval)=(YYSTYPE)currenttype;

}
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 1254 "parser.y"
    {
  assert(currenttype!=NULL);
  currenttype=currentstruct;
  (yyval)=(YYSTYPE)currenttype;
}
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 1260 "parser.y"
    {
  DataType *type=thisCodeFile->FindDataType(GetToken((yyvsp[(2) - (2)])));
  if (type==NULL)
    {
      type=new TypeClassStruct(GetToken((yyvsp[(2) - (2)])), false);
      thisCodeFile->AddDataType(type);
    } 
  currenttype=type;
  (yyval)=(YYSTYPE)currenttype;
}
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 1273 "parser.y"
    {
  paroc_list<TemplateArgument *> *list=new paroc_list<TemplateArgument *>();
  TemplateArgument *v=(TemplateArgument *)(yyvsp[(1) - (1)]);
  list->AddHead(v);
  (yyval)=(YYSTYPE)list;
}
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 1280 "parser.y"
    {
  paroc_list<TemplateArgument *> *list=(paroc_list<TemplateArgument *> *)(yyvsp[(3) - (3)]);
  TemplateArgument *v=(TemplateArgument *)(yyvsp[(1) - (3)]);
  list->AddHead(v);
  (yyval)=(YYSTYPE)list;
}
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 1289 "parser.y"
    {
  TemplateArgument *t=new TemplateArgument;	
  t->type=(DataType *)(yyvsp[(1) - (4)]);
  if ((yyvsp[(2) - (4)])>0)
  {
	//t->type=new TypePtr(NULL,$2,t->type);
	t->type=new TypePtr(NULL, (yyvsp[(2) - (4)]), t->type, constPointerPositions);
  }
  if ((yyvsp[(3) - (4)])!=-1) 
  {
	t->type=new TypeArray(NULL,GetToken((yyvsp[(3) - (4)])), t->type);
  }
  t->isRef=((yyvsp[(4) - (4)])!=0);
  (yyval)=(YYSTYPE)t;
  
}
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 1361 "parser.y"
    {
	(yyval)=0;
	constPointerPositions.clear();
}
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 1366 "parser.y"
    {
	(yyval)=(yyvsp[(3) - (3)])+1; /* modivied by David (added const)*/
	constPointerPositions.push_back(((yyvsp[(2) - (3)]) == 1 ? true : false));
}
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 1374 "parser.y"
    {
  (yyval)=0;
}
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 1378 "parser.y"
    {
  (yyval)=1;
}
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 1384 "parser.y"
    {
  (yyval)=0;
}
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 1388 "parser.y"
    {
  (yyval)=1;
}
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 1404 "parser.y"
    {
  method->isPureVirtual=paroc_utils::isEqual(GetToken((yyvsp[(2) - (2)])),"0");
}
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 1410 "parser.y"
    {
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of constructor\n");
      exit(1);
    }
  if ((yyvsp[(5) - (5)])!=-1)
    {
      ObjDesc &od=((Constructor *)method)->GetOD();
      od.SetCode(GetToken((yyvsp[(5) - (5)])));
    }
  currentClass->AddMember(method);
}
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 1426 "parser.y"
    {
  method=new Constructor(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);

  strcpy(method->name,GetToken((yyvsp[(1) - (1)])));
}
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 1435 "parser.y"
    {
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of destructor\n");
      exit(1);
    }
	strcpy(method->name,currentClass->GetName());
}
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 1444 "parser.y"
    {
  if (!paroc_utils::isEqual(method->name,currentClass->GetName()))
    {
      errormsg("Bad declaration of destructor\n");
      exit(1);
    }
	strcpy(method->name,currentClass->GetName());
	method->isVirtual=true;
	
}
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 1457 "parser.y"
    {
  method=new Destructor(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);
  currentClass->AddMember(method);
  strcpy(method->name,GetToken((yyvsp[(1) - (1)])));
}
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 1587 "parser.y"
    {
	setReturnParam((yyvsp[(2) - (8)]),(yyvsp[(3) - (8)]), 0);
	
	method->isGlobalConst = ((yyvsp[(8) - (8)]) == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 1594 "parser.y"
    {
	setReturnParam((yyvsp[(3) - (9)]),(yyvsp[(4) - (9)]), (yyvsp[(1) - (9)]));
	
	method->isGlobalConst = ((yyvsp[(9) - (9)]) == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 1601 "parser.y"
    {
	setReturnParam((yyvsp[(3) - (9)]),(yyvsp[(4) - (9)]), (yyvsp[(2) - (9)]));
	
	method->isGlobalConst = ((yyvsp[(9) - (9)]) == 1 ? true : false); 
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 129:
/* Line 1787 of yacc.c  */
#line 1608 "parser.y"
    {
	setReturnParam((yyvsp[(4) - (10)]),(yyvsp[(5) - (10)]), ((yyvsp[(1) - (10)]) | (yyvsp[(3) - (10)])));
	
	method->isGlobalConst = ((yyvsp[(10) - (10)]) == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst); 
}
    break;

  case 130:
/* Line 1787 of yacc.c  */
#line 1615 "parser.y"
    {
	setReturnParam((yyvsp[(5) - (11)]),(yyvsp[(6) - (11)]), ((yyvsp[(2) - (11)])|(yyvsp[(4) - (11)])));
	setPOPCMethodeModifier((yyvsp[(1) - (11)]));
	
	method->isGlobalConst = ((yyvsp[(11) - (11)]) == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 131:
/* Line 1787 of yacc.c  */
#line 1622 "parser.y"
    { UpdateMarshalParam((yyvsp[(2) - (9)]),&(method->returnparam) ); }
    break;

  case 132:
/* Line 1787 of yacc.c  */
#line 1623 "parser.y"
    {
	setReturnParam((yyvsp[(7) - (14)]),(yyvsp[(8) - (14)]), ((yyvsp[(4) - (14)]) | (yyvsp[(6) - (14)])));
	
	method->isGlobalConst = ((yyvsp[(13) - (14)]) == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 133:
/* Line 1787 of yacc.c  */
#line 1629 "parser.y"
    { UpdateMarshalParam((yyvsp[(3) - (10)]),&(method->returnparam) ); }
    break;

  case 134:
/* Line 1787 of yacc.c  */
#line 1630 "parser.y"
    {
	setReturnParam((yyvsp[(8) - (15)]),(yyvsp[(9) - (15)]), ((yyvsp[(5) - (15)]) & 7));
	setPOPCMethodeModifier((yyvsp[(1) - (15)]));
	
	method->isGlobalConst = ((yyvsp[(14) - (15)]) == 1 ? true : false);
	errorGlobalMehtode(method->isGlobalConst);
}
    break;

  case 135:
/* Line 1787 of yacc.c  */
#line 1640 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 136:
/* Line 1787 of yacc.c  */
#line 1644 "parser.y"
    {
	/* error if multimple time same reserved word */
	if (((yyvsp[(2) - (2)]) & (yyvsp[(1) - (2)])) != 0)
	{
		errormsg("Multiple semantics keyword");
		exit(1);
	}
  
	(yyval)=(yyvsp[(1) - (2)]) | (yyvsp[(2) - (2)]);
}
    break;

  case 137:
/* Line 1787 of yacc.c  */
#line 1690 "parser.y"
    {
	(yyval)=2;
}
    break;

  case 138:
/* Line 1787 of yacc.c  */
#line 1694 "parser.y"
    {
	(yyval)=4;
}
    break;

  case 139:
/* Line 1787 of yacc.c  */
#line 1698 "parser.y"
    {
	(yyval)=8;
}
    break;

  case 140:
/* Line 1787 of yacc.c  */
#line 1702 "parser.y"
    {
	(yyval)=32;
}
    break;

  case 141:
/* Line 1787 of yacc.c  */
#line 1706 "parser.y"
    {
  (yyval)=16;
}
    break;

  case 142:
/* Line 1787 of yacc.c  */
#line 1710 "parser.y"
    {
  (yyval)=64;
}
    break;

  case 143:
/* Line 1787 of yacc.c  */
#line 1717 "parser.y"
    {
	(yyval)=1;
}
    break;

  case 144:
/* Line 1787 of yacc.c  */
#line 1721 "parser.y"
    {
	(yyval)=2;
}
    break;

  case 145:
/* Line 1787 of yacc.c  */
#line 1725 "parser.y"
    {
	(yyval)=3;
}
    break;

  case 146:
/* Line 1787 of yacc.c  */
#line 1729 "parser.y"
    {
	(yyval)=3;
}
    break;

  case 147:
/* Line 1787 of yacc.c  */
#line 1736 "parser.y"
    {
	(yyval)=0;
}
    break;

  case 148:
/* Line 1787 of yacc.c  */
#line 1740 "parser.y"
    {
	(yyval)=1;
}
    break;

  case 149:
/* Line 1787 of yacc.c  */
#line 1744 "parser.y"
    {
	(yyval)=2;
}
    break;

  case 150:
/* Line 1787 of yacc.c  */
#line 1748 "parser.y"
    {
	(yyval)=3;
}
    break;

  case 151:
/* Line 1787 of yacc.c  */
#line 1752 "parser.y"
    {
	(yyval)=3;
}
    break;

  case 152:
/* Line 1787 of yacc.c  */
#line 1758 "parser.y"
    {
  method=new Method(currentClass,accessmodifier);
  method->SetLineInfo(linenumber-1);
  currentClass->AddMember(method);
  strcpy(method->name,GetToken((yyvsp[(1) - (1)])));
  returntype=currenttype;
  currenttype=NULL;
}
    break;

  case 153:
/* Line 1787 of yacc.c  */
#line 1769 "parser.y"
    {
  (yyval)=-1;
}
    break;

  case 154:
/* Line 1787 of yacc.c  */
#line 1773 "parser.y"
    {
  (yyval)=(yyvsp[(3) - (4)]);
}
    break;

  case 155:
/* Line 1787 of yacc.c  */
#line 1779 "parser.y"
    {
  (yyval)=-1;
}
    break;

  case 156:
/* Line 1787 of yacc.c  */
#line 1784 "parser.y"
    {

  char *odtmp=GetToken((yyvsp[(1) - (12)]));
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken((yyvsp[(3) - (12)])));
      errormsg(tmp);
      exit(1);
    }

  sprintf(tmp,"od.%s(%s,%s,%s);",GetToken((yyvsp[(3) - (12)])),GetToken((yyvsp[(5) - (12)])),GetToken((yyvsp[(7) - (12)])),GetToken((yyvsp[(9) - (12)])));
  if ((yyvsp[(12) - (12)])!=-1) strcat(tmp,GetToken((yyvsp[(12) - (12)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 157:
/* Line 1787 of yacc.c  */
#line 1801 "parser.y"
    {

  char *odtmp=GetToken((yyvsp[(1) - (10)]));
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken((yyvsp[(3) - (10)])));
      errormsg(tmp);
      exit(1);
    }

  sprintf(tmp,"od.%s(%s,%s);",GetToken((yyvsp[(3) - (10)])),GetToken((yyvsp[(5) - (10)])),GetToken((yyvsp[(7) - (10)])));
  if ((yyvsp[(10) - (10)])!=-1) strcat(tmp,GetToken((yyvsp[(10) - (10)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 158:
/* Line 1787 of yacc.c  */
#line 1817 "parser.y"
    {
  char *odtmp=GetToken((yyvsp[(1) - (8)]));
  assert(method!=NULL);
  if (!paroc_utils::isEqual(odtmp,"od"))
    {
      sprintf(tmp,"Invalid OD expression: %s.%s",odtmp,GetToken((yyvsp[(3) - (8)])));
      errormsg(tmp);
      exit(1);
    }
  
  
  //add by Yao, Jun 14 2014
  char *odtmpp=GetToken((yyvsp[(3) - (8)]));
  if (paroc_utils::isEqual(odtmpp,"SensorDes"))
  {    
  	
  	std::string path = GetToken((yyvsp[(5) - (8)]));
	std::string tvalue;
	tvalue = "  char* fpath = " + path + ";\n"; //file path name
	tvalue = tvalue + "  std::ifstream DesFile;\n  DesFile.open(fpath);\n  Json::Reader reader;\n  Json::Value root;\n reader.parse(DesFile,root,false);\n  std::string ppower,pmemory,purl;\n  Json::Value addvalue = root[\"machine\"];\n  for (int i = 0; i < addvalue.size(); ++i)\n  {Json::Value temp_value = addvalue[i];\n   ppower = temp_value[\"power\"].asString();\n  pmemory = temp_value[\"memory\"].asString();\n  purl = temp_value[\"url\"].asString();}\n  if(ppower!=\"\"){int powNum = atoi(ppower.c_str());od.power(powNum);}\n  if(pmemory!=\"\"){int memNum = atoi(pmemory.c_str()); od.memory(memNum); }\n  if(purl!=\"\"){od.url(purl.c_str());}\n";	

	const char* pvalue = tvalue.c_str();
	sprintf(tmp,"%s",pvalue);

  	if ((yyvsp[(8) - (8)])!=-1) strcat(tmp,GetToken((yyvsp[(8) - (8)])));
  	(yyval)=PutToken(tmp);
  }
//end add
  else 
  {
  sprintf(tmp,"od.%s(%s);",GetToken((yyvsp[(3) - (8)])),GetToken((yyvsp[(5) - (8)])));
  if ((yyvsp[(8) - (8)])!=-1) strcat(tmp,GetToken((yyvsp[(8) - (8)])));
  (yyval)=PutToken(tmp);}
}
    break;

  case 159:
/* Line 1787 of yacc.c  */
#line 1853 "parser.y"
    {
    char *odtmp=GetToken((yyvsp[(1) - (6)]));
    if (paroc_utils::isEqual(odtmp,"host"))
      {
	sprintf(tmp,"od.url(%s);",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    errormsg("OD: host should be a string expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else if (paroc_utils::isEqual(odtmp,"jobcontact"))
      {
	sprintf(tmp,"od.joburl(%s);",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    errormsg("OD: jobcontact should be a string expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else if (paroc_utils::isEqual(odtmp,"memory"))
      {
	sprintf(tmp,"od.memory(%s",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken((yyvsp[(4) - (6)])));
	  }
	strcat(tmp,");");
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
      } 
    else if (paroc_utils::isEqual(odtmp,"power"))
      {
	sprintf(tmp,"od.power(%s",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken((yyvsp[(4) - (6)])));
	  }
	strcat(tmp,");");
//Added by clementval
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
      } 
    else if (paroc_utils::isEqual(odtmp,"search"))
      {
	sprintf(tmp,"od.search(%s",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken((yyvsp[(4) - (6)])));
	  }
	if ((yyvsp[(5) - (6)])!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken((yyvsp[(5) - (6)])));
	  }
	strcat(tmp,");");
//End of add
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
      } 
    else if (paroc_utils::isEqual(odtmp,"network"))
    {
	sprintf(tmp,"od.bandwidth(%s",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    strcat(tmp,",");
	    strcat(tmp,GetToken((yyvsp[(4) - (6)])));
	  }
	strcat(tmp,");");
	if ((yyvsp[(6) - (6)])!=-1) strcat(tmp,GetToken((yyvsp[(6) - (6)])));
    } 
    else if (paroc_utils::isEqual(odtmp,"walltime"))
      {
	sprintf(tmp,"od.walltime(%s);",GetToken((yyvsp[(3) - (6)])));
	if ((yyvsp[(4) - (6)])!=-1)
	  {
	    errormsg("OD: walltime  should be a number expression. Non-strict description is not allowed");
	    exit(1);
	  }
      } 
    else
      {
	errormsg("Unknown OD declaration");
	exit(1);
      }
    (yyval)=PutToken(tmp);
}
    break;

  case 160:
/* Line 1787 of yacc.c  */
#line 1945 "parser.y"
    {
    (yyval)=-1;
}
    break;

  case 161:
/* Line 1787 of yacc.c  */
#line 1949 "parser.y"
    {
    (yyval)=(yyvsp[(2) - (2)]);
}
    break;

  case 166:
/* Line 1787 of yacc.c  */
#line 1964 "parser.y"
    {
	Param *t=method->AddNewParam();
	UpdateMarshalParam((yyvsp[(1) - (9)]),t);
	
	
	DataType *type=currenttype;
	if ((yyvsp[(5) - (9)])>0) {
      type=new TypePtr(NULL, (yyvsp[(5) - (9)]) , type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	}

	if ((yyvsp[(6) - (9)])) {
		t->isRef=true;
	}
	
	// Handle standard array passing declaration
	if((yyvsp[(8) - (9)]) == 0){
		t->isArray=true;
		//Find last int as size of array
		int nb=method->params.GetSize()-1;
		
		std::string size_variable_name;
		
		for (int j=0;j<nb;j++) {
			Param &p=*(method->params[j]);
			if(strcmp("int", p.GetType()->GetName())==0){
				size_variable_name.clear();
				size_variable_name.append(p.GetName());
			}
		}
		if(size_variable_name.length() == 0){
			//
			sprintf(tmp,"Could not find size to marshall array: %s\n", GetToken((yyvsp[(7) - (9)])));
			errormsg(tmp);
			exit(1);
		}
		strcpy(tmpSize, size_variable_name.c_str());
		UpdateMarshalParam(67,t);
      type=new TypePtr(NULL, 1, type, constPointerPositions);
      thisCodeFile->AddDataType(type);
	} else if ((yyvsp[(8) - (9)]) != -1) {
		type=new TypeArray(NULL, GetToken((yyvsp[(8) - (9)])) , type);
		thisCodeFile->AddDataType(type);
	}

	t->SetType(type);
	if ((yyvsp[(7) - (9)]) != -1) {
		strcpy(t->name,GetToken((yyvsp[(7) - (9)])));
	} else {
		if(strcmp("void", t->GetType()->GetName()) == 0){
			t->isVoid = true;
		} else {
			sprintf(t->name,"V_%d",++counter);
		}
	}

	t->isConst=(((yyvsp[(2) - (9)])==1) || ((yyvsp[(4) - (9)])==1));
	if ((yyvsp[(9) - (9)]) >= 0) 
		t->defaultVal=strdup(GetToken((yyvsp[(9) - (9)])));
}
    break;

  case 167:
/* Line 1787 of yacc.c  */
#line 2027 "parser.y"
    {
  (yyval)=0;
}
    break;

  case 168:
/* Line 1787 of yacc.c  */
#line 2031 "parser.y"
    {
	(yyval)=(yyvsp[(2) - (3)]);
}
    break;

  case 169:
/* Line 1787 of yacc.c  */
#line 2037 "parser.y"
    {
	(yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 170:
/* Line 1787 of yacc.c  */
#line 2041 "parser.y"
    {
	(yyval)=(yyvsp[(1) - (3)]) | (yyvsp[(3) - (3)]) ;
}
    break;

  case 171:
/* Line 1787 of yacc.c  */
#line 2047 "parser.y"
    {
  (yyval)=PARAM_IN;
}
    break;

  case 172:
/* Line 1787 of yacc.c  */
#line 2051 "parser.y"
    {
  (yyval)=PARAM_OUT;
}
    break;

  case 173:
/* Line 1787 of yacc.c  */
#line 2055 "parser.y"
    {
  strcpy(tmpSize,GetToken((yyvsp[(3) - (3)])));
  (yyval)=PARAMSIZE;
}
    break;

  case 174:
/* Line 1787 of yacc.c  */
#line 2060 "parser.y"
    {
  strcpy(tmpProc,GetToken((yyvsp[(3) - (3)])));
  (yyval)=USERPROC;
}
    break;

  case 175:
/* Line 1787 of yacc.c  */
#line 2068 "parser.y"
    {
	(yyval)=0;
}
    break;

  case 176:
/* Line 1787 of yacc.c  */
#line 2072 "parser.y"
    {
	(yyval)=1;
}
    break;

  case 177:
/* Line 1787 of yacc.c  */
#line 2076 "parser.y"
    {
	(yyval)=2;
}
    break;

  case 178:
/* Line 1787 of yacc.c  */
#line 2082 "parser.y"
    {
	(yyval)=-1;
}
    break;

  case 179:
/* Line 1787 of yacc.c  */
#line 2086 "parser.y"
    {
	(yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 180:
/* Line 1787 of yacc.c  */
#line 2092 "parser.y"
    {
  (yyval)=-1;
}
    break;

  case 181:
/* Line 1787 of yacc.c  */
#line 2096 "parser.y"
    {
  (yyval)=(yyvsp[(2) - (2)]);
}
    break;

  case 182:
/* Line 1787 of yacc.c  */
#line 2109 "parser.y"
    {
  if ((yyvsp[(2) - (2)])<0) (yyval)=(yyvsp[(1) - (2)]);
  else 
    {
      sprintf(tmp, "%s%s",GetToken((yyvsp[(1) - (2)])),GetToken((yyvsp[(2) - (2)])));
      (yyval)=PutToken(tmp);
    }
}
    break;

  case 183:
/* Line 1787 of yacc.c  */
#line 2118 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 184:
/* Line 1787 of yacc.c  */
#line 2122 "parser.y"
    {
    (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 185:
/* Line 1787 of yacc.c  */
#line 2126 "parser.y"
    {
  sprintf(tmp,"%s + %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 186:
/* Line 1787 of yacc.c  */
#line 2131 "parser.y"
    {
  sprintf(tmp,"%s - %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 187:
/* Line 1787 of yacc.c  */
#line 2136 "parser.y"
    {
  sprintf(tmp,"%s * %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 188:
/* Line 1787 of yacc.c  */
#line 2141 "parser.y"
    {
  sprintf(tmp,"%s / %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 189:
/* Line 1787 of yacc.c  */
#line 2146 "parser.y"
    {
  sprintf(tmp,"%s %% %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 190:
/* Line 1787 of yacc.c  */
#line 2151 "parser.y"
    {
  sprintf(tmp,"%s & %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 191:
/* Line 1787 of yacc.c  */
#line 2156 "parser.y"
    {
  sprintf(tmp,"%s | %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 192:
/* Line 1787 of yacc.c  */
#line 2161 "parser.y"
    {
  sprintf(tmp,"%s ^ %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 193:
/* Line 1787 of yacc.c  */
#line 2166 "parser.y"
    {
  sprintf(tmp,"%s && %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 194:
/* Line 1787 of yacc.c  */
#line 2171 "parser.y"
    {
  sprintf(tmp,"%s || %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 195:
/* Line 1787 of yacc.c  */
#line 2176 "parser.y"
    {
  sprintf(tmp,"%s == %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 196:
/* Line 1787 of yacc.c  */
#line 2181 "parser.y"
    {
  sprintf(tmp,"%s=%s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 197:
/* Line 1787 of yacc.c  */
#line 2186 "parser.y"
    {
  sprintf(tmp,"%s ? %s : %s",GetToken((yyvsp[(1) - (5)])), GetToken((yyvsp[(3) - (5)])), GetToken((yyvsp[(5) - (5)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 198:
/* Line 1787 of yacc.c  */
#line 2191 "parser.y"
    {
  sprintf(tmp,"%s != %s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 199:
/* Line 1787 of yacc.c  */
#line 2196 "parser.y"
    {
  sprintf(tmp,"%s>%s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 200:
/* Line 1787 of yacc.c  */
#line 2201 "parser.y"
    {
  sprintf(tmp,"%s<%s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 201:
/* Line 1787 of yacc.c  */
#line 2206 "parser.y"
    {
  sprintf(tmp,"%s>=%s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 202:
/* Line 1787 of yacc.c  */
#line 2211 "parser.y"
    {
  sprintf(tmp,"%s<=%s",GetToken((yyvsp[(1) - (3)])), GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 203:
/* Line 1787 of yacc.c  */
#line 2216 "parser.y"
    {
  sprintf(tmp,"-%s",GetToken((yyvsp[(2) - (2)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 204:
/* Line 1787 of yacc.c  */
#line 2221 "parser.y"
    {
  sprintf(tmp,"(%s)",GetToken((yyvsp[(2) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 205:
/* Line 1787 of yacc.c  */
#line 2226 "parser.y"
    {
  sprintf(tmp,"%s()",GetToken((yyvsp[(1) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 206:
/* Line 1787 of yacc.c  */
#line 2231 "parser.y"
    {
  sprintf(tmp,"%s(%s)",GetToken((yyvsp[(1) - (4)])),GetToken((yyvsp[(3) - (4)])));
  (yyval)=PutToken(tmp);
 
}
    break;

  case 207:
/* Line 1787 of yacc.c  */
#line 2237 "parser.y"
    {
  sprintf(tmp,"*%s",GetToken((yyvsp[(2) - (2)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 208:
/* Line 1787 of yacc.c  */
#line 2242 "parser.y"
    {
  sprintf(tmp,"&%s",GetToken((yyvsp[(2) - (2)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 209:
/* Line 1787 of yacc.c  */
#line 2247 "parser.y"
    {
  sprintf(tmp,"!%s",GetToken((yyvsp[(2) - (2)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 210:
/* Line 1787 of yacc.c  */
#line 2252 "parser.y"
    {
  sprintf(tmp,"~%s",GetToken((yyvsp[(2) - (2)])));
  (yyval)=PutToken(tmp);
}
    break;

  case 211:
/* Line 1787 of yacc.c  */
#line 2259 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 212:
/* Line 1787 of yacc.c  */
#line 2263 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 213:
/* Line 1787 of yacc.c  */
#line 2269 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 214:
/* Line 1787 of yacc.c  */
#line 2273 "parser.y"
    {
  sprintf(tmp,"%s::%s",GetToken((yyvsp[(1) - (3)])),GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);  
}
    break;

  case 215:
/* Line 1787 of yacc.c  */
#line 2281 "parser.y"
    {
  (yyval)=(yyvsp[(1) - (1)]);
}
    break;

  case 216:
/* Line 1787 of yacc.c  */
#line 2285 "parser.y"
    {
  sprintf(tmp,"%s, %s", GetToken((yyvsp[(1) - (3)])),GetToken((yyvsp[(3) - (3)])));
  (yyval)=PutToken(tmp);
}
    break;


/* Line 1787 of yacc.c  */
#line 4397 "y.tab.c"
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
#line 2293 "parser.y"


#include "config.h"

void Usage()
{
  fprintf(stderr,"POP-C++ preprocessor version %s\nparoccpp [-onlyclient | -onlyserver] [POP-C++ source] [C++ source]\n", VERSION);
  exit(1);
}

void CleanStack()
{
  if (typestack.GetCount()) fprintf(stderr,"STRUCT list: %d elements\n",typestack.GetCount());
  currentstruct=NULL;
  structContainer=NULL;
  typestack.RemoveAll();
}

void Push(TypeClassStruct *x)
{
  typestack.AddHead(x);
}

TypeClassStruct *Pop()
{
  POSITION pos=typestack.GetHeadPosition();
  if (pos==NULL) return NULL;
  TypeClassStruct *t=typestack.GetAt(pos);
  typestack.RemoveHead();
  return t;
}

TypeClassStruct *Peek()
{
  POSITION pos=typestack.GetHeadPosition();
  if (pos==NULL) return NULL;
  TypeClassStruct *t=typestack.GetAt(pos);
  return t;
}

// Update marshall options for a specific parameter
void UpdateMarshalParam(int flags, Param *t)
{
	if (flags!=0) {
		if (flags & PARAM_IN) t->isInput=true;
		if (flags & PARAM_OUT) t->isOutput=true;
      if (flags & USERPROC) t->marshalProc=strdup(tmpProc);
      if (flags & PARAMSIZE) t->paramSize=strdup(tmpSize);
	}
}

extern FILE *yyin;

int main(int argc, char **argv)
{
	bool client=true;
	bool broker=true;
	isWarningEnable=true;
	isImplicitPackEnable=false;
	isPOPCPPCompilation=false;
	isAsyncAllocationDisable=false;

	if (paroc_utils::checkremove(&argc,&argv,"-parclass-nobroker")!=NULL){
		
		broker=false;
	
	} 	
	
	
	if (paroc_utils::checkremove(&argc,&argv,"-parclass-nointerface")!=NULL) {
		
		client=false;
	
	}  
	
	if (paroc_utils::checkremove(&argc,&argv,"-no-warning")!=NULL) {
		
		isWarningEnable=false;
	
	}  

	if (paroc_utils::checkremove(&argc,&argv,"-popcpp-compilation")!=NULL) {
		
		isPOPCPPCompilation=true;
	
	}  
	
	if (paroc_utils::checkremove(&argc,&argv,"-no-implicit-pack")!=NULL) {

		isImplicitPackEnable=false;
	
	}
	if (paroc_utils::checkremove(&argc,&argv,"-no-async-allocation")!=NULL) {
		isAsyncAllocationDisable=true;
	
	}	

	int ret;
	indexsource=-1;
	errorcode=0;
	if (argc<2){
		
		Usage();
	
	} else {
		if ((ret=ParseFile(argv[1], ((argc>2) ? argv[2] : NULL), client, broker, isWarningEnable, isImplicitPackEnable, isPOPCPPCompilation, isAsyncAllocationDisable))!=0)	{
			fprintf(stderr,"Parse POP-C++ code failed (error=%d)\n",ret);
			exit(ret);
		}
	}
	return (errorcode!=0);
}

void yyerror(const  char *s)
{
  if (insideClass)
    {
      errormsg(s);
    }
}

void errormsg(const  char *s)
{
  fprintf(stderr,"%s:%d: ERROR: %s\n",filename,linenumber-1,s);
  errorcode=1;
}

int yywrap() {
	return(1);
}


int base=1;

int ParseFile(char *infile, char *outfile, bool client, bool broker, bool isWarningEnable, bool isImplicitPackEnable, bool isPOPCPPCompilation, bool isAsyncAllocationDisable)
{
	if (infile==NULL || *infile=='-'){
		yyin=stdin;
	} else {
      yyin=fopen(infile,"rt");
      if (yyin==NULL) {
	   	perror(infile);
	   	return errno;
		}
      strcpy(filename,infile);
	}
	
	linenumber=1;
	thisCodeFile=new CodeFile(NULL);
	if (outfile!=NULL) {
		
		thisCodeFile->SetOutputName(outfile);	

	}
	

	// Set the code file as core compilation if the compilation flag is present.
	if(isPOPCPPCompilation)
		thisCodeFile->SetAsCoreCompilation();

	// Disable asynchronous parallel object allocation if the compilation flag is present.
	if(isAsyncAllocationDisable)	
		thisCodeFile->DisableAsyncAllocation();	
		
	insideClass=false;
	othercodes.SetSize(0);
	startPos=-1;
	

	int ret=yyparse();
	if (ret==0) {
		FILE *outf;
      if (outfile==NULL || *outfile=='-'){
     		
      	outf=stdout;
      
      } else {
			outf=fopen(outfile,"wt");
	  		if (outf==NULL){
		      ret=errno;
		      perror(outfile);
	   	}
		}
      if (outf!=NULL) {
			CArrayChar output(0,32000);
			thisCodeFile->GenerateCode(output, client, broker);
			fwrite((char *)output,1, output.GetSize(),outf);
      }
      if (outf!=stdout) {
			fclose(outf);
      
		}
	}
  
	othercodes.SetSize(0);

	if (yyin!=stdin) {
		
		fclose(yyin);
	
	}
	delete thisCodeFile;
	thisCodeFile=NULL;
	
	isParclassDeclared = false; 
	
	return ret;
	
}


/* ---- SET normal C++ function return attributes ----- */
void setReturnParam(int pointer, int ref, int const_virtual)
{
	//Old data:  argument_type function_name 
	strcpy(method->returnparam.name,"_RemoteRet");

	DataType *type=returntype;
	if (pointer>0)
	{
		//type=new TypePtr(NULL, pointer, type);
		type=new TypePtr(NULL, pointer, type, constPointerPositions);
		thisCodeFile->AddDataType(type);
		
		constPointerPositions.clear(); // empty used struct
	}

	if (ref)
	{
		method->returnparam.isRef=true;
	}
	
	method->isVirtual=((const_virtual & 1)!=0);
	method->returnparam.isConst = ((const_virtual & 2)!=0);
	
	method->returnparam.SetType(type);
	
}

/* ---- POPC methode attributes ---- */
void setPOPCMethodeModifier(int settings)
{
	
	// TEST Mutex, prallel, seq or CONC (hinden ??)
	if ((settings & 56)==8) 
		method->isConcurrent=true;
	else if ((settings & 56)==32) 
		method->isConcurrent=false;
	else if ((settings & 56)==16)
		method->isMutex= true;
	else if ((settings & 56)!=0)
	{
		errormsg("Multiple seq, conc or mutex keyword");
		exit(1);
	}
	
	if ((settings & 64)!=0)
		method->isHidden=true;
	
	// TEST SYNC or ASYNC
	if ((settings & 6)==6) 
	{
		errormsg("Multiple sync, async keyword");
		exit(1);
	}
	else if ((settings & 6)==4) method->invoketype=invokeasync;
	else if ((settings & 6)==2) method->invoketype=invokesync;
	//else method->invoketype=autoselect;
}

void errorGlobalMehtode(bool isGlobal)
{
	if(isGlobal)
	{
		errormsg("inspectors/const member functions are supported in the current version of POP-C++");
		exit(1);
	}
	else
	{
		return;
	}
}

