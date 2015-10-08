/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1489 of yacc.c.  */
#line 273 "ANSI-C.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

