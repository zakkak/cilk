/* A cilk compiler driver that does not depend on gcc. */
/* use it like you would use gcc.
 * How cilk gets compiled:
 *    foo.cilk  -> foo.cilki  run the preprocessor  (e.g., "gcc -E")  [call this cpp1]
 *    foo.cilki -> foo.cilkc  run cilk2c
 *    foo.cilkc -> foo.i      run cpp again (use gcc -E)
 *    *.i *.s *.o *.a -> foo.s *.o *.a      run gcc
 * You may link  in c functions too
 *    foo.c     -> foo.S      run gcc
 *  note that gcc may skip producing foo.S or foo.o if you invoke it properly. 
 *  We let gcc take care of everything after converting foo.cilk to foo.i
 *
 * Note: Some arguments are compiler specific.
 *    
 * Arguments
 *   *.cilk                   treat as a cilk file
 *   *.cilki                  treat as a cilk file that has been preprocessed
 *   *.cilkc                  treat as a C file that has been produced by cilk2c
 *   *.c *.o *.i other files  treat as the C compielr would
 *   -x <language>            like gcc -x (accept "cilk" or anything that gcc accepts, e.g., "c")  (Probably only works with gcc)
 *   -o <fname>               name of output file
 *   -E0                      stop after preprocessing the cilk (to get .cilki)
 *   -E1                      stop after transforming cilk to c (to get .cilkc)
 *   -E                       stop when all preprocessing is done (when you get a .i file)
 *   -S                       stop when assembly files are produced
 *   -c                       stop when .o files are produced
 *   -D* -A* -include -I      give to cpp
 *   -W*                      give to compiler invocations (usually these are gcc-style warnings.
 *                            however -Wl,* -Wa,* -Wc,* etc are treated specialy, see below.
 *   -save-temps              save the temporary files (also call them foo.i and foo.s instead of /tmp/cXF*xc.i)
 *   -cilk-debug
 *   -cilk-profile
 *   -cilk-span
 *   -cilk-hint
 *   -M -MM                   make dependency rules instead of compiling (-MM is gcc-specific)
 *   -g                       generate debugging information
 *   -O*                      optimize (e.g., -O2, just like gcc)
 *   -f*                      optimization arguments (gcc-specific)
 *   -m*                      machine-specific options (gcc-specific)
 *   --param <argument>       Passed to the compiler (at Matteo's suggestion:  Bug 183)
 *   -woff <argument>         turn off warnings (mipspro C specific)
 *   -Wl,* -l* -L* -static    linker options (passed without modification, e.g., including the "-Wl,".)
 *   -Wp,*                    preprocessor options (works for gcc and mipspro)
 *   -Wa,*                    assembler options (works for gcc and mipspro)
 *   -Wc,*                    C compiler options.  The -Wc, is stripped off.
 *   -ansi                    pass -ansi to the C compiler (works for gcc and mipspro)
 *                            (Note: cilk2c no longer adds non-ansi constructs.)
 *   -pedantic                pass -pedantic to C compiler (the gcc invocations (but Bug 125 did not fix everything.  In particular, nested functions still go through.)
 *   -cc <compiler>           use a different compiler.  E.g. "-cc /usr/bin/cc"
 * 
 * These restrictions come from gcc:
 *  There is no provision for filenames that start with -.
 *  There is no way to treat a file name named foo.c as a .o file or a .a file. 
 *
 * The implementation strategy:
 *  First make a pass to find out what we are doing:
 *   Look for -E0 -E1 -E -S -c -M -MM
 *  Next depending on what we saw, go through and do stuff
 */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"

#include "make-temp-file.h"

#define CILK_H_LOCATION CILKHEADER_DIR "/cilk.h"
#define CILK2C_LOCATION CILK2C_DIR "/cilk2c"

/* the relative path to get from gcc libraries to MipsPro libraries */
/* No longer used!   Delete this. */
/* #define MIPSPRO_LIBS_REL_PATH "/../mipslib" */


static void cleanup (void);
static int match (const char *s1, const char *s2);
static int tailmatch (const char *s1, const char *tail);


static void barf (const char *format, ...) __attribute__((noreturn)) __attribute__((format (printf, 1, 2)));

static int match (const char *s1, const char *s2) {
    return strcmp(s1,s2)==0;
}
static int tailmatch (const char *s1, const char *tail) {
    int l1=strlen(s1);
    int l2=strlen(tail);
    if (l1<l2) return 0;
    return strcmp(s1+l1-l2, tail)==0;
}
static int headmatch (const char *s1, const char *head) {
    int l1=strlen(s1);
    int l2=strlen(head);
    if (l1<l2) return 0;
    return strncmp(s1, head, l2)==0;
}

static void barf (const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    cleanup();
    exit(1);
}

typedef struct my_argv {
    unsigned int argc, limit;
    char const**argv;
} *MY_ARGV;

static MY_ARGV make_my_argv(void) {
    MY_ARGV result = malloc(sizeof(*result));
    enum { INITIAL_LIMIT = 5 };
    result->argc=0;
    result->limit = INITIAL_LIMIT;
    result->argv = malloc(INITIAL_LIMIT*sizeof(char*));
    result->argv[0]=0; /* Always leave a null at the end. */
    /* printf("size=%d argv=%p\n", INITIAL_LIMIT*sizeof(char*), result->argv); */
    return result;
}
static void free_my_argv(MY_ARGV ma) {
    if (ma) {
	free(ma->argv);
	free(ma);
    }
}

