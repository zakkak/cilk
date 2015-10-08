/* -*- c-basic-offset: 5 -*- */
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *
 *************************************************************************/
/*
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
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
#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/main.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include <errno.h>
#include "stringParse.h"
#include "testEquals.h"
#include "atmTransform.h"

#ifndef NO_POPEN
#ifdef NO_PROTOTYPES
extern FILE *popen(const char *, const char *);
extern int pclose(FILE * pipe);
#endif
#endif

/* Set to 1 to enable parser debugging.  Also requires YACC flags */
extern int yydebug;

GLOBAL const char *Executable;
GLOBAL const char *const CompileDate = __DATE__;
GLOBAL const char *PhaseName = "???";

#define CPP_FLAG_LIMIT 2048
#define MAX_LINK_FILES  200

GLOBAL int WarningLevel = WARNING_LEVEL;                   /* values 1-5 are valid; 5 == all */

GLOBAL List *Program;

GLOBAL extern FILE *yyin;	/* file pointer used by the lexer */
PRIVATE const char *input_file = NULL;
PRIVATE const char *output_file = NULL;
PRIVATE const char *input_file_name = NULL; /* the name of the input file, as printed in messages.*/
PRIVATE const char *input_language = NULL;  /* The input language.  If non-null overrides the suffix in input_file_name or input_file. */

/* global flags */
GLOBAL Bool QuietlyIgnore = FALSE;
GLOBAL Bool DebugLex = FALSE;
GLOBAL Bool PrintSymTables = FALSE;
GLOBAL Bool TrackInsertSymbol = FALSE;
GLOBAL Bool PrintAST = FALSE;
GLOBAL Bool PrintASTcoords = FALSE;
GLOBAL Bool PrintLineOffset = FALSE;
GLOBAL Bool IgnoreLineDirectives = FALSE;
GLOBAL Bool ANSIOnly = FALSE;
GLOBAL Bool PrintLiveVars = FALSE;

GLOBAL Bool Parse = TRUE;
GLOBAL Bool SemanticCheck = TRUE;
GLOBAL Bool Analyze = TRUE;
GLOBAL Bool DoHint = FALSE;
GLOBAL Bool DoSimplify = FALSE;
GLOBAL Bool DoProfile = FALSE;
GLOBAL Bool Transform = TRUE;
GLOBAL Bool GenerateOutput = TRUE;
GLOBAL Bool FormatReadably = FALSE;
GLOBAL Bool GenerateSegments = FALSE;

GLOBAL Bool Elide = FALSE;

/* Nondeterminator support */
GLOBAL Bool MemoryChecking = FALSE;
GLOBAL Bool OptimizeChecking = FALSE;
GLOBAL Bool MemoryCheckedp = FALSE;
GLOBAL Bool MemoryWarningp = TRUE;

/* Support for Atomic Transactions */
GLOBAL Bool DoAtomicTransform = FALSE;


/* Command line option for testing
   AST manipulation/wildcard code
*/
GLOBAL Bool TestWildcardMode = FALSE;
GLOBAL Bool UserPromptMode = FALSE;

PRIVATE void print_version_info(FILE * out, const char *pre)
{
     fprintf(out, "%s%s (compiled %s)\n",
	     pre, ident, CompileDate);
     /*exit(0);*/   /* Don't exit with -v.  */
}

PRIVATE void print_copyright(FILE *out, const char *pre)
{
     static const char *lines[] = {
	  "Copyright (c) 1994-2002 MIT Laboratory for Computer Science\n",
	  "Copyright (c) 2000 Matteo Frigo\n",
	  "Copyright (c) 2002 Matteo Frigo\n",
	  "Portions Copyright (c) 1989, 1990 James A. Roskind\n",
	  NULL
     };
     int i;

     for (i = 0; lines[i] != NULL; i++) {
	  fputs(pre, out);
	  fputs(lines[i], out);
     }
     exit(0);
}

