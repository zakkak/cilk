/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     AUTO = 258,
     DOUBLE = 259,
     INT = 260,
     STRUCT = 261,
     BREAK = 262,
     ELSE = 263,
     LONG = 264,
     SWITCH = 265,
     CASE = 266,
     ENUM = 267,
     REGISTER = 268,
     TYPEDEF = 269,
     CHAR = 270,
     EXTERN = 271,
     RETURN = 272,
     UNION = 273,
     CONST = 274,
     FLOAT = 275,
     SHORT = 276,
     UNSIGNED = 277,
     CONTINUE = 278,
     FOR = 279,
     SIGNED = 280,
     VOID = 281,
     DEFAULT = 282,
     GOTO = 283,
     SIZEOF = 284,
     VOLATILE = 285,
     DO = 286,
     IF = 287,
     STATIC = 288,
     WHILE = 289,
     UPLUS = 290,
     UMINUS = 291,
     INDIR = 292,
     ADDRESS = 293,
     POSTINC = 294,
     POSTDEC = 295,
     PREINC = 296,
     PREDEC = 297,
     BOGUS = 298,
     IDENTIFIER = 299,
     STRINGliteral = 300,
     FLOATINGconstant = 301,
     INTEGERconstant = 302,
     OCTALconstant = 303,
     HEXconstant = 304,
     WIDECHARconstant = 305,
     CHARACTERconstant = 306,
     COMPLEX = 307,
     REAL = 308,
     IMAG = 309,
     TYPEDEFname = 310,
     ARROW = 311,
     ICR = 312,
     DECR = 313,
     LS = 314,
     RS = 315,
     LE = 316,
     GE = 317,
     EQ = 318,
     NE = 319,
     ANDAND = 320,
     OROR = 321,
     ELLIPSIS = 322,
     MULTassign = 323,
     DIVassign = 324,
     MODassign = 325,
     PLUSassign = 326,
     MINUSassign = 327,
     LSassign = 328,
     RSassign = 329,
     ANDassign = 330,
     ERassign = 331,
     ORassign = 332,
     INLINE = 333,
     ATTRIBUTE = 334,
     EXTENSION = 335,
     RESTRICT = 336,
     ALIGNOF = 337,
     TYPEOF = 338,
     DECLSPEC_EXPORT = 339,
     DECLSPEC_IMPORT = 340,
     BUILTIN_VA_ARG = 341,
     DESIGNATED_INITIALIZER = 342,
     ARRAY_LABELED_INITIALIZER = 343,
     NOOUTPUT = 344,
     CILKONLY = 345,
     CILKSAFE = 346,
     WILDCARD_T = 347,
     WILDCARD_E = 348,
     WILDCARD_D = 349,
     ASM = 350,
     PROCEDURE = 351,
     SPAWN = 352,
     SYNC = 353,
     INLET = 354,
     ABORT = 355,
     XBEGIN = 356,
     XEND = 357,
     SYNCHED = 358,
     SHAREDVAR = 359,
     PRIVATEVAR = 360
   };
#endif
/* Tokens.  */
#define AUTO 258
#define DOUBLE 259
#define INT 260
#define STRUCT 261
#define BREAK 262
#define ELSE 263
#define LONG 264
#define SWITCH 265
#define CASE 266
#define ENUM 267
#define REGISTER 268
#define TYPEDEF 269
#define CHAR 270
#define EXTERN 271
#define RETURN 272
#define UNION 273
#define CONST 274
#define FLOAT 275
#define SHORT 276
#define UNSIGNED 277
#define CONTINUE 278
#define FOR 279
#define SIGNED 280
#define VOID 281
#define DEFAULT 282
#define GOTO 283
#define SIZEOF 284
#define VOLATILE 285
#define DO 286
#define IF 287
#define STATIC 288
#define WHILE 289
#define UPLUS 290
#define UMINUS 291
#define INDIR 292
#define ADDRESS 293
#define POSTINC 294
#define POSTDEC 295
#define PREINC 296
#define PREDEC 297
#define BOGUS 298
#define IDENTIFIER 299
#define STRINGliteral 300
#define FLOATINGconstant 301
#define INTEGERconstant 302
#define OCTALconstant 303
#define HEXconstant 304
#define WIDECHARconstant 305
#define CHARACTERconstant 306
#define COMPLEX 307
#define REAL 308
#define IMAG 309
#define TYPEDEFname 310
#define ARROW 311
#define ICR 312
#define DECR 313
#define LS 314
#define RS 315
#define LE 316
#define GE 317
#define EQ 318
#define NE 319
#define ANDAND 320
#define OROR 321
#define ELLIPSIS 322
#define MULTassign 323
#define DIVassign 324
#define MODassign 325
#define PLUSassign 326
#define MINUSassign 327
#define LSassign 328
#define RSassign 329
#define ANDassign 330
#define ERassign 331
#define ORassign 332
#define INLINE 333
#define ATTRIBUTE 334
#define EXTENSION 335
#define RESTRICT 336
#define ALIGNOF 337
#define TYPEOF 338
#define DECLSPEC_EXPORT 339
#define DECLSPEC_IMPORT 340
#define BUILTIN_VA_ARG 341
#define DESIGNATED_INITIALIZER 342
#define ARRAY_LABELED_INITIALIZER 343
#define NOOUTPUT 344
#define CILKONLY 345
#define CILKSAFE 346
#define WILDCARD_T 347
#define WILDCARD_E 348
#define WILDCARD_D 349
#define ASM 350
#define PROCEDURE 351
#define SPAWN 352
#define SYNC 353
#define INLET 354
#define ABORT 355
#define XBEGIN 356
#define XEND 357
#define SYNCHED 358
#define SHAREDVAR 359
#define PRIVATEVAR 360




/* Copy the first part of user declarations.  */
#line 1 "ANSI-C.y"


    /* Copyright (C) 1989,1990 James A. Roskind, All rights reserved.
    This grammar was developed  and  written  by  James  A.  Roskind.
    Copying  of  this  grammar  description, as a whole, is permitted
    providing this notice is intact and applicable  in  all  complete
    copies.   Translations as a whole to other parser generator input
    languages  (or  grammar  description  languages)   is   permitted
    provided  that  this  notice is intact and applicable in all such
    copies,  along  with  a  disclaimer  that  the  contents  are   a
    translation.   The reproduction of derived text, such as modified
    versions of this grammar, or the output of parser generators,  is
    permitted,  provided  the  resulting  work includes the copyright
    notice "Portions Copyright (c)  1989,  1990  James  A.  Roskind".
    Derived products, such as compilers, translators, browsers, etc.,
    that  use  this  grammar,  must also provide the notice "Portions
    Copyright  (c)  1989,  1990  James  A.  Roskind"  in   a   manner
    appropriate  to  the  utility,  and in keeping with copyright law
    (e.g.: EITHER displayed when first invoked/executed; OR displayed
    continuously on display terminal; OR via placement in the  object
    code  in  form  readable in a printout, with or near the title of
    the work, or at the end of the file).  No royalties, licenses  or
    commissions  of  any  kind are required to copy this grammar, its
    translations, or derivative products, when the copies are made in
    compliance with this notice. Persons or corporations that do make
    copies in compliance with this notice may charge  whatever  price
    is  agreeable  to  a  buyer, for such copies or derivative works.
    THIS GRAMMAR IS PROVIDED ``AS IS'' AND  WITHOUT  ANY  EXPRESS  OR
    IMPLIED  WARRANTIES,  INCLUDING,  WITHOUT LIMITATION, THE IMPLIED
    WARRANTIES  OF  MERCHANTABILITY  AND  FITNESS  FOR  A  PARTICULAR
    PURPOSE.

    James A. Roskind
    Independent Consultant
    516 Latania Palm Drive
    Indialantic FL, 32903
    (407)729-4348
    jar@ileaf.com
    or ...!uunet!leafusa!jar


    ---end of copyright notice---

 * Copyright (c) 1994 Robert C. Miller
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
 *
 * The changes to this gramamr since James Roskind's original are licensed under the GNU GPL.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

This file is a companion file to a C++ grammar description file.

*/

/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *
 *************************************************************************/

/* FILENAME: C.Y */

/*  This  is a grammar file for the dpANSI C language.  This file was
last modified by J. Roskind on 3/7/90. Version 1.00 */




/* ACKNOWLEDGMENT:

Without the effort expended by the ANSI C standardizing committee,  I
would  have been lost.  Although the ANSI C standard does not include
a fully disambiguated syntax description, the committee has at  least
provided most of the disambiguating rules in narratives.

Several  reviewers  have also recently critiqued this grammar, and/or
assisted in discussions during it's preparation.  These reviewers are
certainly not responsible for the errors I have committed  here,  but
they  are responsible for allowing me to provide fewer errors.  These
colleagues include: Bruce Blodgett, and Mark Langley. */

/* Added by Eric A. Brewer */

#define _Y_TAB_H_  /* prevents redundant inclusion of y.tab.h */
#include "ast.h"
#include "stringParse.h"