static void pusharg(MY_ARGV ma, const char *s) {
    if (ma->argc+1>=ma->limit) { /* Always leave one extra for the NULL at the end. */
	ma->limit*=2;
	ma->argv=realloc(ma->argv, ma->limit*sizeof(char*));
	/* printf("new size is %d, ptr is %p\n", ma->limit*sizeof(char*), ma->argv); */
    }
    ma->argv[ma->argc++] = s;
    ma->argv[ma->argc] = 0;
    assert(ma->argc<=ma->limit);
}
static void pushargs(MY_ARGV to_ma, const MY_ARGV from_ma) {
    unsigned int i;
    for (i=0; i<from_ma->argc; i++) {
	pusharg(to_ma, from_ma->argv[i]);
    }
}
static void print_my_argv(FILE *f, MY_ARGV ma, const char *fout) {
    unsigned int i;
    for (i=0; i<ma->argc; i++) {
	if (i>0) fprintf(f, " ");
	fprintf(f, "%s", ma->argv[i]);
    }
    assert(ma->argv[ma->argc]==0);
    assert(ma->argc+1<=ma->limit);
    if (fout) {
	fprintf(f, " > %s\n", fout);
    }
    fprintf(f, "\n");
}

/****** stringlist ******/

typedef struct stringlist *STRINGLIST;
struct stringlist {
    const char *first;
    STRINGLIST  rest;
};

static void pushstring (STRINGLIST *sl, const char *s) {
    STRINGLIST n=malloc(sizeof(*n));
    assert(n);
    n->first=s;
    n->rest =*sl;
    *sl = n;
}

typedef struct pairlist *PAIRLIST;
struct pairlist {
    const char *a;
    const char *b;
    PAIRLIST    rest;
};

static void pushpair (PAIRLIST *sl, const char *a, const char *b) {
    PAIRLIST n=malloc(sizeof(*n));
    assert(n);
    n->a = a;
    n->b = b;
    n->rest =*sl;
    *sl = n;
}


/**********************************************************\
 * Utilities to understand the -x args and file suffixes. *
\**********************************************************/


enum LANG {
    L_none, L_cilk, L_cilki, L_cilkc, L_c, L_m, L_cpp, L_h, L_i, L_ii, L_s, L_S
};

enum LANG language_of_xarg (const char *xarg) {
    if (match(xarg, "none")) return L_none;
    else if (match(xarg, "cilk")) return L_cilk;
    else if (match(xarg, "cilki")) return L_cilki;
    else if (match(xarg, "cilkc")) return L_cilkc;
    else if (match(xarg, "c")) return L_c;
    else if (match(xarg, "objective-c")) return L_m;
    else if (match(xarg, "c++")) return L_cpp;
    else if (match(xarg, "c-header")) return L_h;
    else if (match(xarg, "cpp-output")) return L_i;
    else if (match(xarg, "c++cpp-output")) return L_ii;
    else if (match(xarg, "assembler")) return L_s;
    else if (match(xarg, "assembler-with-cpp")) return L_S;
    else barf("What is this language? %s", xarg);
}

const char *xarg_of_lang (enum LANG l) {
    switch (l) {
    case L_none: return "none";
    case L_cilk: return "cilk";
    case L_cilki: return "cilki";
    case L_cilkc: return "cilkc";
    case L_c: return "c";
    case L_m: return "objective-c";
    case L_cpp: return "c++";
    case L_h: return "c-header";
    case L_i: return "cpp-output";
    case L_ii: return "c++cpp-output";
    case L_s: return "assembler";
    case L_S: return "assembler-with-cpp";
    }
    barf("This shouldn't happen: %s:%d", __FILE__, __LINE__);
}

enum LANG language_of_file(enum LANG latest_xarg, const char *fname) {
    if (latest_xarg!=L_none) return latest_xarg;
    else if (tailmatch(fname, ".cilk")) return L_cilk;
    else if (tailmatch(fname, ".cilki")) return L_cilki;
    else if (tailmatch(fname, ".cilkc")) return L_cilkc;
    else if (tailmatch(fname, ".c")) return L_c;
    else if (tailmatch(fname, ".m")) return L_m;
    else if (tailmatch(fname, ".C")
	     || tailmatch(fname, ".cc")
	     || tailmatch(fname, ".cxx")
	     || tailmatch(fname, ".cpp")) return L_cpp;
    else if (tailmatch(fname, ".h")) return L_h;
    else if (tailmatch(fname, ".i")) return L_i;
    else if (tailmatch(fname, ".ii")) return L_ii;
    else if (tailmatch(fname, ".s")) return L_s;
    else if (tailmatch(fname, ".S")) return L_S;
    else return L_none;
}


/******************************************\
 * Variables gotten from parsing the args *
\******************************************/

/* filled in by parse_args1 */
enum stope { STOP_AT_CILKI, /* stop after running cpp on cilk sources*/
	     STOP_AT_CILKC, /* stop after running cilk2c */
	     STOP_AT_I,     /* stop after cpp on cilkc and cpp on .c files (-E to gcc) */
	     STOP_AT_S,     /* stop after assembly. */
	     STOP_AT_O,     /* stop after generating the .o file (i.e., pass -c to gcc */
	     STOP_AT_M,     /* print dependencies only. */
	     STOP_AT_END } stop_at = STOP_AT_END;

