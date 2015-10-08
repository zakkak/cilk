
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *
 *************************************************************************/
/*
 * Copyright (c) 1994-2002 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
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
 *
 */

#ifndef _BASICS_H_
#define _BASICS_H_
#include "../runtime/cilk-sysdep.h"
FILE_IDENTITY(basics_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/basics.h $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
/* We also need ../config.h for things like inline */
#include "../config.h"

#ifdef NO_PROTOTYPES
extern int fprintf(FILE *, const char *,...);
/* extern int      fputc(char, FILE*); */
extern int fputs(const char *, FILE *);
extern FILE *fopen(const char *, const char *);
extern int fclose(FILE *);
extern int printf(const char *,...);
extern int sscanf(const char *, const char *,...);
extern int _flsbuf(unsigned char, FILE *);
/* extern void     bcopy(char *, char *, int); */
extern int toupper(int);
extern const void *memcpy(void *, const void *, size_t);
extern int fflush(FILE *);
#endif

#define GLOBAL
#define PRIVATE static

/* for generic prototypes (use Generic *) */
typedef void Generic;
typedef void **GenericREF;	/* address of a Generic, for pass by reference */

typedef int Bool;
#define TRUE 1
#define FALSE 0

/* assertion checking */
#undef assert
#ifdef NDEBUG
 #define assert(x) ((void) 0)
#else
 #define assert(x)  ((x) ? (void)0 : (void)Fail(__FILE__, __LINE__, #x))
#endif

#define FAIL(str) \
  Fail(__FILE__, __LINE__, str)
#define UNREACHABLE FAIL("UNREACHABLE")

#define PLURAL(i) (((i) == 1) ? "" : "s")
#define TABSTRING "    "

/* Compiling the linux kernel hit this limit when it was 256.*/
#define MAX_FILES       1024
#define MAX_FILENAME    200
#define MAX_SCOPE_DEPTH 100
#define MAX_OPERATORS   600

/* Basic Typedefs */

typedef struct nodeStruct Node;
typedef struct tablestruct SymbolTable;
typedef int OpType;

typedef struct coord {
     int line;
     short offset;
     short file;
} Coord;

GLOBAL extern Coord UnknownCoord,DontPrintCoord;
#define IsUnknownCoord(coord)  ((coord).file == UnknownCoord.file)
#define IsDontPrintCoord(coord)  ((coord).file == DontPrintCoord.file)

#define PRINT_COORD(out, c) \
    { if (PrintLineOffset) \
      fprintf(out,"%s:%d:%d", FileNames[(c).file], (int)(c).line, \
              (int)(c).offset); \
    else fprintf(out, "%s:%d", FileNames[(c).file], (int) (c).line); }

#define REFERENCE(var)  ((var)->u.decl.references++)
#define VAR_NAME(var)   ((var)->u.decl.name)

static inline void minf_coord (Coord *x, Coord *y) {
    if (x->file!=y->file) {            /*nothing*/  /* If the files are not the same, return the first coord.  This case is just hard to get right.*/
    } else if (x->line<y->line) {      /*nothing*/
    } else if (x->line>y->line) {      *x=*y;
    } else if (x->offset<=y->offset) { /*nothing*/
    } else {                             *x=*y;
    }
}

/* Prototypes/definitions from other files */

#include "heap.h"
#include "list.h"
#include "symbol.h"
#include "gbuf.h"

/* Basic Global Variables */

GLOBAL extern const float VersionNumber;     /* main.c */
GLOBAL extern const char *const VersionDate; /* main.c */
GLOBAL extern const char * Executable;       /* program name, main.c */
GLOBAL extern List *Program;                 /* main.c */
GLOBAL extern int WarningLevel;              /* main.c */
GLOBAL extern int Line, LineOffset, Errors, Warnings;    /* warning.c */
GLOBAL extern unsigned int CurrentFile;      /* c4.l: current file number */
GLOBAL extern char *Filename;                /* c4.l */
GLOBAL extern char *FileNames[MAX_FILES];    /* c4.l: file # to name mapping*/
GLOBAL extern const char *PhaseName;         /* main.c */
/*GLOBAL extern const char *InputFileRoot;*/     /* main.c */
GLOBAL extern Bool FileIncludedp[MAX_FILES]; /* c4.l */
GLOBAL extern Bool CurrentIncludedp;         /* c4.l */
GLOBAL extern Bool GenerateSegments;         /* main.c */

/* nondeterminator support in cilk2c. */
GLOBAL extern Bool MemoryChecking;           /* main.c */
GLOBAL extern Bool OptimizeChecking;         /* main.c */
GLOBAL extern Bool MemoryCheckedp;           /* main.c */
GLOBAL extern Bool MemoryWarningp;           /* main.c */

/* ANSI defines the following name spaces (K&R A11.1, pg 227): */
GLOBAL extern SymbolTable *Identifiers, *Labels, *Tags;

/* This table is used to ensure consistency across the translation unit */
GLOBAL extern SymbolTable *Externals;

/* Global Flags */
GLOBAL extern Bool DebugLex;                 /* main.c */
GLOBAL extern Bool PrintLineOffset;          /* main.c */
GLOBAL extern Bool IgnoreLineDirectives;     /* main.c */
GLOBAL extern Bool ANSIOnly;                 /* main.c */
GLOBAL extern Bool FormatReadably;           /* main.c */
GLOBAL extern Bool PrintLiveVars;            /* main.c */
GLOBAL extern Bool Transform;                /* main.c */  /* Probably should be called Transform_Cilk_to_C */
GLOBAL extern Bool PrintASTcoords;           /* main.c */

/* Basic Global Procedures */

/* pretty-printing */
GLOBAL void DPN(Node *n);
GLOBAL void DPL(List *list);
GLOBAL void PrintNode(GBUF *out, Node *node, int tab_depth);
#define fPrintNode(file,node,tab_depth) WITH_FILE_GBUF(gb,file,PrintNode(gb,node,tab_depth))
GLOBAL int  print_float(GBUF *, float);
GLOBAL int  print_double(GBUF *, double);
GLOBAL void PrintCRSpaces(GBUF *out, int spaces);
GLOBAL void PrintSpaces(GBUF *out, int spaces);
GLOBAL void PrintList(GBUF *out, List *list, int tab_depth);
#define fPrintList(file,l,tab_depth) WITH_FILE_GBUF(gb,file,PrintList(gb,l,tab_depth))
GLOBAL int PrintOp(GBUF *, OpType op);	/* operators.c */
GLOBAL void CharToText(char *array, unsigned char value);
GLOBAL inline int PrintChar(GBUF *out, int c);	/* print.c */
GLOBAL int PrintString(GBUF *out, const char *string);	/* print.c */

/* warning.c */
GLOBAL NoReturn(Fail(const char *file, int line, const char *msg));
GLOBAL void SyntaxError(const char *fmt, ...);
GLOBAL void Warning(int level, const char *fmt, ...);
GLOBAL void Hint(const char *fmt,...);
GLOBAL void SyntaxErrorCoord(Coord c, const char *fmt, ...);
GLOBAL void WarningCoord(int level, Coord c, const char *fmt, ...);
GLOBAL void HintCoord(Coord c, const char *fmt,...);

/* parsing phase */
GLOBAL int yyparse(void), yylex(void);
GLOBAL char *UniqueString(const char *string);	/* strings.c */
GLOBAL void SetFile(const char *filename, int line, const char *language);	/* c4.l */

/* verification */
GLOBAL void VerifyParse(List *program);		/* verify-parse.c */

/* semantic check -- sem-check.c */
GLOBAL List *SemanticCheckProgram(List *program);
GLOBAL Node *SemCheckNode(Node *node);
GLOBAL List *SemCheckList(List *list);

/* hints phase -- hint.c */
GLOBAL List *HintProgram(List *program);

/* Simplify phase simplify.c */
GLOBAL List *SimplifyProgram(List *program);

/* Profile phase profile.c */
GLOBAL List *ProfileProgram(List *program);

/* Side effects computations sideeffects.c */
GLOBAL Bool SideeffectsNode(Node*);

/* transform phase -- transform.c */
GLOBAL List *TransformProgram(List *program);
GLOBAL List *ElideProgram(List *program);
GLOBAL List *LinkProgram(List *program, const char *filename_roots[], int n);

/* output phase -- output.c */
GLOBAL void OutputProgram(FILE *out, List *program);

GLOBAL void OutputStmtList(FILE *outfile, List *myStmtList);

GLOBAL void output_constant_to_GBUF(GBUF *, Node *, Bool /*with_name*/);
GLOBAL void output_prim_type_to_GBUF(GBUF *gb, Node *type, char *lastc);
#endif				/* ifndef _BASICS_H_ */
