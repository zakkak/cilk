%{

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

 GLOBAL inline Bool AcceptWildcards() {
   return (parseInputType != EntireProgram);
 }
 
 GLOBAL Bool OmitLookups() {
   return (OmitLookupsFlag && (parseInputType != EntireProgram));
   /*  return FALSE; */
 }


%}

/* This refined grammar resolves several typedef ambiguities  in  the 
draft  proposed  ANSI  C  standard  syntax  down  to  1  shift/reduce 
conflict, as reported by a YACC process.  Note  that  the  one  shift 
reduce  conflicts  is the traditional if-if-else conflict that is not 
resolved by the grammar.  This ambiguity can  be  removed  using  the 
method  described in the Dragon Book (2nd edition), but this does not 
appear worth the effort.

There was quite a bit of effort made to reduce the conflicts to  this 
level,  and  an  additional effort was made to make the grammar quite 
similar to the C++ grammar being developed in  parallel.   Note  that 
this grammar resolves the following ANSI C ambiguity as follows:

ANSI  C  section  3.5.6,  "If  the [typedef name] is redeclared at an 
inner scope, the type specifiers shall not be omitted  in  the  inner 
declaration".   Supplying type specifiers prevents consideration of T 
as a typedef name in this grammar.  Failure to supply type specifiers 
forced the use of the TYPEDEFname as a type specifier.
              
ANSI C section 3.5.4.3, "In a parameter declaration, a single typedef 
name in parentheses is  taken  to  be  an  abstract  declarator  that 
specifies  a  function  with  a  single  parameter,  not as redundant 
parentheses around the identifier".  This is extended  to  cover  the 
following cases:

typedef float T;
int noo(const (T[5]));
int moo(const (T(int)));
...

Where  again the '(' immediately to the left of 'T' is interpreted as 
being the start of a parameter type list,  and  not  as  a  redundant 
paren around a redeclaration of T.  Hence an equivalent code fragment 
is:

typedef float T;
int noo(const int identifier1 (T identifier2 [5]));
int moo(const int identifier1 (T identifier2 (int identifier3)));
...

*/
/* %debug */