const char *last_stop_at = 0;
static int verbose=0;
static int do_print_version = 0;
static int do_print_help = 0;
static int cilk_profile=0, cilk_debug=0, cilk_critical_path=0, cilk_hint=0;
static int save_temps = 0; /* true if we saw -save-temps */
static const char *stop_at_m_arg=0;
static int do_nothing = 0;

/* These are filled in by parse_args2 */ 
MY_ARGV cilkc_args=0;   /* Additional arguments to pass to cilkc */
MY_ARGV cc_early_args=0;  /* Additional arguments to pass to gcc if we are stopping early (-S or -c) and just compiling a .i file */
MY_ARGV cc_args=0;      /* All the arguments that will be passed to the big final call to cc */
MY_ARGV cpp1_args=0;    /* All the arguments, except for the name of the .cilk input file and the name of the output file, to be passed to the first preprocessor */

PAIRLIST i_files=0;     /* These are only the .i files produced by the cilk->cilki->cilkc->i chain
			 * And only if we are producing an output
			 * For the i_files the first elt is the .i file and the second is output file. */
PAIRLIST cilkc_files=0; /* the first elt of each pair is the .cilkc file, the second (if any) is .i file */
PAIRLIST cilki_files=0; /* the first elt of the pair is the .cilki file, the second (if any) is the .cilkc file */
PAIRLIST cilk_files=0;  /* the first elt of the pair is the .cilk file, the second (if any) is the .cilki file */

int n_gcc_files=0;      /* How many files are sent to gcc. */
int n_input_files_that_can_make_one_output_file=0; /* How many input files produce a unique output file */

const char *output_name=0;

/* This is modified as the arguments are parsed */
enum LANG current_input_x = L_none; /* What is the most recent -x argument seen? */
enum LANG current_output_x = L_none; /* What is the most recent -x argument stuffed into the gcc arglist */

#define VERBOSE(level,expr) { if (verbose>=(level)) (expr); }

/* 
 * the compiler to be used for the last compilation phase, when it not gcc.
 * for now, assume it is MipsPro compiler.
 */
static const char*   compiler;

static const char* get_cc ()
{
  if (compiler == NULL) {
    return CILKC_PTHREAD_CC;
  } else {
    return compiler;
  }
}

/****** temp files ******/
static STRINGLIST all_tempfiles, more_tempfiles_to_delete;

/* new_suffix includes the '.' */
static const char* replace_suffix (const char *filename, const char *new_suffix) {
    char *enda = strrchr(filename, '.');
    if (enda==0) {
	int flen = strlen(filename);
	int len = flen + strlen(new_suffix) + 1;
	char *result = malloc(len);
	strcpy(result, filename); /* This won't overflow. */
	strcpy(result+flen, new_suffix); /* Nor will this */
	return result;
    } else {
	int flen = enda-filename;
	int len = flen + strlen(new_suffix) + 1;
	char *result = malloc(len);
	strncpy(result, filename, flen);
	strcpy(result+flen, new_suffix);
	return result;
    }
}

/* suffix includes the `.` */
static const char * newtempfile (const char *suffix, const char *org_filename) {
    const char *fname;
    if (save_temps) {
	fname = replace_suffix(org_filename, suffix);
    } else {
	fname = make_temp_file(suffix);
	pushstring(&all_tempfiles, fname);
    }
    return fname;
}

void note_stop_at (enum stope sa, const char *arg) {
    if (stop_at!=STOP_AT_END) barf("%s incompatible with %s", last_stop_at,arg);
    stop_at = sa;
}

/* parse_args figures out where we are stopping, and also what order-independent flags have been set. */
void parse_args1 (int argc, const char *argv[]) {
    argc--; argv++;
    while (argc>0) {
      /*printf("argv = %s\n", *argv);*/
	if (match(*argv, "-E0"))      note_stop_at(STOP_AT_CILKI, *argv);
	else if (match(*argv, "-E1")) note_stop_at(STOP_AT_CILKC, *argv);
	else if (match(*argv, "-E"))  note_stop_at(STOP_AT_I, *argv);
	else if (match(*argv, "-c"))  note_stop_at(STOP_AT_O, *argv);
	else if (match(*argv, "-S"))  note_stop_at(STOP_AT_S, *argv);
	else if (match(*argv, "-v") || match(*argv, "-v2")) {
	    verbose=2;
	} else if (match(*argv,"-v1")) {
	    verbose=1;
	} else if (match(*argv, "-cilk-profile")) {
	    cilk_profile=1;
	} else if (match(*argv, "-cilk-debug")) {
	    cilk_debug=1;
	} else if (match(*argv, "-cilk-span") || 
		   match(*argv, "-cilk-critical-path")) {
	    cilk_critical_path=1;
	    cilk_profile=1;
	} else if (match(*argv, "-cilk-hint")) {
	    cilk_hint=1;
	} else if (match(*argv, "-save-temps")) {
	    save_temps=1;
	} else if (match(*argv, "-M") ||
		   match(*argv, "-MM")) {
	  /* These only to cpp, and they stop after doing the preprocessing */
	    note_stop_at(STOP_AT_M, *argv);
	    stop_at_m_arg=*argv;
	} else if (match(*argv, "-o")) {
	    if (argc==1) barf("Missing arg to -o");
	    argc--; argv++;
	    if (output_name!=0) barf("Two output files specified");
	    output_name=*argv;
	} else if (match(*argv, "-n")) {
	    do_nothing = 1;
	}
	argc--; argv++;
    }
}