PRIVATE void usage(Bool print_all_options, int exitcode)
{
     fprintf(stderr, "Usage: %s [options] [file]\n", Executable);

     fprintf(stderr,
	     "\n"
	     "Translates Cilk 5.3 source file and writes it to output\n"
	     "If [file] is omitted, uses standard input.\n"
	     "\n");

     fprintf(stderr, "General Options:\n");
     fprintf(stderr,
	     "\t-help              Print this description\n");
     fprintf(stderr,
	     "\t-options           Print all options\n");
     fprintf(stderr,
	     "\t-copy              Print the copyright information\n");
     fprintf(stderr,
	     "\t-v                 Print version information\n");

     if (print_all_options) {
	  fprintf(stderr, "Phase Options:\n");
	  fprintf(stderr,
		  "\t-nosem             Don't semantic-check\n");
	  fprintf(stderr,
		  "\t-noanalyze         Don't perform dataflow analysis\n");
	  fprintf(stderr,
		  "\t-notrans           Don't transform syntax tree from Cilk to a C program with calls to the cilk runtime system.  This means the input must be C code.\n");
	  fprintf(stderr,
		  "\t-elide             Transform the Cilk program to a serial C program by eliding the Cilk keywords\n");
	  fprintf(stderr,
		  "\t-noprint           Don't write C output\n");

	  fprintf(stderr, "Output Options:\n");
	  fprintf(stderr,
		  "\t-o <name>          Write C output to <name> (if no -o is provided, writes to stdout)\n");
	  fprintf(stderr,
		  "\t-N                 Don't emit line directives\n");
	  fprintf(stderr,
                  "\t-nd                produce nondeterminator code.  Not tested lately.\n");

	  fprintf(stderr, "Warning Options:\n");
	  fprintf(stderr,
		  "\t-ansi              Disable GCC extensions and undefine __GNUC__\n");
	  fprintf(stderr,
		  "\t-W<n>              Set warning level; <n> in 1-5. Default=%d\n",
		  WARNING_LEVEL);
	  fprintf(stderr,
		  "\t-Wall              Same as -W5\n");
	  fprintf(stderr,
		  "\t-il                Ignore line directives (use actual line numbers)\n");
	  fprintf(stderr,
		  "\t-offset            Print offset within the line in warnings/errors\n");
	  fprintf(stderr,
		  "\t-name <x>          Use <x> as filename in messages, in #line directives, and in determining the source language.\n"
		  "\t                   Note:  If the name of the input ends in .cilk or .cilkh the file is parsed as Cilk, otherwise as C.\n");
	  fprintf(stderr,
		  "\t-x <lang>          Use <lang> as the input language.  The options are `c' and `cilk'.\n"
		  "\t                   -x <lang> overrides the suffix given by -name.\n");
	  fprintf(stderr,
		  "\t-hint              Run the hint phase to suggest potential problems parallelising the code.\n");
	  fprintf(stderr,
		  "\t-simplify          Simplify the C code (removing some gcc-isms, for example) (Implied by -profile).\n");
	  fprintf(stderr,
		  "\t-profile           Insert profiling code (for loads and stores.\n");
	  fprintf(stderr, "Debugging Options:\n");
	  fprintf(stderr,
		  "\t-lex               Show lexical tokens\n");
	  fprintf(stderr,
		  "\t-yydebug           Track parser stack and actions\n");
	  fprintf(stderr,
		  "\t-insert            Track symbol creation\n");
	  fprintf(stderr,
		  "\t-sym               Print out symbol tables after parse\n");
	  fprintf(stderr,
		  "\t-ast               Print out syntax tree (after last phase)\n");
	  fprintf(stderr,
		  "\t-ast-coords        Print out syntax tree (after last phase) and also print coordinates (file, line, and offset)\n");
	  fprintf(stderr,
		  "\t-live              Print live variables as cmts in C output\n");
	  fprintf(stderr,
		  "\t-testUser          Interactive test mode for parsing wildcard strings \n");
	  fprintf(stderr,
		  "\t-testWC            Test mode for parsing wildcard strings \n");
	  fprintf(stderr,
		  "\t-atomic            Do transformation for atomic code \n");
	  fprintf(stderr, "CC Compatibility Options:\n");
	  fprintf(stderr,
		  "\t--                 Toggles ignoring of unknown options\n"
		  "\t                   (for makefile compatibility with cc)\n");
	 

	  fprintf(stderr, "\n");
     }
     exit(exitcode);
}