%union {
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



/* Define terminal tokens */

%token <tok> '&' '*' '+' '-' '~' '!'
%token <tok> '<' '>' '|' '^' '%' '/' '(' ')' '.' '?' ';'

%token <tok> '{' '}' ',' '[' ']' ':'

/* ANSI keywords, extensions below */
%token <tok> AUTO            DOUBLE          INT             STRUCT
%token <tok> BREAK           ELSE            LONG            SWITCH
%token <tok> CASE            ENUM            REGISTER        TYPEDEF
%token <tok> CHAR            EXTERN          RETURN          UNION
%token <tok> CONST           FLOAT           SHORT           UNSIGNED
%token <tok> CONTINUE        FOR             SIGNED          VOID
%token <tok> DEFAULT         GOTO            SIZEOF          VOLATILE
%token <tok> DO              IF              STATIC          WHILE

/* unary op tokens added by Eric Brewer */
%token <tok> UPLUS UMINUS INDIR ADDRESS POSTINC POSTDEC PREINC PREDEC BOGUS

/* ANSI Grammar suggestions */
%token <n> IDENTIFIER STRINGliteral
%token <n> FLOATINGconstant
%token <n> INTEGERconstant OCTALconstant HEXconstant WIDECHARconstant
%token <n> CHARACTERconstant

/* c99 extensions */
%token <tok> COMPLEX REAL IMAG

/* New Lexical element, whereas ANSI suggested non-terminal */

/* 
   Lexer distinguishes this from an identifier.
   An identifier that is CURRENTLY in scope as a typedef name is provided
   to the parser as a TYPEDEFname
*/
%token <n> TYPEDEFname 

/* Multi-Character operators */
%token <tok>  ARROW            /*    ->                              */
%token <tok>  ICR DECR         /*    ++      --                      */
%token <tok>  LS RS            /*    <<      >>                      */
%token <tok>  LE GE EQ NE      /*    <=      >=      ==      !=      */
%token <tok>  ANDAND OROR      /*    &&      ||                      */
%token <tok>  ELLIPSIS         /*    ...                             */

/* modifying assignment operators */
%token <tok> '='
%token <tok> MULTassign  DIVassign    MODassign   /*   *=      /=      %=      */
%token <tok> PLUSassign  MINUSassign              /*   +=      -=              */
%token <tok> LSassign    RSassign                 /*   <<=     >>=             */
%token <tok> ANDassign   ERassign     ORassign    /*   &=      ^=      |=      */

/* GCC extensions */
%token <tok> INLINE
%token <tok> ATTRIBUTE
%token <tok> EXTENSION
%token <tok> RESTRICT
%token <tok> ALIGNOF
%token <tok> TYPEOF
/* MS C extensions */
%token <tok> DECLSPEC_EXPORT
%token <tok> DECLSPEC_IMPORT

/* gcc-specific support for stdarg.h */
%token <tok> BUILTIN_VA_ARG

/* need a token for the  `.d=x, .d=c' desiginated initializer syntax.  This is used as a binop. */
/* Also a colon can be a binop for the obsolete pre-gcc-2.5 version of the designated initializer. */
%token <tok> DESIGNATED_INITIALIZER
%token <tok> ARRAY_LABELED_INITIALIZER

/* 
 * cilk2c extension.  The __nooutput__ keyword denotes a declaration
 * that should not be output to the C code
 */
%token <tok> NOOUTPUT

/* 
 * cilk2c extension.  The __cilkonly__ keyword denotes a declaration
 * that can only be used in Cilk code
 */
%token <tok> CILKONLY

/* 
 * cilk2c extension.  The hint system does not issue warnings
 * for __cilksafe__ global variables.
 */
%token <tok> CILKSAFE

/*
 * Tokens for wildcards. 
 *
 */
%token <n> WILDCARD_T WILDCARD_E WILDCARD_D



%type <tok> lblock rblock

%type <L> translation.unit external.definition 
%type <n> function.definition

%type <n> constant string.literal.list
%type <n> typeof.expression
%type <n> primary.expression postfix.expression unary.expression
%type <n> cast.expression multiplicative.expression additive.expression
%type <n> shift.expression relational.expression equality.expression
%type <n> AND.expression exclusive.OR.expression inclusive.OR.expression
%type <n> logical.AND.expression logical.OR.expression conditional.expression
%type <n> assignment.expression constant.expression expression.opt
%type <n> attrib any.word asm.opt asm

%type <n> initializer.opt initializer basic.initializer initializer.list bracketed.initializer.list
%type <n> bit.field.size bit.field.size.opt enumerator.value.opt

%type <n> statement labeled.statement expression.statement
%type <n> selection.statement iteration.statement jump.statement
%type <n> compound.statement compound.statement.no.new.scope

%type <n> basic.declaration.specifier basic.type.specifier
%type <n> type.name expression type.specifier type.specifier.nosue
%type <n> declaration.specifier declaration.specifier.nosue
%type <n> typedef.declaration.specifier typedef.type.specifier typeof.type.specifier
%type <n> abstract.declarator unary.abstract.declarator
%type <n> postfixing.abstract.declarator array.abstract.declarator
%type <n> postfix.abstract.declarator old.function.declarator
%type <n> struct.or.union.specifier struct.or.union elaborated.type.name
%type <n> sue.type.specifier sue.declaration.specifier enum.specifier

%type <n> parameter.declaration
%type <n> identifier.declarator parameter.typedef.declarator
%type <n> declarator paren.typedef.declarator
%type <n> clean.typedef.declarator simple.paren.typedef.declarator
%type <n> unary.identifier.declarator paren.identifier.declarator
%type <n> postfix.identifier.declarator clean.postfix.typedef.declarator
%type <n> paren.postfix.typedef.declarator postfix.old.function.declarator
%type <n> struct.identifier.declarator struct.declarator

%type <L> declaration declaration.list declaring.list default.declaring.list
%type <L> argument.expression.list identifier.list statement.list
%type <L> parameter.type.list parameter.list
%type <L> struct.declaration.list struct.declaration struct.declaring.list
%type <L> struct.default.declaring.list enumerator.list
%type <L> old.function.declaration.list
%type <L> nonnull.expression.list

%type <n> unary.operator assignment.operator
%type <n> identifier.or.typedef.name

%type <tq> type.qualifier type.qualifier.list declaration.qualifier.list
%type <tq> declaration.qualifier storage.class 
%type <tq> pointer.type.qualifier pointer.type.qualifier.list
%type <tq> attributes.opt attributes attribute
%type <L> attribute.list 

%type <n>  basic.type.name 

/* GCC extensions */
%token <tok> ASM
%type <n> gcc.statement
%type <tok> gcc.extension
%type <n> asm.argument
%type <L> asm.argument.list asm.argument.list.opt
%type <L> asm.clobbered.list

/* Cilk extensions */
%token <tok>  PROCEDURE SPAWN SYNC
%type <n>  cilk.statement spawn.procedure spawn.postfix.expression

%token <tok>  INLET ABORT
%token <tok>  XBEGIN XEND
%type <tok>  xlblock xrblock
%type <L> inlet.call.argument.expression.list

%token <tok>  SYNCHED
%token <tok> SHAREDVAR PRIVATEVAR
%type <n> cilk.expression
%type <L> spawn.args
/* end Cilk extensions */

%start prog.start

%%
prog.start: /*P*/
          /* empty*/ {
                       if (ANSIOnly) SyntaxError("ANSI C forbids an empty source file");
                       Program = GrabPragmas(NULL);
                     }
        | translation.unit { Program = JoinLists($1, GrabPragmas(0)); parseInputType = EntireProgram; }
        | string.start.symbol statement.list { StmtListOutput = $2; parseInputType = StatementListString; }
        | string.start.symbol declaration.list {DeclListOutput = $2; parseInputType = DeclListString; }
        ;

string.start.symbol:
        '?' {}
;
        

/*****************************************************************************
 *                                                                           *
 *                               EXPRESSIONS                                 *
 *                                                                           *
 *****************************************************************************/

primary.expression:             /* P */ /* 6.3.1 EXTENDED */  
          /* A typedef name cannot be used as a variable.  Fill in type later */
          IDENTIFIER           { $$ = $1; }
        | constant
        | string.literal.list
        | '(' expression ')'    { $2->coord = $1;   /* We used to set the coord only for comma expressions.  We really need to do it everywhere. */ 
                                  $2->parenthesized = TRUE;
                                  $$ = $2; }

          /* GCC-inspired non ANSI-C extension */
        | '(' lblock statement.list rblock ')'
            { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
               $$ = MakeBlockCoord(NULL, NULL, GrabPragmas($3), $1, $4);
	    }
        | '(' lblock declaration.list statement.list rblock ')'
            { if (ANSIOnly)
	         SyntaxError("statement expressions not allowed with -ansi switch");
              $$ = MakeBlockCoord(NULL, $3, GrabPragmas($4), $1, $5);
	    }
        | cilk.expression
	| WILDCARD_E { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               $$ = $1;
	             }
        | WILDCARD_D { if (!AcceptWildcards())
	               {
			 SyntaxError("Wildcard tokens not allowed in program. \n");
		       }
	               $$ = $1;
	             }
        ;

postfix.expression:             /* P */ /* 6.3.2 CLARIFICATION */
          primary.expression
        | postfix.expression '[' expression ']'
            { $$ = ExtendArray($1, $3, $2); }
        | postfix.expression '(' ')'
            { $$ = MakeCallCoord($1, NULL, $2); }
        | postfix.expression '(' argument.expression.list ')'
            { $$ = MakeCallCoord($1, $3, $2); }
        | postfix.expression '.' IDENTIFIER
            { $$ = MakeBinopCoord('.', $1, $3, $2); }
        | postfix.expression ARROW IDENTIFIER
            { $$ = MakeBinopCoord(ARROW, $1, $3, $2); }
        | postfix.expression ICR
            { $$ = MakeUnaryCoord(POSTINC, $1, $2); }
        | postfix.expression DECR
            { $$ = MakeUnaryCoord(POSTDEC, $1, $2); }

          /* EXTENSION: TYPEDEFname can be used to name a struct/union field.  This is no longer an extension, it is simply allowed. */
        | postfix.expression '.'   TYPEDEFname
            { $$ = MakeBinopCoord('.', $1, $3, $2); }
        | postfix.expression ARROW TYPEDEFname
            { $$ = MakeBinopCoord(ARROW, $1, $3, $2); }

        | postfix.expression '.' WILDCARD_D
             { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       $$ = MakeBinopCoord('.', $1, $3, $2);
	     }
        | postfix.expression ARROW WILDCARD_D
            { if (!AcceptWildcards()) {
	         SyntaxError("Wildcard tokens not allowed in program. \n");
               }
	       $$ = MakeBinopCoord(ARROW, $1, $3, $2);
	    }
        ;

argument.expression.list:       /* P */ /* 6.3.2 */
          assignment.expression
            { $$ = MakeNewList($1); }
        | argument.expression.list ',' assignment.expression
            { $$ = AppendItem($1, $3); }
        ;

unary.expression:               /* P */ /* 6.3.3 */
          postfix.expression
            { $$ = OmitLookups() ? $1 : LookupPostfixExpression($1);}
        | ICR unary.expression
            { $$ = MakeUnaryCoord(PREINC, $2, $1); }
        | DECR unary.expression
            { $$ = MakeUnaryCoord(PREDEC, $2, $1); }
        | unary.operator cast.expression
            { $1->u.unary.expr = $2;
              $$ = $1; }
        | gcc.extension cast.expression
            { $$ = $2; }
        | SIZEOF unary.expression
            { $$ = MakeUnaryCoord(SIZEOF, $2, $1); }
        | SIZEOF '(' type.name ')'
            { $$ = MakeUnaryCoord(SIZEOF, $3, $1); }
        | ALIGNOF unary.expression
            { $$ = MakeUnaryCoord(ALIGNOF, $2, $1); }
        | ALIGNOF '(' type.name ')'
            { $$ = MakeUnaryCoord(ALIGNOF, $3, $1); }
        | BUILTIN_VA_ARG '(' assignment.expression ',' type.name ')'
            { $$ = MakeBuiltinVaArgCoord($3, $5, $1); }
        | IMAG unary.expression
            { $$ = MakeUnaryCoord(IMAG, $2, $1); }
        | REAL unary.expression
            { $$ = MakeUnaryCoord(REAL, $2, $1); }
        ;

unary.operator:                 /* P */ /* 6.3.3 */
          '&'     { $$ = MakeUnaryCoord('&', NULL, $1); }
        | '*'     { $$ = MakeUnaryCoord('*', NULL, $1); }
        | '+'     { $$ = MakeUnaryCoord('+', NULL, $1); }
        | '-'     { $$ = MakeUnaryCoord('-', NULL, $1); }
        | '~'     { $$ = MakeUnaryCoord('~', NULL, $1); }
        | '!'     { $$ = MakeUnaryCoord('!', NULL, $1); }
        ;

cast.expression:                /* P */ /* 6.3.4 */
          unary.expression
        | '(' type.name ')' bracketed.initializer.list { if (ANSIOnly)
 	                                                   SyntaxError("constructors not allowed with -ansi switch");
                                                         $$ = MakeConstructorCoord($2, $4, $1); 
                                                       }
        | '(' type.name ')' cast.expression { $$ = MakeCastCoord($2, $4, $1); }
        ;

multiplicative.expression:      /* P */ /* 6.3.5 */
          cast.expression
        | multiplicative.expression '*' cast.expression
            { $$ = MakeBinopCoord('*', $1, $3, $2); }
        | multiplicative.expression '/' cast.expression
            { $$ = MakeBinopCoord('/', $1, $3, $2); }
        | multiplicative.expression '%' cast.expression
            { $$ = MakeBinopCoord('%', $1, $3, $2); }
        ;

additive.expression:            /* P */ /* 6.3.6 */
          multiplicative.expression
        | additive.expression '+' multiplicative.expression
            { $$ = MakeBinopCoord('+', $1, $3, $2); }
        | additive.expression '-' multiplicative.expression
            { $$ = MakeBinopCoord('-', $1, $3, $2); }
        ;

shift.expression:               /* P */ /* 6.3.7 */
          additive.expression
        | shift.expression LS additive.expression
            { $$ = MakeBinopCoord(LS, $1, $3, $2); }
        | shift.expression RS additive.expression
            { $$ = MakeBinopCoord(RS, $1, $3, $2); }
        ;

relational.expression:          /* P */ /* 6.3.8 */
          shift.expression
        | relational.expression '<' shift.expression
            { $$ = MakeBinopCoord('<', $1, $3, $2); }
        | relational.expression '>' shift.expression
            { $$ = MakeBinopCoord('>', $1, $3, $2); }
        | relational.expression LE shift.expression
            { $$ = MakeBinopCoord(LE, $1, $3, $2); }
        | relational.expression GE shift.expression
            { $$ = MakeBinopCoord(GE, $1, $3, $2); }
        ;

equality.expression:            /* P */ /* 6.3.9 */
          relational.expression
        | equality.expression EQ relational.expression
            { $$ = MakeBinopCoord(EQ, $1, $3, $2); }
        | equality.expression NE relational.expression
            { $$ = MakeBinopCoord(NE, $1, $3, $2); }
        ;

AND.expression:                 /* P */ /* 6.3.10 */
          equality.expression
        | AND.expression '&' equality.expression
            { $$ = MakeBinopCoord('&', $1, $3, $2); }
        ;

exclusive.OR.expression:        /* P */ /* 6.3.11 */
          AND.expression
        | exclusive.OR.expression '^' AND.expression
            { 
              WarnAboutPrecedence('^', $1);
              WarnAboutPrecedence('^', $3);
	      $$ = MakeBinopCoord('^', $1, $3, $2); }
        ;

inclusive.OR.expression:        /* P */ /* 6.3.12 */
          exclusive.OR.expression
        | inclusive.OR.expression '|' exclusive.OR.expression
            { WarnAboutPrecedence('|', $1);
              WarnAboutPrecedence('|', $3);
              $$ = MakeBinopCoord('|', $1, $3, $2); }
        ;

logical.AND.expression:         /* P */ /* 6.3.13 */
          inclusive.OR.expression
        | logical.AND.expression ANDAND inclusive.OR.expression
            { $$ = MakeBinopCoord(ANDAND, $1, $3, $2); }
        ;

logical.OR.expression:          /* P */ /* 6.3.14 */
          logical.AND.expression
        | logical.OR.expression OROR logical.AND.expression
            { WarnAboutPrecedence(OROR, $1);
              WarnAboutPrecedence(OROR, $3);
              $$ = MakeBinopCoord(OROR, $1, $3, $2); }
        ;

conditional.expression:         /* P */ /* 6.3.15 */
          logical.OR.expression
        | logical.OR.expression '?' expression ':' conditional.expression
            { $$ = MakeTernaryCoord($1, $3, $5, $2, $4); }
        | logical.OR.expression '?' ':' conditional.expression
            {
	      if (ANSIOnly)
	        SyntaxError("ISO C forbids omitting the middle term of a ?: expression");
	      $$ = MakeTernaryCoord($1, NULL, $4, $2, $3);
	    }
        ;

assignment.expression:          /* P */ /* 6.3.16 */
          conditional.expression
        | cast.expression assignment.operator assignment.expression
            { $2->u.binop.left = $1;
              $2->u.binop.right = $3;
              $$ = $2; }
        ;

assignment.operator:            /* P */ /* 6.3.16 */
          '='             { $$ = MakeBinopCoord('=', NULL, NULL, $1); }
        | MULTassign      { $$ = MakeBinopCoord(MULTassign, NULL, NULL, $1);  }
        | DIVassign       { $$ = MakeBinopCoord(DIVassign, NULL, NULL, $1);   }
        | MODassign       { $$ = MakeBinopCoord(MODassign, NULL, NULL, $1);   }
        | PLUSassign      { $$ = MakeBinopCoord(PLUSassign, NULL, NULL, $1);  }
        | MINUSassign     { $$ = MakeBinopCoord(MINUSassign, NULL, NULL, $1); }
        | LSassign        { $$ = MakeBinopCoord(LSassign, NULL, NULL, $1);    }
        | RSassign        { $$ = MakeBinopCoord(RSassign, NULL, NULL, $1);    }
        | ANDassign       { $$ = MakeBinopCoord(ANDassign, NULL, NULL, $1);   }
        | ERassign        { $$ = MakeBinopCoord(ERassign, NULL, NULL, $1);    }
        | ORassign        { $$ = MakeBinopCoord(ORassign, NULL, NULL, $1);    }
        ;

expression:                     /* P */ /* 6.3.17 */
          assignment.expression
        | expression ',' assignment.expression
            {  
              if ($1->typ == Comma) 
                {
		  AppendItem($1->u.comma.exprs, $3);
		  $$ = $1;
		}
              else
                {
		    $$ = MakeCommaCoord(AppendItem(MakeNewList($1), $3), $1->coord);
		}
	    }
        ;

constant.expression:            /* P */ /* 6.4   */
          conditional.expression { $$ = $1; }
        ;

expression.opt:                 /* P */ /* For convenience */
          /* Nothing */   { $$ = (Node *) NULL; }
        | expression      { $$ = $1; }
        ;

/******************************************************************************
 
                                DECLARATIONS		
 							
     The following is different from the ANSI C specified grammar.
 The changes were made to disambiguate typedef's presence in
 declaration.specifiers (vs. in the declarator for redefinition) to
 allow struct/union/enum tag declarations without declarators, and to
 better reflect the parsing of declarations (declarators must be
 combined with declaration.specifiers ASAP so that they are visible in
 scope).
	
 Example of typedef use as either a declaration.specifier or a
 declarator:

   typedef int T; struct S { T T; }; / redefinition of T as member
   name

 Example of legal and invalid statements detected by this grammar:
								
   int;                syntax error: vacuous declaration      
   struct S;           no error: tag is defined or elaborated 

 Example of result of proper declaration binding: 

  Declare "a" with a type in the name space BEFORE parsing initializer

   int a = sizeof(a);							
 
  Declare "b" with a type before parsing "c" 

   int b, c[sizeof(b)];							

******************************************************************************/



/*                        */    /* ? */ /* ?.?.? */
declaration: /*P*/
          declaring.list ';'
            { $$ = $1; }
        | default.declaring.list ';'
            { $$ = $1; }
        | sue.declaration.specifier ';'
            { $$ = MakeNewList(ForceNewSU($1, $2)); }
        | sue.type.specifier ';'
            { $$ = MakeNewList(ForceNewSU($1, $2)); }
/* I made an inlet just a Proc with T_INLET qualifier.  I'm not sure
 * I added all of the cases for defining a local procedure yet.
 * These rules were just moved from the function.definition: rule.  -KHR
 */
        | declaration.specifier identifier.declarator
            {
	      Node *decl = SetDeclType($2, $1, Redecl);
	      $2 = DefineProc(FALSE, decl);
	    }
          compound.statement.no.new.scope
            { $$ = MakeNewList(SetProcBody($2, $4)); }
        | declaration.specifier.nosue attributes identifier.declarator
            {    
		 SetDeclAttribs($3, $2.tq);
		 $3 = DefineProc(FALSE, SetDeclType($3, $1, Redecl));
	    }
          compound.statement.no.new.scope
            { $$ = MakeNewList(SetProcBody($3, $5)); }
        | type.specifier identifier.declarator
	    {
	      Node *decl = SetDeclType($2, $1, Redecl);
	      $2 = DefineProc(FALSE, decl);
	      $2->pragmas_before = GrabPragmas(0);
	    }
          compound.statement.no.new.scope
            {
		$$ = AppendItem($2->pragmas_before,
				SetProcBody($2, $4));
		$2->pragmas_before = 0;
	    }
        | type.specifier.nosue attributes identifier.declarator
	    {
	      SetDeclAttribs($3, $2.tq);
	      $3 = DefineProc(FALSE, SetDeclType($3, $1, Redecl));
	      $3->pragmas_before = GrabPragmas(0);
	    }
          compound.statement.no.new.scope
            {
		$$ = AppendItem($3->pragmas_before,
				SetProcBody($3, $5));
		$3->pragmas_before = 0;
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
declaring.list: /*P*/
          declaration.specifier 
          declarator
            { 
	      SetDeclType($2, $1, Redecl);
	    }
          asm.opt { SetDeclAsm($2, $4); }
          attributes.opt { SetDeclAttribs($2, $6.tq); }
          initializer.opt
            { 
              $$ = MakeNewList(SetDeclInit($2, $8)); 
            }
        | declaration.specifier.nosue
	  attributes
          declarator
            { 
	      SetDeclAttribs($3, $2.tq);
	      SetDeclType($3, $1, Redecl);
	    }
          asm.opt { SetDeclAsm($3, $5); }
          attributes.opt { SetDeclAttribs($3, $7.tq); }
          initializer.opt
            { 
              $$ = MakeNewList(SetDeclInit($3, $9)); 
            }
        | type.specifier 
          declarator 
            { 
              SetDeclType($2, $1, Redecl);
            }
          asm.opt { SetDeclAsm($2, $4); }
          attributes.opt { SetDeclAttribs($2, $6.tq); }
          initializer.opt
            { 
              $$ = MakeNewList(SetDeclInit($2, $8)); 
	    }
        | type.specifier.nosue
	  attributes
          declarator 
            { 
		 SetDeclAttribs($3, $2.tq);
		 SetDeclType($3, $1, Redecl);
            }
          asm.opt { SetDeclAsm($3, $5); }
          attributes.opt { SetDeclAttribs($3, $7.tq); }
          initializer.opt
            { 
		 $$ = MakeNewList(SetDeclInit($3, $9)); 
	    }
        | declaring.list ',' declarator
            { 
	      $$ = AppendDecl($1, $3, Redecl);
	    }
          asm.opt { SetDeclAsm($3, $5); }
          attributes.opt { SetDeclAttribs($3, $7.tq); }
          initializer.opt
            { 
              SetDeclInit($3, $9); 
	      if (yydebug) {
		  //printf("%s:%d ", __FILE__, __LINE__); DPL($7); 
		  printf("%s:%d ", __FILE__, __LINE__); DPN($3);
	      }
            }

        /******** ERROR PRODUCTIONS ********/
        | /* error production: catch missing identifier */
          declaration.specifier error
            { 
              SyntaxError("declaration without a variable"); 
            }
          asm.opt 
          attributes.opt
          initializer.opt
            { 
              $$ = NULL; /* empty list */ 
            }
        | /* error production: catch missing identifier */
          type.specifier error
            { 
              SyntaxError("declaration without a variable"); 
            }
          asm.opt 
          attributes.opt
          initializer.opt
            { 
              $$ = NULL; /* empty list */ 
            }
/*        | gcc.extension declaration { $$ = $2; }*/
        | declaring.list ',' error
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Note that if a typedef were redeclared, then a decl-spec must be supplied */
default.declaring.list:  /*P*/ /* Can't  redeclare typedef names */
          declaration.qualifier.list identifier.declarator
            { 
              SetDeclType($2, MakeDefaultPrimType($1.tq, $1.coord), NoRedecl);
            }
          attributes.opt {
              SetDeclAttribs($2, $4.tq);
          }
	  initializer.opt
            { 
		$$ = MakeNewList(SetDeclInit($2, $6)); 
            }
        | type.qualifier.list identifier.declarator
            { 
              SetDeclType($2, MakeDefaultPrimType($1.tq, $1.coord), NoRedecl);
            }
          attributes.opt {
	    SetDeclAttribs($2, $4.tq);
	  }
	  initializer.opt
            { 
              $$ = MakeNewList(SetDeclInit($2, $6)); 
	    }
        | default.declaring.list ',' identifier.declarator
            { $$ = AppendDecl($1, $3, NoRedecl); }
          attributes.opt { SetDeclAttribs($3, $5.tq); }
	  initializer.opt
            { SetDeclInit($3, $7); }

        /********  ERROR PRODUCTIONS ********/
        | /* error production: catch missing identifier */
          declaration.qualifier.list error
            { 
              SyntaxError("declaration without a variable"); 
	    }
          attributes.opt
          initializer.opt
            { 
              $$ = NULL; /* empty list */ 
	    }
        | /* error production: catch missing identifier */
          type.qualifier.list error
            { 
              SyntaxError("declaration without a variable"); 
	    }
          attributes.opt
          initializer.opt
            { 
              $$ = NULL; /* empty list */ 
            }
        | default.declaring.list ',' error
        ;

/*                        */    /* ? */ /* ?.?.? */
declaration.specifier: /*P*/
          declaration.specifier.nosue
        | sue.declaration.specifier          /* struct/union/enum  */
        ;

declaration.specifier.nosue: /*P*/
          basic.declaration.specifier        /* Arithmetic or void */
            { $$ = FinishPrimType($1); }
        | typedef.declaration.specifier      /* typedef            */
	| WILDCARD_T { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               $$ = $1;
	             }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* StorageClass + Arithmetic or void */
basic.declaration.specifier:  /*P*/
          basic.type.specifier storage.class
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); } 
        | declaration.qualifier.list basic.type.name
            { $$ = TypeQualifyNode($2, $1.tq, & $1.coord); }
        | basic.declaration.specifier declaration.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        | basic.declaration.specifier basic.type.name
            { $$ = MergePrimTypes($1, $2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* StorageClass + struct/union/enum */
sue.declaration.specifier: /*P*/   
          sue.type.specifier storage.class
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        | declaration.qualifier.list elaborated.type.name attributes.opt
            { $$ = TypeQualifyNode($2, tq_union($1.tq, $3.tq), & $1.coord); }
        | sue.declaration.specifier declaration.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Storage Class + typedef types */
typedef.declaration.specifier:  /*P*/      
          typedef.type.specifier storage.class
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        | declaration.qualifier.list TYPEDEFname
            { $$ = ConvertIdToTdef($2, $1.tq, GetTypedefType($2));
              $$->coord = $1.coord; }
        | typedef.declaration.specifier declaration.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Type qualifier AND storage class */
declaration.qualifier.list:  /*P*/
          storage.class attributes.opt
            {
		 $$.tq = MergeTypeQuals($1.tq, $2.tq, $2.coord);
		 $$.coord = $1.coord; 
	    }
        | type.qualifier.list attributes.opt storage.class attributes.opt
            {
		 $$.tq = MergeTypeQuals($1.tq, $2.tq, $2.coord);
		 $$.tq = MergeTypeQuals($$.tq, $3.tq, $3.coord);
		 $$.coord = $1.coord; 
	    }
        | declaration.qualifier.list declaration.qualifier
            {
              $$.tq = MergeTypeQuals($1.tq, $2.tq, $2.coord);
              $$.coord = $1.coord; 
	    }
/*
        | declaration.qualifier.list  attribute
           {  $$.tq = tq_union($1.tq, tq_attribute($2));
	      $$.coord = $1.coord; }
*/
        ;

/*                        */    /* ? */ /* ?.?.? */
declaration.qualifier: /*P*/
          type.qualifier 
          | storage.class 
        ;

typeof.expression: 
          expression
        | type.specifier
        ;

/*                        */    /* ? */ /* ?.?.? */
type.specifier: /*P*/
          type.specifier.nosue
        | sue.type.specifier                 /* Struct/Union/Enum  */
        ;

type.specifier.nosue: /*P*/
          basic.type.specifier    /* Arithmetic or void */
            { $$ = FinishPrimType($1); }
        | typedef.type.specifier
        | typeof.type.specifier
        ;

typeof.type.specifier:
	  TYPEOF '(' typeof.expression ')'   /* gcc extension      */
  	    /*
	     * the call to SemCheckNode is gross.  We should really
	     * produce a typeof node and reconstruct the type
	     * later, but I am too tired now to do it.
	     */
            {
		$$ = NodeCopy(NodeDataType(SemCheckNode($3)), Subtree); /* copy the node so we can remove type qualifiers without hurting the code */	      
		SetCoords($$, $1, Subtree);
		NodeUpdateTq($$, tq_remove_everything); /* Take off all the type qualifiers */
	    }
	| type.qualifier.list TYPEOF '(' typeof.expression ')'   /* gcc extension      */
  	    /*
	     * the call to SemCheckNode is gross.  We should really
	     * produce a typeof node and reconstruct the type
	     * later, but I am too tired now to do it.
	     */
            {
		/* we have to copy the node because we don't want the type qualifiers to modify the original node. */
		Node *thetype=SetCoords(NodeCopy(NodeDataType(SemCheckNode($4)), Subtree), $2, Subtree);
		NodeUpdateTq(thetype, tq_remove_everything); /* Take off all the type qualifiers */
		$$ = TypeQualifyNode(thetype, $1.tq, & $1.coord); /* Add on the new type qualifiers */
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
basic.type.specifier: /*P*/
          basic.type.name            /* Arithmetic or void */
        | type.qualifier.list basic.type.name
            { $$ = TypeQualifyNode($2, $1.tq, & $1.coord);  }
        | basic.type.specifier type.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        | basic.type.specifier basic.type.name
            { $$ = MergePrimTypes($1, $2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
sue.type.specifier:
          elaborated.type.name              /* struct/union/enum */
        | type.qualifier.list elaborated.type.name
            { $$ = TypeQualifyNode($2, $1.tq, & $1.coord); }
        | sue.type.specifier type.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* typedef types */
typedef.type.specifier:  /*P*/             
          TYPEDEFname
            { $$ = ConvertIdToTdef($1, EMPTY_TQ, GetTypedefType($1)); }
        | type.qualifier.list TYPEDEFname
            { $$ = ConvertIdToTdef($2, $1.tq, GetTypedefType($2)); $$->coord = $1.coord; }
        | typedef.type.specifier type.qualifier
            { $$ = TypeQualifyNode($1, $2.tq, & $2.coord); }
        ;

/*                        */    /* ? */ /* ?.?.? */
type.qualifier.list: /*P*/
          /*attributes.opt*/ type.qualifier /*attributes.opt */
            { /*$$.tq = tq_union($1.tq, $2.tq);*/
	      $$.coord = $1.coord; }
        | type.qualifier.list type.qualifier
            { $$.tq = MergeTypeQuals($1.tq, $2.tq, $2.coord);
              $$.coord = $1.coord; }
        ;

pointer.type.qualifier.list:
          pointer.type.qualifier
        | pointer.type.qualifier.list pointer.type.qualifier
            { $$.tq = MergeTypeQuals($1.tq, $2.tq, $2.coord);
              $$.coord = $1.coord; }
        ;

/*                        */    /* ? */ /* ?.?.? */
elaborated.type.name: /*P*/
          struct.or.union.specifier
        | enum.specifier
        ;

/*                        */    /* ? */ /* ?.?.? */
declarator: /*P*/
          paren.typedef.declarator       /* would be ambiguous as parameter */
        | parameter.typedef.declarator   /* not ambiguous as param          */
        | identifier.declarator
        | old.function.declarator
            {
	      Warning(2, "function prototype parameters must have types");
              $$ = AddDefaultParameterTypes($1);
            }
        | gcc.extension declarator  {  $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Redundant '(' placed immediately to the left of the TYPEDEFname  */
paren.typedef.declarator: /*P*/
          paren.postfix.typedef.declarator
        | '*' attributes.opt paren.typedef.declarator  /* UNUSED attributes.opt */
            { $$ = SetDeclType($3, MakePtrCoord(EMPTY_TQ, NULL, $1), Redecl);
               }
        | '*' attributes.opt '(' simple.paren.typedef.declarator ')'  /* UNUSED attributes.opt */
            { $$ = SetDeclType($4, MakePtrCoord(EMPTY_TQ, NULL, $1), Redecl); 
               }
        | '*' attributes.opt pointer.type.qualifier.list '(' simple.paren.typedef.declarator ')'  /* UNUSED attributes.opt */
            { $$ = SetDeclType($5, MakePtrCoord(   $3.tq,    NULL, $1), Redecl);
               }
        | '*' attributes.opt pointer.type.qualifier.list paren.typedef.declarator  /* UNUSED attributes.opt */
            { $$ = SetDeclType($4, MakePtrCoord(   $3.tq,    NULL, $1), Redecl); 
               }
        ;
        
/*                        */    /* ? */ /* ?.?.? */
/* Redundant '(' to left of TYPEDEFname */
paren.postfix.typedef.declarator: /*P*/ 
          '(' paren.typedef.declarator ')'
            { $$ = $2;  
              }
        | '(' simple.paren.typedef.declarator postfixing.abstract.declarator ')'
            { $$ = ModifyDeclType($2, $3); 
               }
        | '(' paren.typedef.declarator ')' postfixing.abstract.declarator
            { $$ = ModifyDeclType($2, $4); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
simple.paren.typedef.declarator: /*P*/
          TYPEDEFname
            { $$ = ConvertIdToDecl($1, EMPTY_TQ, NULL, NULL, NULL); }
        | '(' simple.paren.typedef.declarator ')'
            { $$ = $2;  
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.typedef.declarator: /*P*/
          TYPEDEFname 
            { $$ = ConvertIdToDecl($1, EMPTY_TQ, NULL, NULL, NULL); }
        | TYPEDEFname postfixing.abstract.declarator
            { $$ = ConvertIdToDecl($1, EMPTY_TQ, $2, NULL, NULL);   }
        | clean.typedef.declarator
        ;

/*
   The  following have at least one '*'. There is no (redundant) 
   '(' between the '*' and the TYPEDEFname. 
*/
/*                        */    /* ? */ /* ?.?.? */
clean.typedef.declarator: /*P*/
          clean.postfix.typedef.declarator
        | '*' attributes.opt parameter.typedef.declarator  /* UNUSED attributes.opt */
            { $$ = SetDeclType($3, MakePtrCoord(EMPTY_TQ, NULL, $1), Redecl); 
               }
        | '*' attributes.opt pointer.type.qualifier.list parameter.typedef.declarator   /* UNUSED attributes.opt */ 
            { $$ = SetDeclType($4, MakePtrCoord($3.tq, NULL, $1), Redecl); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
clean.postfix.typedef.declarator: /*P*/
          '(' clean.typedef.declarator ')'
            { $$ = $2; 
               }
        | '(' clean.typedef.declarator ')' postfixing.abstract.declarator
            { $$ = ModifyDeclType($2, $4); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
abstract.declarator: /*P*/
          unary.abstract.declarator
        | postfix.abstract.declarator
        | postfixing.abstract.declarator
        ;

/*                        */    /* ? */ /* ?.?.? */
unary.abstract.declarator: /*P*/
          '*' attributes.opt  /* UNUSED attributes.opt */
            { $$ = MakePtrCoord(EMPTY_TQ, NULL, $1); }
        | '*' attributes.opt pointer.type.qualifier.list   /* UNUSED attributes.opt */
            { $$ = MakePtrCoord($3.tq, NULL, $1); }
        | '*' attributes.opt abstract.declarator  /* UNUSED attributes.opt */
            { $$ = SetBaseType($3, MakePtrCoord(EMPTY_TQ, NULL, $1)); 
               }
        | '*' attributes.opt pointer.type.qualifier.list abstract.declarator  /* UNUSED attributes.opt */
            { $$ = SetBaseType($4, MakePtrCoord($3.tq, NULL, $1)); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfix.abstract.declarator: /*P*/
          '(' unary.abstract.declarator ')'
            { $$ = $2; 
               }
        | '(' postfix.abstract.declarator ')'
            { $$ = $2; 
               }
        | '(' postfixing.abstract.declarator ')'
            { $$ = $2; 
               }
        | '(' unary.abstract.declarator ')' postfixing.abstract.declarator
            { $$ = SetBaseType($2, $4); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfixing.abstract.declarator: /*P*/
          array.abstract.declarator     { $$ = $1;                   }
        | '(' ')'                       { $$ = MakeFdclCoord(EMPTY_TQ, NULL, NULL, $1); }
        | '(' parameter.type.list ')'   
            { $$ = DemoteProcArgs(MakeFdclCoord(EMPTY_TQ, $2, NULL, $1)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
identifier.declarator: /*P*/
          unary.identifier.declarator
        | paren.identifier.declarator
        ;

/*                        */    /* ? */ /* ?.?.? */
unary.identifier.declarator: /*P293*/
          postfix.identifier.declarator
        | '*' attributes.opt identifier.declarator
            { //printf("%s:%d ", __FILE__, __LINE__); DPL($2);
              SetDeclAttribs($3, $2.tq);
              $$ = ModifyDeclType($3, MakePtrCoord(EMPTY_TQ, NULL, $1)); 
              }
        | '*' attributes.opt pointer.type.qualifier.list identifier.declarator
            { $$ = ModifyDeclType($4, MakePtrCoord(tq_union($2.tq, $3.tq),    NULL, $1)); 
               }
        ;
        
/*                        */    /* ? */ /* ?.?.? */
postfix.identifier.declarator: /*P296*/
          paren.identifier.declarator postfixing.abstract.declarator
            { $$ = ModifyDeclType($1, $2); }
        | '(' unary.identifier.declarator ')'
            { $$ = $2; 
               }
        | '(' unary.identifier.declarator ')' postfixing.abstract.declarator
            { $$ = ModifyDeclType($2, $4); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
paren.identifier.declarator: /*P299*/
          IDENTIFIER
            { $$ = ConvertIdToDecl($1, EMPTY_TQ, NULL, NULL, NULL); }
        | '(' paren.identifier.declarator ')'
            { $$ = $2; 
               }
        | WILDCARD_D
            { if (!AcceptWildcards())
	      {
	        SyntaxError("Wildcard tokens not allowed in program. \n");
              }
	      $$ = ConvertIdToDecl($1, EMPTY_TQ, NULL, NULL, NULL);
	    }
         ;

/*                        */    /* ? */ /* ?.?.? */
old.function.declarator: /*P301*/
          postfix.old.function.declarator
            { 
              $$ = $1; 
            }
        | '*' attributes.opt old.function.declarator /* UNUSED attributes.opt */
            { $$ = SetDeclType($3, MakePtrCoord(EMPTY_TQ, NULL, $1), SU); 
               }
        | '*' attributes.opt pointer.type.qualifier.list old.function.declarator  /* UNUSED attributes.opt */
            { $$ = SetDeclType($4, MakePtrCoord($3.tq, NULL, $1), SU); 
               }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfix.old.function.declarator: /*P*/
          paren.identifier.declarator '(' identifier.list ')'  
            { $$ = ModifyDeclType($1, MakeFdclCoord(EMPTY_TQ, $3, NULL, $2)); }
        | '(' old.function.declarator ')'
            { $$ = $2; 
               }
        | '(' old.function.declarator ')' postfixing.abstract.declarator
            { $$ = ModifyDeclType($2, $4); 
               }
        ;

/* 
    ANSI C section 3.7.1 states  

      "An identifier declared as a typedef name shall not be redeclared 
       as a parameter".  

    Hence the following is based only on IDENTIFIERs 
*/
/*                        */    /* ? */ /* ?.?.? */
identifier.list: /*P*/ /* only used by postfix.old.function.declarator */
          IDENTIFIER
            { $$ = MakeNewList($1); }
        | identifier.list ',' IDENTIFIER
            { $$ = AppendItem($1, $3); }
        ;

/*                        */    /* ? */ /* ?.?.? */
identifier.or.typedef.name: /*P*/
          IDENTIFIER
        | TYPEDEFname
        ;

/*                        */    /* ? */ /* ?.?.? */
type.name: /*P*/
          type.specifier
            { $$ = FinishType($1); }
        | type.specifier abstract.declarator
            { $$ = FinishType(SetBaseType($2, $1)); }
        | type.qualifier.list /* DEFAULT_INT */
            { $$ = MakeDefaultPrimType($1.tq, $1.coord); }
        | type.qualifier.list /* DEFAULT_INT */ abstract.declarator
	    { $$ = SetBaseType($2, MakeDefaultPrimType($1.tq, $1.coord)); }
        | WILDCARD_T { if (!AcceptWildcards())
	               {
			  SyntaxError("Wildcard tokens not allowed in program. \n");
	               }
	               $$ = $1;
	             }
        ;


/* Productions for __attribute__ adapted from the original in gcc 2.6.3. */
attributes.opt:
          /* empty */
  		{ $$.tq = EMPTY_TQ; $$.coord = UnknownCoord; }
	| attributes
  	        { $$ = $1; }
	;

attributes:
	 attribute 
              { $$ = $1; }
       | attributes attribute 
	      {
		  $$.tq = tq_union ($1.tq, $2.tq);
		  $$.coord = $1.coord;
	      }
	;

attribute:
        ATTRIBUTE '(' '(' attribute.list ')' ')'
		{ if (ANSIOnly)
	            SyntaxError("__attribute__ not allowed with -ansi switch");
                  $$.tq = tq_attribute($4); $$.coord=UnknownCoord;
		}

	;

attribute.list:
      attrib
		{ $$ = MakeNewList($1); }
	| attribute.list ',' attrib
		{ $$ = AppendItem($1, $3); }
	;
 
attrib:
    /* empty */
		{ $$ = NULL; }
	| any.word
		{ $$ = ConvertIdToAttrib($1, NULL); }
	| any.word '(' IDENTIFIER ')'
		{ $$ = ConvertIdToAttrib($1, MakeNewList($3)); }
	| any.word '(' IDENTIFIER ',' nonnull.expression.list ')'
                { $$ = ConvertIdToAttrib($1, ConsItem($3, $5)); }
        /* Adding this one creates two shift/reduce conflicts, but it seems necessary to handle the section attributes.*/
        /* The problem is that attrib should match all of the following
         *     aligned(8)         (that is this last rule)
         *     packed             (that is the first rule)
         *     section("shared")  (that is this last rule)
	 * The problem is that the IDENTIFIER, if parsed as an expression, will
	 *     be parsed as an undeclared variable.  (It could even be a type name
	 *     which will be parsed badly too.
         */
	| any.word '(' nonnull.expression.list ')'
                { $$ = ConvertIdToAttrib($1, $3); }
	;

nonnull.expression.list:
          argument.expression.list
             { $$ = $1; }
        ;

/* productions for asm in declarations */
asm.opt:
	  /* empty */
             { $$ = NULL; }    
        | asm
	;
asm:
          ASM '(' string.literal.list ')' 
             { $$ = MakeAsmCoord(EMPTY_TQ, $3, NULL, NULL, NULL, $1); }
;

any.word:
	  IDENTIFIER
	| TYPEDEFname
	| CONST { $$ = MakeIdCoord(UniqueString("const"), $1); } 
	;

/*                        */    /* ? */ /* ?.?.? */
initializer.opt: /*P*/
          /* nothing */                  { $$ = NULL; }
        | '=' initializer                { $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
initializer: /*P*/
	  basic.initializer
        | IDENTIFIER ':' basic.initializer     { $$ = MakeBinopCoord(':', $1, $3, $2); }
        | TYPEDEFname ':' basic.initializer     { $$ = MakeBinopCoord(':', $1, $3, $2); }
        | '.' IDENTIFIER '=' basic.initializer { $$ = MakeBinopCoord(DESIGNATED_INITIALIZER, $2, $4, $3); }
        | '.' TYPEDEFname '=' basic.initializer { $$ = MakeBinopCoord(DESIGNATED_INITIALIZER, $2, $4, $3); }
        | '[' assignment.expression ']' basic.initializer { $$ = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, $2, $4, $3); }
        | '[' assignment.expression ']' '=' basic.initializer { $$ = MakeBinopCoord(ARRAY_LABELED_INITIALIZER, $2, $5, $3); }
        | '[' assignment.expression ELLIPSIS assignment.expression ']' basic.initializer { $$ = MakeBinopCoord(ARRAY_LABELED_INITIALIZER,
														   MakeBinopCoord(ELLIPSIS, $2, $4, $3),
														   $6, $5); }
        ;

basic.initializer:
           bracketed.initializer.list
        | assignment.expression          { $$ = $1; }
        ;

bracketed.initializer.list:
          '{' '}'                        { if (ANSIOnly) SyntaxError("ISO C forbits empty initializer braces"); /* See Bug 83 */
	                                   $$ = MakeInitializerCoord(NULL, $1);
                                         }
        | '{' initializer.list '}'       { $$ = $2; $$->coord = $1; }
        | '{' initializer.list ',' '}'   { $$ = $2; $$->coord = $1; }
        ;

initializer.list: /*P*/
          initializer
            { $$ = MakeInitializerCoord(MakeNewList($1), $1->coord); }
        | initializer.list ',' initializer
            { 
              assert($1->typ == Initializer);
              AppendItem($1->u.initializer.exprs, $3);
              $$ = $1; 
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.type.list: /*P*/
          parameter.list
        | parameter.list ',' ELLIPSIS   { $$ = AppendItem($1, EllipsisNode); }

        /******** ERROR PRODUCTIONS (EAB) ********/
        | ELLIPSIS
            { Node *n = MakePrimCoord(EMPTY_TQ, Void, $1);
	      SyntaxErrorCoord(n->coord, "First argument cannot be `...'");
              $$ = MakeNewList(n);
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.list: /*P*/
          parameter.declaration
            { $$ = MakeNewList($1); }
        | parameter.list ',' parameter.declaration
            { $$ = AppendItem($1, $3); }

        /******** ERROR PRODUCTIONS (EAB) ********/
        | parameter.declaration '=' initializer
            { 
	      SyntaxErrorCoord($1->coord, "formals cannot have initializers");
              $$ = MakeNewList($1); 
            }
        | parameter.list ',' error
            { $$ = $1; }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.declaration: /*P*/
          attributes.opt declaration.specifier
            { $$ = $2; }
        | attributes.opt declaration.specifier abstract.declarator
            { $$ = SetBaseType($3, $2); 
            }
        | attributes.opt declaration.specifier identifier.declarator
            { $$ = SetDeclType($3, $2, Formal); 
            }
        | attributes.opt declaration.specifier parameter.typedef.declarator
            { $$ = SetDeclType($3, $2, Formal); 
            }
        | attributes.opt declaration.qualifier.list /* DEFAULT_INT */ 
            { $$ = MakeDefaultPrimType($2.tq, $2.coord); }
        | attributes.opt declaration.qualifier.list /* DEFAULT_INT */ abstract.declarator
            { $$ = SetBaseType($3, MakeDefaultPrimType($2.tq, $2.coord)); }
        | attributes.opt declaration.qualifier.list /* DEFAULT_INT */ identifier.declarator
            { $$ = SetDeclType($3, MakeDefaultPrimType($2.tq, $2.coord), Formal); }
        | attributes.opt type.specifier
            { $$ = $2; }
        | attributes.opt type.specifier abstract.declarator
            { $$ = SetBaseType($3, $2); 
            }
        | attributes.opt type.specifier identifier.declarator attributes.opt
            {
	      $$ = SetDeclType($3, TypeQualifyNode($2, tq_union($1.tq, $4.tq), NULL), Formal);
	    }
        | attributes.opt type.specifier parameter.typedef.declarator
            { $$ = SetDeclType($3, $2, Formal); 
            }
        | attributes.opt type.qualifier.list /* DEFAULT_INT */ 
            { $$ = MakeDefaultPrimType($2.tq, $2.coord); }
        | attributes.opt type.qualifier.list /* DEFAULT_INT */ abstract.declarator
            { $$ = SetBaseType($3, MakeDefaultPrimType($2.tq, $2.coord)); }
        | attributes.opt type.qualifier.list /* DEFAULT_INT */ identifier.declarator
            { $$ = SetDeclType($3, MakeDefaultPrimType(tq_union($1.tq, $2.tq), $2.coord), Formal); }
        ;

/*                        */    /* ? */ /* ?.?.? */
array.abstract.declarator: /*P*/
          '[' ']'
            { $$ = MakeAdclCoord(EMPTY_TQ, NULL, NULL, $1); }
        | '[' constant.expression ']'
            { $$ = MakeAdclCoord(EMPTY_TQ, NULL, $2, $1); }
        | array.abstract.declarator '[' constant.expression ']'
            { $$ = SetBaseType($1, MakeAdclCoord(EMPTY_TQ, NULL, $3, $2)); }
        | array.abstract.declarator '[' ']'
            { $$ = SetBaseType($1, MakeAdclCoord(EMPTY_TQ, NULL, NULL, $2)); }

        /******** ERROR PRODUCTIONS ********/
/*
        | / error production: catch empty dimension that isn't first /
          array.abstract.declarator '[' ']'
            { 
              SyntaxError("array declaration with invalid empty dimension");
              $$ = SetBaseType($1, MakeAdclCoord(EMPTY_TQ, NULL, SintOne, $2)); 
            }
*/
        ;

/********************************************************************************
*										*
*                      STRUCTURES, UNION, and ENUMERATORS			*
*										*
********************************************************************************/

/*                        */    /* ? */ /* ?.?.? */
struct.or.union.specifier: /*P*/
          struct.or.union '{' struct.declaration.list '}'
            { 
              $$ = SetSUdclNameFields($1, NULL, $3, $2, $4, NULL);
            }
        | struct.or.union identifier.or.typedef.name
          '{' struct.declaration.list '}'
            { 
              $$ = SetSUdclNameFields($1, $2, $4, $3, $5, NULL);
	    }
        | struct.or.union identifier.or.typedef.name
            { 
              $$ = SetSUdclName($1, $2, $1->coord);
	    }
        /* EAB: create rules for empty structure declarations */
        | struct.or.union '{' '}'
            { 
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              $$ = SetSUdclNameFields($1, NULL, NULL, $2, $3, NULL);
	    }
        | struct.or.union identifier.or.typedef.name '{' '}'
            { 
              if (ANSIOnly)
                 Warning(1, "empty structure declaration");
              $$ = SetSUdclNameFields($1, $2, NULL, $3, $4, NULL); 
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.or.union: /*P*/ 
          STRUCT   { $$ = MakeSdclCoord(EMPTY_TQ, NULL, $1); }
        | UNION    { $$ = MakeUdclCoord(EMPTY_TQ, NULL, $1); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaration.list: /*P*/
          attributes.opt struct.declaration 
            { 
              $$ = $2; 
              NodeUpdateTq2(FirstItem($$), tq_union, $1.tq); 
            }
        | struct.declaration.list attributes.opt struct.declaration
            { 
	      NodeUpdateTq2(FirstItem($3), tq_union, $2.tq);
              $$ = JoinLists($1, $3);
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaration: /*P*/
          struct.declaring.list ';'
        | struct.default.declaring.list ';'
        | gcc.extension struct.declaration { $$ = $2; NodeUpdateTq((Node*)(FirstItem($$)), tq_add_extension); ((Node*)(FirstItem($$)))->coord = $1; }
        ;

/* doesn't redeclare typedef */
/*                        */    /* ? */ /* ?.?.? */
struct.default.declaring.list: /*P*/        
          type.qualifier.list struct.identifier.declarator
            { 
	      $$ = MakeNewList(SetDeclType($2,
					    MakeDefaultPrimType($1.tq, $1.coord),
					    SU)); 
	    }
        | struct.default.declaring.list ',' struct.identifier.declarator
            { $$ = AppendDecl($1, $3, SU); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaring.list:  /*P*/       
          type.specifier struct.declarator
            { $$ = MakeNewList(SetDeclType($2, $1, SU)); }
        | type.specifier.nosue attributes struct.declarator
          {
	       SetDeclAttribs($3, $2.tq);
	       $$ = MakeNewList(SetDeclType($3, $1, SU));
	  }
        | struct.or.union '{' struct.declaration.list '}'
            {
		 /* gcc extension: unnamed struct/union fields */
		 Node *a = SetSUdclFields($1, NULL, $3, $2, $4, NULL);
		 Node *d = MakeDeclCoord(NULL, EMPTY_TQ, a, NULL, NULL, a->coord);
		 NodeSetDeclLocation(d, TQ_SU_DECL);
		 $$ = MakeNewList(d);
	    }
        | struct.declaring.list ',' struct.declarator
            { $$ = AppendDecl($1, $3, SU); }
        ;


/*                        */    /* ? */ /* ?.?.? */
struct.declarator: /*P*/
          declarator bit.field.size.opt attributes.opt
            { SetDeclAttribs($1, $3.tq);
              $$ = SetDeclBitSize($1, $2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.identifier.declarator: /*P*/
          identifier.declarator bit.field.size.opt attributes.opt
            { $$ = SetDeclBitSize($1, $2);
              SetDeclAttribs($1, $3.tq); }
        ;

/*                        */    /* ? */ /* ?.?.? */
bit.field.size.opt: /*P*/
          /* nothing */   { $$ = NULL; }
        | bit.field.size
        ;

/*                        */    /* ? */ /* ?.?.? */
bit.field.size: /*P*/
          ':' constant.expression { $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
enum.specifier: /*P*/
          ENUM '{' enumerator.list comma.opt '}'
            { $$ = BuildEnum(NULL, $3, $1, $2, $5); }
        | ENUM identifier.or.typedef.name '{' enumerator.list comma.opt '}'
            { $$ = BuildEnum($2, $4, $1, $3, $6);   }
        | ENUM identifier.or.typedef.name
            { $$ = BuildEnum($2, NULL, $1, $2->coord, $2->coord); }
        ;

/*                        */    /* ? */ /* ?.?.? */
enumerator.list: /*P*/
          identifier.or.typedef.name enumerator.value.opt
            { $$ = MakeNewList(BuildEnumConst($1, $2)); }
        | enumerator.list ',' identifier.or.typedef.name enumerator.value.opt
            { $$ = AppendItem($1, BuildEnumConst($3, $4)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
enumerator.value.opt: /*P*/
          /* Nothing */               { $$ = NULL; }
        | '=' constant.expression     { $$ = $2;   }
        ;

comma.opt: /* not strictly ANSI */
          /* Nothing */    { }
        | ','              { }
        ;

/********************************************************************************
*										*
*                                  STATEMENTS					*
*										*
********************************************************************************/

statement:                      /* P */ /* 6.6   */
          labeled.statement
        | compound.statement
        | expression.statement
        | selection.statement
        | iteration.statement
        | jump.statement
	| gcc.statement
        | cilk.statement
          /******** ERROR PRODUCTIONS ********/
        | error ';'
           {  $$ = NULL; }
        ;

labeled.statement:              /* P */ /* 6.6.1 */
          IDENTIFIER ':'             
           { $$ = BuildLabel($1); }
        | CASE constant.expression ':'
            { $$ = AddContainee(MakeCaseCoord($2, NULL, $1)); }
        | DEFAULT ':'
            { $$ = AddContainee(MakeDefaultCoord(NULL, $1)); }

          /* Required extension to 6.6.1 */
        | TYPEDEFname ':'
            { $$ = BuildLabel($1); }
        ;

compound.statement:             /* P */ /* 6.6.2 */
          lblock rblock
            { $$ = MakeBlockCoord(PrimVoid, NULL, NULL, $1, $2); }
        | lblock declaration.list rblock
            {
		$$ = MakeBlockCoord(PrimVoid, GrabPragmas($2), NULL, $1, $3);
	    }
        | lblock statement.list rblock
            {
		MaybeCheckLabelAtEndOfCompoundStatement($2);
		$$ = MakeBlockCoord(PrimVoid, NULL, GrabPragmas($2), $1, $3);
	    }
        | lblock declaration.list statement.list rblock
            {
		MaybeCheckLabelAtEndOfCompoundStatement($3);
		$$ = MakeBlockCoord(PrimVoid, $2, GrabPragmas($3), $1, $4);
	    }
        /* Repeat above rules for transactional blocks */
        | xlblock xrblock
            { $$ = MakeXBlockCoord(PrimVoid, NULL, NULL, $1, $2); }
        | xlblock declaration.list xrblock
            {
		$$ = MakeXBlockCoord(PrimVoid, GrabPragmas($2), NULL, $1, $3);
	    }
        | xlblock statement.list xrblock
            {
		MaybeCheckLabelAtEndOfCompoundStatement($2);
		$$ = MakeXBlockCoord(PrimVoid, NULL, GrabPragmas($2), $1, $3);
	    }
        | xlblock declaration.list statement.list xrblock
            {
		MaybeCheckLabelAtEndOfCompoundStatement($3);
		$$ = MakeXBlockCoord(PrimVoid, $2, GrabPragmas($3), $1, $4);
	    }
        ;

lblock: '{'  { EnterScope(); } ;
rblock: '}'  { ExitScope(); }  ;

/* Addition for transactions */
xlblock: XBEGIN { EnterScope(); };
xrblock: XEND   { ExitScope();  };

/* compound.statement.no.new.scope is used by function.definition,
   since the new scope is begun with formal argument declarations,
   not with the opening '{' */
compound.statement.no.new.scope:             /* P */ /* 6.6.2 */
          '{' '}'
            { $$ = MakeBlockCoord(PrimVoid, NULL, NULL, $1, $2); }
        | '{' declaration.list '}'
            { $$ = MakeBlockCoord(PrimVoid, GrabPragmas($2), NULL, $1, $3); }
        | '{' statement.list '}'
            {
		MaybeCheckLabelAtEndOfCompoundStatement($2);
		$$ = MakeBlockCoord(PrimVoid, NULL, GrabPragmas($2), $1, $3);
	    }
        | '{' declaration.list statement.list '}'
            {
		MaybeCheckLabelAtEndOfCompoundStatement($3);
		$$ = MakeBlockCoord(PrimVoid, $2, GrabPragmas($3), $1, $4);
	    }
        ;



declaration.list:               /* P */ /* 6.6.2 */
          declaration                   { $$ = GrabPragmas($1); }
        | declaration.list declaration  { $$ = JoinLists(GrabPragmas($1),
                                                         $2); }
        ;

statement.list:                 /* P */ /* 6.6.2 */
          statement                   { $$ = GrabPragmas(MakeNewList($1)); }
        | statement.list statement    { $$ = AppendItem(GrabPragmas($1), 
                                                        $2); }
        ;

expression.statement:           /* P */ /* 6.6.3 */
          expression.opt ';' {
            $$=$1;
	    if ($$ && $$->typ==Block) $$->parenthesized=TRUE; /* Expression statements get their parenthesized bits set, so we can remember that it is an expression statement, rather than just a compound statement */
          }
        ;

selection.statement:            /* P */ /* 6.6.4 */
          IF '(' expression ')' statement
            { $$ = MakeIfCoord($3, $5, $1); }
        | IF '(' expression ')' statement ELSE statement
            { $$ = MakeIfElseCoord($3, $5, $7, $1, $6); }
        | SWITCH { PushContainer(Switch); } '(' expression ')' statement
            { $$ = PopContainer(MakeSwitchCoord($4, $6, NULL, $1)); }
        ;

iteration.statement:            /* P */ /* 6.6.5 */
          WHILE 
            { PushContainer(While);} 
          '(' expression ')' statement
            { $$ = PopContainer(MakeWhileCoord($4, $6, $1)); }
        | DO 
            { PushContainer(Do);} 
          statement WHILE '(' expression ')' ';'
            { $$ = PopContainer(MakeDoCoord($3, $6, $1, $4)); }
        | FOR '(' expression.opt ';' expression.opt ';' expression.opt ')'  
            { PushContainer(For);} 
          statement
            { $$ = PopContainer(MakeForCoord($3, $5, $7, $10, $1)); }

        /******** ERROR PRODUCTIONS (EAB) ********/
        | FOR '(' error ';' expression.opt ';' expression.opt ')'  
            { PushContainer(For);} 
          statement
            { $$ = PopContainer(MakeForCoord(NULL, $5, $7, $10, $1)); }
        | FOR '(' expression.opt ';' expression.opt ';' error ')'  
            { PushContainer(For);} 
          statement
            { $$ = PopContainer(MakeForCoord($3, $5, NULL, $10, $1)); }
        | FOR '(' expression.opt ';' error ';' expression.opt ')'  
            { PushContainer(For);} 
          statement
            { $$ = PopContainer(MakeForCoord($3, NULL, $7, $10, $1)); }
        | FOR '(' error ')' { PushContainer(For);} statement
            { $$ = PopContainer(MakeForCoord(NULL, SintZero, NULL, $6, $1)); }
        ;

jump.statement:                 /* P */ /* 6.6.6 */
          GOTO IDENTIFIER ';'
            { $$ = ResolveGoto($2, $1); }
        | CONTINUE ';'
            { $$ = AddContainee(MakeContinueCoord(NULL, $1)); }
        | BREAK ';'
            { $$ = AddContainee(MakeBreakCoord(NULL, $1)); }
        | RETURN expression.opt ';'
            { $$ = AddReturn(MakeReturnCoord($2, $1)); }

        /* Required extension/clarification to 6.6.6 */
        | GOTO TYPEDEFname ';'
            { $$ = ResolveGoto($2, $1); }
        ;

/* GCC extensions */
asm.argument.list.opt:
            { $$ = NULL; }
        | asm.argument.list
        ;
asm.argument.list:
          asm.argument
            { $$ = MakeNewList($1); }
        | asm.argument.list ',' asm.argument
            { $$ = AppendItem($1, $3); }
        ;
asm.argument:
          string.literal.list '(' assignment.expression ')'
            { $$ = MakeAsmArgCoord($1, $3, $2); }
        ;
asm.clobbered.list:
          string.literal.list
            { $$ = MakeNewList($1); }
        | asm.clobbered.list ',' string.literal.list
            { $$ = AppendItem($1, $3); }
        ;
gcc.statement:
          ASM '(' string.literal.list ')' ';'
            { $$ = MakeAsmCoord(EMPTY_TQ, $3, NULL, NULL, NULL, $1); }
        | ASM '(' string.literal.list ':' asm.argument.list.opt ')' ';'
            { $$ = MakeAsmCoord(EMPTY_TQ, $3, $5, NULL, NULL, $1); }
	| ASM '(' string.literal.list ':' asm.argument.list.opt ':' asm.argument.list.opt ')' ';'
            { $$ = MakeAsmCoord(EMPTY_TQ, $3, $5, $7, NULL, $1); }
	| ASM '(' string.literal.list ':' asm.argument.list.opt ':' asm.argument.list.opt ':' asm.clobbered.list ')' ';'
            { $$ = MakeAsmCoord(EMPTY_TQ, $3, $5, $7, $9, $1); }
	| ASM type.qualifier '(' string.literal.list ')' ';'
            { $$ = MakeAsmCoord($2.tq, $4, NULL, NULL, NULL, $1); }
	| ASM type.qualifier '(' string.literal.list ':' asm.argument.list.opt ')' ';'
            { $$ = MakeAsmCoord($2.tq, $4, $6, NULL, NULL, $1); }
	| ASM type.qualifier '(' string.literal.list ':' asm.argument.list.opt ':' asm.argument.list.opt ')' ';'
            { $$ = MakeAsmCoord($2.tq, $4, $6, $8, NULL, $1); }
	| ASM type.qualifier '(' string.literal.list ':' asm.argument.list.opt ':' asm.argument.list.opt ':' asm.clobbered.list ')' ';'
            { $$ = MakeAsmCoord($2.tq, $4, $6, $8, $10, $1); }
        ;

/********************************************************************************
*										*
*                            EXTERNAL DEFINITIONS                               *
*										*
********************************************************************************/

translation.unit:               /* P */ /* 6.7   */
          external.definition	                  { $$ = GrabPragmas($1); }
        | translation.unit external.definition    { $$ = JoinLists($1, GrabPragmas($2)); }
        ;

external.definition:            /* P */ /* 6.7   */
          declaration
            {
              if (yydebug)
                {
                  printf("external.definition # declaration\n");
                  fPrintNode(stdout, FirstItem($1), 0); 
                  printf("\n\n\n");
		}
              $$ = $1;
            }
        | function.definition  
            { 
              if (yydebug)
                {
                  printf("external.definition # function.definition\n");
                  fPrintNode(stdout, $1, 0); 
                  printf("\n\n\n");
                }
              $$ = MakeNewList($1);
            }
        | gcc.extension external.definition
 	   {  $$ = $2; }
        | /* Nothing */ ';' { $$ = (List *) NULL; }
        ;

function.definition:            /* P */ /* BASED ON 6.7.1 */
          identifier.declarator
            { 
              $1 = DefineProc(FALSE, 
                              SetDeclType($1,
					  MakeDefaultPrimType(EMPTY_TQ, $1->coord),
					  Redecl));
            }
          compound.statement.no.new.scope
            { $$ = SetProcBody($1, $3); }
        | identifier.declarator BOGUS
            /* this rule is never used, it exists solely to force the parser to
	       read the '{' token for the previous rule, thus starting a create
	       scope in the correct place */
/*  Moved to declaration: rule.  Move others???   -KHR
        | declaration.specifier      identifier.declarator
            { $2 = DefineProc(FALSE, SetDeclType($2, $1, Redecl)); }
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | type.specifier             identifier.declarator
            { $2 = DefineProc(FALSE, SetDeclType($2, $1, Redecl)); }
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
*/
        | declaration.qualifier.list identifier.declarator
            { 
	      Node *decl = SetDeclType($2,
				       MakeDefaultPrimType($1.tq, $1.coord),
				       Redecl);
	      $2 = DefineProc(FALSE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | type.qualifier.list        identifier.declarator
            { 
	      Node *decl = SetDeclType($2,
				       MakeDefaultPrimType($1.tq, $1.coord),
				       Redecl);
	      $2 = DefineProc(FALSE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | old.function.declarator
            { 
              $1 = DefineProc(TRUE, 
                              SetDeclType($1,
					  MakeDefaultPrimType(EMPTY_TQ, $1->coord),
					  Redecl));
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($1, $3); }
        | declaration.specifier old.function.declarator 
            {  Node *decl = SetDeclType($2, $1, Redecl);  

               AddParameterTypes(decl, NULL);
               $2 = DefineProc(TRUE, decl);
            }
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | type.specifier old.function.declarator
            { Node *decl = SetDeclType($2, $1, Redecl);

              AddParameterTypes(decl, NULL);
              $2 = DefineProc(TRUE, decl);
            }
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | declaration.qualifier.list old.function.declarator
            { Node *type = MakeDefaultPrimType($1.tq, $1.coord),
                   *decl = SetDeclType($2, type, Redecl);

              AddParameterTypes(decl, NULL);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | type.qualifier.list        old.function.declarator
            { Node *type = MakeDefaultPrimType($1.tq, $1.coord),
                   *decl = SetDeclType($2, type, Redecl);

              AddParameterTypes(decl, NULL);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $4); }
        | old.function.declarator old.function.declaration.list
            { Node *type = MakeDefaultPrimType(EMPTY_TQ, $1->coord),
                   *decl = SetDeclType($1, type, Redecl);
              AddParameterTypes(decl, $2);
              $1 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($1, $4); }
        | declaration.specifier old.function.declarator old.function.declaration.list
            { Node *decl = SetDeclType($2, $1, Redecl);
              AddParameterTypes(decl, $3);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $5); }
        | type.specifier old.function.declarator old.function.declaration.list
            { Node *decl = SetDeclType($2, $1, Redecl);
              AddParameterTypes(decl, $3);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $5); }
        | declaration.qualifier.list old.function.declarator old.function.declaration.list
            { Node *type = MakeDefaultPrimType($1.tq, $1.coord),
                   *decl = SetDeclType($2, type, Redecl);

              AddParameterTypes(decl, $3);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $5); }
        | type.qualifier.list old.function.declarator old.function.declaration.list
            { Node *type = MakeDefaultPrimType($1.tq, $1.coord), 
                   *decl = SetDeclType($2, type, Redecl);
				       

              AddParameterTypes(decl, $3);
              $2 = DefineProc(TRUE, decl);
            } 
          compound.statement.no.new.scope
            { $$ = SetProcBody($2, $5); }
        ;


old.function.declaration.list:
             { OldStyleFunctionDefinition = TRUE; }
             declaration.list
             { OldStyleFunctionDefinition = FALSE; 
               $$ = $2; }
        ;

/********************************************************************************
*										*
*                          CONSTANTS and LITERALS                               *
*										*
********************************************************************************/

/* 
  CONSTANTS.  Note ENUMERATIONconstant is treated like a variable with a type
  of "enumeration constant" (elsewhere)
*/
constant: /*P*/
          FLOATINGconstant      { $$ = $1; }
        | INTEGERconstant       { $$ = $1; }
        | OCTALconstant         { $$ = $1; }
        | HEXconstant           { $$ = $1; }
        | CHARACTERconstant     { $$ = $1; }
        ;

/* STRING LITERALS */
string.literal.list: /*P*/
          STRINGliteral         { $$ = $1; }
        | string.literal.list STRINGliteral
            { const char *first_text  = $1->u.Const.text;
              const char *second_text = $2->u.Const.text;
              int   length = strlen(first_text) + strlen(second_text) + 1;
              char *buffer = HeapNewArray(char, length);
              char *new_text, *new_val;
	
              /* since text (which includes quotes and escape codes)
		 is always longer than value, it's safe to use buffer
		 to concat both */
              strcpy(buffer, NodeConstantStringValue($1));
	      strcat(buffer, NodeConstantStringValue($2));
              new_val = UniqueString(buffer);

              strcpy(buffer, first_text);
	      strcat(buffer, second_text);
              new_text = buffer;
              $$ = MakeStringTextCoord(new_text, new_val, $1->coord);
	     }
        ;

type.qualifier: /*P*/
          CONST     { $$.tq = TQ_CONST;    $$.coord = $1; } 
        | VOLATILE  { $$.tq = TQ_VOLATILE; $$.coord = $1; }
        | INLINE    { $$.tq = TQ_INLINE;   $$.coord = $1; } /* gcc extension */
        | RESTRICT  { $$.tq = TQ_RESTRICT; $$.coord = $1; } /* C9X extension */
        | COMPLEX   { $$.tq = TQ_COMPLEX;  $$.coord = $1; } /* c9x extension */
        | PROCEDURE { $$.tq = TQ_PROCEDURE;$$.coord = $1; } /* Cilk extension */
        | INLET     { $$.tq = TQ_INLET;    $$.coord = $1; } /* Cilk extension */
        | SHAREDVAR { $$.tq = TQ_SHARED;   $$.coord = $1; } /* Cilk extension */
        | PRIVATEVAR{ $$.tq = TQ_PRIVATE;  $$.coord = $1; } /* Cilk extension */
        | NOOUTPUT  { $$.tq = TQ_NOOUTPUT; $$.coord = $1; } /* cilk2c extension */
        | CILKONLY  { $$.tq = TQ_CILKONLY; $$.coord = $1; } /* cilk2c extension */
        | CILKSAFE  { $$.tq = TQ_CILKSAFE; $$.coord = $1; } /* cilk2c extension */
        ;

pointer.type.qualifier: /*P*/
          CONST     { $$.tq = TQ_CONST;    $$.coord = $1; } 
        | VOLATILE  { $$.tq = TQ_VOLATILE; $$.coord = $1; }
        | RESTRICT  { $$.tq = TQ_RESTRICT; $$.coord = $1; } /* C9X extension */
        | PROCEDURE { $$.tq = TQ_PROCEDURE;$$.coord = $1; } /* Cilk extension */
        | INLET     { $$.tq = TQ_INLET;    $$.coord = $1; } /* Cilk extension */
        | CILKONLY  { $$.tq = TQ_CILKONLY; $$.coord = $1; } /* cilk2c extension */
        | CILKSAFE  { $$.tq = TQ_CILKSAFE; $$.coord = $1; } /* cilk2c extension */
        ;

storage.class: /*P*/
          TYPEDEF   { $$.tq = TQ_TYPEDEF;  $$.coord = $1; } 
        | EXTERN    { $$.tq = TQ_EXTERN;   $$.coord = $1; } 
        | STATIC    { $$.tq = TQ_STATIC;   $$.coord = $1; } 
        | AUTO      { $$.tq = TQ_AUTO;     $$.coord = $1; } 
        | REGISTER  { $$.tq = TQ_REGISTER; $$.coord = $1; } 
        ;

basic.type.name: /*P*/
          VOID      { $$ = StartPrimType(Void, $1);    } 
        | CHAR      { $$ = StartPrimType(Char, $1);     } 
        | INT       { $$ = StartPrimType(Int_ParseOnly, $1);     } 
        | FLOAT     { $$ = StartPrimType(Float, $1);   } 
        | DOUBLE    { $$ = StartPrimType(Double, $1);  } 

        | SIGNED    { $$ = StartPrimType(Signed, $1);  } 
        | UNSIGNED  { $$ = StartPrimType(Unsigned, $1);} 

        | SHORT     { $$ = StartPrimType(Short, $1);   } 
        | LONG      { $$ = StartPrimType(Long, $1);    }
        ;

/* Cilk extensions */
/* postfix expressions without function calls */
spawn.postfix.expression:            
          primary.expression
        | spawn.postfix.expression '[' expression ']'
            { $$ = ExtendArray($1, $3, $2); }
        | spawn.postfix.expression '.' IDENTIFIER
            { $$ = MakeBinopCoord('.', $1, $3, $2); }
        | spawn.postfix.expression ARROW IDENTIFIER
            { $$ = MakeBinopCoord(ARROW, $1, $3, $2); }
        | spawn.postfix.expression ICR
            { $$ = MakeUnaryCoord(POSTINC, $1, $2); }
        | spawn.postfix.expression DECR
            { $$ = MakeUnaryCoord(POSTDEC, $1, $2); }

          /* EXTENSION: TYPEDEFname can be used to name a struct/union field */
        | spawn.postfix.expression '.'   TYPEDEFname
            { $$ = MakeBinopCoord('.', $1, $3, $2); }
        | spawn.postfix.expression ARROW TYPEDEFname
            { $$ = MakeBinopCoord(ARROW, $1, $3, $2); }
        ;

spawn.procedure:
          spawn.postfix.expression
           { $$ = OmitLookups() ? $1 : LookupPostfixExpression($1);}
        ;

spawn.args:
        /* Nothing */              { $$ = (List *) NULL; }
        | argument.expression.list { $$ = $1; }
        ;


cilk.statement:
/* spawn, no receiver */
          SPAWN spawn.procedure '(' spawn.args ')' ';' 
              { $$ = MakeSpawnCoord(NULL, '=', $2, $4, FALSE, $1); }

/* spawn, receiver */
        | cast.expression assignment.operator SPAWN spawn.procedure
            '(' spawn.args ')' ';' 
              { $$ = MakeSpawnCoord($1, $2->u.binop.op, $4, $6, FALSE, $3); }

/* sync */
        | SYNC ';'
             { $$ = MakeSyncCoord($1); }

/* inlet, no receiver */
        | postfix.expression '(' inlet.call.argument.expression.list ')' ';'
             { $$ = MakeInletCallCoord((OmitLookups() ? $1 : LookupPostfixExpression($1)), $3, $2); }

/* inlet, receiver
        | cast.expression assignment.operator postfix.expression
            '(' inlet.call.argument.expression.list ')' ';'
             { $$ = MakeInletCallCoord($1, (OmitLookups() ? $3 : LookupPostfixExpression($3)), $5, FALSE, $4); }
 */

        | ABORT ';'
            { $$ = MakeAbortCoord($1); }
        ;

inlet.call.argument.expression.list:
          SPAWN spawn.procedure '(' spawn.args ')'
              { $$ = MakeNewList(MakeSpawnCoord(NULL, '=', $2, $4, TRUE, $1)); }
        | SPAWN spawn.procedure '(' spawn.args ')' ',' argument.expression.list
              { $$ = ConsItem(MakeSpawnCoord(NULL, '=', $2, $4, TRUE, $1), $7); }
        ;

cilk.expression:
          SYNCHED
              { $$ = MakeSynchedCoord($1); }
        ;
/* end Cilk extensions */

/* gcc extensions */
gcc.extension:
	EXTENSION
	;

%%
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