void note_i_file (const char *fname, const char *org_fname) {
    const char *suffix=0;
    /*const char *speced=0;*/
    switch (stop_at) {
    case STOP_AT_M: /* .i files from cilk2c have no dependencies */
    case STOP_AT_CILKI:
    case STOP_AT_CILKC:
    case STOP_AT_I: break;
    case STOP_AT_S: suffix=".s"; /*speced="-S";*/ goto earlystop;
    case STOP_AT_O: suffix=".o"; /*speced="-c";*/ goto earlystop;
    earlystop:
	{
	    const char *outfname = output_name ? output_name : replace_suffix(org_fname, suffix);
	    pushpair(&i_files, fname, outfname);
	}
	break;
    case STOP_AT_END:
	if (current_output_x!=L_i && current_output_x!=L_none) {
	    pusharg(cc_args, "-x");
	    pusharg(cc_args, xarg_of_lang(L_i));
	    current_output_x = L_i;
	}
	n_gcc_files++;
	pusharg(cc_args, fname);
	break;
    }
}

void note_cilkc_file (const char *fname, const char *org_fname) {
    switch (stop_at) {
    case STOP_AT_M:       /* M files don't have any dependencies. */
    case STOP_AT_CILKI:
    case STOP_AT_CILKC: return;
    case STOP_AT_I:
	{
	    const char *outfname = output_name ? output_name : replace_suffix(org_fname, ".i");
	    pushpair(&cilkc_files, fname, outfname);
	}
	break;   
    default:
	{
	    const char *outfname = newtempfile(".i", fname);
	    pushpair(&cilkc_files, fname, outfname);
	    note_i_file(outfname, org_fname);
	}
	break;
    }
}
void note_cilki_file (const char *fname, const char *org_fname) {
    switch (stop_at) {
    case STOP_AT_M:
	pushpair(&cilki_files, fname, 0);
	return;
    case STOP_AT_CILKI:
        return;
    default:
	{
	    const char *outfname = newtempfile(".cilkc", fname);
	    pushpair(&cilki_files, fname, outfname);
	    note_cilkc_file(outfname, org_fname);
	}
	return;
    }
}

void note_cilk_file (const char *fname, const char *org_fname) {
    switch (stop_at) {
    case STOP_AT_M:
	pushpair(&cilk_files, fname, 0);
	return;
    default:
	{
	    const char *outfname = newtempfile(".cilki", fname);
	    pushpair(&cilk_files, fname, outfname);
	    note_cilki_file(outfname, org_fname);
	}
	return;
    }
}