PRIVATE void unknown_option(char *option)
{
     if (!QuietlyIgnore) {
	  fprintf(stderr, "Unknown option: `%s'\n\n", option);
	  usage(FALSE, 1);
     }
}

/***********************************************************************\
 * Handle command-line arguments
\***********************************************************************/

PRIVATE void handle_options(int argc, char *argv[])
{
     int i;

     for (i = 1; i < argc; i++) {
	  if (argv[i][0] == '-') {
	       switch (argv[i][1]) {
		   case '-':
			if (strcmp(argv[i], "--") == 0) {
			     QuietlyIgnore = !QuietlyIgnore;
			} else {
			     unknown_option(argv[i]);
			}
			break;
		   case 'a':
			if (strcmp(argv[i], "-ansi") == 0) {
			     ANSIOnly = TRUE;
			} else if (strcmp(argv[i], "-ast") == 0) {
			     PrintAST = TRUE;
			} else if (strcmp(argv[i], "-ast-coords") == 0) {
			     PrintAST = TRUE;
			     PrintASTcoords = TRUE;
			} else if (strcmp(argv[i], "-atomic") == 0) {
			     printf("Planning to do atomic transform \n");
			     DoAtomicTransform = TRUE;
			} else {
			     unknown_option(argv[i]);
			}
			break;

		   case 'N':
			FormatReadably = TRUE;
			break;
		   case 'W':
			if (strcmp(argv[i], "-Wall") == 0) {
			     WarningLevel = 5;
			} else {
			     int c = atoi(&argv[i][2]);
			     if (c < 0 || c > 5) {
				  unknown_option(argv[i]);
			     } else {
				  WarningLevel = c;
			     }
			}
			break;
		   case 'c':
			if (strcmp(argv[i], "-copy") == 0)
			     print_copyright(stderr, "");
			else
			     unknown_option(argv[i]);
			break;
	           case 'e':
			if (strcmp(argv[i], "-elide") == 0) {
			     printf("eliding\n");
			     Transform = FALSE;  /* -elide implies -notrans */
			     Elide = TRUE;
			} else
			     unknown_option(argv[i]);
			break;
 	           case 'h':
		        if (strcmp(argv[i], "-hint") == 0)
			     DoHint = TRUE;
			else if (strcmp(argv[i], "-h") == 0)
			     usage(FALSE, 0);
			else
			     unknown_option(argv[i]);
			break;
		   case 'i':
			if (strcmp(argv[i], "-insert") == 0) {
			     TrackInsertSymbol = TRUE;
			} else if (strcmp(argv[i], "-il") == 0) {
			     IgnoreLineDirectives = TRUE;
			} else
			     unknown_option(argv[i]);
			break;
		   case 'l':
			if (strcmp(argv[i], "-lex") == 0)
			     DebugLex = TRUE;
			else if (strcmp(argv[i], "-live") == 0)
			     PrintLiveVars = TRUE;
			else
			     unknown_option(argv[i]);
			break;
		   case 'n':
			if (strcmp(argv[i], "-name") == 0) {
			     i++;
			     if (i>=argc) { fprintf(stderr, "Missing argument\n"); usage(FALSE, 1); }
			     
			     if (input_file_name != NULL) {
				  fprintf(stderr,
					  "Multiple input files names defined, using `%s'\n",
					  input_file_name);
			     } else {
				  input_file_name = argv[i];
			     }
			} else if (strcmp(argv[i], "-nosem") == 0) {
			     SemanticCheck = FALSE;
			} else if (strcmp(argv[i], "-notrans") == 0) {
			     Transform = FALSE;
			} else if (strcmp(argv[i], "-noprint") == 0) {
			     GenerateOutput = FALSE;
			} else if (strcmp(argv[i], "-noanalyze") == 0) {
			     Analyze = FALSE;
			} else if (strcmp(argv[i], "-nd") == 0) {
                             MemoryCheckedp = MemoryChecking = TRUE;
			} else if (strcmp(argv[i], "-ndopt") == 0) {
			     OptimizeChecking = TRUE;
			} else
			     unknown_option(argv[i]);
			break;
		   case 'o':
			if (strcmp(argv[i], "-o") == 0) {
			     i++;
			     if (i>=argc) { fprintf(stderr, "Missing argument\n"); usage(FALSE, 1); }
			     output_file = argv[i];
			} else if (strcmp(argv[i], "-offset") == 0)
			     PrintLineOffset = TRUE;
			else if (strcmp(argv[i], "-options") == 0) {
			     usage(TRUE, 0);
			     exit(0);
			} else
			     unknown_option(argv[i]);
			break;
		   case 'p':
			if (strcmp(argv[i], "-profile") ==0)
			     DoProfile = TRUE;
			else
			     unknown_option(argv[i]);
			break;
		   case 's':
			if (strcmp(argv[i], "-sym") == 0)
			     PrintSymTables = TRUE;
			else if (strcmp(argv[i], "-simplify") == 0)
			     DoSimplify = TRUE;
			else 
			     unknown_option(argv[i]);
			break;
	           case 't':
			if (strcmp(argv[i], "-testWC") == 0)
			     TestWildcardMode = TRUE;
			else if (strcmp(argv[i], "-testUser") == 0)
			     UserPromptMode = TRUE;
			else
			     unknown_option(argv[i]);
			break;
		   case 'v':
			if (strcmp(argv[i], "-v") == 0)
			     print_version_info(stderr, "");
			else
			     unknown_option(argv[i]);
			break;
	           case 'x':
			if (strcmp(argv[i], "-x") == 0) {
			     i++;
			     if (i>=argc) { fprintf(stderr, "Missing argument\n"); usage(FALSE, 1); }
			     input_language = argv[i];
			} else {
			     unknown_option(argv[i]);
			}
			break;
		   case 'y':
			if (strcmp(argv[i], "-yydebug") == 0)
			     yydebug = 1;
			else
			     unknown_option(argv[i]);
			break;
		   default:
			unknown_option(argv[i]);
	       }
	  } else {
	       if (input_file != NULL) {
		    fprintf(stderr, "Multiple input files defined, using `%s'\n",
			    argv[i]);
	       }
	       input_file = argv[i];
	  }
     }

}