FILE_IDENTITY(ident_ansi_c_y,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/ANSI-C.y $ $LastChangedBy: bradley $ $Rev: 2836 $ $Date: 2006-04-12 08:36:07 -0400 (Wed, 12 Apr 2006) $");

int yydebug=0;

extern int yylex(void);
PRIVATE void WarnAboutPrecedence(OpType op, Node *node);

PRIVATE void yyerror(const char *msg)
  {
    StringParseError();
    SyntaxError(msg);
  }

/* End of create code (EAB) */


 PRIVATE Bool OmitLookupsFlag = TRUE;

 GLOBAL void SetLookupFlag(Bool dolookup) {
   OmitLookupsFlag = dolookup;
 }

 BisonInputType parseInputType = EntireProgram;

 GLOBAL void SetBisonInputType(BisonInputType t) {
   parseInputType = t;
 }

 GLOBAL BisonInputType GetBisonInputType() {
   return parseInputType;
 }

 #define AcceptWildcards() (parseInputType != EntireProgram)

 GLOBAL Bool OmitLookups() {
   return (OmitLookupsFlag && (parseInputType != EntireProgram));
   /*  return FALSE; */
 }




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 194 "ANSI-C.y"
{
    Node      *n;
    List      *L;

  /* tq: type qualifiers */
    struct {
        TypeQual   tq;
	Coord      coord;   /* coordinates where type quals began */
    } tq;

  /* tok: token coordinates */
    Coord tok;
}
/* Line 187 of yacc.c.  */
#line 470 "ANSI-C.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 483 "ANSI-C.c"

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
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  87
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5798

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  184
/* YYNRULES -- Number of rules.  */
#define YYNRULES  518
/* YYNRULES -- Number of states.  */
#define YYNSTATES  898

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   360

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     8,     2,     2,     2,    13,     3,     2,
      15,    16,     4,     5,    22,     6,    17,    14,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    25,    19,
       9,    91,    10,    18,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    24,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    20,    11,    21,     7,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    12,    14,    16,    18,
      20,    24,    30,    37,    39,    41,    43,    45,    50,    54,
      59,    63,    67,    70,    73,    77,    81,    85,    89,    91,
      95,    97,   100,   103,   106,   109,   112,   117,   120,   125,
     132,   135,   138,   140,   142,   144,   146,   148,   150,   152,
     157,   162,   164,   168,   172,   176,   178,   182,   186,   188,
     192,   196,   198,   202,   206,   210,   214,   216,   220,   224,
     226,   230,   232,   236,   238,   242,   244,   248,   250,   254,
     256,   262,   267,   269,   273,   275,   277,   279,   281,   283,
     285,   287,   289,   291,   293,   295,   297,   301,   303,   304,
     306,   309,   312,   315,   318,   319,   324,   325,   331,   332,
     337,   338,   344,   345,   346,   347,   356,   357,   358,   359,
     369,   370,   371,   372,   381,   382,   383,   384,   394,   395,
     396,   397,   407,   408,   415,   416,   423,   427,   428,   429,
     436,   437,   438,   445,   446,   447,   455,   456,   462,   463,
     469,   473,   475,   477,   479,   481,   483,   486,   489,   492,
     495,   498,   502,   505,   508,   511,   514,   517,   522,   525,
     527,   529,   531,   533,   535,   537,   539,   541,   543,   548,
     554,   556,   559,   562,   565,   567,   570,   573,   575,   578,
     581,   583,   586,   588,   591,   593,   595,   597,   599,   601,
     603,   606,   608,   612,   618,   625,   630,   634,   639,   644,
     646,   650,   652,   655,   657,   659,   663,   668,   672,   677,
     679,   681,   683,   686,   690,   694,   699,   703,   707,   711,
     716,   718,   721,   725,   727,   729,   731,   735,   740,   743,
     747,   752,   754,   758,   760,   762,   766,   771,   776,   780,
     785,   787,   791,   793,   795,   797,   800,   802,   805,   807,
     808,   810,   812,   815,   822,   824,   828,   829,   831,   836,
     843,   848,   850,   851,   853,   858,   860,   862,   864,   865,
     868,   870,   874,   878,   883,   888,   893,   899,   906,   908,
     910,   913,   917,   922,   924,   928,   930,   934,   936,   938,
     942,   946,   950,   953,   957,   961,   965,   968,   972,   976,
     979,   983,   988,   992,   995,   999,  1003,  1006,  1010,  1015,
    1019,  1024,  1030,  1033,  1037,  1042,  1044,  1046,  1049,  1053,
    1056,  1059,  1062,  1065,  1069,  1072,  1076,  1081,  1085,  1089,
    1093,  1094,  1096,  1099,  1105,  1112,  1115,  1118,  1123,  1124,
    1127,  1128,  1130,  1132,  1134,  1136,  1138,  1140,  1142,  1144,
    1146,  1149,  1152,  1156,  1159,  1162,  1165,  1169,  1173,  1178,
    1181,  1185,  1189,  1194,  1196,  1198,  1200,  1202,  1205,  1209,
    1213,  1218,  1220,  1223,  1225,  1228,  1231,  1237,  1245,  1246,
    1253,  1254,  1261,  1262,  1271,  1272,  1283,  1284,  1295,  1296,
    1307,  1308,  1319,  1320,  1327,  1331,  1334,  1337,  1341,  1345,
    1346,  1348,  1350,  1354,  1359,  1361,  1365,  1371,  1379,  1389,
    1401,  1408,  1417,  1428,  1441,  1443,  1446,  1448,  1450,  1453,
    1455,  1456,  1460,  1463,  1464,  1469,  1470,  1475,  1476,  1480,
    1481,  1486,  1487,  1492,  1493,  1498,  1499,  1504,  1505,  1510,
    1511,  1517,  1518,  1524,  1525,  1531,  1532,  1538,  1539,  1542,
    1544,  1546,  1548,  1550,  1552,  1554,  1557,  1559,  1561,  1563,
    1565,  1567,  1569,  1571,  1573,  1575,  1577,  1579,  1581,  1583,
    1585,  1587,  1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,
    1605,  1607,  1609,  1611,  1613,  1615,  1617,  1619,  1621,  1623,
    1625,  1630,  1634,  1638,  1641,  1644,  1648,  1652,  1654,  1655,
    1657,  1664,  1673,  1676,  1682,  1685,  1691,  1699,  1701
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     131,     0,    -1,    -1,   283,    -1,   132,   265,    -1,   132,
     264,    -1,    18,    -1,    67,    -1,   301,    -1,   302,    -1,
      15,   152,    16,    -1,    15,   259,   265,   260,    16,    -1,
      15,   259,   264,   265,   260,    16,    -1,   312,    -1,   117,
      -1,   118,    -1,   133,    -1,   134,    23,   152,    24,    -1,
     134,    15,    16,    -1,   134,    15,   135,    16,    -1,   134,
      17,    67,    -1,   134,    79,    67,    -1,   134,    80,    -1,
     134,    81,    -1,   134,    17,    78,    -1,   134,    79,    78,
      -1,   134,    17,   118,    -1,   134,    79,   118,    -1,   150,
      -1,   135,    22,   150,    -1,   134,    -1,    80,   136,    -1,
      81,   136,    -1,   137,   138,    -1,   313,   138,    -1,    52,
     136,    -1,    52,    15,   223,    16,    -1,   106,   136,    -1,
     106,    15,   223,    16,    -1,   110,    15,   150,    22,   223,
      16,    -1,    77,   136,    -1,    76,   136,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
     136,    -1,    15,   223,    16,   236,    -1,    15,   223,    16,
     138,    -1,   138,    -1,   139,     4,   138,    -1,   139,    14,
     138,    -1,   139,    13,   138,    -1,   139,    -1,   140,     5,
     139,    -1,   140,     6,   139,    -1,   140,    -1,   141,    82,
     140,    -1,   141,    83,   140,    -1,   141,    -1,   142,     9,
     141,    -1,   142,    10,   141,    -1,   142,    84,   141,    -1,
     142,    85,   141,    -1,   142,    -1,   143,    86,   142,    -1,
     143,    87,   142,    -1,   143,    -1,   144,     3,   143,    -1,
     144,    -1,   145,    12,   144,    -1,   145,    -1,   146,    11,
     145,    -1,   146,    -1,   147,    88,   146,    -1,   147,    -1,
     148,    89,   147,    -1,   148,    -1,   148,    18,   152,    25,
     149,    -1,   148,    18,    25,   149,    -1,   149,    -1,   138,
     151,   150,    -1,    91,    -1,    92,    -1,    93,    -1,    94,
      -1,    95,    -1,    96,    -1,    97,    -1,    98,    -1,    99,
      -1,   100,    -1,   101,    -1,   150,    -1,   152,    22,   150,
      -1,   149,    -1,    -1,   152,    -1,   160,    19,    -1,   178,
      19,    -1,   190,    19,    -1,   199,    19,    -1,    -1,   187,
     215,   156,   263,    -1,    -1,   188,   225,   215,   157,   263,
      -1,    -1,   195,   215,   158,   263,    -1,    -1,   196,   225,
     215,   159,   263,    -1,    -1,    -1,    -1,   187,   204,   161,
     230,   162,   224,   163,   233,    -1,    -1,    -1,    -1,   188,
     225,   204,   164,   230,   165,   224,   166,   233,    -1,    -1,
      -1,    -1,   195,   204,   167,   230,   168,   224,   169,   233,
      -1,    -1,    -1,    -1,   196,   225,   204,   170,   230,   171,
     224,   172,   233,    -1,    -1,    -1,    -1,   160,    22,   204,
     173,   230,   174,   224,   175,   233,    -1,    -1,   187,     1,
     176,   230,   224,   233,    -1,    -1,   195,     1,   177,   230,
     224,   233,    -1,   160,    22,     1,    -1,    -1,    -1,   192,
     215,   179,   224,   180,   233,    -1,    -1,    -1,   201,   215,
     181,   224,   182,   233,    -1,    -1,    -1,   178,    22,   215,
     183,   224,   184,   233,    -1,    -1,   192,     1,   185,   224,
     233,    -1,    -1,   201,     1,   186,   224,   233,    -1,   178,
      22,     1,    -1,   188,    -1,   190,    -1,   189,    -1,   191,
      -1,   116,    -1,   198,   305,    -1,   192,   306,    -1,   189,
     193,    -1,   189,   306,    -1,   199,   305,    -1,   192,   203,
     224,    -1,   190,   193,    -1,   200,   305,    -1,   192,    78,
      -1,   191,   193,    -1,   305,   224,    -1,   201,   224,   305,
     224,    -1,   192,   193,    -1,   303,    -1,   305,    -1,   152,
      -1,   195,    -1,   196,    -1,   199,    -1,   198,    -1,   200,
      -1,   197,    -1,   107,    15,   194,    16,    -1,   201,   107,
      15,   194,    16,    -1,   306,    -1,   201,   306,    -1,   198,
     303,    -1,   198,   306,    -1,   203,    -1,   201,   203,    -1,
     199,   303,    -1,    78,    -1,   201,    78,    -1,   200,   303,
      -1,   303,    -1,   201,   303,    -1,   304,    -1,   202,   304,
      -1,   242,    -1,   252,    -1,   205,    -1,   208,    -1,   215,
      -1,   219,    -1,   313,   204,    -1,   206,    -1,     4,   224,
     205,    -1,     4,   224,    15,   207,    16,    -1,     4,   224,
     202,    15,   207,    16,    -1,     4,   224,   202,   205,    -1,
      15,   205,    16,    -1,    15,   207,   214,    16,    -1,    15,
     205,    16,   214,    -1,    78,    -1,    15,   207,    16,    -1,
      78,    -1,    78,   214,    -1,   209,    -1,   210,    -1,     4,
     224,   208,    -1,     4,   224,   202,   208,    -1,    15,   209,
      16,    -1,    15,   209,    16,   214,    -1,   212,    -1,   213,
      -1,   214,    -1,     4,   224,    -1,     4,   224,   202,    -1,
       4,   224,   211,    -1,     4,   224,   202,   211,    -1,    15,
     212,    16,    -1,    15,   213,    16,    -1,    15,   214,    16,
      -1,    15,   212,    16,   214,    -1,   241,    -1,    15,    16,
      -1,    15,   238,    16,    -1,   216,    -1,   218,    -1,   217,
      -1,     4,   224,   215,    -1,     4,   224,   202,   215,    -1,
     218,   214,    -1,    15,   216,    16,    -1,    15,   216,    16,
     214,    -1,    67,    -1,    15,   218,    16,    -1,   118,    -1,
     220,    -1,     4,   224,   219,    -1,     4,   224,   202,   219,
      -1,   218,    15,   221,    16,    -1,    15,   219,    16,    -1,
      15,   219,    16,   214,    -1,    67,    -1,   221,    22,    67,
      -1,    67,    -1,    78,    -1,   195,    -1,   195,   211,    -1,
     201,    -1,   201,   211,    -1,   116,    -1,    -1,   225,    -1,
     226,    -1,   225,   226,    -1,   103,    15,    15,   227,    16,
      16,    -1,   228,    -1,   227,    22,   228,    -1,    -1,   232,
      -1,   232,    15,    67,    16,    -1,   232,    15,    67,    22,
     229,    16,    -1,   232,    15,   229,    16,    -1,   135,    -1,
      -1,   231,    -1,   119,    15,   302,    16,    -1,    67,    -1,
      78,    -1,    42,    -1,    -1,    91,   234,    -1,   235,    -1,
      67,    25,   235,    -1,    78,    25,   235,    -1,    17,    67,
      91,   235,    -1,    17,    78,    91,   235,    -1,    23,   150,
      24,   235,    -1,    23,   150,    24,    91,   235,    -1,    23,
     150,    90,   150,    24,   235,    -1,   236,    -1,   150,    -1,
      20,    21,    -1,    20,   237,    21,    -1,    20,   237,    22,
      21,    -1,   234,    -1,   237,    22,   234,    -1,   239,    -1,
     239,    22,    90,    -1,    90,    -1,   240,    -1,   239,    22,
     240,    -1,   240,    91,   234,    -1,   239,    22,     1,    -1,
     224,   187,    -1,   224,   187,   211,    -1,   224,   187,   215,
      -1,   224,   187,   208,    -1,   224,   192,    -1,   224,   192,
     211,    -1,   224,   192,   215,    -1,   224,   195,    -1,   224,
     195,   211,    -1,   224,   195,   215,   224,    -1,   224,   195,
     208,    -1,   224,   201,    -1,   224,   201,   211,    -1,   224,
     201,   215,    -1,    23,    24,    -1,    23,   153,    24,    -1,
     241,    23,   153,    24,    -1,   241,    23,    24,    -1,   243,
      20,   244,    21,    -1,   243,   222,    20,   244,    21,    -1,
     243,   222,    -1,   243,    20,    21,    -1,   243,   222,    20,
      21,    -1,    29,    -1,    41,    -1,   224,   245,    -1,   244,
     224,   245,    -1,   247,    19,    -1,   246,    19,    -1,   313,
     245,    -1,   201,   249,    -1,   246,    22,   249,    -1,   195,
     248,    -1,   196,   225,   248,    -1,   243,    20,   244,    21,
      -1,   247,    22,   248,    -1,   204,   250,   224,    -1,   215,
     250,   224,    -1,    -1,   251,    -1,    25,   153,    -1,    35,
      20,   253,   255,    21,    -1,    35,   222,    20,   253,   255,
      21,    -1,    35,   222,    -1,   222,   254,    -1,   253,    22,
     222,   254,    -1,    -1,    91,   153,    -1,    -1,    22,    -1,
     257,    -1,   258,    -1,   266,    -1,   267,    -1,   269,    -1,
     277,    -1,   282,    -1,   310,    -1,     1,    19,    -1,    67,
      25,    -1,    34,   153,    25,    -1,    50,    25,    -1,    78,
      25,    -1,   259,   260,    -1,   259,   264,   260,    -1,   259,
     265,   260,    -1,   259,   264,   265,   260,    -1,   261,   262,
      -1,   261,   264,   262,    -1,   261,   265,   262,    -1,   261,
     264,   265,   262,    -1,    20,    -1,    21,    -1,   125,    -1,
     126,    -1,    20,    21,    -1,    20,   264,    21,    -1,    20,
     265,    21,    -1,    20,   264,   265,    21,    -1,   155,    -1,
     264,   155,    -1,   256,    -1,   265,   256,    -1,   154,    19,
      -1,    55,    15,   152,    16,   256,    -1,    55,    15,   152,
      16,   256,    31,   256,    -1,    -1,    33,   268,    15,   152,
      16,   256,    -1,    -1,    57,   270,    15,   152,    16,   256,
      -1,    -1,    54,   271,   256,    57,    15,   152,    16,    19,
      -1,    -1,    47,    15,   154,    19,   154,    19,   154,    16,
     272,   256,    -1,    -1,    47,    15,     1,    19,   154,    19,
     154,    16,   273,   256,    -1,    -1,    47,    15,   154,    19,
     154,    19,     1,    16,   274,   256,    -1,    -1,    47,    15,
     154,    19,     1,    19,   154,    16,   275,   256,    -1,    -1,
      47,    15,     1,    16,   276,   256,    -1,    51,    67,    19,
      -1,    46,    19,    -1,    30,    19,    -1,    40,   154,    19,
      -1,    51,    78,    19,    -1,    -1,   279,    -1,   280,    -1,
     279,    22,   280,    -1,   302,    15,   150,    16,    -1,   302,
      -1,   281,    22,   302,    -1,   119,    15,   302,    16,    19,
      -1,   119,    15,   302,    25,   278,    16,    19,    -1,   119,
      15,   302,    25,   278,    25,   278,    16,    19,    -1,   119,
      15,   302,    25,   278,    25,   278,    25,   281,    16,    19,
      -1,   119,   303,    15,   302,    16,    19,    -1,   119,   303,
      15,   302,    25,   278,    16,    19,    -1,   119,   303,    15,
     302,    25,   278,    25,   278,    16,    19,    -1,   119,   303,
      15,   302,    25,   278,    25,   278,    25,   281,    16,    19,
      -1,   284,    -1,   283,   284,    -1,   155,    -1,   285,    -1,
     313,   284,    -1,    19,    -1,    -1,   215,   286,   263,    -1,
     215,    66,    -1,    -1,   192,   215,   287,   263,    -1,    -1,
     201,   215,   288,   263,    -1,    -1,   219,   289,   263,    -1,
      -1,   187,   219,   290,   263,    -1,    -1,   195,   219,   291,
     263,    -1,    -1,   192,   219,   292,   263,    -1,    -1,   201,
     219,   293,   263,    -1,    -1,   219,   299,   294,   263,    -1,
      -1,   187,   219,   299,   295,   263,    -1,    -1,   195,   219,
     299,   296,   263,    -1,    -1,   192,   219,   299,   297,   263,
      -1,    -1,   201,   219,   299,   298,   263,    -1,    -1,   300,
     264,    -1,    69,    -1,    70,    -1,    71,    -1,    72,    -1,
      74,    -1,    68,    -1,   302,    68,    -1,    42,    -1,    53,
      -1,   102,    -1,   105,    -1,    75,    -1,   120,    -1,   123,
      -1,   128,    -1,   129,    -1,   113,    -1,   114,    -1,   115,
      -1,    42,    -1,    53,    -1,   105,    -1,   120,    -1,   123,
      -1,   114,    -1,   115,    -1,    37,    -1,    39,    -1,    56,
      -1,    26,    -1,    36,    -1,    49,    -1,    38,    -1,    28,
      -1,    43,    -1,    27,    -1,    48,    -1,    45,    -1,    44,
      -1,    32,    -1,   133,    -1,   307,    23,   152,    24,    -1,
     307,    17,    67,    -1,   307,    79,    67,    -1,   307,    80,
      -1,   307,    81,    -1,   307,    17,    78,    -1,   307,    79,
      78,    -1,   307,    -1,    -1,   135,    -1,   121,   308,    15,
     309,    16,    19,    -1,   138,   151,   121,   308,    15,   309,
      16,    19,    -1,   122,    19,    -1,   134,    15,   311,    16,
      19,    -1,   124,    19,    -1,   121,   308,    15,   309,    16,
      -1,   121,   308,    15,   309,    16,    22,   135,    -1,   127,
      -1,   104,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   395,   395,   399,   400,   401,   405,   417,   418,   419,
     420,   425,   430,   435,   436,   442,   451,   452,   454,   456,
     458,   460,   462,   464,   468,   470,   473,   479,   488,   490,
     495,   497,   499,   501,   504,   506,   508,   510,   512,   514,
     516,   518,   523,   524,   525,   526,   527,   528,   532,   533,
     537,   541,   542,   544,   546,   551,   552,   554,   559,   560,
     562,   567,   568,   570,   572,   574,   579,   580,   582,   587,
     588,   593,   594,   602,   603,   610,   611,   616,   617,   624,
     625,   627,   636,   637,   644,   645,   646,   647,   648,   649,
     650,   651,   652,   653,   654,   658,   659,   674,   678,   679,
     721,   723,   725,   727,   734,   733,   741,   740,   748,   747,
     760,   759,   777,   780,   781,   775,   789,   793,   794,   786,
     801,   804,   805,   799,   813,   817,   818,   810,   824,   827,
     828,   823,   841,   840,   852,   851,   862,   869,   872,   868,
     880,   883,   879,   891,   892,   890,   899,   898,   909,   908,
     917,   922,   923,   927,   929,   930,   941,   943,   945,   947,
     954,   956,   958,   965,   967,   970,   977,   982,   988,  1002,
    1003,  1007,  1008,  1013,  1014,  1018,  1020,  1021,  1025,  1036,
    1052,  1053,  1055,  1057,  1063,  1064,  1066,  1073,  1075,  1077,
    1083,  1086,  1092,  1093,  1100,  1101,  1106,  1107,  1108,  1109,
    1114,  1120,  1121,  1124,  1127,  1130,  1138,  1141,  1144,  1151,
    1153,  1160,  1162,  1164,  1173,  1174,  1177,  1184,  1187,  1194,
    1195,  1196,  1201,  1203,  1205,  1208,  1215,  1218,  1221,  1224,
    1231,  1232,  1233,  1239,  1240,  1245,  1246,  1251,  1258,  1260,
    1263,  1270,  1272,  1275,  1286,  1290,  1293,  1300,  1302,  1305,
    1320,  1322,  1328,  1329,  1334,  1336,  1338,  1340,  1342,  1354,
    1355,  1360,  1362,  1370,  1379,  1381,  1387,  1388,  1390,  1392,
    1403,  1408,  1415,  1416,  1419,  1424,  1425,  1426,  1431,  1432,
    1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1450,  1451,
    1455,  1458,  1459,  1463,  1465,  1475,  1476,  1479,  1488,  1490,
    1494,  1499,  1505,  1507,  1510,  1513,  1516,  1518,  1520,  1522,
    1524,  1527,  1531,  1534,  1536,  1538,  1544,  1546,  1548,  1550,
    1572,  1576,  1581,  1586,  1592,  1602,  1603,  1608,  1613,  1622,
    1623,  1624,  1630,  1636,  1642,  1644,  1649,  1657,  1664,  1671,
    1678,  1679,  1684,  1689,  1691,  1693,  1699,  1701,  1707,  1708,
    1712,  1713,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1732,  1737,  1739,  1741,  1745,  1750,  1752,  1756,  1761,  1767,
    1769,  1773,  1778,  1785,  1786,  1789,  1790,  1796,  1798,  1800,
    1805,  1815,  1816,  1821,  1822,  1827,  1834,  1836,  1838,  1838,
    1844,  1843,  1848,  1847,  1852,  1851,  1858,  1857,  1862,  1861,
    1866,  1865,  1869,  1869,  1874,  1876,  1878,  1880,  1884,  1890,
    1891,  1894,  1896,  1900,  1904,  1906,  1910,  1912,  1914,  1916,
    1918,  1920,  1922,  1924,  1935,  1936,  1940,  1950,  1960,  1962,
    1967,  1966,  1975,  1990,  1989,  1999,  1998,  2008,  2007,  2017,
    2016,  2025,  2024,  2033,  2032,  2042,  2041,  2051,  2050,  2059,
    2058,  2066,  2065,  2073,  2072,  2082,  2081,  2095,  2095,  2112,
    2113,  2114,  2115,  2116,  2121,  2122,  2144,  2145,  2146,  2147,
    2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2159,  2160,
    2161,  2162,  2163,  2164,  2165,  2169,  2170,  2171,  2172,  2173,
    2177,  2178,  2179,  2180,  2181,  2183,  2184,  2186,  2187,  2193,
    2194,  2196,  2198,  2200,  2202,  2206,  2208,  2213,  2218,  2219,
    2225,  2229,  2234,  2238,  2247,  2252,  2254,  2259,  2266
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'&'", "'*'", "'+'", "'-'", "'~'", "'!'",
  "'<'", "'>'", "'|'", "'^'", "'%'", "'/'", "'('", "')'", "'.'", "'?'",
  "';'", "'{'", "'}'", "','", "'['", "']'", "':'", "AUTO", "DOUBLE", "INT",
  "STRUCT", "BREAK", "ELSE", "LONG", "SWITCH", "CASE", "ENUM", "REGISTER",
  "TYPEDEF", "CHAR", "EXTERN", "RETURN", "UNION", "CONST", "FLOAT",
  "SHORT", "UNSIGNED", "CONTINUE", "FOR", "SIGNED", "VOID", "DEFAULT",
  "GOTO", "SIZEOF", "VOLATILE", "DO", "IF", "STATIC", "WHILE", "UPLUS",
  "UMINUS", "INDIR", "ADDRESS", "POSTINC", "POSTDEC", "PREINC", "PREDEC",
  "BOGUS", "IDENTIFIER", "STRINGliteral", "FLOATINGconstant",
  "INTEGERconstant", "OCTALconstant", "HEXconstant", "WIDECHARconstant",
  "CHARACTERconstant", "COMPLEX", "REAL", "IMAG", "TYPEDEFname", "ARROW",
  "ICR", "DECR", "LS", "RS", "LE", "GE", "EQ", "NE", "ANDAND", "OROR",
  "ELLIPSIS", "'='", "MULTassign", "DIVassign", "MODassign", "PLUSassign",
  "MINUSassign", "LSassign", "RSassign", "ANDassign", "ERassign",
  "ORassign", "INLINE", "ATTRIBUTE", "EXTENSION", "RESTRICT", "ALIGNOF",
  "TYPEOF", "DECLSPEC_EXPORT", "DECLSPEC_IMPORT", "BUILTIN_VA_ARG",
  "DESIGNATED_INITIALIZER", "ARRAY_LABELED_INITIALIZER", "NOOUTPUT",
  "CILKONLY", "CILKSAFE", "WILDCARD_T", "WILDCARD_E", "WILDCARD_D", "ASM",
  "PROCEDURE", "SPAWN", "SYNC", "INLET", "ABORT", "XBEGIN", "XEND",
  "SYNCHED", "SHAREDVAR", "PRIVATEVAR", "$accept", "prog.start",
  "string.start.symbol", "primary.expression", "postfix.expression",
  "argument.expression.list", "unary.expression", "unary.operator",
  "cast.expression", "multiplicative.expression", "additive.expression",
  "shift.expression", "relational.expression", "equality.expression",
  "AND.expression", "exclusive.OR.expression", "inclusive.OR.expression",
  "logical.AND.expression", "logical.OR.expression",
  "conditional.expression", "assignment.expression", "assignment.operator",
  "expression", "constant.expression", "expression.opt", "declaration",
  "@1", "@2", "@3", "@4", "declaring.list", "@5", "@6", "@7", "@8", "@9",
  "@10", "@11", "@12", "@13", "@14", "@15", "@16", "@17", "@18", "@19",
  "@20", "@21", "default.declaring.list", "@22", "@23", "@24", "@25",
  "@26", "@27", "@28", "@29", "declaration.specifier",
  "declaration.specifier.nosue", "basic.declaration.specifier",
  "sue.declaration.specifier", "typedef.declaration.specifier",
  "declaration.qualifier.list", "declaration.qualifier",
  "typeof.expression", "type.specifier", "type.specifier.nosue",
  "typeof.type.specifier", "basic.type.specifier", "sue.type.specifier",
  "typedef.type.specifier", "type.qualifier.list",
  "pointer.type.qualifier.list", "elaborated.type.name", "declarator",
  "paren.typedef.declarator", "paren.postfix.typedef.declarator",
  "simple.paren.typedef.declarator", "parameter.typedef.declarator",
  "clean.typedef.declarator", "clean.postfix.typedef.declarator",
  "abstract.declarator", "unary.abstract.declarator",
  "postfix.abstract.declarator", "postfixing.abstract.declarator",
  "identifier.declarator", "unary.identifier.declarator",
  "postfix.identifier.declarator", "paren.identifier.declarator",
  "old.function.declarator", "postfix.old.function.declarator",
  "identifier.list", "identifier.or.typedef.name", "type.name",
  "attributes.opt", "attributes", "attribute", "attribute.list", "attrib",
  "nonnull.expression.list", "asm.opt", "asm", "any.word",
  "initializer.opt", "initializer", "basic.initializer",
  "bracketed.initializer.list", "initializer.list", "parameter.type.list",
  "parameter.list", "parameter.declaration", "array.abstract.declarator",
  "struct.or.union.specifier", "struct.or.union",
  "struct.declaration.list", "struct.declaration",
  "struct.default.declaring.list", "struct.declaring.list",
  "struct.declarator", "struct.identifier.declarator",
  "bit.field.size.opt", "bit.field.size", "enum.specifier",
  "enumerator.list", "enumerator.value.opt", "comma.opt", "statement",
  "labeled.statement", "compound.statement", "lblock", "rblock", "xlblock",
  "xrblock", "compound.statement.no.new.scope", "declaration.list",
  "statement.list", "expression.statement", "selection.statement", "@30",
  "iteration.statement", "@31", "@32", "@33", "@34", "@35", "@36", "@37",
  "jump.statement", "asm.argument.list.opt", "asm.argument.list",
  "asm.argument", "asm.clobbered.list", "gcc.statement",
  "translation.unit", "external.definition", "function.definition", "@38",
  "@39", "@40", "@41", "@42", "@43", "@44", "@45", "@46", "@47", "@48",
  "@49", "@50", "old.function.declaration.list", "@51", "constant",
  "string.literal.list", "type.qualifier", "pointer.type.qualifier",
  "storage.class", "basic.type.name", "spawn.postfix.expression",
  "spawn.procedure", "spawn.args", "cilk.statement",
  "inlet.call.argument.expression.list", "cilk.expression",
  "gcc.extension", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    38,    42,    43,    45,   126,    33,    60,
      62,   124,    94,    37,    47,    40,    41,    46,    63,    59,
     123,   125,    44,    91,    93,    58,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,    61,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   130,   131,   131,   131,   131,   132,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   135,   135,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   137,   137,   137,   137,   137,   138,   138,
     138,   139,   139,   139,   139,   140,   140,   140,   141,   141,
     141,   142,   142,   142,   142,   142,   143,   143,   143,   144,
     144,   145,   145,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   150,   150,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   152,   152,   153,   154,   154,
     155,   155,   155,   155,   156,   155,   157,   155,   158,   155,
     159,   155,   161,   162,   163,   160,   164,   165,   166,   160,
     167,   168,   169,   160,   170,   171,   172,   160,   173,   174,
     175,   160,   176,   160,   177,   160,   160,   179,   180,   178,
     181,   182,   178,   183,   184,   178,   185,   178,   186,   178,
     178,   187,   187,   188,   188,   188,   189,   189,   189,   189,
     190,   190,   190,   191,   191,   191,   192,   192,   192,   193,
     193,   194,   194,   195,   195,   196,   196,   196,   197,   197,
     198,   198,   198,   198,   199,   199,   199,   200,   200,   200,
     201,   201,   202,   202,   203,   203,   204,   204,   204,   204,
     204,   205,   205,   205,   205,   205,   206,   206,   206,   207,
     207,   208,   208,   208,   209,   209,   209,   210,   210,   211,
     211,   211,   212,   212,   212,   212,   213,   213,   213,   213,
     214,   214,   214,   215,   215,   216,   216,   216,   217,   217,
     217,   218,   218,   218,   219,   219,   219,   220,   220,   220,
     221,   221,   222,   222,   223,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   227,   227,   228,   228,   228,   228,
     228,   229,   230,   230,   231,   232,   232,   232,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   235,   235,
     236,   236,   236,   237,   237,   238,   238,   238,   239,   239,
     239,   239,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   241,   241,   241,   241,
     242,   242,   242,   242,   242,   243,   243,   244,   244,   245,
     245,   245,   246,   246,   247,   247,   247,   247,   248,   249,
     250,   250,   251,   252,   252,   252,   253,   253,   254,   254,
     255,   255,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   257,   257,   257,   257,   258,   258,   258,   258,   258,
     258,   258,   258,   259,   260,   261,   262,   263,   263,   263,
     263,   264,   264,   265,   265,   266,   267,   267,   268,   267,
     270,   269,   271,   269,   272,   269,   273,   269,   274,   269,
     275,   269,   276,   269,   277,   277,   277,   277,   277,   278,
     278,   279,   279,   280,   281,   281,   282,   282,   282,   282,
     282,   282,   282,   282,   283,   283,   284,   284,   284,   284,
     286,   285,   285,   287,   285,   288,   285,   289,   285,   290,
     285,   291,   285,   292,   285,   293,   285,   294,   285,   295,
     285,   296,   285,   297,   285,   298,   285,   300,   299,   301,
     301,   301,   301,   301,   302,   302,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   304,   304,
     304,   304,   304,   304,   304,   305,   305,   305,   305,   305,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   308,   309,   309,
     310,   310,   310,   310,   310,   311,   311,   312,   313
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     2,     1,     1,     1,     1,
       3,     5,     6,     1,     1,     1,     1,     4,     3,     4,
       3,     3,     2,     2,     3,     3,     3,     3,     1,     3,
       1,     2,     2,     2,     2,     2,     4,     2,     4,     6,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       5,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     0,     1,
       2,     2,     2,     2,     0,     4,     0,     5,     0,     4,
       0,     5,     0,     0,     0,     8,     0,     0,     0,     9,
       0,     0,     0,     8,     0,     0,     0,     9,     0,     0,
       0,     9,     0,     6,     0,     6,     3,     0,     0,     6,
       0,     0,     6,     0,     0,     7,     0,     5,     0,     5,
       3,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     3,     2,     2,     2,     2,     2,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     5,
       1,     2,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     5,     6,     4,     3,     4,     4,     1,
       3,     1,     2,     1,     1,     3,     4,     3,     4,     1,
       1,     1,     2,     3,     3,     4,     3,     3,     3,     4,
       1,     2,     3,     1,     1,     1,     3,     4,     2,     3,
       4,     1,     3,     1,     1,     3,     4,     4,     3,     4,
       1,     3,     1,     1,     1,     2,     1,     2,     1,     0,
       1,     1,     2,     6,     1,     3,     0,     1,     4,     6,
       4,     1,     0,     1,     4,     1,     1,     1,     0,     2,
       1,     3,     3,     4,     4,     4,     5,     6,     1,     1,
       2,     3,     4,     1,     3,     1,     3,     1,     1,     3,
       3,     3,     2,     3,     3,     3,     2,     3,     3,     2,
       3,     4,     3,     2,     3,     3,     2,     3,     4,     3,
       4,     5,     2,     3,     4,     1,     1,     2,     3,     2,
       2,     2,     2,     3,     2,     3,     4,     3,     3,     3,
       0,     1,     2,     5,     6,     2,     2,     4,     0,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     2,     2,     2,     3,     3,     4,     2,
       3,     3,     4,     1,     1,     1,     1,     2,     3,     3,
       4,     1,     2,     1,     2,     2,     5,     7,     0,     6,
       0,     6,     0,     8,     0,    10,     0,    10,     0,    10,
       0,    10,     0,     6,     3,     2,     2,     3,     3,     0,
       1,     1,     3,     4,     1,     3,     5,     7,     9,    11,
       6,     8,    10,    12,     1,     2,     1,     1,     2,     1,
       0,     3,     2,     0,     4,     0,     4,     0,     3,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     2,     2,     3,     3,     1,     0,     1,
       6,     8,     2,     5,     2,     5,     7,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,   259,     0,     6,   429,   488,   494,   492,   325,   498,
       0,   489,   485,   491,   486,   326,   466,   493,   497,   496,
     495,   490,   467,   487,   241,   470,   187,   468,   518,   469,
       0,   475,   476,   477,   155,   243,   471,   472,   473,   474,
       0,     0,   426,     0,     0,     0,   151,   153,   152,   154,
       0,     0,   173,   177,   175,   174,   176,     0,   184,   430,
     233,   235,   234,   457,   244,   194,     0,   195,     3,   424,
     427,   190,   259,   180,     0,     0,     0,   260,   261,     0,
       0,     0,     0,   252,   253,   345,     0,     1,     0,    42,
      43,    44,    45,    46,    47,     0,   373,     0,   388,     0,
      98,     0,     0,     0,     0,     0,   392,     0,   390,     7,
     464,   459,   460,   461,   462,   463,     0,     0,   187,     0,
       0,     0,     0,    14,    15,     0,     0,     0,     0,   375,
     517,    16,    30,    48,     0,    51,    55,    58,    61,    66,
      69,    71,    73,    75,    77,    79,    82,    95,    99,     0,
     381,     0,     0,     0,     0,   383,   352,   353,     0,     0,
       5,     0,   354,   355,   356,   357,   358,     8,     9,   359,
      13,     0,   100,     0,   101,     0,   132,   259,     0,   211,
     112,   196,   201,   197,   213,   214,   198,   457,     0,     0,
     158,   169,   170,   159,   102,   162,   165,   146,   164,   168,
     259,   137,   457,   157,   134,   120,   198,   457,     0,   182,
     156,   183,   103,   186,   160,   189,   163,   148,   188,     0,
     185,   140,   457,     0,   191,   181,   432,     0,   259,     0,
     238,   230,     0,   447,     0,   259,   322,   425,   166,   428,
       0,   478,   479,   480,   483,   484,   481,   482,     0,   236,
     245,   192,   262,   239,   242,   248,   348,   350,     0,     7,
      30,    51,   171,     0,   172,   173,   175,   174,   176,     0,
     360,   258,     0,   254,   256,     0,     0,   406,     0,    51,
      97,     0,     0,   405,     0,   363,     0,     0,     0,    35,
       0,     0,     0,   361,     0,    41,    40,   364,    31,    32,
       0,    37,     0,     0,     0,   499,   507,     0,   512,   514,
       0,     0,     0,     0,    22,    23,    33,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   385,
     199,   259,     0,   137,   234,   140,   374,   365,     0,     0,
     376,   369,     0,     0,   382,     0,   384,   465,    34,   136,
     128,   198,   150,   143,   272,     0,     0,   209,     0,     0,
       0,   259,   212,   272,     0,     0,   449,   200,   116,   198,
     259,   161,   259,     0,     0,   453,   272,   272,     0,     0,
     451,   124,   198,   259,     0,   259,     0,     0,   455,   259,
       0,   431,   231,   250,   297,     0,     0,     0,   295,   298,
     316,     0,     0,   438,     0,   458,   323,     0,   259,   259,
     266,   237,   246,   193,   240,   249,     0,   346,   351,     0,
     350,     0,     0,   178,    10,   259,   259,   255,   219,   220,
     221,   257,     0,     0,     0,     0,   362,   407,     0,     0,
     404,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   503,   504,   508,    18,     0,     0,    28,
       0,    20,    24,    26,     0,    21,    25,    27,     0,    83,
      52,    54,    53,    56,    57,    59,    60,    62,    63,    64,
      65,    67,    68,    70,    72,    74,    76,     0,     0,    78,
      96,     0,     0,   366,     0,   367,   370,     0,   371,   272,
     259,     0,   259,   273,     0,     0,   202,   215,     0,   206,
       0,   217,   113,   105,   440,     0,   272,     0,   278,   138,
     434,   444,     0,   259,   121,   109,   442,     0,   272,     0,
     278,     0,   141,   436,   446,     0,   167,   377,     0,     0,
     247,     0,   302,   151,   152,   306,   309,   174,   259,   232,
       0,     0,   317,   319,     0,   448,     0,   173,     0,     0,
     327,     0,     0,     0,   320,     0,   324,   259,   277,   275,
     276,     0,   264,   267,   349,   348,   343,     0,   222,     0,
       0,     0,     0,    50,    49,     0,     0,     0,   402,    98,
       0,    36,     0,     0,     0,    38,     0,     0,   409,     0,
     501,   505,     0,   502,   506,   509,     0,     0,    19,     0,
       0,    17,     0,    81,     0,     0,   368,   372,   129,   144,
       0,   278,     0,     0,   205,   216,   210,   208,   207,   218,
     259,   450,   117,   107,     0,   147,   278,   454,   278,   259,
     452,   125,   111,   149,   179,   278,   456,   378,     0,   379,
     251,   259,   259,   305,   303,   304,   259,   259,   307,   308,
     312,   310,   259,   314,   315,   301,   296,   299,     0,     0,
       7,     0,   289,   300,   280,   288,   318,   340,   334,     0,
     340,   332,   259,   330,     0,   329,     0,   331,   328,   321,
       0,   266,     0,   347,   344,   223,   224,   226,   227,   228,
     290,   293,     0,     0,    11,     0,     0,     0,     0,     0,
       0,   386,     0,     0,   416,     0,   410,   411,     0,     0,
     409,   500,     0,   508,    29,   513,   508,    80,   259,   278,
       0,   133,   203,     0,   114,   259,   279,   139,   135,   122,
     259,   142,   380,   222,   222,   311,     0,     0,     0,     0,
       0,     0,   259,   341,   335,   259,   259,   333,   337,   263,
     265,     7,   271,     0,   225,   229,   291,     0,    12,   389,
     403,    98,    98,     0,     0,     0,   391,    39,     0,   409,
       0,     0,   420,     0,   510,     0,     0,   130,   145,   274,
     204,   278,   118,   278,   126,   223,   223,     0,     0,     0,
       0,   281,   282,   342,   338,   339,   320,   268,     0,   270,
     292,   294,     0,     0,     0,     0,     0,   387,   417,     0,
     412,     0,     0,   409,   515,     0,   278,   115,   278,   123,
     278,   283,   284,     0,   285,     0,     0,   396,   400,   398,
     394,   393,     0,     0,   413,   421,     0,     0,   511,   131,
     119,   127,   286,     0,   269,     0,     0,     0,     0,   418,
       0,   414,     0,     0,   516,   287,   397,   401,   399,   395,
       0,     0,   422,     0,   419,   415,     0,   423
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    40,    41,   131,   260,   625,   133,   134,   261,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   328,   148,   281,   149,   150,   384,   537,   398,   549,
      43,   383,   650,   811,   536,   755,   848,   397,   659,   813,
     548,   760,   850,   519,   748,   846,   374,   396,    44,   392,
     656,   405,   665,   520,   749,   390,   403,   151,    46,    47,
      48,    49,   152,   199,   263,   153,    52,    53,    54,    55,
      56,   154,   248,    58,   697,   181,   182,   379,   183,   184,
     185,   716,   448,   449,   450,   249,    60,    61,    62,   350,
      64,   415,   236,   275,   416,    77,    78,   591,   592,   783,
     522,   523,   593,   655,   693,   694,   695,   722,   417,   418,
     419,   231,    65,    66,   428,   580,   581,   582,   698,   701,
     772,   773,    67,   257,   437,   439,   155,   156,   157,   158,
     357,   159,   361,   411,   160,   161,   162,   163,   278,   164,
     292,   290,   878,   875,   877,   876,   726,   165,   735,   736,
     737,   880,   166,    68,    69,    70,   227,   393,   406,   232,
     385,   399,   394,   407,   424,   535,   547,   542,   555,   233,
     234,   167,   168,    71,   251,    72,    73,   306,   307,   626,
     169,   480,   170,   171
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -698
static const yytype_int16 yypact[] =
{
    3427,   -56,    52,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
      46,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
      44,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
     132,  1587,  -698,    81,   337,   722,   -56,  5435,  3837,  2699,
    1930,   805,   -56,  -698,  5435,  5336,  2699,  1716,  -698,    85,
    -698,  -698,   110,   185,  -698,  -698,   324,  -698,  3533,  -698,
    -698,  -698,   -56,  -698,  3533,   158,   851,   -56,  -698,   236,
     492,   243,    -3,  -698,  -698,   287,  2460,  -698,   316,  -698,
    -698,  -698,  -698,  -698,  -698,  2333,  -698,   321,  -698,  4973,
    4973,   361,   378,   391,    70,  5052,  -698,   411,  -698,   416,
    -698,  -698,  -698,  -698,  -698,  -698,  5131,  5131,   424,  5131,
    5131,  5210,   421,  -698,  -698,   911,  1211,   434,   439,  -698,
    -698,  -698,   972,  -698,  4973,  1320,   328,   606,   548,   199,
     582,   467,   466,   490,   441,     1,  -698,  -698,   511,   527,
    -698,   722,  2036,   805,  1823,  -698,  -698,  -698,  1066,  1195,
    5391,  2225,  -698,  -698,  -698,  -698,  -698,  -698,   483,  -698,
    -698,  4973,  -698,   874,  -698,   431,  -698,   -56,    34,   200,
    -698,  -698,  -698,  -698,  -698,  -698,   538,   291,   356,   943,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
     -56,   542,   551,  -698,  -698,  -698,   567,   730,   943,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,   576,
    -698,   589,   596,   623,  -698,  -698,  -698,   600,   183,  4419,
    -698,   612,   600,  -698,  5391,    11,   603,  -698,  -698,  -698,
     626,  -698,  -698,  -698,  -698,  -698,  -698,  -698,   851,  -698,
    -698,  -698,  -698,   200,  -698,   200,   554,   633,    -3,  -698,
    1074,  1320,   511,   642,  -698,  -698,  5669,  1688,  1688,  5650,
    -698,  -698,   153,    56,  3960,   645,  1587,  -698,   648,  -698,
    -698,   640,   651,  -698,  3172,  -698,   653,   663,  2333,  -698,
    3091,  4973,   675,  -698,  4498,  -698,  -698,  -698,  -698,  -698,
    2333,  -698,  4973,   624,   682,  -698,   553,   684,  -698,  -698,
    4261,   245,  4973,   270,  -698,  -698,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  4577,  4973,
    4973,  4973,  4973,  4973,  4973,  4973,  4973,  4973,  4973,  4973,
    4973,  4973,  4973,  4973,  4973,  4973,  4657,  4973,  4973,  -698,
    -698,   -56,    77,  -698,   200,  -698,  -698,  -698,  1066,  2589,
    -698,  -698,  1195,  2714,  -698,   424,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,   593,  2313,    34,  -698,   701,   200,
     709,    20,  -698,   593,   600,   600,  -698,  -698,  -698,   712,
     -56,  -698,   -56,   600,   600,  -698,   593,   593,   600,   600,
    -698,  -698,   720,   -56,  2460,   -56,   600,   600,  -698,   -56,
    1329,  -698,  -698,  -698,  -698,   376,  5391,   727,   729,   662,
    -698,   734,  4736,  -698,   600,  5391,  -698,  5524,    40,    64,
       6,  -698,  -698,  -698,  -698,  -698,  4973,  -698,    -3,   738,
     633,  4815,  4973,  -698,  -698,   -56,   770,  -698,  -698,  -698,
    -698,  -698,  4894,  1587,  2589,  4973,  -698,  -698,   357,   741,
    -698,  -698,   745,   705,   402,  4973,   749,   744,   146,   624,
     135,  4973,   152,  -698,  -698,  4973,  -698,  1211,   423,  -698,
     755,  -698,  -698,  -698,   263,  -698,  -698,  -698,  1211,  -698,
    -698,  -698,  -698,   328,   328,   606,   606,   548,   548,   548,
     548,   199,   199,   582,   467,   466,   490,  4973,   382,   441,
    -698,  1880,   524,  -698,  2589,  -698,  -698,  2714,  -698,   593,
     -56,   757,   -56,  -698,    34,  2402,  -698,  -698,   569,   200,
     760,   200,  -698,  -698,  -698,   600,   593,   600,   687,  -698,
    -698,  -698,   600,   -56,  -698,  -698,  -698,   600,   593,   600,
     687,   765,  -698,  -698,  -698,   600,  -698,  -698,  1458,  2841,
    -698,   706,   876,  -698,  2699,  3637,   876,  2699,  3752,  -698,
    2134,  4182,  -698,  -698,   759,  -698,   356,   -56,  3856,   352,
    -698,   435,   470,  5524,  -698,  5524,  -698,    86,  -698,  -698,
    -698,   440,  -698,   769,  -698,   554,  -698,   767,  1986,   774,
     776,   782,  4024,  -698,  -698,  2589,   785,   453,  -698,  4973,
    3253,  -698,   787,  3091,   465,  -698,  5552,   784,   624,   252,
    -698,  -698,   292,  -698,  -698,   786,   788,   795,  -698,  4973,
     793,  -698,   802,  -698,  4973,  1880,  -698,  -698,  -698,  -698,
     624,   687,   599,    34,  -698,  -698,  -698,  -698,  -698,  -698,
     -56,  -698,  -698,  -698,  4182,  -698,   687,  -698,   687,   -56,
    -698,  -698,  -698,  -698,  -698,   687,  -698,  -698,  2966,  -698,
    -698,   -56,   854,  -698,  -698,  -698,   -56,  1060,  -698,  -698,
    -698,  -698,   -56,  -698,  -698,  -698,  -698,  -698,   154,  4973,
     794,   816,  -698,  -698,  -698,  -698,  -698,   825,  -698,   943,
     825,  -698,    11,  -698,    77,  -698,   356,  -698,  -698,  -698,
     837,     6,  5289,  -698,  -698,  1986,  -698,   200,  -698,  -698,
    -698,  -698,   659,   840,  -698,  3091,  3091,   799,   838,   842,
    4973,   828,  3091,   847,  -698,   165,   843,  -698,    26,   848,
     624,  -698,   849,  4973,  -698,  -698,  4973,  -698,   -56,   687,
      30,  -698,  -698,   704,  -698,   -56,  -698,  -698,  -698,  -698,
     -56,  -698,  -698,  2079,  1773,  -698,   780,   790,    31,  4894,
    4894,  4973,   -56,  -698,  -698,   -56,    95,  -698,  -698,  -698,
    -698,   487,   786,   860,  -698,  -698,  -698,  4103,  -698,  -698,
    -698,  4973,  4973,  3334,   506,  3091,  -698,  -698,   863,   624,
     624,  4973,  -698,   181,  -698,   871,   872,  -698,  -698,  -698,
    -698,   687,  -698,   687,  -698,  2079,  1773,  4894,  4894,  4340,
    4973,  -698,  -698,  -698,  -698,  -698,   526,  -698,  4973,  -698,
    -698,  -698,   878,   886,   894,   898,   873,  -698,  -698,   222,
    -698,   900,   877,   624,   857,   903,   687,  -698,   687,  -698,
     687,  -698,  -698,  4894,  -698,   901,   908,  -698,  -698,  -698,
    -698,  -698,   915,   624,  -698,  -698,   241,  4973,  -698,  -698,
    -698,  -698,  -698,  4894,  -698,  3091,  3091,  3091,  3091,  -698,
     508,   483,   920,   624,   786,  -698,  -698,  -698,  -698,  -698,
     923,   624,  -698,   520,  -698,   483,   926,  -698
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -698,  -698,  -698,  -109,   -32,  -284,   880,  -698,   -41,   375,
     400,   175,   398,   586,   587,   602,   610,   601,  -698,   -93,
     162,   698,   -66,  -218,   -90,     8,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,     4,   544,  -698,
     545,  -698,    21,   -33,   558,   159,    36,  -698,   269,    61,
     301,   279,  -270,   -13,   762,  -139,  -698,  -371,  -247,  -147,
    -698,  -165,  -424,  -396,   -29,    12,    -1,  -698,    32,   104,
    -698,  -698,    17,  -258,    39,   -39,   -74,  -698,   256,   140,
     138,  -698,  -698,    90,  -584,   909,   521,  -698,  -698,  -698,
     405,  -698,  -698,  -425,  -394,  -158,  -698,  -698,  -654,   272,
     277,  -698,  -698,   725,   389,   562,  -108,  -698,  -698,    45,
    -271,  -698,  -338,   513,    33,   480,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -697,  -698,
     188,   108,  -698,  -698,     9,  -698,  -698,  -698,  -698,  -698,
    -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,  -698,   284,
    -698,  -698,   -86,   775,  -220,   215,    88,  -698,  -423,  -141,
    -698,  -698,  -698,    23
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -446
static const yytype_int16 yytable[] =
{
     135,    79,   579,   252,    45,   528,   280,   189,    42,   132,
     282,   421,    59,   208,   190,   195,   196,   305,   721,   346,
     262,    50,   599,    74,   516,   518,   478,    85,   433,   272,
     462,   380,   426,   230,    80,   587,   412,   200,   177,   378,
      76,   801,   466,   803,   220,   774,   809,    75,   588,   376,
     600,   230,   778,   366,   627,   819,     1,   186,   279,    86,
     445,   584,   201,   206,    83,   632,    82,     2,   188,   221,
     756,   446,    45,   589,   188,    84,    42,   237,    45,   229,
      59,   351,    42,   239,   590,   586,    59,   513,   515,    50,
     347,    74,   352,   316,   367,    50,   223,    74,   367,   256,
     172,    24,   839,   173,    63,   525,    81,   709,   447,   451,
     414,   238,   377,    83,    75,   252,   826,   135,   135,    24,
     135,   820,   265,    75,    84,   228,   132,   132,   527,   132,
     368,   265,    87,   229,   252,   193,   280,   286,   203,   200,
     276,   220,   211,    75,    24,   225,   866,   267,   287,   187,
     382,   226,    35,   642,   202,   207,   267,   478,   579,    51,
     579,   222,   617,   186,   353,   206,   355,    75,   364,   444,
      35,   618,    63,   240,   188,   348,   188,    79,    63,   637,
     250,   798,   463,   606,   354,   371,   354,   373,   279,    75,
     799,   358,   362,   223,   459,    35,   188,   842,    75,   412,
     371,   389,   620,   831,   574,  -437,   843,   354,   336,   337,
      80,   188,   188,   621,   367,   381,   375,   468,   594,   623,
     402,   766,   272,   229,   434,   464,   435,    51,   272,   380,
     624,   188,   767,    51,   272,   135,   526,   378,   862,   391,
     203,   635,   225,   636,   132,   264,   484,   863,   599,   135,
     413,   366,   253,   599,   273,   366,   220,   882,   132,   255,
     431,   220,   192,   192,   192,   192,   883,   425,   739,   210,
     214,   216,   753,   414,   427,   256,   600,   740,   645,    57,
     508,   600,    81,   338,   339,   348,    75,   631,   490,   491,
     492,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   433,   279,   258,   776,   453,
    -199,  -439,   481,  -199,   348,   673,   741,   135,   135,   680,
     367,   135,   135,   482,   265,   230,   132,   132,   715,   280,
     132,   132,   329,   276,   723,   270,   265,   485,   262,   276,
     277,   330,   331,   280,   235,   276,   366,    57,   486,   267,
     530,    79,   432,    57,   211,   266,   174,   225,   733,   175,
     177,   267,   225,   483,   266,   269,   364,   192,   305,   135,
     364,   178,   702,   608,   274,    79,   609,   380,   132,   305,
     283,   279,  -199,   619,   512,   378,   644,   268,   487,   607,
     511,    83,   560,   284,  -199,   279,   268,   674,   561,   614,
     678,   681,    84,   683,   348,   622,   366,   634,    80,   366,
    -199,   603,   135,   135,   633,   433,   285,   601,   613,    83,
     562,   132,   132,    24,   348,   707,   291,   708,   782,   538,
      84,   539,   372,   364,   179,   351,   302,   565,   409,   628,
     265,   293,   550,   558,   552,   629,   352,   273,   556,   297,
     583,   366,   265,   308,   703,   595,   710,   704,   309,   273,
      28,   364,   711,   577,   467,   267,   279,   585,   427,   725,
     342,   386,   479,   135,    35,   348,   135,   567,   343,   250,
      81,   732,   132,   230,   598,   132,   395,   348,   267,   705,
     489,   400,   706,   815,   816,   433,   380,   366,    24,   530,
     647,   344,   649,   827,   378,   731,   408,   228,   254,   828,
     510,   497,   498,   499,   500,   229,   527,   135,   135,   727,
     729,   532,   836,    79,   890,   380,   132,   132,   348,   345,
     891,   195,   738,   348,   543,   544,   896,   431,   699,   381,
     254,   747,   891,   354,   782,  -336,   349,   229,  -336,    35,
     784,   367,   200,   823,   750,   220,    80,   266,  -104,   639,
     366,   641,  -433,   264,   135,   220,   364,   274,   645,   266,
     470,  -443,   135,   132,   675,   566,   471,   679,   682,   274,
     684,   132,   658,   884,   381,   646,   576,  -108,   371,   268,
     700,   404,   229,   279,   354,   433,   433,   354,   354,   188,
     354,   268,   805,   479,   489,   806,   583,   223,   583,  -435,
     354,   332,   333,   530,   381,   752,  -445,   789,   790,   577,
     410,   577,   229,   429,   796,   252,   585,   135,    81,   432,
     334,   335,   472,   473,   474,   422,   132,   479,   359,   363,
     663,   430,    79,   601,   267,   436,   267,   431,   601,     5,
     784,   784,   265,   203,   738,   438,   225,   638,   443,    11,
      12,   452,    14,   455,   794,   456,   225,   354,   340,   341,
     457,    79,   460,   266,   652,    80,    79,   267,   280,    23,
     786,   787,   461,   269,   135,   135,   661,   837,   785,   754,
     465,   135,   110,   132,   132,   568,   266,   469,   759,   475,
     132,   832,   833,   835,   512,   268,   578,   493,   494,   512,
     763,   371,   521,   738,   738,   764,   700,   529,   371,   381,
     810,   765,   188,   176,   530,   531,   177,   229,   268,   188,
     279,   751,  -106,   692,   495,   496,   354,   178,   501,   502,
    -110,   427,   576,   569,   576,   423,   757,    81,   758,  -199,
    -441,   570,  -199,   571,   135,   761,   454,   738,   572,   596,
     610,   611,   612,   132,   692,   615,   616,   886,   887,   888,
     889,   630,   640,   670,   445,   273,   648,   881,   654,   192,
     192,   664,   214,   696,   712,   446,   412,   807,   714,    24,
     717,   744,   718,   229,   812,   354,   354,   881,   719,   814,
     179,   724,   730,   734,   742,   895,   204,   180,   629,   177,
     743,   824,   745,   205,   825,   585,   692,   746,   791,   769,
     178,  -199,   191,   191,   191,   191,    28,   431,   431,   209,
     213,   215,   224,  -199,   135,   135,   135,   135,   514,   808,
      35,   770,   517,   132,   132,   132,   132,   354,   354,  -199,
     771,   768,   266,   779,   266,     1,   788,   792,   671,   795,
     414,   793,   578,   797,   578,   800,     2,   802,   804,   672,
     412,   817,    24,    75,   479,   369,   829,   229,   177,   867,
     671,   818,   838,   179,   268,   266,   268,   844,   845,   178,
     559,   672,   861,   241,   857,   274,   865,   533,   534,   229,
     304,   847,   858,   849,   242,   479,   540,   541,   479,    28,
     859,   545,   546,   180,   860,   205,   864,   268,    24,   553,
     554,    24,   868,    35,   874,   873,   303,   191,   503,   224,
     504,   692,   692,   605,   879,   370,   869,   575,   870,   892,
     871,    24,   894,    24,   414,   897,   505,   177,   509,   692,
     387,   388,   179,    16,   179,   506,   243,    75,   178,   442,
     563,   564,   551,   841,    22,   244,   245,   780,   856,    35,
     401,   246,    35,   604,   247,   687,   777,   775,    28,   692,
     692,   692,   855,   440,   713,   289,    25,   310,   840,   311,
     479,   893,    35,     0,    35,   312,   295,   296,     0,   298,
     299,   301,   597,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,    27,     0,   692,    29,     0,     0,     0,
       0,   179,     0,     0,    31,    32,    33,     0,     0,   479,
       0,    36,     0,     0,    37,   692,     0,     0,   668,    38,
      39,   209,   213,   215,   224,     0,    75,    28,   651,   224,
     653,   313,   314,   315,     0,   657,     0,     0,     0,     0,
     660,    35,   662,     0,   676,     0,     0,    88,   666,    89,
      90,    91,    92,    93,    94,   677,   412,     0,     0,     0,
       0,    95,     0,   229,     0,   -98,    96,   356,     0,   441,
       0,   311,     5,     6,     7,     8,    97,   312,     9,    98,
      99,    10,    11,    12,    13,    14,   100,    15,    16,    17,
      18,    19,   101,   102,    20,    21,   103,   104,   105,    22,
     106,   107,    23,   108,     0,     0,     0,    24,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,   114,     0,
     115,    25,   116,   117,   118,     0,   119,   120,     0,     0,
     414,     0,     0,   313,   314,   315,     0,     0,     0,     0,
       0,     0,     0,    75,     0,     0,     0,     0,    27,     0,
      28,    29,   121,    30,     0,     0,   122,     0,    35,    31,
      32,    33,    34,   123,   124,   125,    36,   126,   127,    37,
     128,   129,     0,   130,    38,    39,    88,     0,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,   -98,    96,     0,     0,     0,     0,
       0,     5,     6,     7,     8,    97,   294,     9,    98,    99,
      10,    11,    12,    13,    14,   100,    15,    16,    17,    18,
      19,   101,   102,    20,    21,   103,   104,   105,    22,   106,
     107,    23,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   109,   110,   111,   112,   113,   114,     0,   115,
      25,   116,   117,   118,     0,   119,   120,     0,   259,   110,
     111,   112,   113,   114,     0,   115,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    28,
      29,   121,    30,     0,     0,   122,     0,     0,    31,    32,
      33,    34,   123,   124,   125,    36,   126,   127,    37,   128,
     129,   360,   130,    38,    39,     0,     0,     0,   123,   124,
      88,     0,    89,    90,    91,    92,    93,    94,   130,   191,
     191,     0,   213,   224,    95,     0,     0,     0,   -98,    96,
     557,     0,     0,   224,     0,     5,     6,     7,     8,    97,
       0,     9,    98,    99,    10,    11,    12,    13,    14,   100,
      15,    16,    17,    18,    19,   101,   102,    20,    21,   103,
     104,   105,    22,   106,   107,    23,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   110,   111,   112,
     113,   114,     0,   115,    25,   116,   117,   118,     0,   119,
     120,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,    28,    29,   121,    30,     0,     0,   122,
       0,     0,    31,    32,    33,    34,   123,   124,   125,    36,
     126,   127,    37,   128,   129,     0,   130,    38,    39,    88,
       0,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,   -98,    96,   667,
       0,     0,     0,     0,     5,     6,     7,     8,    97,     0,
       9,    98,    99,    10,    11,    12,    13,    14,   100,    15,
      16,    17,    18,    19,   101,   102,    20,    21,   103,   104,
     105,    22,   106,   107,    23,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   109,   110,   111,   112,   113,
     114,     0,   115,    25,   116,   117,   118,     0,   119,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,    28,    29,   121,    30,     0,     0,   122,     0,
       0,    31,    32,    33,    34,   123,   124,   125,    36,   126,
     127,    37,   128,   129,     0,   130,    38,    39,    88,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,   -98,    96,     0,     0,
       0,     0,     0,     5,     6,     7,     8,    97,     0,     9,
      98,    99,    10,    11,    12,    13,    14,   100,    15,    16,
      17,    18,    19,   101,   102,    20,    21,   103,   104,   105,
      22,   106,   107,    23,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110,   111,   112,   113,   114,
       0,   115,    25,   116,   117,   118,     0,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   821,   822,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,    28,    29,   121,    30,     0,     0,   122,     0,     0,
      31,    32,    33,    34,   123,   124,   125,    36,   126,   127,
      37,   128,   129,     0,   130,    38,    39,   217,     0,     0,
       1,     0,     0,     0,     0,     0,   851,   852,   854,     0,
      16,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,  -259,     6,     7,     8,     0,     0,     9,     0,
       0,    10,  -259,  -259,    13,  -259,     0,    15,    16,    17,
      18,    19,   872,    25,    20,    21,     0,     0,     0,    22,
       0,     0,  -259,     0,     0,     0,     0,   676,     0,     0,
       0,     0,   885,    24,     0,     0,     0,     0,   677,     0,
      27,    25,     0,    29,   218,     0,   229,     0,     0,     0,
       0,    31,    32,    33,     0,     0,     0,     0,    36,     0,
       0,    37,     0,     0,     0,   241,    38,    39,    27,    75,
       0,    29,     0,   219,   217,     0,   242,   351,     0,    31,
      32,    33,     0,     0,    35,     0,    36,     0,   352,    37,
      24,     0,     0,     0,    38,    39,     0,     0,     0,  -259,
       6,     7,     8,     0,     0,     9,     0,     0,    10,  -259,
    -259,    13,  -259,     0,    15,    16,    17,    18,    19,     0,
       0,    20,    21,     0,     0,     0,    22,     0,   243,  -259,
       0,     0,     0,     0,   351,     0,     0,   244,   245,     0,
      24,    35,     0,   246,     0,   352,   247,     0,    25,     0,
       0,   218,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,    27,    75,     0,    29,     0,
     219,   197,     0,   242,     1,     0,    31,    32,    33,     0,
       0,    35,     0,    36,     0,     2,    37,    24,     0,     0,
       0,    38,    39,     0,     0,     0,     5,     6,     7,     8,
       0,     0,     9,     0,     0,    10,    11,    12,    13,    14,
       0,    15,    16,    17,    18,    19,     0,     0,    20,    21,
       0,     0,     0,    22,     0,   243,    23,     0,     0,     0,
     445,     0,     0,     0,   244,   245,     0,    24,    35,     0,
     246,   446,     0,   247,     0,    25,     0,     0,   198,   229,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,    27,     0,     0,    29,     0,   197,     0,   242,
     351,     0,     0,    31,    32,    33,     0,     0,    35,     0,
      36,   352,     0,    37,     0,     0,     0,     0,    38,    39,
       0,     0,     5,     6,     7,     8,     0,     0,     9,     0,
       0,    10,    11,    12,    13,    14,     0,    15,    16,    17,
      18,    19,     0,   671,    20,    21,     0,     0,     0,    22,
       0,   243,    23,     0,   672,     0,     0,     0,     0,     0,
     244,   245,   229,    24,     0,     0,   246,     0,     0,   247,
       0,    25,     0,     0,   198,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   685,     0,     0,    27,     0,
       0,    29,     0,     0,     0,     0,    24,     0,     0,    31,
      32,    33,     0,     0,    35,     0,    36,   179,     0,    37,
    -259,  -259,  -259,  -259,    38,    39,  -259,     0,     0,  -259,
    -259,  -259,  -259,  -259,     0,  -259,  -259,  -259,  -259,  -259,
       0,     0,  -259,  -259,   243,     0,     0,  -259,     0,     0,
    -259,     0,     0,   244,   245,     0,     0,    35,     0,   246,
       0,     0,   247,     0,     0,     0,     0,     0,     0,  -259,
       0,     0,  -259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   686,    -4,    88,     0,    89,    90,
      91,    92,    93,    94,     0,     0,  -259,    75,     0,  -259,
      95,  -259,     0,     0,   -98,    96,     0,  -259,  -259,  -259,
    -259,     0,     0,     0,  -259,    97,     0,  -259,    98,    99,
       0,     0,  -259,  -259,     0,   100,     0,     0,     0,     0,
       0,   101,   102,     0,     0,   103,   104,   105,     0,   106,
     107,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   109,   110,   111,   112,   113,   114,     0,   115,
       0,   116,   117,   365,     0,   119,   120,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,    28,
       0,   121,     0,     0,     0,   122,    89,    90,    91,    92,
      93,    94,   123,   124,   125,     0,   126,   127,    95,   128,
     129,     0,   130,    96,     0,   241,     0,     0,     0,     0,
       6,     7,     8,     0,     0,     9,   242,     0,    10,     0,
       0,    13,     0,     0,    15,    16,    17,    18,    19,     0,
      24,    20,    21,     0,     0,   105,    22,     0,     0,     0,
       0,   179,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   110,   111,   112,   113,   114,   177,   115,    25,   116,
     117,    26,     0,   119,   120,     0,     0,   643,   243,     0,
       0,     0,     0,     0,     0,     0,     0,   244,   245,     0,
       0,    35,     0,   246,     0,    27,   247,    28,    29,   121,
      30,     0,     0,   122,   241,     0,    31,    32,    33,   271,
     123,   124,     0,    36,     0,   242,    37,     0,     0,     0,
     130,    38,    39,    89,    90,    91,    92,    93,    94,    24,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
     179,     0,     0,     0,     0,     0,     0,     6,     7,     8,
       0,     0,     9,     0,     0,    10,     0,     0,    13,     0,
       0,    15,    16,    17,    18,    19,     0,   243,    20,    21,
       0,     0,   105,    22,     0,     0,   244,   245,     0,     0,
      35,     0,   246,     0,     0,   247,     0,   259,   110,   111,
     112,   113,   114,     0,   115,    25,   116,   117,    26,     0,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,    28,    29,   121,    30,     0,     0,
     122,     0,     0,    31,    32,    33,     0,   123,   124,     0,
      36,     0,     0,    37,     0,     0,     0,   130,    38,    39,
      88,     0,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,   -98,    96,
     356,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    98,    99,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,   101,   102,     0,     0,   103,
     104,   105,     0,   106,   107,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   110,   111,   112,
     113,   114,     0,   115,     0,   116,   117,   365,     0,   119,
     120,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   121,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,   125,     0,
     126,   127,     0,   128,   129,    88,   130,    89,    90,    91,
      92,    93,    94,     0,     0,     5,     0,     0,     0,    95,
       0,     0,     0,   -98,    96,    11,    12,     0,    14,     0,
       0,    16,     0,     0,    97,     0,     0,    98,    99,     0,
       0,     0,    22,     0,   100,    23,     0,     0,     0,     0,
     101,   102,     0,     0,   103,   104,   105,     0,   106,   107,
       0,   108,     0,     0,    25,     0,     0,     0,     0,     0,
       0,   109,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,   365,     0,   119,   120,     0,     0,     0,     0,
       0,    27,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,    31,    32,    33,     0,     0,     0,    28,    36,
     121,     0,    37,     0,   122,     0,     0,    38,    39,     0,
       0,   123,   124,   125,     0,   126,   127,     0,   128,   129,
     360,   130,    88,     0,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
     -98,    96,   669,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,   101,   102,     0,
       0,   103,   104,   105,     0,   106,   107,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,   365,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
     125,     0,   126,   127,     0,   128,   129,    88,   130,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,   -98,    96,   762,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     0,     0,    98,
      99,     0,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,   101,   102,     0,     0,   103,   104,   105,     0,
     106,   107,     0,   108,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,   365,     0,   119,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   121,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,   123,   124,   125,     0,   126,   127,     0,
     128,   129,    88,   130,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
     -98,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,    98,    99,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,   101,   102,     0,
       0,   103,   104,   105,     0,   106,   107,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,   365,
       0,   119,   120,   458,     0,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,   -98,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
     125,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     110,   111,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   119,   120,   728,     0,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,   -98,     0,     0,     0,    28,     0,   121,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,     0,     0,   119,   120,   834,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,     0,     0,    95,
     -98,     0,     0,     0,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,     0,     0,   119,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,     0,    28,     0,
     121,     0,     2,     0,   122,     3,     4,     0,     0,     0,
       0,   123,   124,     5,     6,     7,     8,     0,     0,     9,
       0,   130,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,     0,     0,    20,    21,     0,     0,     0,
      22,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,    28,    29,     0,    30,     0,     0,     1,     0,     0,
      31,    32,    33,    34,     0,    35,     0,    36,     2,     0,
      37,     0,     4,     0,     0,    38,    39,     0,     0,     5,
       6,     7,     8,     0,     0,     9,     0,     0,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,     0,
       0,    20,    21,     0,     0,     0,    22,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    26,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,     0,    28,    29,     0,
      30,   676,     0,     0,     0,     0,    31,    32,    33,    34,
       0,    35,   677,    36,     0,     0,    37,     0,     0,     0,
     229,    38,    39,     5,     6,     7,     8,     0,     0,     9,
       0,     0,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,     0,     0,    20,    21,     0,     0,     0,
      22,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,   198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     0,     0,    35,   676,    36,     0,     0,
      37,     0,     0,     0,     0,    38,    39,   677,  -313,     0,
       0,     0,     0,     0,  -313,   229,     0,     0,     0,     6,
       7,     8,     0,     0,     9,     0,     0,    10,     0,     0,
      13,     0,     0,    15,    16,    17,    18,    19,     0,     0,
      20,    21,     0,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
     218,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,    75,   194,    29,     0,   219,
     351,     0,     0,     5,     0,    31,    32,    33,     0,     0,
      35,   352,    36,    11,    12,    37,    14,     0,     0,    16,
      38,    39,     0,     6,     7,     8,     0,     0,     9,     0,
      22,    10,     0,    23,    13,     0,     0,    15,    16,    17,
      18,    19,     0,     0,    20,    21,     0,     0,     0,    22,
       0,     0,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,   218,     0,     0,     0,     0,    27,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    33,     0,     0,     0,     0,    36,    27,     0,
      37,    29,     0,   219,   445,    38,    39,     0,     0,    31,
      32,    33,     0,     0,    35,   446,    36,     0,     0,    37,
       0,     0,     0,   229,    38,    39,     0,     6,     7,     8,
       0,     0,     9,     0,     0,    10,     0,     0,    13,     0,
       0,    15,    16,    17,    18,    19,     0,     0,    20,    21,
       0,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,    25,     0,     0,   218,    95,
       0,   688,     0,     0,   602,   720,     0,   689,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,    29,     0,   219,     0,     0,
       0,     0,     0,    31,    32,    33,   105,     0,     0,     0,
      36,     0,     0,    37,     0,     0,     0,     0,    38,    39,
       0,   690,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,   691,     0,   119,   120,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,     0,
     688,     0,     0,   602,   830,     0,   689,     0,    28,     0,
     121,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     690,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,   691,     0,   119,   120,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,    95,     0,   688,
       0,     0,   602,     0,     0,   689,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   690,
     110,   111,   112,   113,   114,     0,   115,     0,   116,   117,
     691,     0,   119,   120,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,    95,   476,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   121,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,     0,
       0,   119,   120,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
     602,     0,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,   477,     0,     0,     0,     0,     0,   130,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   110,   111,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     119,   120,    89,    90,    91,    92,    93,    94,     0,     0,
       0,   853,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,   420,    28,     0,   121,     0,     0,     0,
     122,     0,     0,     0,     0,     0,     0,   123,   124,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   259,   110,   111,   112,
     113,   114,     0,   115,     0,   116,   117,     0,     0,   119,
     120,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,    96,     0,
       0,     0,     0,    28,     0,   121,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   110,   111,   112,   113,
     114,     0,   115,     0,   116,   117,     0,     0,   119,   120,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   121,     0,     0,     0,   122,     0,
       0,     0,     0,     0,     0,   123,   124,     0,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   110,   111,   112,   113,   114,
       0,   115,     0,   116,   117,     0,     0,   119,   120,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    28,   507,   121,     0,     0,     0,   122,     0,     0,
       0,     0,     0,     0,   123,   124,     0,     0,   488,     0,
       0,     0,     0,     0,   130,     0,     0,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   259,   110,   111,   112,   113,   114,
       0,   115,     0,   116,   117,     0,     0,   119,   120,    89,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
     573,    28,     0,   121,     0,     0,     0,   122,     0,     0,
       0,     0,     0,     0,   123,   124,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   110,   111,   112,   113,   114,     0,
     115,     0,   116,   117,     0,     0,   119,   120,    89,    90,
      91,    92,    93,    94,     0,     0,     0,     0,     0,     0,
      95,   476,     0,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   121,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,     0,     0,
       0,     0,     0,   130,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,   110,   111,   112,   113,   114,     0,   115,
       0,   116,   117,     0,     0,   119,   120,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,   602,     0,     0,     0,     0,    28,
       0,   121,     0,     0,     0,   122,     0,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   259,   110,   111,   112,   113,   114,     0,   115,     0,
     116,   117,     0,     0,   119,   120,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     121,     0,     0,     0,   122,     0,     0,     0,     0,     0,
       0,   123,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     259,   110,   111,   112,   113,   114,     0,   115,     0,   116,
     117,     0,     0,   119,   120,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,   288,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   121,
       0,     0,     0,   122,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     110,   111,   112,   113,   114,     0,   115,     0,   116,   117,
       0,     0,   119,   120,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   121,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,   123,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   259,   110,
     111,   112,   113,   114,     0,   115,     0,   116,   117,     0,
       0,   119,   120,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,     0,     0,   300,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   121,     0,     0,
       0,   122,     0,     0,     0,     0,     0,     0,   123,   124,
       0,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   259,   110,   111,
     112,   113,   114,     0,   115,     0,   116,   117,     0,     0,
     119,   120,    89,    90,    91,    92,    93,    94,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   121,     0,     0,     0,
     122,     0,     0,     0,     0,     0,     0,   123,   124,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   781,   110,   111,   112,
     113,   114,     5,   115,     0,   116,   117,     0,     0,   119,
     120,     0,    11,    12,     0,    14,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,    23,    28,     0,   121,     0,     0,     0,   122,
       0,     0,     0,     0,     0,     0,   123,   124,     0,     0,
       0,    25,     0,     0,     0,     0,   130,     5,     6,     7,
       8,     0,     0,     9,     0,     0,    10,    11,    12,    13,
      14,     0,    15,    16,    17,    18,    19,     0,    27,    20,
      21,    29,     0,     0,    22,     0,     0,    23,     0,    31,
      32,    33,     0,     0,     0,     0,    36,     0,     0,    37,
       0,     5,     6,     7,    38,    39,    25,     9,     0,    26,
       0,    11,    12,    13,    14,     0,     0,    16,    17,    18,
      19,     0,     0,    20,    21,     0,     0,     0,    22,     0,
       0,    23,     0,    27,     0,     0,    29,     0,    30,     0,
       0,     0,     0,     0,    31,    32,    33,    34,     0,     0,
      25,    36,     0,     0,    37,     0,     0,     0,     0,    38,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
      29,     0,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     6,     7,     8,     0,    36,     9,     0,    37,    10,
       0,     0,    13,    38,    39,    15,    16,    17,    18,    19,
       0,     0,    20,    21,     0,     0,     0,    22,     0,     6,
       7,     8,     0,     0,     9,     0,     0,    10,     0,     0,
      13,     0,     0,    15,    16,    17,    18,    19,     0,    25,
      20,    21,    26,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,    25,    28,    29,
      26,    30,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,     0,     0,    36,     0,     0,    37,     0,     0,
       0,     0,    38,    39,    27,     0,     0,    29,     0,    30,
       0,     0,     0,     0,     0,    31,    32,    33,   271,     0,
       0,     0,    36,     0,     0,    37,     0,     6,     7,     8,
      38,    39,     9,     0,     0,    10,     0,     0,    13,     0,
       0,    15,    16,    17,    18,    19,     6,     7,    20,    21,
       0,     9,     0,    22,     0,     0,     0,    13,     0,     0,
       0,    16,    17,    18,    19,     0,     0,    20,    21,     0,
       0,     0,    22,     0,     0,    25,     0,     0,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,    29,     0,   219,     0,     0,
       0,     0,     0,    31,    32,    33,     0,     0,     0,     0,
      36,    27,     0,    37,    29,     0,     0,     0,    38,    39,
       0,     0,    31,    32,    33,     0,     0,     0,     0,    36,
       0,     0,    37,     0,     0,     0,     0,    38,    39
};

static const yytype_int16 yycheck[] =
{
      41,     2,   427,    77,     0,   376,    99,    46,     0,    41,
     100,   229,     0,    52,    47,    48,    49,   126,   602,    18,
      86,     0,   446,     0,   362,   363,   310,    10,   248,    95,
     288,   178,    21,    62,     2,   429,    16,    50,     4,   178,
       1,    15,   300,   740,    57,   699,    16,   103,    42,    15,
     446,    80,   706,   161,   477,    24,     4,    45,    99,    15,
       4,    21,    50,    51,    67,   488,    20,    15,    45,    57,
     654,    15,    68,    67,    51,    78,    68,    68,    74,    23,
      68,     4,    74,    74,    78,    21,    74,   358,   359,    68,
      89,    68,    15,   134,    68,    74,    57,    74,    68,    82,
      19,    67,   799,    22,     0,   375,     2,    21,   273,   274,
      90,    72,    78,    67,   103,   189,    21,   158,   159,    67,
     161,    90,    86,   103,    78,    15,   158,   159,   375,   161,
     171,    95,     0,    23,   208,    47,   229,    67,    50,   152,
      95,   154,    54,   103,    67,    57,   843,    86,    78,    45,
     179,    66,   118,   524,    50,    51,    95,   441,   583,     0,
     585,    57,    16,   151,   152,   153,   154,   103,   160,    16,
     118,    25,    68,    15,   151,    22,   153,   178,    74,   517,
      76,    16,   290,   454,   152,   173,   154,   175,   229,   103,
      25,   158,   159,   154,   284,   118,   173,    16,   103,    16,
     188,   189,    67,   787,   422,    20,    25,   175,     9,    10,
     178,   188,   189,    78,    68,    15,   177,   303,   436,    67,
     208,    67,   288,    23,   253,   291,   255,    68,   294,   376,
      78,   208,    78,    74,   300,   276,   375,   376,    16,   200,
     152,   511,   154,   514,   276,    86,   312,    25,   672,   290,
      67,   359,    16,   677,    95,   363,   269,    16,   290,    16,
     248,   274,    47,    48,    49,    50,    25,   234,    16,    54,
      55,    56,   643,    90,   235,   258,   672,    25,   525,     0,
     346,   677,   178,    84,    85,    22,   103,    24,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   525,   347,    20,   702,   276,
      19,    20,    67,    22,    22,   562,    24,   358,   359,   566,
      68,   362,   363,    78,   288,   354,   358,   359,   598,   422,
     362,   363,     4,   288,   605,    19,   300,    67,   404,   294,
      19,    13,    14,   436,    20,   300,   454,    68,    78,   288,
     379,   352,   248,    74,   266,    86,    19,   269,   616,    22,
       4,   300,   274,   118,    95,    86,   358,   152,   477,   410,
     362,    15,    20,    16,    95,   376,    19,   524,   410,   488,
      19,   422,    91,   469,   352,   524,   525,    86,   118,   455,
     351,    67,    16,    15,   103,   436,    95,   562,    22,   465,
     565,   566,    78,   568,    22,   471,   514,    25,   376,   517,
     119,   452,   453,   454,   507,   635,    25,   446,    16,    67,
     416,   453,   454,    67,    22,   583,    15,   585,   712,   390,
      78,   392,     1,   425,    78,     4,    15,   416,   223,    16,
     404,    25,   403,   410,   405,    22,    15,   288,   409,    25,
     427,   559,   416,    19,    19,   438,    16,    22,    19,   300,
     104,   453,    22,   427,   302,   404,   507,   428,   429,    16,
       3,   187,   310,   514,   118,    22,   517,   416,    12,   375,
     376,    16,   514,   512,   445,   517,   202,    22,   427,    19,
     328,   207,    22,   763,   764,   715,   643,   605,    67,   528,
     529,    11,   531,    16,   643,   613,   222,    15,    16,    22,
     348,   336,   337,   338,   339,    23,   763,   558,   559,   609,
     610,   383,    16,   524,    16,   672,   558,   559,    22,    88,
      22,   564,   618,    22,   396,   397,    16,   525,   577,    15,
      16,   634,    22,   511,   828,    19,    19,    23,    22,   118,
     715,    68,   565,   771,   640,   568,   524,   288,    20,   520,
     668,   522,    20,   404,   605,   578,   558,   288,   815,   300,
      17,    20,   613,   605,   562,   416,    23,   565,   566,   300,
     568,   613,   543,   867,    15,    16,   427,    20,   576,   288,
     578,    15,    23,   634,   562,   815,   816,   565,   566,   576,
     568,   300,   743,   441,   442,   746,   583,   568,   585,    20,
     578,     5,     6,   642,    15,    16,    20,   725,   726,   583,
      20,   585,    23,    20,   732,   699,   587,   668,   524,   525,
      82,    83,    79,    80,    81,    23,   668,   475,   158,   159,
     550,    15,   643,   672,   583,    91,   585,   635,   677,    26,
     815,   816,   616,   565,   740,    22,   568,   519,    16,    36,
      37,    16,    39,    15,   730,    25,   578,   635,    86,    87,
      19,   672,    19,   404,   536,   643,   677,   616,   771,    56,
      21,    22,    19,   404,   725,   726,   548,   795,   717,   650,
      15,   732,    68,   725,   726,   416,   427,    15,   659,    15,
     732,   791,   792,   793,   672,   404,   427,   332,   333,   677,
     671,   699,   119,   799,   800,   676,   704,    16,   706,    15,
      16,   682,   699,     1,   753,    16,     4,    23,   427,   706,
     771,   641,    20,   571,   334,   335,   704,    15,   340,   341,
      20,   702,   583,    16,   585,   232,   656,   643,   658,    19,
      20,    22,    22,    91,   795,   665,   276,   843,    24,    21,
      19,    16,    57,   795,   602,    16,    22,   875,   876,   877,
     878,    16,    15,    67,     4,   616,    16,   863,    91,   564,
     565,    16,   567,    24,    15,    15,    16,   748,    21,    67,
      16,   629,    16,    23,   755,   763,   764,   883,    16,   760,
      78,    16,    15,    19,    16,   891,     1,    45,    22,     4,
      15,   772,    19,    51,   775,   776,   654,    15,    19,    25,
      15,    91,    47,    48,    49,    50,   104,   815,   816,    54,
      55,    56,    57,   103,   875,   876,   877,   878,   358,   749,
     118,    25,   362,   875,   876,   877,   878,   815,   816,   119,
      25,   689,   583,    16,   585,     4,    16,    19,     4,    31,
      90,    19,   583,    16,   585,    22,    15,    19,    19,    15,
      16,    91,    67,   103,   712,     1,    16,    23,     4,    22,
       4,    91,    19,    78,   583,   616,   585,    16,    16,    15,
     410,    15,    19,    42,    16,   616,    19,   384,   385,    23,
     125,   811,    16,   813,    53,   743,   393,   394,   746,   104,
      16,   398,   399,   151,    16,   153,    16,   616,    67,   406,
     407,    67,    19,   118,    16,    24,    15,   152,   342,   154,
     343,   769,   770,   453,    19,   173,   846,   424,   848,    19,
     850,    67,    19,    67,    90,    19,   344,     4,   347,   787,
     188,   189,    78,    42,    78,   345,   105,   103,    15,   261,
     416,   416,   404,   801,    53,   114,   115,   711,   828,   118,
     208,   120,   118,   452,   123,   570,   704,   700,   104,   817,
     818,   819,   820,   258,   595,   105,    75,    15,   800,    17,
     828,   883,   118,    -1,   118,    23,   116,   117,    -1,   119,
     120,   121,   440,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,   102,    -1,   853,   105,    -1,    -1,    -1,
      -1,    78,    -1,    -1,   113,   114,   115,    -1,    -1,   867,
      -1,   120,    -1,    -1,   123,   873,    -1,    -1,   558,   128,
     129,   266,   267,   268,   269,    -1,   103,   104,   535,   274,
     537,    79,    80,    81,    -1,   542,    -1,    -1,    -1,    -1,
     547,   118,   549,    -1,     4,    -1,    -1,     1,   555,     3,
       4,     5,     6,     7,     8,    15,    16,    -1,    -1,    -1,
      -1,    15,    -1,    23,    -1,    19,    20,    21,    -1,    15,
      -1,    17,    26,    27,    28,    29,    30,    23,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    76,    77,    78,    -1,    80,    81,    -1,    -1,
      90,    -1,    -1,    79,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,   107,    -1,    -1,   110,    -1,   118,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    15,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    76,    77,    78,    -1,    80,    81,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,   107,    -1,    -1,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,    -1,   117,   118,
       1,    -1,     3,     4,     5,     6,     7,     8,   127,   564,
     565,    -1,   567,   568,    15,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,   578,    -1,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    -1,    80,
      81,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,   107,    -1,    -1,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    76,    77,    78,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,   107,    -1,    -1,   110,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    19,    20,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    76,    77,    78,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   769,   770,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,   107,    -1,    -1,   110,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,     1,    -1,    -1,
       4,    -1,    -1,    -1,    -1,    -1,   817,   818,   819,    -1,
      42,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,   853,    75,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,    -1,   873,    67,    -1,    -1,    -1,    -1,    15,    -1,
     102,    75,    -1,   105,    78,    -1,    23,    -1,    -1,    -1,
      -1,   113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    42,   128,   129,   102,   103,
      -1,   105,    -1,   107,     1,    -1,    53,     4,    -1,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    15,   123,
      67,    -1,    -1,    -1,   128,   129,    -1,    -1,    -1,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    -1,    53,    -1,   105,    56,
      -1,    -1,    -1,    -1,     4,    -1,    -1,   114,   115,    -1,
      67,   118,    -1,   120,    -1,    15,   123,    -1,    75,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,   102,   103,    -1,   105,    -1,
     107,     1,    -1,    53,     4,    -1,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    15,   123,    67,    -1,    -1,
      -1,   128,   129,    -1,    -1,    -1,    26,    27,    28,    29,
      -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,    39,
      -1,    41,    42,    43,    44,    45,    -1,    -1,    48,    49,
      -1,    -1,    -1,    53,    -1,   105,    56,    -1,    -1,    -1,
       4,    -1,    -1,    -1,   114,   115,    -1,    67,   118,    -1,
     120,    15,    -1,   123,    -1,    75,    -1,    -1,    78,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,     1,    -1,    53,
       4,    -1,    -1,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    15,    -1,   123,    -1,    -1,    -1,    -1,   128,   129,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    -1,     4,    48,    49,    -1,    -1,    -1,    53,
      -1,   105,    56,    -1,    15,    -1,    -1,    -1,    -1,    -1,
     114,   115,    23,    67,    -1,    -1,   120,    -1,    -1,   123,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,     1,    -1,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    67,    -1,    -1,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    78,    -1,   123,
      26,    27,    28,    29,   128,   129,    32,    -1,    -1,    35,
      36,    37,    38,    39,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    48,    49,   105,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,   102,   103,    -1,   105,
      15,   107,    -1,    -1,    19,    20,    -1,   113,   114,   115,
     116,    -1,    -1,    -1,   120,    30,    -1,   123,    33,    34,
      -1,    -1,   128,   129,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    78,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,   104,
      -1,   106,    -1,    -1,    -1,   110,     3,     4,     5,     6,
       7,     8,   117,   118,   119,    -1,   121,   122,    15,   124,
     125,    -1,   127,    20,    -1,    42,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    32,    53,    -1,    35,    -1,
      -1,    38,    -1,    -1,    41,    42,    43,    44,    45,    -1,
      67,    48,    49,    -1,    -1,    52,    53,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,     4,    74,    75,    76,
      77,    78,    -1,    80,    81,    -1,    -1,    15,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,   102,   123,   104,   105,   106,
     107,    -1,    -1,   110,    42,    -1,   113,   114,   115,   116,
     117,   118,    -1,   120,    -1,    53,   123,    -1,    -1,    -1,
     127,   128,   129,     3,     4,     5,     6,     7,     8,    67,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,
      -1,    41,    42,    43,    44,    45,    -1,   105,    48,    49,
      -1,    -1,    52,    53,    -1,    -1,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    76,    77,    78,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,   107,    -1,    -1,
     110,    -1,    -1,   113,   114,   115,    -1,   117,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,   127,   128,   129,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    50,
      51,    52,    -1,    54,    55,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    78,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,    -1,
     121,   122,    -1,   124,   125,     1,   127,     3,     4,     5,
       6,     7,     8,    -1,    -1,    26,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    19,    20,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      -1,    -1,    53,    -1,    40,    56,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    50,    51,    52,    -1,    54,    55,
      -1,    57,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,    -1,    -1,    -1,   104,   120,
     106,    -1,   123,    -1,   110,    -1,    -1,   128,   129,    -1,
      -1,   117,   118,   119,    -1,   121,   122,    -1,   124,   125,
     126,   127,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,    -1,   121,   122,    -1,   124,   125,     1,   127,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    78,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,   119,    -1,   121,   122,    -1,
     124,   125,     1,   127,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    78,
      -1,    80,    81,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    19,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,    -1,   121,   122,    -1,   124,   125,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      -1,    -1,    80,    81,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    -1,    -1,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    -1,    -1,    80,    81,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    15,    -1,   110,    18,    19,    -1,    -1,    -1,
      -1,   117,   118,    26,    27,    28,    29,    -1,    -1,    32,
      -1,   127,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,    -1,   107,    -1,    -1,     4,    -1,    -1,
     113,   114,   115,   116,    -1,   118,    -1,   120,    15,    -1,
     123,    -1,    19,    -1,    -1,   128,   129,    -1,    -1,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,    -1,
     107,     4,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
      -1,   118,    15,   120,    -1,    -1,   123,    -1,    -1,    -1,
      23,   128,   129,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,    -1,    -1,   118,     4,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,   128,   129,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    19,   105,    -1,   107,
       4,    -1,    -1,    26,    -1,   113,   114,   115,    -1,    -1,
     118,    15,   120,    36,    37,   123,    39,    -1,    -1,    42,
     128,   129,    -1,    27,    28,    29,    -1,    -1,    32,    -1,
      53,    35,    -1,    56,    38,    -1,    -1,    41,    42,    43,
      44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,   102,    -1,
     123,   105,    -1,   107,     4,   128,   129,    -1,    -1,   113,
     114,   115,    -1,    -1,   118,    15,   120,    -1,    -1,   123,
      -1,    -1,    -1,    23,   128,   129,    -1,    27,    28,    29,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,
      -1,    41,    42,    43,    44,    45,    -1,    -1,    48,    49,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    75,    -1,    -1,    78,    15,
      -1,    17,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,    52,    -1,    -1,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,   128,   129,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    78,    -1,    80,    81,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      17,    -1,    -1,    20,    21,    -1,    23,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    78,    -1,    80,    81,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,
      -1,    -1,    20,    -1,    -1,    23,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      78,    -1,    80,    81,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    -1,
      -1,    80,    81,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,
      80,    81,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    91,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,   104,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,    80,
      81,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    76,    77,    -1,    -1,    80,    81,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    -1,    -1,    80,    81,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    25,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    76,    77,    -1,    -1,    80,    81,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    76,    77,    -1,    -1,    80,    81,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    76,    77,    -1,    -1,    80,    81,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,   104,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      76,    77,    -1,    -1,    80,    81,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    76,
      77,    -1,    -1,    80,    81,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    -1,    76,    77,
      -1,    -1,    80,    81,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    -1,    76,    77,    -1,
      -1,    80,    81,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    76,    77,    -1,    -1,
      80,    81,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    67,    68,    69,    70,
      71,    72,    26,    74,    -1,    76,    77,    -1,    -1,    80,
      81,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    56,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,   127,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    -1,   102,    48,
      49,   105,    -1,    -1,    53,    -1,    -1,    56,    -1,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,   123,
      -1,    26,    27,    28,   128,   129,    75,    32,    -1,    78,
      -1,    36,    37,    38,    39,    -1,    -1,    42,    43,    44,
      45,    -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,    -1,   102,    -1,    -1,   105,    -1,   107,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,    -1,
      75,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,   128,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,    27,    28,    29,    -1,   120,    32,    -1,   123,    35,
      -1,    -1,    38,   128,   129,    41,    42,    43,    44,    45,
      -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    75,
      48,    49,    78,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    75,   104,   105,
      78,   107,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,   128,   129,   102,    -1,    -1,   105,    -1,   107,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
      -1,    -1,   120,    -1,    -1,   123,    -1,    27,    28,    29,
     128,   129,    32,    -1,    -1,    35,    -1,    -1,    38,    -1,
      -1,    41,    42,    43,    44,    45,    27,    28,    48,    49,
      -1,    32,    -1,    53,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    75,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,   107,    -1,    -1,
      -1,    -1,    -1,   113,   114,   115,    -1,    -1,    -1,    -1,
     120,   102,    -1,   123,   105,    -1,    -1,    -1,   128,   129,
      -1,    -1,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,   128,   129
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     4,    15,    18,    19,    26,    27,    28,    29,    32,
      35,    36,    37,    38,    39,    41,    42,    43,    44,    45,
      48,    49,    53,    56,    67,    75,    78,   102,   104,   105,
     107,   113,   114,   115,   116,   118,   120,   123,   128,   129,
     131,   132,   155,   160,   178,   187,   188,   189,   190,   191,
     192,   195,   196,   197,   198,   199,   200,   201,   203,   215,
     216,   217,   218,   219,   220,   242,   243,   252,   283,   284,
     285,   303,   305,   306,   313,   103,   224,   225,   226,   216,
     218,   219,    20,    67,    78,   222,    15,     0,     1,     3,
       4,     5,     6,     7,     8,    15,    20,    30,    33,    34,
      40,    46,    47,    50,    51,    52,    54,    55,    57,    67,
      68,    69,    70,    71,    72,    74,    76,    77,    78,    80,
      81,   106,   110,   117,   118,   119,   121,   122,   124,   125,
     127,   133,   134,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   152,   154,
     155,   187,   192,   195,   201,   256,   257,   258,   259,   261,
     264,   265,   266,   267,   269,   277,   282,   301,   302,   310,
     312,   313,    19,    22,    19,    22,     1,     4,    15,    78,
     204,   205,   206,   208,   209,   210,   215,   219,   313,   225,
     193,   303,   305,   306,    19,   193,   193,     1,    78,   193,
     203,   215,   219,   306,     1,   204,   215,   219,   225,   303,
     305,   306,    19,   303,   305,   303,   305,     1,    78,   107,
     203,   215,   219,   224,   303,   306,    66,   286,    15,    23,
     214,   241,   289,   299,   300,    20,   222,   284,   224,   284,
      15,    42,    53,   105,   114,   115,   120,   123,   202,   215,
     219,   304,   226,    16,    16,    16,   222,   253,    20,    67,
     134,   138,   152,   194,   195,   196,   198,   199,   200,   201,
      19,   116,   152,   195,   201,   223,   259,    19,   268,   138,
     149,   153,   154,    19,    15,    25,    67,    78,    15,   136,
     271,    15,   270,    25,    15,   136,   136,    25,   136,   136,
      15,   136,    15,    15,   303,   133,   307,   308,    19,    19,
      15,    17,    23,    79,    80,    81,   138,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   151,     4,
      13,    14,     5,     6,    82,    83,     9,    10,    84,    85,
      86,    87,     3,    12,    11,    88,    18,    89,    22,    19,
     219,     4,    15,   215,   218,   215,    21,   260,   264,   265,
     126,   262,   264,   265,   155,    78,   256,    68,   138,     1,
     204,   215,     1,   215,   176,   224,    15,    78,   205,   207,
     209,    15,   214,   161,   156,   290,   299,   204,   204,   215,
     185,   224,   179,   287,   292,   299,   177,   167,   158,   291,
     299,   204,   215,   186,    15,   181,   288,   293,   299,   305,
      20,   263,    16,    67,    90,   221,   224,   238,   239,   240,
      24,   153,    23,   263,   294,   264,    21,   224,   244,    20,
      15,   215,   219,   304,   214,   214,    91,   254,    22,   255,
     253,    15,   151,    16,    16,     4,    15,   211,   212,   213,
     214,   211,    16,   264,   265,    15,    25,    19,     1,   154,
      19,    19,   223,   256,   152,    15,   223,   150,   302,    15,
      17,    23,    79,    80,    81,    15,    16,   121,   135,   150,
     311,    67,    78,   118,   152,    67,    78,   118,   121,   150,
     138,   138,   138,   139,   139,   140,   140,   141,   141,   141,
     141,   142,   142,   143,   144,   145,   146,    25,   152,   147,
     150,   224,   218,   260,   265,   260,   262,   265,   262,   173,
     183,   119,   230,   231,    15,   202,   205,   208,   207,    16,
     214,    16,   230,   263,   263,   295,   164,   157,   224,   224,
     263,   263,   297,   230,   230,   263,   263,   296,   170,   159,
     224,   194,   224,   263,   263,   298,   224,    21,   264,   265,
      16,    22,   187,   188,   190,   192,   195,   199,   201,    16,
      22,    91,    24,    24,   153,   263,   195,   196,   201,   243,
     245,   246,   247,   313,    21,   224,    21,   244,    42,    67,
      78,   227,   228,   232,   153,   222,    21,   255,   224,   212,
     213,   214,    20,   138,   236,   265,   260,   152,    16,    19,
      19,    16,    57,    16,   152,    16,    22,    16,    25,   302,
      67,    78,   152,    67,    78,   135,   309,   308,    16,    22,
      16,    24,   308,   149,    25,   202,   260,   262,   230,   224,
      15,   224,   207,    15,   205,   208,    16,   214,    16,   214,
     162,   263,   230,   263,    91,   233,   180,   263,   224,   168,
     263,   230,   263,   233,    16,   182,   263,    21,   265,    21,
      67,     4,    15,   208,   211,   215,     4,    15,   211,   215,
     208,   211,   215,   211,   215,     1,    90,   240,    17,    23,
      67,    78,   150,   234,   235,   236,    24,   204,   248,   225,
     215,   249,    20,    19,    22,    19,    22,   245,   245,    21,
      16,    22,    15,   254,    21,   202,   211,    16,    16,    16,
      21,   234,   237,   260,    16,    16,   276,   154,     1,   154,
      15,   256,    16,   223,    19,   278,   279,   280,   302,    16,
      25,    24,    16,    15,   150,    19,    15,   149,   174,   184,
     302,   233,    16,   207,   224,   165,   234,   233,   233,   224,
     171,   233,    21,   224,   224,   224,    67,    78,   150,    25,
      25,    25,   250,   251,   248,   250,   244,   249,   248,    16,
     228,    67,   135,   229,   211,   214,    21,    22,    16,   256,
     256,    19,    19,    19,   152,    31,   256,    16,    16,    25,
      22,    15,    19,   278,    19,   309,   309,   224,   233,    16,
      16,   163,   224,   169,   224,   202,   202,    91,    91,    24,
      90,   235,   235,   153,   224,   224,    21,    16,    22,    16,
      21,   234,   154,   154,     1,   154,    16,   256,    19,   278,
     280,   150,    16,    25,    16,    16,   175,   233,   166,   233,
     172,   235,   235,    91,   235,   150,   229,    16,    16,    16,
      16,    19,    16,    25,    16,    19,   278,    22,    19,   233,
     233,   233,   235,    24,    16,   273,   275,   274,   272,    19,
     281,   302,    16,    25,   135,   235,   256,   256,   256,   256,
      16,    22,    19,   281,    19,   302,    16,    19
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
#line 395 "ANSI-C.y"
    {
                       if (ANSIOnly) SyntaxError("ANSI C forbids an empty source file");
                       Program = GrabPragmas(NULL);
                     }
    break;

  case 3:
#line 399 "ANSI-C.y"
    { Program = JoinLists((yyvsp[(1) - (1)].L), GrabPragmas(0)); parseInputType = EntireProgram; }
    break;

  case 4:
#line 400 "ANSI-C.y"
    { StmtListOutput = (yyvsp[(2) - (2)].L); parseInputType = StatementListString; }
    break;

  case 5:
#line 401 "ANSI-C.y"
    {DeclListOutput = (yyvsp[(2) - (2)].L); parseInputType = DeclListString; }
    break;

  case 6:
#line 405 "ANSI-C.y"
    {}
    break;

  case 7:
#line 417 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 10:
#line 420 "ANSI-C.y"
    { (yyvsp[(2) - (3)].n)->coord = (yyvsp[(1) - (3)].tok);   /* We used to set the coord only for comma expressions.  We really need to do it everywhere. */
                                  (yyvsp[(2) - (3)].n)->parenthesized = TRUE;
                                  (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 11:
#line 426 "ANSI-C.y"
    { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
               (yyval.n) = MakeBlockCoord(NULL, NULL, GrabPragmas((yyvsp[(3) - (5)].L)), (yyvsp[(1) - (5)].tok), (yyvsp[(4) - (5)].tok));
	    }
    break;

  case 12:
#line 431 "ANSI-C.y"
    { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
              (yyval.n) = MakeBlockCoord(NULL, (yyvsp[(3) - (6)].L), GrabPragmas((yyvsp[(4) - (6)].L)), (yyvsp[(1) - (6)].tok), (yyvsp[(5) - (6)].tok));
	    }
    break;

  case 14:
#line 436 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 15:
#line 442 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 17:
#line 453 "ANSI-C.y"
    { (yyval.n) = ExtendArray((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 18:
#line 455 "ANSI-C.y"
    { (yyval.n) = MakeCallCoord((yyvsp[(1) - (3)].n), NULL, (yyvsp[(2) - (3)].tok)); }
    break;

  case 19:
#line 457 "ANSI-C.y"
    { (yyval.n) = MakeCallCoord((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok)); }
    break;

  case 20:
#line 459 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 21:
#line 461 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 22:
#line 463 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTINC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 23:
#line 465 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTDEC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 24:
#line 469 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 25:
#line 471 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 26:
#line 474 "ANSI-C.y"
    { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok));
	     }
    break;

  case 27:
#line 480 "ANSI-C.y"
    { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok));
	    }
    break;

  case 28:
#line 489 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 29:
#line 491 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 30:
#line 496 "ANSI-C.y"
    { (yyval.n) = OmitLookups() ? (yyvsp[(1) - (1)].n) : LookupPostfixExpression((yyvsp[(1) - (1)].n));}
    break;

  case 31:
#line 498 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(PREINC, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 32:
#line 500 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(PREDEC, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 33:
#line 502 "ANSI-C.y"
    { (yyvsp[(1) - (2)].n)->u.unary.expr = (yyvsp[(2) - (2)].n);
              (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 34:
#line 505 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 35:
#line 507 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(SIZEOF, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 36:
#line 509 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(SIZEOF, (yyvsp[(3) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 37:
#line 511 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(ALIGNOF, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 38:
#line 513 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(ALIGNOF, (yyvsp[(3) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 39:
#line 515 "ANSI-C.y"
    { (yyval.n) = MakeBuiltinVaArgCoord((yyvsp[(3) - (6)].n), (yyvsp[(5) - (6)].n), (yyvsp[(1) - (6)].tok)); }
    break;

  case 40:
#line 517 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(IMAG, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 41:
#line 519 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(REAL, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tok)); }
    break;

  case 42:
#line 523 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('&', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 43:
#line 524 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('*', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 44:
#line 525 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('+', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 45:
#line 526 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('-', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 46:
#line 527 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('~', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 47:
#line 528 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord('!', NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 49:
#line 533 "ANSI-C.y"
    { if (ANSIOnly)
 	                                                   SyntaxError("constructors not allowed with -ansi switch");
                                                         (yyval.n) = MakeConstructorCoord((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(1) - (4)].tok));
                                                       }
    break;

  case 50:
#line 537 "ANSI-C.y"
    { (yyval.n) = MakeCastCoord((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(1) - (4)].tok)); }
    break;

  case 52:
#line 543 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('*', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 53:
#line 545 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('/', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 54:
#line 547 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('%', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 56:
#line 553 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('+', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 57:
#line 555 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('-', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 59:
#line 561 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LS, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 60:
#line 563 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(RS, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 62:
#line 569 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('<', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 63:
#line 571 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('>', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 64:
#line 573 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 65:
#line 575 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(GE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 67:
#line 581 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(EQ, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 68:
#line 583 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(NE, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 70:
#line 589 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('&', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 72:
#line 595 "ANSI-C.y"
    {
              WarnAboutPrecedence('^', (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence('^', (yyvsp[(3) - (3)].n));
	      (yyval.n) = MakeBinopCoord('^', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 74:
#line 604 "ANSI-C.y"
    { WarnAboutPrecedence('|', (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence('|', (yyvsp[(3) - (3)].n));
              (yyval.n) = MakeBinopCoord('|', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 76:
#line 612 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ANDAND, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 78:
#line 618 "ANSI-C.y"
    { WarnAboutPrecedence(OROR, (yyvsp[(1) - (3)].n));
              WarnAboutPrecedence(OROR, (yyvsp[(3) - (3)].n));
              (yyval.n) = MakeBinopCoord(OROR, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 80:
#line 626 "ANSI-C.y"
    { (yyval.n) = MakeTernaryCoord((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(2) - (5)].tok), (yyvsp[(4) - (5)].tok)); }
    break;

  case 81:
#line 628 "ANSI-C.y"
    {
	      if (ANSIOnly)
	        SyntaxError("ISO C forbids omitting the middle term of a ?: expression");
	      (yyval.n) = MakeTernaryCoord((yyvsp[(1) - (4)].n), NULL, (yyvsp[(4) - (4)].n), (yyvsp[(2) - (4)].tok), (yyvsp[(3) - (4)].tok));
	    }
    break;

  case 83:
#line 638 "ANSI-C.y"
    { (yyvsp[(2) - (3)].n)->u.binop.left = (yyvsp[(1) - (3)].n);
              (yyvsp[(2) - (3)].n)->u.binop.right = (yyvsp[(3) - (3)].n);
              (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 84:
#line 644 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('=', NULL, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 85:
#line 645 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MULTassign, NULL, NULL, (yyvsp[(1) - (1)].tok));  }
    break;

  case 86:
#line 646 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DIVassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 87:
#line 647 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MODassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 88:
#line 648 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(PLUSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));  }
    break;

  case 89:
#line 649 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(MINUSassign, NULL, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 90:
#line 650 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(LSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 91:
#line 651 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(RSassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 92:
#line 652 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ANDassign, NULL, NULL, (yyvsp[(1) - (1)].tok));   }
    break;

  case 93:
#line 653 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ERassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 94:
#line 654 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ORassign, NULL, NULL, (yyvsp[(1) - (1)].tok));    }
    break;

  case 96:
#line 660 "ANSI-C.y"
    {
              if ((yyvsp[(1) - (3)].n)->typ == Comma)
                {
		  AppendItem((yyvsp[(1) - (3)].n)->u.comma.exprs, (yyvsp[(3) - (3)].n));
		  (yyval.n) = (yyvsp[(1) - (3)].n);
		}
              else
                {
		    (yyval.n) = MakeCommaCoord(AppendItem(MakeNewList((yyvsp[(1) - (3)].n)), (yyvsp[(3) - (3)].n)), (yyvsp[(1) - (3)].n)->coord);
		}
	    }
    break;

  case 97:
#line 674 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 98:
#line 678 "ANSI-C.y"
    { (yyval.n) = (Node *) NULL; }
    break;

  case 99:
#line 679 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 100:
#line 722 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (2)].L); }
    break;

  case 101:
#line 724 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (2)].L); }
    break;

  case 102:
#line 726 "ANSI-C.y"
    { (yyval.L) = MakeNewList(ForceNewSU((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok))); }
    break;

  case 103:
#line 728 "ANSI-C.y"
    { (yyval.L) = MakeNewList(ForceNewSU((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok))); }
    break;

  case 104:
#line 734 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
	    }
    break;

  case 105:
#line 739 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n))); }
    break;

  case 106:
#line 741 "ANSI-C.y"
    {
		 SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
		 (yyvsp[(3) - (3)].n) = DefineProc(FALSE, SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl));
	    }
    break;

  case 107:
#line 746 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetProcBody((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n))); }
    break;

  case 108:
#line 748 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
	      (yyvsp[(2) - (2)].n)->pragmas_before = GrabPragmas(0);
	    }
    break;

  case 109:
#line 754 "ANSI-C.y"
    {
		(yyval.L) = AppendItem((yyvsp[(2) - (4)].n)->pragmas_before,
				SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)));
		(yyvsp[(2) - (4)].n)->pragmas_before = 0;
	    }
    break;

  case 110:
#line 760 "ANSI-C.y"
    {
	      SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	      (yyvsp[(3) - (3)].n) = DefineProc(FALSE, SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl));
	      (yyvsp[(3) - (3)].n)->pragmas_before = GrabPragmas(0);
	    }
    break;

  case 111:
#line 766 "ANSI-C.y"
    {
		(yyval.L) = AppendItem((yyvsp[(3) - (5)].n)->pragmas_before,
				SetProcBody((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)));
		(yyvsp[(3) - (5)].n)->pragmas_before = 0;
	    }
    break;

  case 112:
#line 777 "ANSI-C.y"
    {
	      SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
	    }
    break;

  case 113:
#line 780 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 114:
#line 781 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].tq).tq); }
    break;

  case 115:
#line 783 "ANSI-C.y"
    {
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (8)].n), (yyvsp[(8) - (8)].n)));
            }
    break;

  case 116:
#line 789 "ANSI-C.y"
    {
	      SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	      SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
	    }
    break;

  case 117:
#line 793 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 118:
#line 794 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 119:
#line 796 "ANSI-C.y"
    {
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n)));
            }
    break;

  case 120:
#line 801 "ANSI-C.y"
    {
              SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);
            }
    break;

  case 121:
#line 804 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 122:
#line 805 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].tq).tq); }
    break;

  case 123:
#line 807 "ANSI-C.y"
    {
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (8)].n), (yyvsp[(8) - (8)].n)));
	    }
    break;

  case 124:
#line 813 "ANSI-C.y"
    {
		 SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
		 SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
            }
    break;

  case 125:
#line 817 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 126:
#line 818 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 127:
#line 820 "ANSI-C.y"
    {
		 (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n)));
	    }
    break;

  case 128:
#line 824 "ANSI-C.y"
    {
	      (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), Redecl);
	    }
    break;

  case 129:
#line 827 "ANSI-C.y"
    { SetDeclAsm((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 130:
#line 828 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].tq).tq); }
    break;

  case 131:
#line 830 "ANSI-C.y"
    {
              SetDeclInit((yyvsp[(3) - (9)].n), (yyvsp[(9) - (9)].n));
	      if (yydebug) {
		  //printf("%s:%d ", __FILE__, __LINE__); DPL($7);
		  printf("%s:%d ", __FILE__, __LINE__); DPN((yyvsp[(3) - (9)].n));
	      }
            }
    break;

  case 132:
#line 841 "ANSI-C.y"
    {
              SyntaxError("declaration without a variable");
            }
    break;

  case 133:
#line 847 "ANSI-C.y"
    {
              (yyval.L) = NULL; /* empty list */
            }
    break;

  case 134:
#line 852 "ANSI-C.y"
    {
              SyntaxError("declaration without a variable");
            }
    break;

  case 135:
#line 858 "ANSI-C.y"
    {
              (yyval.L) = NULL; /* empty list */
            }
    break;

  case 137:
#line 869 "ANSI-C.y"
    {
              SetDeclType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord), NoRedecl);
            }
    break;

  case 138:
#line 872 "ANSI-C.y"
    {
              SetDeclAttribs((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].tq).tq);
          }
    break;

  case 139:
#line 876 "ANSI-C.y"
    {
		(yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].n)));
            }
    break;

  case 140:
#line 880 "ANSI-C.y"
    {
              SetDeclType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord), NoRedecl);
            }
    break;

  case 141:
#line 883 "ANSI-C.y"
    {
	    SetDeclAttribs((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].tq).tq);
	  }
    break;

  case 142:
#line 887 "ANSI-C.y"
    {
              (yyval.L) = MakeNewList(SetDeclInit((yyvsp[(2) - (6)].n), (yyvsp[(6) - (6)].n)));
	    }
    break;

  case 143:
#line 891 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), NoRedecl); }
    break;

  case 144:
#line 892 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].tq).tq); }
    break;

  case 145:
#line 894 "ANSI-C.y"
    { SetDeclInit((yyvsp[(3) - (7)].n), (yyvsp[(7) - (7)].n)); }
    break;

  case 146:
#line 899 "ANSI-C.y"
    {
              SyntaxError("declaration without a variable");
	    }
    break;

  case 147:
#line 904 "ANSI-C.y"
    {
              (yyval.L) = NULL; /* empty list */
	    }
    break;

  case 148:
#line 909 "ANSI-C.y"
    {
              SyntaxError("declaration without a variable");
	    }
    break;

  case 149:
#line 914 "ANSI-C.y"
    {
              (yyval.L) = NULL; /* empty list */
            }
    break;

  case 153:
#line 928 "ANSI-C.y"
    { (yyval.n) = FinishPrimType((yyvsp[(1) - (1)].n)); }
    break;

  case 155:
#line 930 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 156:
#line 942 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 157:
#line 944 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord); }
    break;

  case 158:
#line 946 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 159:
#line 948 "ANSI-C.y"
    { (yyval.n) = MergePrimTypes((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 160:
#line 955 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 161:
#line 957 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (3)].n), tq_union((yyvsp[(1) - (3)].tq).tq, (yyvsp[(3) - (3)].tq).tq), & (yyvsp[(1) - (3)].tq).coord); }
    break;

  case 162:
#line 959 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 163:
#line 966 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 164:
#line 968 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, GetTypedefType((yyvsp[(2) - (2)].n)));
              (yyval.n)->coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 165:
#line 971 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 166:
#line 978 "ANSI-C.y"
    {
		 (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
		 (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord;
	    }
    break;

  case 167:
#line 983 "ANSI-C.y"
    {
		 (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (4)].tq).tq, (yyvsp[(2) - (4)].tq).tq, (yyvsp[(2) - (4)].tq).coord);
		 (yyval.tq).tq = MergeTypeQuals((yyval.tq).tq, (yyvsp[(3) - (4)].tq).tq, (yyvsp[(3) - (4)].tq).coord);
		 (yyval.tq).coord = (yyvsp[(1) - (4)].tq).coord;
	    }
    break;

  case 168:
#line 989 "ANSI-C.y"
    {
              (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord;
	    }
    break;

  case 175:
#line 1019 "ANSI-C.y"
    { (yyval.n) = FinishPrimType((yyvsp[(1) - (1)].n)); }
    break;

  case 178:
#line 1031 "ANSI-C.y"
    {
		(yyval.n) = NodeCopy(NodeDataType(SemCheckNode((yyvsp[(3) - (4)].n))), Subtree); /* copy the node so we can remove type qualifiers without hurting the code */
		SetCoords((yyval.n), (yyvsp[(1) - (4)].tok), Subtree);
		NodeUpdateTq((yyval.n), tq_remove_everything); /* Take off all the type qualifiers */
	    }
    break;

  case 179:
#line 1042 "ANSI-C.y"
    {
		/* we have to copy the node because we don't want the type qualifiers to modify the original node. */
		Node *thetype=SetCoords(NodeCopy(NodeDataType(SemCheckNode((yyvsp[(4) - (5)].n))), Subtree), (yyvsp[(2) - (5)].tok), Subtree);
		NodeUpdateTq(thetype, tq_remove_everything); /* Take off all the type qualifiers */
		(yyval.n) = TypeQualifyNode(thetype, (yyvsp[(1) - (5)].tq).tq, & (yyvsp[(1) - (5)].tq).coord); /* Add on the new type qualifiers */
	    }
    break;

  case 181:
#line 1054 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord);  }
    break;

  case 182:
#line 1056 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 183:
#line 1058 "ANSI-C.y"
    { (yyval.n) = MergePrimTypes((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 185:
#line 1065 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, & (yyvsp[(1) - (2)].tq).coord); }
    break;

  case 186:
#line 1067 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 187:
#line 1074 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(1) - (1)].n), EMPTY_TQ, GetTypedefType((yyvsp[(1) - (1)].n))); }
    break;

  case 188:
#line 1076 "ANSI-C.y"
    { (yyval.n) = ConvertIdToTdef((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].tq).tq, GetTypedefType((yyvsp[(2) - (2)].n))); (yyval.n)->coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 189:
#line 1078 "ANSI-C.y"
    { (yyval.n) = TypeQualifyNode((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tq).tq, & (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 190:
#line 1084 "ANSI-C.y"
    { /*$$.tq = tq_union($1.tq, $2.tq);*/
	      (yyval.tq).coord = (yyvsp[(1) - (1)].tq).coord; }
    break;

  case 191:
#line 1087 "ANSI-C.y"
    { (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 193:
#line 1094 "ANSI-C.y"
    { (yyval.tq).tq = MergeTypeQuals((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord);
              (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord; }
    break;

  case 199:
#line 1110 "ANSI-C.y"
    {
	      Warning(2, "function prototype parameters must have types");
              (yyval.n) = AddDefaultParameterTypes((yyvsp[(1) - (1)].n));
            }
    break;

  case 200:
#line 1114 "ANSI-C.y"
    {  (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 202:
#line 1122 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), Redecl);
               }
    break;

  case 203:
#line 1125 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (5)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (5)].tok)), Redecl);
               }
    break;

  case 204:
#line 1128 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(5) - (6)].n), MakePtrCoord(   (yyvsp[(3) - (6)].tq).tq,    NULL, (yyvsp[(1) - (6)].tok)), Redecl);
               }
    break;

  case 205:
#line 1131 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord(   (yyvsp[(3) - (4)].tq).tq,    NULL, (yyvsp[(1) - (4)].tok)), Redecl);
               }
    break;

  case 206:
#line 1139 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
              }
    break;

  case 207:
#line 1142 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(3) - (4)].n));
               }
    break;

  case 208:
#line 1145 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
               }
    break;

  case 209:
#line 1152 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 210:
#line 1154 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 211:
#line 1161 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 212:
#line 1163 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (2)].n), EMPTY_TQ, (yyvsp[(2) - (2)].n), NULL, NULL);   }
    break;

  case 215:
#line 1175 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), Redecl);
               }
    break;

  case 216:
#line 1178 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok)), Redecl);
               }
    break;

  case 217:
#line 1185 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 218:
#line 1188 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
               }
    break;

  case 222:
#line 1202 "ANSI-C.y"
    { (yyval.n) = MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 223:
#line 1204 "ANSI-C.y"
    { (yyval.n) = MakePtrCoord((yyvsp[(3) - (3)].tq).tq, NULL, (yyvsp[(1) - (3)].tok)); }
    break;

  case 224:
#line 1206 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)));
               }
    break;

  case 225:
#line 1209 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok)));
               }
    break;

  case 226:
#line 1216 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 227:
#line 1219 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 228:
#line 1222 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 229:
#line 1225 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
               }
    break;

  case 230:
#line 1231 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n);                   }
    break;

  case 231:
#line 1232 "ANSI-C.y"
    { (yyval.n) = MakeFdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 232:
#line 1234 "ANSI-C.y"
    { (yyval.n) = DemoteProcArgs(MakeFdclCoord(EMPTY_TQ, (yyvsp[(2) - (3)].L), NULL, (yyvsp[(1) - (3)].tok))); }
    break;

  case 236:
#line 1247 "ANSI-C.y"
    { //printf("%s:%d ", __FILE__, __LINE__); DPL($2);
              SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
              (yyval.n) = ModifyDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)));
              }
    break;

  case 237:
#line 1252 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(4) - (4)].n), MakePtrCoord(tq_union((yyvsp[(2) - (4)].tq).tq, (yyvsp[(3) - (4)].tq).tq),    NULL, (yyvsp[(1) - (4)].tok)));
               }
    break;

  case 238:
#line 1259 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 239:
#line 1261 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 240:
#line 1264 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
               }
    break;

  case 241:
#line 1271 "ANSI-C.y"
    { (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL); }
    break;

  case 242:
#line 1273 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 243:
#line 1276 "ANSI-C.y"
    { if (!AcceptWildcards())
	      {
	        SyntaxError("Wildcard tokens not allowed in program. \n");
              }
	      (yyval.n) = ConvertIdToDecl((yyvsp[(1) - (1)].n), EMPTY_TQ, NULL, NULL, NULL);
	    }
    break;

  case 244:
#line 1287 "ANSI-C.y"
    {
              (yyval.n) = (yyvsp[(1) - (1)].n);
            }
    break;

  case 245:
#line 1291 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakePtrCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (3)].tok)), SU);
               }
    break;

  case 246:
#line 1294 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(4) - (4)].n), MakePtrCoord((yyvsp[(3) - (4)].tq).tq, NULL, (yyvsp[(1) - (4)].tok)), SU);
               }
    break;

  case 247:
#line 1301 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(1) - (4)].n), MakeFdclCoord(EMPTY_TQ, (yyvsp[(3) - (4)].L), NULL, (yyvsp[(2) - (4)].tok))); }
    break;

  case 248:
#line 1303 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);
               }
    break;

  case 249:
#line 1306 "ANSI-C.y"
    { (yyval.n) = ModifyDeclType((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
               }
    break;

  case 250:
#line 1321 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 251:
#line 1323 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 254:
#line 1335 "ANSI-C.y"
    { (yyval.n) = FinishType((yyvsp[(1) - (1)].n)); }
    break;

  case 255:
#line 1337 "ANSI-C.y"
    { (yyval.n) = FinishType(SetBaseType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n))); }
    break;

  case 256:
#line 1339 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(1) - (1)].tq).tq, (yyvsp[(1) - (1)].tq).coord); }
    break;

  case 257:
#line 1341 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(2) - (2)].n), MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord)); }
    break;

  case 258:
#line 1342 "ANSI-C.y"
    { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               (yyval.n) = (yyvsp[(1) - (1)].n);
	             }
    break;

  case 259:
#line 1354 "ANSI-C.y"
    { (yyval.tq).tq = EMPTY_TQ; (yyval.tq).coord = UnknownCoord; }
    break;

  case 260:
#line 1356 "ANSI-C.y"
    { (yyval.tq) = (yyvsp[(1) - (1)].tq); }
    break;

  case 261:
#line 1361 "ANSI-C.y"
    { (yyval.tq) = (yyvsp[(1) - (1)].tq); }
    break;

  case 262:
#line 1363 "ANSI-C.y"
    {
		  (yyval.tq).tq = tq_union ((yyvsp[(1) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).tq);
		  (yyval.tq).coord = (yyvsp[(1) - (2)].tq).coord;
	      }
    break;

  case 263:
#line 1371 "ANSI-C.y"
    { if (ANSIOnly)
	            SyntaxError("__attribute__ not allowed with -ansi switch");
                  (yyval.tq).tq = tq_attribute((yyvsp[(4) - (6)].L)); (yyval.tq).coord=UnknownCoord;
		}
    break;

  case 264:
#line 1380 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 265:
#line 1382 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 266:
#line 1387 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 267:
#line 1389 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (1)].n), NULL); }
    break;

  case 268:
#line 1391 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (4)].n), MakeNewList((yyvsp[(3) - (4)].n))); }
    break;

  case 269:
#line 1393 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (6)].n), ConsItem((yyvsp[(3) - (6)].n), (yyvsp[(5) - (6)].L))); }
    break;

  case 270:
#line 1404 "ANSI-C.y"
    { (yyval.n) = ConvertIdToAttrib((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].L)); }
    break;

  case 271:
#line 1409 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;

  case 272:
#line 1415 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 274:
#line 1420 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (4)].n), NULL, NULL, NULL, (yyvsp[(1) - (4)].tok)); }
    break;

  case 277:
#line 1426 "ANSI-C.y"
    { (yyval.n) = MakeIdCoord(UniqueString("const"), (yyvsp[(1) - (1)].tok)); }
    break;

  case 278:
#line 1431 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 279:
#line 1432 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 281:
#line 1438 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(':', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 282:
#line 1439 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(':', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 283:
#line 1440 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DESIGNATED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 284:
#line 1441 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(DESIGNATED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 285:
#line 1442 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n), (yyvsp[(3) - (4)].tok)); }
    break;

  case 286:
#line 1443 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, (yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(3) - (5)].tok)); }
    break;

  case 287:
#line 1444 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARRAY_LABELED_INITIALIZER,
														   MakeBinopCoord(ELLIPSIS, (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].n), (yyvsp[(3) - (6)].tok)),
														   (yyvsp[(6) - (6)].n), (yyvsp[(5) - (6)].tok)); }
    break;

  case 289:
#line 1451 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 290:
#line 1455 "ANSI-C.y"
    { if (ANSIOnly) SyntaxError("ISO C forbits empty initializer braces"); /* See Bug 83 */
	                                   (yyval.n) = MakeInitializerCoord(NULL, (yyvsp[(1) - (2)].tok));
                                         }
    break;

  case 291:
#line 1458 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); (yyval.n)->coord = (yyvsp[(1) - (3)].tok); }
    break;

  case 292:
#line 1459 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (4)].n); (yyval.n)->coord = (yyvsp[(1) - (4)].tok); }
    break;

  case 293:
#line 1464 "ANSI-C.y"
    { (yyval.n) = MakeInitializerCoord(MakeNewList((yyvsp[(1) - (1)].n)), (yyvsp[(1) - (1)].n)->coord); }
    break;

  case 294:
#line 1466 "ANSI-C.y"
    {
              assert((yyvsp[(1) - (3)].n)->typ == Initializer);
              AppendItem((yyvsp[(1) - (3)].n)->u.initializer.exprs, (yyvsp[(3) - (3)].n));
              (yyval.n) = (yyvsp[(1) - (3)].n);
            }
    break;

  case 296:
#line 1476 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), EllipsisNode); }
    break;

  case 297:
#line 1480 "ANSI-C.y"
    { Node *n = MakePrimCoord(EMPTY_TQ, Void, (yyvsp[(1) - (1)].tok));
	      SyntaxErrorCoord(n->coord, "First argument cannot be `...'");
              (yyval.L) = MakeNewList(n);
            }
    break;

  case 298:
#line 1489 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 299:
#line 1491 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 300:
#line 1495 "ANSI-C.y"
    {
	      SyntaxErrorCoord((yyvsp[(1) - (3)].n)->coord, "formals cannot have initializers");
              (yyval.L) = MakeNewList((yyvsp[(1) - (3)].n));
            }
    break;

  case 301:
#line 1500 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (3)].L); }
    break;

  case 302:
#line 1506 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 303:
#line 1508 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n));
            }
    break;

  case 304:
#line 1511 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal);
            }
    break;

  case 305:
#line 1514 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal);
            }
    break;

  case 306:
#line 1517 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 307:
#line 1519 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord)); }
    break;

  case 308:
#line 1521 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord), Formal); }
    break;

  case 309:
#line 1523 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 310:
#line 1525 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n));
            }
    break;

  case 311:
#line 1528 "ANSI-C.y"
    {
	      (yyval.n) = SetDeclType((yyvsp[(3) - (4)].n), TypeQualifyNode((yyvsp[(2) - (4)].n), tq_union((yyvsp[(1) - (4)].tq).tq, (yyvsp[(4) - (4)].tq).tq), NULL), Formal);
	    }
    break;

  case 312:
#line 1532 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n), Formal);
            }
    break;

  case 313:
#line 1535 "ANSI-C.y"
    { (yyval.n) = MakeDefaultPrimType((yyvsp[(2) - (2)].tq).tq, (yyvsp[(2) - (2)].tq).coord); }
    break;

  case 314:
#line 1537 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(3) - (3)].n), MakeDefaultPrimType((yyvsp[(2) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).coord)); }
    break;

  case 315:
#line 1539 "ANSI-C.y"
    { (yyval.n) = SetDeclType((yyvsp[(3) - (3)].n), MakeDefaultPrimType(tq_union((yyvsp[(1) - (3)].tq).tq, (yyvsp[(2) - (3)].tq).tq), (yyvsp[(2) - (3)].tq).coord), Formal); }
    break;

  case 316:
#line 1545 "ANSI-C.y"
    { (yyval.n) = MakeAdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(1) - (2)].tok)); }
    break;

  case 317:
#line 1547 "ANSI-C.y"
    { (yyval.n) = MakeAdclCoord(EMPTY_TQ, NULL, (yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 318:
#line 1549 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(1) - (4)].n), MakeAdclCoord(EMPTY_TQ, NULL, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok))); }
    break;

  case 319:
#line 1551 "ANSI-C.y"
    { (yyval.n) = SetBaseType((yyvsp[(1) - (3)].n), MakeAdclCoord(EMPTY_TQ, NULL, NULL, (yyvsp[(2) - (3)].tok))); }
    break;

  case 320:
#line 1573 "ANSI-C.y"
    {
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (4)].n), NULL, (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL);
            }
    break;

  case 321:
#line 1578 "ANSI-C.y"
    {
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (5)].n), (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].L), (yyvsp[(3) - (5)].tok), (yyvsp[(5) - (5)].tok), NULL);
	    }
    break;

  case 322:
#line 1582 "ANSI-C.y"
    {
              (yyval.n) = SetSUdclName((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n)->coord);
	    }
    break;

  case 323:
#line 1587 "ANSI-C.y"
    {
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (3)].n), NULL, NULL, (yyvsp[(2) - (3)].tok), (yyvsp[(3) - (3)].tok), NULL);
	    }
    break;

  case 324:
#line 1593 "ANSI-C.y"
    {
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              (yyval.n) = SetSUdclNameFields((yyvsp[(1) - (4)].n), (yyvsp[(2) - (4)].n), NULL, (yyvsp[(3) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL);
	    }
    break;

  case 325:
#line 1602 "ANSI-C.y"
    { (yyval.n) = MakeSdclCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 326:
#line 1603 "ANSI-C.y"
    { (yyval.n) = MakeUdclCoord(EMPTY_TQ, NULL, (yyvsp[(1) - (1)].tok)); }
    break;

  case 327:
#line 1609 "ANSI-C.y"
    {
              (yyval.L) = (yyvsp[(2) - (2)].L);
              NodeUpdateTq2(FirstItem((yyval.L)), tq_union, (yyvsp[(1) - (2)].tq).tq);
            }
    break;

  case 328:
#line 1614 "ANSI-C.y"
    {
	      NodeUpdateTq2(FirstItem((yyvsp[(3) - (3)].L)), tq_union, (yyvsp[(2) - (3)].tq).tq);
              (yyval.L) = JoinLists((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].L));
	    }
    break;

  case 331:
#line 1624 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(2) - (2)].L); NodeUpdateTq((Node*)(FirstItem((yyval.L))), tq_add_extension); ((Node*)(FirstItem((yyval.L))))->coord = (yyvsp[(1) - (2)].tok); }
    break;

  case 332:
#line 1631 "ANSI-C.y"
    {
	      (yyval.L) = MakeNewList(SetDeclType((yyvsp[(2) - (2)].n),
					    MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
					    SU));
	    }
    break;

  case 333:
#line 1637 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), SU); }
    break;

  case 334:
#line 1643 "ANSI-C.y"
    { (yyval.L) = MakeNewList(SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), SU)); }
    break;

  case 335:
#line 1645 "ANSI-C.y"
    {
	       SetDeclAttribs((yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tq).tq);
	       (yyval.L) = MakeNewList(SetDeclType((yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), SU));
	  }
    break;

  case 336:
#line 1650 "ANSI-C.y"
    {
		 /* gcc extension: unnamed struct/union fields */
		 Node *a = SetSUdclFields((yyvsp[(1) - (4)].n), NULL, (yyvsp[(3) - (4)].L), (yyvsp[(2) - (4)].tok), (yyvsp[(4) - (4)].tok), NULL);
		 Node *d = MakeDeclCoord(NULL, EMPTY_TQ, a, NULL, NULL, a->coord);
		 NodeSetDeclLocation(d, TQ_SU_DECL);
		 (yyval.L) = MakeNewList(d);
	    }
    break;

  case 337:
#line 1658 "ANSI-C.y"
    { (yyval.L) = AppendDecl((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n), SU); }
    break;

  case 338:
#line 1665 "ANSI-C.y"
    { SetDeclAttribs((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].tq).tq);
              (yyval.n) = SetDeclBitSize((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n)); }
    break;

  case 339:
#line 1672 "ANSI-C.y"
    { (yyval.n) = SetDeclBitSize((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n));
              SetDeclAttribs((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].tq).tq); }
    break;

  case 340:
#line 1678 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 342:
#line 1684 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 343:
#line 1690 "ANSI-C.y"
    { (yyval.n) = BuildEnum(NULL, (yyvsp[(3) - (5)].L), (yyvsp[(1) - (5)].tok), (yyvsp[(2) - (5)].tok), (yyvsp[(5) - (5)].tok)); }
    break;

  case 344:
#line 1692 "ANSI-C.y"
    { (yyval.n) = BuildEnum((yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].L), (yyvsp[(1) - (6)].tok), (yyvsp[(3) - (6)].tok), (yyvsp[(6) - (6)].tok));   }
    break;

  case 345:
#line 1694 "ANSI-C.y"
    { (yyval.n) = BuildEnum((yyvsp[(2) - (2)].n), NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].n)->coord, (yyvsp[(2) - (2)].n)->coord); }
    break;

  case 346:
#line 1700 "ANSI-C.y"
    { (yyval.L) = MakeNewList(BuildEnumConst((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n))); }
    break;

  case 347:
#line 1702 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (4)].L), BuildEnumConst((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n))); }
    break;

  case 348:
#line 1707 "ANSI-C.y"
    { (yyval.n) = NULL; }
    break;

  case 349:
#line 1708 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n);   }
    break;

  case 350:
#line 1712 "ANSI-C.y"
    { }
    break;

  case 351:
#line 1713 "ANSI-C.y"
    { }
    break;

  case 360:
#line 1733 "ANSI-C.y"
    {  (yyval.n) = NULL; }
    break;

  case 361:
#line 1738 "ANSI-C.y"
    { (yyval.n) = BuildLabel((yyvsp[(1) - (2)].n)); }
    break;

  case 362:
#line 1740 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeCaseCoord((yyvsp[(2) - (3)].n), NULL, (yyvsp[(1) - (3)].tok))); }
    break;

  case 363:
#line 1742 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeDefaultCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 364:
#line 1746 "ANSI-C.y"
    { (yyval.n) = BuildLabel((yyvsp[(1) - (2)].n)); }
    break;

  case 365:
#line 1751 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 366:
#line 1753 "ANSI-C.y"
    {
		(yyval.n) = MakeBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 367:
#line 1757 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 368:
#line 1762 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 369:
#line 1768 "ANSI-C.y"
    { (yyval.n) = MakeXBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 370:
#line 1770 "ANSI-C.y"
    {
		(yyval.n) = MakeXBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 371:
#line 1774 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeXBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 372:
#line 1779 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeXBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 373:
#line 1785 "ANSI-C.y"
    { EnterScope(); }
    break;

  case 374:
#line 1786 "ANSI-C.y"
    { ExitScope(); }
    break;

  case 375:
#line 1789 "ANSI-C.y"
    { EnterScope(); }
    break;

  case 376:
#line 1790 "ANSI-C.y"
    { ExitScope();  }
    break;

  case 377:
#line 1797 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, NULL, NULL, (yyvsp[(1) - (2)].tok), (yyvsp[(2) - (2)].tok)); }
    break;

  case 378:
#line 1799 "ANSI-C.y"
    { (yyval.n) = MakeBlockCoord(PrimVoid, GrabPragmas((yyvsp[(2) - (3)].L)), NULL, (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok)); }
    break;

  case 379:
#line 1801 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(2) - (3)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, NULL, GrabPragmas((yyvsp[(2) - (3)].L)), (yyvsp[(1) - (3)].tok), (yyvsp[(3) - (3)].tok));
	    }
    break;

  case 380:
#line 1806 "ANSI-C.y"
    {
		MaybeCheckLabelAtEndOfCompoundStatement((yyvsp[(3) - (4)].L));
		(yyval.n) = MakeBlockCoord(PrimVoid, (yyvsp[(2) - (4)].L), GrabPragmas((yyvsp[(3) - (4)].L)), (yyvsp[(1) - (4)].tok), (yyvsp[(4) - (4)].tok));
	    }
    break;

  case 381:
#line 1815 "ANSI-C.y"
    { (yyval.L) = GrabPragmas((yyvsp[(1) - (1)].L)); }
    break;

  case 382:
#line 1816 "ANSI-C.y"
    { (yyval.L) = JoinLists(GrabPragmas((yyvsp[(1) - (2)].L)),
                                                         (yyvsp[(2) - (2)].L)); }
    break;

  case 383:
#line 1821 "ANSI-C.y"
    { (yyval.L) = GrabPragmas(MakeNewList((yyvsp[(1) - (1)].n))); }
    break;

  case 384:
#line 1822 "ANSI-C.y"
    { (yyval.L) = AppendItem(GrabPragmas((yyvsp[(1) - (2)].L)),
                                                        (yyvsp[(2) - (2)].n)); }
    break;

  case 385:
#line 1827 "ANSI-C.y"
    {
            (yyval.n)=(yyvsp[(1) - (2)].n);
	    if ((yyval.n) && (yyval.n)->typ==Block) (yyval.n)->parenthesized=TRUE; /* Expression statements get their parenthesized bits set, so we can remember that it is an expression statement, rather than just a compound statement */
          }
    break;

  case 386:
#line 1835 "ANSI-C.y"
    { (yyval.n) = MakeIfCoord((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n), (yyvsp[(1) - (5)].tok)); }
    break;

  case 387:
#line 1837 "ANSI-C.y"
    { (yyval.n) = MakeIfElseCoord((yyvsp[(3) - (7)].n), (yyvsp[(5) - (7)].n), (yyvsp[(7) - (7)].n), (yyvsp[(1) - (7)].tok), (yyvsp[(6) - (7)].tok)); }
    break;

  case 388:
#line 1838 "ANSI-C.y"
    { PushContainer(Switch); }
    break;

  case 389:
#line 1839 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeSwitchCoord((yyvsp[(4) - (6)].n), (yyvsp[(6) - (6)].n), NULL, (yyvsp[(1) - (6)].tok))); }
    break;

  case 390:
#line 1844 "ANSI-C.y"
    { PushContainer(While);}
    break;

  case 391:
#line 1846 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeWhileCoord((yyvsp[(4) - (6)].n), (yyvsp[(6) - (6)].n), (yyvsp[(1) - (6)].tok))); }
    break;

  case 392:
#line 1848 "ANSI-C.y"
    { PushContainer(Do);}
    break;

  case 393:
#line 1850 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeDoCoord((yyvsp[(3) - (8)].n), (yyvsp[(6) - (8)].n), (yyvsp[(1) - (8)].tok), (yyvsp[(4) - (8)].tok))); }
    break;

  case 394:
#line 1852 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 395:
#line 1854 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), (yyvsp[(5) - (10)].n), (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 396:
#line 1858 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 397:
#line 1860 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord(NULL, (yyvsp[(5) - (10)].n), (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 398:
#line 1862 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 399:
#line 1864 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), (yyvsp[(5) - (10)].n), NULL, (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 400:
#line 1866 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 401:
#line 1868 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord((yyvsp[(3) - (10)].n), NULL, (yyvsp[(7) - (10)].n), (yyvsp[(10) - (10)].n), (yyvsp[(1) - (10)].tok))); }
    break;

  case 402:
#line 1869 "ANSI-C.y"
    { PushContainer(For);}
    break;

  case 403:
#line 1870 "ANSI-C.y"
    { (yyval.n) = PopContainer(MakeForCoord(NULL, SintZero, NULL, (yyvsp[(6) - (6)].n), (yyvsp[(1) - (6)].tok))); }
    break;

  case 404:
#line 1875 "ANSI-C.y"
    { (yyval.n) = ResolveGoto((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 405:
#line 1877 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeContinueCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 406:
#line 1879 "ANSI-C.y"
    { (yyval.n) = AddContainee(MakeBreakCoord(NULL, (yyvsp[(1) - (2)].tok))); }
    break;

  case 407:
#line 1881 "ANSI-C.y"
    { (yyval.n) = AddReturn(MakeReturnCoord((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok))); }
    break;

  case 408:
#line 1885 "ANSI-C.y"
    { (yyval.n) = ResolveGoto((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].tok)); }
    break;

  case 409:
#line 1890 "ANSI-C.y"
    { (yyval.L) = NULL; }
    break;

  case 411:
#line 1895 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 412:
#line 1897 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 413:
#line 1901 "ANSI-C.y"
    { (yyval.n) = MakeAsmArgCoord((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 414:
#line 1905 "ANSI-C.y"
    { (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n)); }
    break;

  case 415:
#line 1907 "ANSI-C.y"
    { (yyval.L) = AppendItem((yyvsp[(1) - (3)].L), (yyvsp[(3) - (3)].n)); }
    break;

  case 416:
#line 1911 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (5)].n), NULL, NULL, NULL, (yyvsp[(1) - (5)].tok)); }
    break;

  case 417:
#line 1913 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (7)].n), (yyvsp[(5) - (7)].L), NULL, NULL, (yyvsp[(1) - (7)].tok)); }
    break;

  case 418:
#line 1915 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (9)].n), (yyvsp[(5) - (9)].L), (yyvsp[(7) - (9)].L), NULL, (yyvsp[(1) - (9)].tok)); }
    break;

  case 419:
#line 1917 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord(EMPTY_TQ, (yyvsp[(3) - (11)].n), (yyvsp[(5) - (11)].L), (yyvsp[(7) - (11)].L), (yyvsp[(9) - (11)].L), (yyvsp[(1) - (11)].tok)); }
    break;

  case 420:
#line 1919 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (6)].tq).tq, (yyvsp[(4) - (6)].n), NULL, NULL, NULL, (yyvsp[(1) - (6)].tok)); }
    break;

  case 421:
#line 1921 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (8)].tq).tq, (yyvsp[(4) - (8)].n), (yyvsp[(6) - (8)].L), NULL, NULL, (yyvsp[(1) - (8)].tok)); }
    break;

  case 422:
#line 1923 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (10)].tq).tq, (yyvsp[(4) - (10)].n), (yyvsp[(6) - (10)].L), (yyvsp[(8) - (10)].L), NULL, (yyvsp[(1) - (10)].tok)); }
    break;

  case 423:
#line 1925 "ANSI-C.y"
    { (yyval.n) = MakeAsmCoord((yyvsp[(2) - (12)].tq).tq, (yyvsp[(4) - (12)].n), (yyvsp[(6) - (12)].L), (yyvsp[(8) - (12)].L), (yyvsp[(10) - (12)].L), (yyvsp[(1) - (12)].tok)); }
    break;

  case 424:
#line 1935 "ANSI-C.y"
    { (yyval.L) = GrabPragmas((yyvsp[(1) - (1)].L)); }
    break;

  case 425:
#line 1936 "ANSI-C.y"
    { (yyval.L) = JoinLists((yyvsp[(1) - (2)].L), GrabPragmas((yyvsp[(2) - (2)].L))); }
    break;

  case 426:
#line 1941 "ANSI-C.y"
    {
              if (yydebug)
                {
                  printf("external.definition # declaration\n");
                  fPrintNode(stdout, FirstItem((yyvsp[(1) - (1)].L)), 0);
                  printf("\n\n\n");
		}
              (yyval.L) = (yyvsp[(1) - (1)].L);
            }
    break;

  case 427:
#line 1951 "ANSI-C.y"
    {
              if (yydebug)
                {
                  printf("external.definition # function.definition\n");
                  fPrintNode(stdout, (yyvsp[(1) - (1)].n), 0);
                  printf("\n\n\n");
                }
              (yyval.L) = MakeNewList((yyvsp[(1) - (1)].n));
            }
    break;

  case 428:
#line 1961 "ANSI-C.y"
    {  (yyval.L) = (yyvsp[(2) - (2)].L); }
    break;

  case 429:
#line 1962 "ANSI-C.y"
    { (yyval.L) = (List *) NULL; }
    break;

  case 430:
#line 1967 "ANSI-C.y"
    {
              (yyvsp[(1) - (1)].n) = DefineProc(FALSE,
                              SetDeclType((yyvsp[(1) - (1)].n),
					  MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (1)].n)->coord),
					  Redecl));
            }
    break;

  case 431:
#line 1974 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 433:
#line 1990 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n),
				       MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
				       Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
            }
    break;

  case 434:
#line 1997 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 435:
#line 1999 "ANSI-C.y"
    {
	      Node *decl = SetDeclType((yyvsp[(2) - (2)].n),
				       MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
				       Redecl);
	      (yyvsp[(2) - (2)].n) = DefineProc(FALSE, decl);
            }
    break;

  case 436:
#line 2006 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 437:
#line 2008 "ANSI-C.y"
    {
              (yyvsp[(1) - (1)].n) = DefineProc(TRUE,
                              SetDeclType((yyvsp[(1) - (1)].n),
					  MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (1)].n)->coord),
					  Redecl));
            }
    break;

  case 438:
#line 2015 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 439:
#line 2017 "ANSI-C.y"
    {  Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);

               AddParameterTypes(decl, NULL);
               (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 440:
#line 2023 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 441:
#line 2025 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 442:
#line 2031 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 443:
#line 2033 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (2)].n), type, Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 444:
#line 2040 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 445:
#line 2042 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (2)].tq).tq, (yyvsp[(1) - (2)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (2)].n), type, Redecl);

              AddParameterTypes(decl, NULL);
              (yyvsp[(2) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 446:
#line 2049 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 447:
#line 2051 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType(EMPTY_TQ, (yyvsp[(1) - (2)].n)->coord),
                   *decl = SetDeclType((yyvsp[(1) - (2)].n), type, Redecl);
              AddParameterTypes(decl, (yyvsp[(2) - (2)].L));
              (yyvsp[(1) - (2)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 448:
#line 2057 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 449:
#line 2059 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 450:
#line 2064 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 451:
#line 2066 "ANSI-C.y"
    { Node *decl = SetDeclType((yyvsp[(2) - (3)].n), (yyvsp[(1) - (3)].n), Redecl);
              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 452:
#line 2071 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 453:
#line 2073 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (3)].tq).tq, (yyvsp[(1) - (3)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (3)].n), type, Redecl);

              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 454:
#line 2080 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 455:
#line 2082 "ANSI-C.y"
    { Node *type = MakeDefaultPrimType((yyvsp[(1) - (3)].tq).tq, (yyvsp[(1) - (3)].tq).coord),
                   *decl = SetDeclType((yyvsp[(2) - (3)].n), type, Redecl);


              AddParameterTypes(decl, (yyvsp[(3) - (3)].L));
              (yyvsp[(2) - (3)].n) = DefineProc(TRUE, decl);
            }
    break;

  case 456:
#line 2090 "ANSI-C.y"
    { (yyval.n) = SetProcBody((yyvsp[(2) - (5)].n), (yyvsp[(5) - (5)].n)); }
    break;

  case 457:
#line 2095 "ANSI-C.y"
    { OldStyleFunctionDefinition = TRUE; }
    break;

  case 458:
#line 2097 "ANSI-C.y"
    { OldStyleFunctionDefinition = FALSE;
               (yyval.L) = (yyvsp[(2) - (2)].L); }
    break;

  case 459:
#line 2112 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 460:
#line 2113 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 461:
#line 2114 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 462:
#line 2115 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 463:
#line 2116 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 464:
#line 2121 "ANSI-C.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 465:
#line 2123 "ANSI-C.y"
    { const char *first_text  = (yyvsp[(1) - (2)].n)->u.Const.text;
              const char *second_text = (yyvsp[(2) - (2)].n)->u.Const.text;
              int   length = strlen(first_text) + strlen(second_text) + 1;
              char *buffer = HeapNewArray(char, length);
              char *new_text, *new_val;

              /* since text (which includes quotes and escape codes)
		 is always longer than value, it's safe to use buffer
		 to concat both */
              strcpy(buffer, NodeConstantStringValue((yyvsp[(1) - (2)].n)));
	      strcat(buffer, NodeConstantStringValue((yyvsp[(2) - (2)].n)));
              new_val = UniqueString(buffer);

              strcpy(buffer, first_text);
	      strcat(buffer, second_text);
              new_text = buffer;
              (yyval.n) = MakeStringTextCoord(new_text, new_val, (yyvsp[(1) - (2)].n)->coord);
	     }
    break;

  case 466:
#line 2144 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CONST;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 467:
#line 2145 "ANSI-C.y"
    { (yyval.tq).tq = TQ_VOLATILE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 468:
#line 2146 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLINE;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 469:
#line 2147 "ANSI-C.y"
    { (yyval.tq).tq = TQ_RESTRICT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 470:
#line 2148 "ANSI-C.y"
    { (yyval.tq).tq = TQ_COMPLEX;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 471:
#line 2149 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PROCEDURE;(yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 472:
#line 2150 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLET;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 473:
#line 2151 "ANSI-C.y"
    { (yyval.tq).tq = TQ_SHARED;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 474:
#line 2152 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PRIVATE;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 475:
#line 2153 "ANSI-C.y"
    { (yyval.tq).tq = TQ_NOOUTPUT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 476:
#line 2154 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKONLY; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 477:
#line 2155 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKSAFE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 478:
#line 2159 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CONST;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 479:
#line 2160 "ANSI-C.y"
    { (yyval.tq).tq = TQ_VOLATILE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 480:
#line 2161 "ANSI-C.y"
    { (yyval.tq).tq = TQ_RESTRICT; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 481:
#line 2162 "ANSI-C.y"
    { (yyval.tq).tq = TQ_PROCEDURE;(yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 482:
#line 2163 "ANSI-C.y"
    { (yyval.tq).tq = TQ_INLET;    (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 483:
#line 2164 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKONLY; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 484:
#line 2165 "ANSI-C.y"
    { (yyval.tq).tq = TQ_CILKSAFE; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 485:
#line 2169 "ANSI-C.y"
    { (yyval.tq).tq = TQ_TYPEDEF;  (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 486:
#line 2170 "ANSI-C.y"
    { (yyval.tq).tq = TQ_EXTERN;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 487:
#line 2171 "ANSI-C.y"
    { (yyval.tq).tq = TQ_STATIC;   (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 488:
#line 2172 "ANSI-C.y"
    { (yyval.tq).tq = TQ_AUTO;     (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 489:
#line 2173 "ANSI-C.y"
    { (yyval.tq).tq = TQ_REGISTER; (yyval.tq).coord = (yyvsp[(1) - (1)].tok); }
    break;

  case 490:
#line 2177 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Void, (yyvsp[(1) - (1)].tok));    }
    break;

  case 491:
#line 2178 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Char, (yyvsp[(1) - (1)].tok));     }
    break;

  case 492:
#line 2179 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Int_ParseOnly, (yyvsp[(1) - (1)].tok));     }
    break;

  case 493:
#line 2180 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Float, (yyvsp[(1) - (1)].tok));   }
    break;

  case 494:
#line 2181 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Double, (yyvsp[(1) - (1)].tok));  }
    break;

  case 495:
#line 2183 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Signed, (yyvsp[(1) - (1)].tok));  }
    break;

  case 496:
#line 2184 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Unsigned, (yyvsp[(1) - (1)].tok));}
    break;

  case 497:
#line 2186 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Short, (yyvsp[(1) - (1)].tok));   }
    break;

  case 498:
#line 2187 "ANSI-C.y"
    { (yyval.n) = StartPrimType(Long, (yyvsp[(1) - (1)].tok));    }
    break;

  case 500:
#line 2195 "ANSI-C.y"
    { (yyval.n) = ExtendArray((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].tok)); }
    break;

  case 501:
#line 2197 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 502:
#line 2199 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 503:
#line 2201 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTINC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 504:
#line 2203 "ANSI-C.y"
    { (yyval.n) = MakeUnaryCoord(POSTDEC, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].tok)); }
    break;

  case 505:
#line 2207 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord('.', (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 506:
#line 2209 "ANSI-C.y"
    { (yyval.n) = MakeBinopCoord(ARROW, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].tok)); }
    break;

  case 507:
#line 2214 "ANSI-C.y"
    { (yyval.n) = OmitLookups() ? (yyvsp[(1) - (1)].n) : LookupPostfixExpression((yyvsp[(1) - (1)].n));}
    break;

  case 508:
#line 2218 "ANSI-C.y"
    { (yyval.L) = (List *) NULL; }
    break;

  case 509:
#line 2219 "ANSI-C.y"
    { (yyval.L) = (yyvsp[(1) - (1)].L); }
    break;

  case 510:
#line 2226 "ANSI-C.y"
    { (yyval.n) = MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].L), FALSE, (yyvsp[(1) - (6)].tok)); }
    break;

  case 511:
#line 2231 "ANSI-C.y"
    { (yyval.n) = MakeSpawnCoord((yyvsp[(1) - (8)].n), (yyvsp[(2) - (8)].n)->u.binop.op, (yyvsp[(4) - (8)].n), (yyvsp[(6) - (8)].L), FALSE, (yyvsp[(3) - (8)].tok)); }
    break;

  case 512:
#line 2235 "ANSI-C.y"
    { (yyval.n) = MakeSyncCoord((yyvsp[(1) - (2)].tok)); }
    break;

  case 513:
#line 2239 "ANSI-C.y"
    { (yyval.n) = MakeInletCallCoord((OmitLookups() ? (yyvsp[(1) - (5)].n) : LookupPostfixExpression((yyvsp[(1) - (5)].n))), (yyvsp[(3) - (5)].L), (yyvsp[(2) - (5)].tok)); }
    break;

  case 514:
#line 2248 "ANSI-C.y"
    { (yyval.n) = MakeAbortCoord((yyvsp[(1) - (2)].tok)); }
    break;

  case 515:
#line 2253 "ANSI-C.y"
    { (yyval.L) = MakeNewList(MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].L), TRUE, (yyvsp[(1) - (5)].tok))); }
    break;

  case 516:
#line 2255 "ANSI-C.y"
    { (yyval.L) = ConsItem(MakeSpawnCoord(NULL, '=', (yyvsp[(2) - (7)].n), (yyvsp[(4) - (7)].L), TRUE, (yyvsp[(1) - (7)].tok)), (yyvsp[(7) - (7)].L)); }
    break;

  case 517:
#line 2260 "ANSI-C.y"
    { (yyval.n) = MakeSynchedCoord((yyvsp[(1) - (1)].tok)); }
    break;


/* Line 1267 of yacc.c.  */
#line 6207 "ANSI-C.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 2269 "ANSI-C.y"

/* ----end of grammar----*/


PRIVATE void WarnAboutPrecedence(OpType op, Node *node)
{
  if (node->typ == Binop && !node->parenthesized) {
    OpType subop = node->u.binop.op;

    if (op == OROR && subop == ANDAND)
      WarningCoord(4, node->coord, "suggest parentheses around && in operand of ||");
    else if ((op == '|' || op == '^') &&
	     (subop == '+' || subop == '-' || subop == '&' || subop == '^') &&
	     op != subop)
      WarningCoord(4, node->coord, "suggest parentheses around arithmetic in operand of %c", op);
  }
}