/* return 0 on success */
void parse_args2 (int argc, const char *argv[]) {
    int n_input_files = 0;
    cilkc_args = make_my_argv();
    cc_early_args = make_my_argv();
    cc_args = make_my_argv();
    cpp1_args = make_my_argv();
    argc--; argv++;
    while (argc>0) {
      /*printf("argv=%s\n", *argv);*/
      if (match(*argv, "-cc")) {
	  if (argc == 1) 
	      barf("Missing arg to -cc");
	  argc--; argv++;
	  compiler = *argv;
      } else if (
	   match(*argv, "-E0")
	   || match(*argv, "-E1")
	   || match(*argv, "-E")
	   || match(*argv, "-c")
	   || match(*argv, "-S")
	   || match(*argv, "-v")
	   || match(*argv, "-v2")
	   || match(*argv,"-v1")
	   || match(*argv, "-cilk-profile")
	   || match(*argv, "-cilk-debug")
	   || match(*argv, "-cilk-span")
	   || match(*argv, "-cilk-critical-path")
	   || match(*argv, "-cilk-hint")
	   || match(*argv, "-save-temps") /* leave temp files (*.cilkc, *.cilki, *.i) generated from *.cilk */
	   || match(*argv, "-M")   /* generate Make rules */
	   || match(*argv, "-MM")
	   || match(*argv, "-n")
	   ) {
	    /* nothing, since we already parsed those.*/ 
	} else if (match(*argv, "-o")) {
	    /* nothing, since we already parsed those.*/ 
	    argv++; argc--;
	} else if (match(*argv, "-I")) {
	    /* Pass this and the next argument to the preprocessor (which means also cc at the end). */
	    if (argc==1) barf("Missing arg to %s", *argv);
	    pusharg(cpp1_args, *argv);
	    pusharg(cc_args, *argv);
	    argc--; argv++;
	    pusharg(cpp1_args, *argv);
	    pusharg(cc_args, *argv);
	} else if (match(*argv, "--param")) {
	    /* Pass this and the next argument to the compiler. */
	    if (argc==1) barf("Missing arg to %s", *argv);
	    pusharg(cc_args, *argv);
	    argc--; argv++;
	    pusharg(cc_args, *argv);
	} else if (headmatch(*argv,"-D") ||
		   headmatch(*argv,"-A")  || /* assertions for preprocessor */
		   headmatch(*argv,"-include")  ||
		   headmatch(*argv,"-I") ||
		   headmatch(*argv,"-Wp,")) { /* pass options through the driver to preprocessor */
	    /* Pass these to the preprocessor. */
	    pusharg(cpp1_args, *argv);
	    pusharg(cc_args, *argv);
	} else if (match(*argv, "-l") ||
		   match(*argv, "-L")
		   ) {
	    if (argc==1) barf("Missing arg to %s", *argv);
	    pusharg(cc_args, *argv);
	    argc--; argv++;
	    pusharg(cc_args, *argv);
	} else if (match(*argv, "-g") ||         /* generate debug info */
		   headmatch(*argv, "-O") ||     /* optimization level */
		   match(*argv, "-ansi") ||
		   match(*argv, "-pedantic")
		   ) {
	    pusharg(cc_args, *argv);
	    pusharg(cc_early_args, *argv);
	} else if (match(*argv, "-woff")) { /* -woff is MIPSPRO specific */
	    if (argc == 1) barf("Missing arg to -woff");
	    argc--; argv++;
	    pusharg(cc_args, "-woff");
	    pusharg(cc_args, *argv);
	    pusharg(cc_early_args, "-woff");
	    pusharg(cc_early_args, *argv);
	} else if (headmatch(*argv, "-Wa,") || /* extra arguments to pass to the assembler (gcc specific) */
		   headmatch(*argv, "-Wp,") || /* extra arguments to pass to the preprocessor (gcc specific) */
		   headmatch(*argv, "-m") || /* -m is gcc specific */
		   headmatch(*argv, "-f")) { /* -f is gcc specific */
	    /* All of these are simply passed through without modification */
	    pusharg(cc_early_args, *argv);
	    pusharg(cc_args,       *argv);
	    pusharg(cpp1_args,     *argv);
	} else if (headmatch(*argv, "-Wc,")){  /* extra arguments to pass to the C compiler (works with any compiler by stripping of the -Wc, */
	    pusharg(cc_early_args, (*argv)+4);
	    pusharg(cc_args,       (*argv)+4);
	    pusharg(cpp1_args,     (*argv)+4);
	} else if (headmatch(*argv, "-Wl,") || /* linker options bypassing compiler driver */
		   headmatch(*argv, "-l") || /* if it was "-l foo.a" then the previous case matched, otherwise it is "-lfoo.a" here. */
		   headmatch(*argv, "-L") ||
		   match(*argv, "-static")
		   ) {
	    pusharg(cc_args, *argv);
	} else if (match(*argv, "-W") || match(*argv, "-Wall")) {
	    pusharg(cilkc_args, "-W5");
	    /* gcc-specific */
	    pusharg(cc_early_args, *argv);
	    pusharg(cc_args, *argv);
	    /* printf("pushed %s to cc_args\n", *argv); */
	} else if (headmatch(*argv, "-W")) {
	    if (strlen(*argv)==2 && '1'<=*argv[1] && *argv[1] <= '5') {
		pusharg(cilkc_args, *argv);
	    } else {
		 /* gcc-specific */
		 pusharg(cc_early_args, *argv);
		 pusharg(cc_args, *argv);
	    }
	} else if (match(*argv, "-x")) {
	    if (compiler != NULL) {
		fprintf(stderr, "Warning:  -x probably doesn't work except with gcc.  I'll try anyway\n");
	    }
	    if (argc==1) barf("Missing arg to -x");
	    /* Don't actually push the -x args to the gcc arglist until we get a file. */
	    argc--; argv++;
	    current_input_x = language_of_xarg(*argv);
	} else if (match(*argv, "-version")) {
	    do_print_version=1;
	} else if (match(*argv, "-help")) {
	    do_print_help=1;
	} else if (**argv!='-') { /* It is a filename */
	    int finish_up = (stop_at==STOP_AT_END) || (stop_at==STOP_AT_M);
	    enum LANG lang_of_this_arg = language_of_file(current_input_x, *argv);

	    n_input_files++;

	    switch (lang_of_this_arg) {
	    case L_cilk: 
	    case L_cilki:
	    case L_cilkc:
	    case L_c:
	    case L_m:
	    case L_cpp:
	    case L_S:
	      /*
	       * cilk, cilki, cilkc, c, m, and cc files produce a separate output unless we finish
	       * also assembler-with-cpp produces an output unless we finish
	       */
	      if (!finish_up) n_input_files_that_can_make_one_output_file++; /* cilk files produce a separate output unless we finish */
		break;
	    case L_h:
	      break;  /* .h files never produce a separate output */
	    case L_i:
	    case L_ii:
	      /* .i and .ii files produce an output if we do -S or -c */
		if (stop_at==STOP_AT_S || stop_at==STOP_AT_O) n_input_files_that_can_make_one_output_file++;
		break;
	    case L_s:
	      /* .s files produce an output if we do -c */
		if (stop_at==STOP_AT_O) n_input_files_that_can_make_one_output_file++;
		break;
	    case L_none:
	      /* .o files and other random files produce no separate output */
		break;
	    }
	    /**/;
	    switch (lang_of_this_arg) {
	    case L_cilk:  note_cilk_file(*argv, *argv);  break;
	    case L_cilki: note_cilki_file(*argv, *argv); break;
	    case L_cilkc: note_cilkc_file(*argv, *argv); break;
	    case L_c:
	    case L_m:
	    case L_cpp:
	    case L_h:
	    case L_i:
	    case L_ii:
	    case L_s:
	    case L_S:
	    case L_none:
		if ((lang_of_this_arg==current_output_x)) {
		    /* don't need to emit anything if last emitted -x arg matches the language */
		} else if (current_output_x==L_none && lang_of_this_arg==language_of_file(L_none, *argv)) {
		    /* don't need to emit anthing if we emitted -x none but the suffix tells us the right answer */
		} else {
		    pusharg(cc_args, "-x");
		    pusharg(cc_args, xarg_of_lang(lang_of_this_arg));
		    current_output_x = lang_of_this_arg;
		}
		n_gcc_files++;
		pusharg(cc_args, *argv);
		break;
	    }
	} else {
	    barf("What is this? %s", *argv);
	}
	argc--; argv++;
    }
    
    if (output_name && (n_input_files_that_can_make_one_output_file>1))
	barf("You specified an output file, but more than one input file produces an output file in this mode");

    if (n_input_files==0 && !do_print_version && !do_print_help)
	barf("You specified no input files");

    pusharg(cpp1_args, "-I" CILKHEADER_DIR );
#   ifdef CILK_RTSBUILD_DIR
    pusharg(cpp1_args, "-I" CILK_RTSBUILD_DIR );
#   endif
    pusharg(cpp1_args, "-D__CILK__");
    pusharg(cpp1_args, "-D__CILK2C__");
    pusharg(cpp1_args, "-D__REENTRANT");
    if (cilk_debug)   pusharg(cpp1_args, "-DCILK_DEBUG=1");
    if (cilk_profile) {
	pusharg(cpp1_args, "-DCILK_STATS=1");
	pusharg(cpp1_args, "-DCILK_TIMING=1");
    }
    if (cilk_critical_path) {
	if (!cilk_profile) {
	    barf("-cilk-span requires -cilk-profile");
	}
	pusharg(cpp1_args, "-DCILK_CRITICAL_PATH=1");
    }
    if (cilk_debug) {
	pusharg(cpp1_args, "-DCILK_DEBUG=1");
    }
}