/***********************************************************************\
 * ANSI C symbol tables
\***********************************************************************/

GLOBAL SymbolTable *Identifiers, *Labels, *Tags, *Externals;

PRIVATE void shadow_var(Node *create, Node *shadowed)
{
     /* the two are equal only for redundant function/extern declarations */
     if (create != shadowed && WarningLevel == 5) {
	  WarningCoord(5, create->coord,
		  "`%s' shadows previous declaration", VAR_NAME(create));
	  fprintf(stderr, "\tPrevious declaration: ");
	  PRINT_COORD(stderr, shadowed->coord);
	  fputc('\n', stderr);
     }
}

PRIVATE void init_symbol_tables(Bool shadow_warnings)
{
     ShadowProc shadow_proc;

     if (shadow_warnings)
	  shadow_proc = (ShadowProc) shadow_var;
     else
	  shadow_proc = NULL;

     Identifiers = NewSymbolTable("Identifiers", Nested,
				shadow_proc, (ExitscopeProc) OutOfScope);
     Labels = NewSymbolTable("Labels", Flat,
			     NULL, (ExitscopeProc) EndOfLabelScope);
     Tags = NewSymbolTable("Tags", Nested,
	    shadow_warnings ? (ShadowProc) ShadowTag : (ShadowProc) NULL,
			   NULL);
     Externals = NewSymbolTable("Externals", Flat,
				NULL, (ExitscopeProc) OutOfScope);
}


/***********************************************************************\
 * Determine input file
\***********************************************************************/

PRIVATE FILE *get_input(void)
{
     if (input_file_name) {
	  SetFile(input_file_name, 0, input_language);
     } else if (input_file) {
	  SetFile(input_file, 0, input_language);
     } else {
	  SetFile("/dev/stdin", 0, input_language);
     }
     if (input_file) {
	  FILE *result=fopen(input_file, "r");
	  if (!result) {
	       fprintf(stderr, "Could not open file %s (%s)", input_file, strerror(errno));
	  }
	  return result;
     } else {
	  return stdin;
     }
}