/* can't handle quoted spaces */
void push_stringified_args (MY_ARGV argv, const char *sargs)
{
     char *CF=strdup(sargs);
     char *b, *e;
     char old;

     b = CF;

     for (;;) {
	  while (*b == ' ')
	       ++b;

	  e = b;
	  while (*e != ' ' && *e != 0)
	       ++e;

	  old = *e; *e = 0;
	  if (b != e) 
	       pusharg(argv, strdup(b));
	  *e = old;

	  if (*e == 0) break;
	  b = e + 1;
     }
     free(CF);
}



static int my_system(MY_ARGV ma,
		     const char *write_to_file_for_stdout /* if NULL then do nothing to stdout, otherwise open the file and write to it. */
		     ) {
    pid_t child;
    int status;
    if (do_nothing) return 0;
    if ((child=fork())==0) {
	if (write_to_file_for_stdout) {
	    int fout = creat(write_to_file_for_stdout, S_IRUSR | S_IWUSR);
	    assert(fout>=0);
	    dup2(fout, 1);
	    close(fout);
	}
	execvp(ma->argv[0], (char **)ma->argv);
	abort();
    }
    waitpid(child, &status, 0);
    /* printf("status=%d exited=%d\n", status, WIFEXITED(status)); */
    return !(WIFEXITED(status) && WEXITSTATUS(status)==0);
}

void do_M (void) {
    if (cilk_files) {
	/* Create dependencies for cilk programs . */
	MY_ARGV ma = make_my_argv();
	push_stringified_args(ma, get_cc());
	pusharg(ma, stop_at_m_arg);
	pusharg(ma, "-xc");
	pusharg(ma, "-include");
	pusharg(ma, CILK_H_LOCATION);
	{
	    PAIRLIST cf;
	    for (cf=cilk_files; cf; cf=cf->rest) {
		assert(cf->b==0);
		pusharg(ma, cf->a);
	    }
	}
	pushargs(ma, cpp1_args);
	push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1, print_my_argv(stderr, ma, 0));
	if (my_system(ma, 0)!=0)
	    barf("creating dependencies for cilk files");
	free_my_argv(ma);
    }
    if (n_gcc_files>0) {
	MY_ARGV ma = make_my_argv();
	push_stringified_args(ma, get_cc());
	pusharg(ma, stop_at_m_arg);
	pushargs(ma, cc_args);
	push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1, print_my_argv(stderr, ma, 0));
	if (my_system(ma, 0)!=0)
	    barf("creating dependencies for cilk files");
	free_my_argv(ma);
    }
}

/*
 *
 */
void do_cpp1(void) {
    PAIRLIST cf;
    for (cf=cilk_files; cf; cf=cf->rest) {
	MY_ARGV ma = make_my_argv();
	push_stringified_args(ma, get_cc());
	pusharg(ma, "-E");
	pusharg(ma, "-xc");
	pusharg(ma, "-include");
	pusharg(ma, CILK_H_LOCATION);
	pusharg(ma, cf->a);

	assert(cf->b);
	pushargs(ma, cpp1_args);

	push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1, print_my_argv(stderr, ma, cf->b));
	if (my_system(ma, cf->b)!=0)  barf("failed running cpp1");
	free_my_argv(ma);
    }
}

void do_cilk2c(void) {
    PAIRLIST cf;
    for (cf=cilki_files; cf; cf=cf->rest) {
	MY_ARGV ma = make_my_argv();
	pusharg(ma, CILK2C_LOCATION);
	pusharg(ma, cf->a);
	assert(cf->b);
	pusharg(ma, "-o");
	pusharg(ma, cf->b);
	if (cilk_hint) pusharg(ma, "-hint");
	pushargs(ma, cilkc_args);
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1,print_my_argv(stderr, ma, 0));
	if (my_system(ma, 0)!=0) barf("failed running cilk2c");
	free_my_argv(ma);
    }
}

void do_cpp2(void) {
    PAIRLIST cf;
    for (cf=cilkc_files; cf; cf=cf->rest) {
	MY_ARGV ma = make_my_argv();
	push_stringified_args(ma, get_cc());
	pusharg(ma, "-E");
	pusharg(ma, "-I" CILKHEADER_DIR);
#       ifdef CILK_RTSBUILD_DIR
	pusharg(cpp1_args, "-I" CILK_RTSBUILD_DIR );
#       endif
	pusharg(ma, "-D_REENTRANT");
	pusharg(ma, "-x");
	pusharg(ma, "c");
	pusharg(ma, cf->a);
	assert(cf->b);

	push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1,print_my_argv(stderr, ma, cf->b));
	if (my_system(ma, cf->b)!=0) barf("failed running cpp2");
	free_my_argv(ma);
    }
}

void do_cc(void) {
    if (i_files) {
	PAIRLIST pl;
	/* The i_files variable was only filled in if we are stoping after -S or -c */
	assert(stop_at==STOP_AT_S || stop_at==STOP_AT_O);
	for (pl=i_files; pl; pl=pl->rest) {
	    MY_ARGV ma = make_my_argv();
	    push_stringified_args(ma, get_cc());
	    pusharg(ma, pl->a);
	    pusharg(ma, "-o");
	    pusharg(ma, pl->b);
	    pushargs(ma, cc_early_args);
	    if (stop_at==STOP_AT_S) pusharg(ma, "-S");
	    else if (stop_at==STOP_AT_O) pusharg(ma, "-c");
	    else abort(); /* This cannot happen.  See the assert() above. */
	    push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	    if (save_temps) pusharg(ma, "-save-temps");
	    VERBOSE(2, pusharg(ma, "-v"));
	    VERBOSE(1, print_my_argv(stderr, ma, 0));
	    if (my_system(ma, 0)!=0) barf("failed running the final %s to produce %s", get_cc(), pl->b);
	    free_my_argv(ma);
	}
    }
    if (n_gcc_files>0) {
	MY_ARGV ma = make_my_argv();
	push_stringified_args(ma, get_cc());
	switch (stop_at) {
	case STOP_AT_M:     barf("I called final cc when I was supposed to do -M");
	case STOP_AT_CILKI:
	case STOP_AT_CILKC: barf("I called final cc when I was supposed to stop earlier");
	case STOP_AT_I: pusharg(ma, "-E"); break;
	case STOP_AT_S: pusharg(ma, "-S"); break;
	case STOP_AT_O: pusharg(ma, "-c"); break;
	case STOP_AT_END: break;
	}
	pushargs(ma, cc_args);
	if (stop_at==STOP_AT_END) {
	    pusharg(ma, "-L" LIBS_DIR);
	    pusharg(ma, "-L" LIBS2_DIR);
	    if (cilk_debug) {
	      if (cilk_profile) { pusharg(ma, "-lcilkrt0gp"); pusharg(ma, "-lcilk.g.p"); }
		else              { pusharg(ma, "-lcilkrt0g");  pusharg(ma, "-lcilk.g"); }
	    } else {
		if (cilk_profile) { pusharg(ma, "-lcilkrt0p");  pusharg(ma, "-lcilk.p"); }
		else              { pusharg(ma, "-lcilkrt0");   pusharg(ma, "-lcilk"); }
	    }
#           ifdef WITH_GNU_LD
	       pusharg(ma, "-Wl,-rpath," LIBS_DIR);
#           endif
	}
	push_stringified_args(ma, CILKC_PTHREAD_CFLAGS);
	push_stringified_args(ma, CILKC_PTHREAD_LIBS);
	if (output_name) {
	    pusharg(ma, "-o");
	    pusharg(ma, output_name);
	}
	VERBOSE(2, pusharg(ma, "-v"));
	VERBOSE(1, print_my_argv(stderr, ma, 0));
	if (my_system(ma, 0)!=0) barf("failed running the final %s", get_cc());
	free_my_argv(ma);
    }
}