/***********************************************************************\
 * Main
\***********************************************************************/

GLOBAL int main(int argc, char *argv[])
{
     Bool parsed_bad;

     Executable = argv[0];

     handle_options(argc, argv);

     InitTypes();
     init_symbol_tables(TRUE);
     InitOperatorTable();

     if (TestWildcardMode) {
	  
	  RunAllTests();
	  return Errors;
     }
     if (UserPromptMode) {
	  UserPromptParseTest();
	  return Errors;
     }

     /* The usual mode of operation */
	  
     if (Parse) {
	  PhaseName = "Parsing";
	  yyin = get_input();
	  if (!yyin) { Errors++; goto done;}
	  parsed_bad = yyparse();

	  if (Level != 0) {
	       SyntaxError("unexpected end of file");
	  }
	  
	  if (parsed_bad) { assert(Errors>0); } /* if this is wrong, then we didn't notice an error. */
     } else
	  Program = NULL;

     if (PrintSymTables) {
	  PrintSymbolTable(stdout, Externals);
     }
#ifndef NDEBUG
     if (Errors == 0) {
	  PhaseName = "Verification";
	  VerifyParse(Program);
     }
#endif

     PhaseName = "Semantic Check";
     if (Errors == 0 && SemanticCheck)
	  Program = SemanticCheckProgram(Program);

     if (DoSimplify || DoProfile) {
	  /* We must simplify if we are doing profiling.*/
	  Program = SimplifyProgram(Program);
     }

     PhaseName = "Analyze";
     if (Errors == 0 && Analyze)
	  AnalyzeProgram(Program);


     if (DoAtomicTransform) {
	  printf("This is where we would do the atomic transform \n");
	  /*  Better do another semantic-check after we do this ? */

	  printf("Doing pre-simplify. \n");
	  Program = PreSimplifyList(Program);
	  printf("Finished Pre-Simplify...\n");
	  printf("Doing AtmTransform \n");
	  Program = AtomicTransformProgram(Program);
	  printf("Finished ATM transform. \n");
	       
	  /* Program = SemanticCheckProgram(Program);*/
     }

     PhaseName = "Hint";
     if (Errors == 0 && DoHint)
	  HintProgram(Program);

     PhaseName = "Profile";
     if (Errors == 0 && DoProfile) {
	  Program = ProfileProgram(Program);
     }

     assert(!(Transform && Elide));   /* If doing Elide then we should have !Transform.*/
     PhaseName = "Transform";
     if (Errors == 0 && Transform) {
	  Program = TransformProgram(Program);
     }
     PhaseName = "Elide";
     if (Errors == 0 && Elide) {
	  Program = ElideProgram(Program);
     }
     PhaseName = "Output";
     if (Errors == 0 && GenerateOutput) {
	  FILE *fp;
	  if (output_file == NULL) {
	       fp = stdout;
	  } else {
	       if ((fp = fopen(output_file, "w")) == NULL) {
		    printf("Unable to open \"%s\" for output file\n", output_file);
		    Errors++;
		    goto done;
	       }
	  }
	  if (!FormatReadably) {
	       if (input_file_name) fprintf(fp, "# 1 \"%s\"\n", input_file_name);
	       else if (input_file) fprintf(fp, "# 1 \"%s\"\n", input_file);
	  }
	  OutputProgram(fp, Program);
	  /* If the output is not to stdout, then close fp. */
	  if (output_file != NULL) fclose(fp);
     }

 done:
     if (Errors > 0) {
	  fprintf(stderr, "\nCompilation Failed: %d error%s, %d warning%s\n",
		  Errors, PLURAL(Errors),
		  Warnings, PLURAL(Warnings));
     } else if (Warnings > 0) {
	  if (WarningLevel >= 5)
	       fprintf(stderr, "\nCompilation Successful (%d warning%s)\n",
		       Warnings,
		       PLURAL(Warnings));
     }
     if (PrintAST) {
	  fPrintList(stdout, Program, -1);
	  fprintf(stdout, "\n");
     }
     /* cleanup */

     return (Errors);
}