void print_help (void) {
    char *usage[] = {
	"cilkc usage:\n",
	"Filenames:\n",
	" *.cilk              cilk files.\n",
	" *.cilki             preprocessed (by cpp) cilk files.\n",
	" *.cilkc             C files produced by running cilk2c on cilk files.\n",
	" *.c *.o *.i *.a ... Anything else that doesn't parse as an argument is treated as a filename and is passed to the C compiler..\n",
	"Options to control when to stop:\n",
	" -E0                 produce .cilki files (preprocess, but do not run cilk2c).\n",
	" -E1                 produce .cilkc files (preprocess, and run cilk2c, but do not preprocess again.\n",
	" -E                  produce .i preprocessed files (preprocess, run cilk2c, preprocess again, and  preprocess any C files.\n",
	" -S                  produce .s assembly files.\n",
	" -c                  produce .o object files.\n",
	" -M                  produce dependency files suitable for make.\n",
	" -MM                 like -M but see the gcc manual for the difference.\n",
	" otherwise produce executable files.\n",
	"Options to control the output file names:\n",
	" -o <filename>       Specify the name of the output file.\n",
	" -save-temps         Save all the temporary files, like gcc -save-temps (may be gcc-specific).\n",
	"Options to control diagnostic output:\n",
	" -v1                 print to stderr the set of compiler commands that cilkc runs.\n",
	" -v2                 also pass -v to the compilers, and print their outputs.\n",
	" -v                  the same as -v2.\n",
	" -n                  Do not actually run the compiler commands.  Useful with -v1.\n",
	" -W<arg>             Specify warnings to print.  Particularly useful ones are -W, -Wall and -W5 when using gcc.\n",
	" -woff numberlist    Turns of certain warnings in the mipspro C compiler.\n",
	" -ansi               Pass -ansi to the C compiler.\n",
	" -pedantic           Pass -pedantic to the C compiler.\n",
	" -version            Print the version number and quit.\n",
	" -help               Print this help message and quit.\n",
	"Preprocessing options:\n",
	" -I <path>           Add <path> to the search path for included files.\n",
	" -include=<path>     Include filename <path> before preprocessing.\n",
	" -D<def>             Define a preprocessing macro.\n",
	" -A<def>             Define a preprocessing predicate.\n",
	" -Wp,<arg>           Pass <arg> to the preprocessor.\n",
	"Code generation options:\n",
	" -cilk-profile       produce profiling code.\n",
	" -cilk-span	      produce profiling code that also computes critical path (requires -cilk-profile).\n",
	" -cilk-debug         produce debugging code in the cilk output.\n",
	" -cilk-hint          produce hint code (not documented or really even supported).\n",
	" -nd                 produce nondeterminator code.  Almost works in 9/2004.  Contact Jeremy Fineman (jfineman@theory.lcs.mit.edu) for details.\n",
	" -m<arg>             machine-specific options (passed to the preprocessor and the code generator.  gcc-specific).\n",
	" -g                  produce symbolic debugging information in the execugable.\n",
	" -O<arg>             specify an optimization level.\n",
	"Linking options:\n",
	" -l<libname>         use library <libname> .\n",
	" -L<path>            add <path> to the search path for libraries.\n",
	" -static             perform static linking.\n",
	" -Wl,<arg>           pass <arg> to the linker.\n",
	"Options to control input files:\n",
	" -x <lang>           Specify the language of the remaining files:  (gcc specific)\n",
	"                      <lang>        file type:\n",
	"                      none          use the filename suffix to determine its type.\n",
	"                      cilk         .cilk files\n",
	"                      cilki        .cilki files (preprocessed cilk files)\n",
	"                      cilkc        .cilkc files (C files preprocessed and then translated to C, need to be preprocessed again\n",
	"                      c            .c files\n",
	"                      objective-c   .m or .C files (objective c)\n",
	"                      c++           .cpp .cc .cxx (c++ files)\n",
	"                      c-header      .h files\n",
	"                      cpp-output    .i files (preprocessed c files)\n",
	"                      c++cpp-output .ii files (preprocessed c++ files)\n",
	"                      assembler     .s files\n",
	"                      assembler-with-cpp .S files\n",
	"Options to control the C compiler:\n",
	" -Wc,*                pass the * argument to the C compiler.  For example when using the mipspro compiler you could do\n",
        "                          -Wc,-use_readwrite_const    to put constants into readwrite memory, or\n",
	"                          -Wc,-Wa,-foo                which would pass -Wa,-foo to the C compiler, which in turn would pass -foo to the assembler.\n",
	"                               That is suppposed to be equivalent to just passing -Wa,-foo, but it may be subtly different.\n",
	"Any other argument is treated as a file to be linked (.o .a .so etc.)\n",
	0,
    };
    int i;
    for (i=0; usage[i]; i++) {
	printf("%s", usage[i]);
    }
    
}

void do_operations (void) {
    if (verbose>=1 || do_print_version) {
	fprintf(stderr, "cilkc %s\n", "$Rev: 2491 $ $Date: 2005-10-07 10:10:47 -0400 (Fri, 07 Oct 2005) $");
    }
    if (do_print_help) {
	print_help();
    }
    if (stop_at==STOP_AT_M) {
	do_M();
    } else {
	do_cpp1();
	if (stop_at==STOP_AT_CILKI) return;
	do_cilk2c();
	if (stop_at==STOP_AT_CILKC) return;
	do_cpp2();
	do_cc();
    }
}

static void cleanup (void) {
    if (all_tempfiles) {
	STRINGLIST tl;
	assert(!save_temps);
	for (tl=all_tempfiles; tl; tl=tl->rest) {
	    unlink(tl->first);
	}
    }
    {
	STRINGLIST tl;
	for (tl=more_tempfiles_to_delete; tl; tl=tl->rest) {
	    unlink(tl->first);
	}
    }
    free_my_argv(cilkc_args);
    free_my_argv(cc_early_args);
    free_my_argv(cc_args);
    free_my_argv(cpp1_args);
}

int main (int argc, const char *argv[]) {
    parse_args1(argc, argv);
    parse_args2(argc, argv);
    do_operations();
    cleanup();
    return 0;
}
