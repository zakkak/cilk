/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Massachusetts Institute of Technology
 * Copyright (c) 2002 Bradley C. Kuszmaul
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 *  USA.
 *
 */

/***********************************************************\
 * Command line parsing
\***********************************************************/

#include <cilk.h>
#include <cilk-internal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cmdline.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

enum {
     NONE, NPROC, STATS, NO_STATS, HELP, STACK, YIELD, NO_YIELD,
     PTHREAD_STACKSIZE,
     POSIX_LOCKS, MEMORY_LOCKS,
     INFOFILE, DUMP_CORE, NO_DUMP_CORE, PINNED_PROC,  ALLOC_BATCH,
     FIRST_STOP, FLOAT_COMM, ALL_SETS, BRELLY, RECYCLE_MEM,
     VERSION_, END_OPTIONS
};

static struct options {
     char *string;
     int option;
     char *help;
} optarray[] = {
     {
	  "", END_OPTIONS, "-- : end of option parsing"
     },
#ifndef CILK_ND
     {
	  "nproc", NPROC, "--nproc <n> : set number of processors"
#else
     {
	  "nproc", NPROC, "-nproc <n> : ineffective option, # of processors = 1 for Nondeterminator"
     },
#endif
     },
     {
	  "stack", STACK, "--stack <n> : set stack depth"
     },
     {
	  "pthread-stacksize", PTHREAD_STACKSIZE, "--pthread-stacksize <n> : set the size of the stack used by each worker thread"
     },
     {
	  "stats", STATS, "--stats <n> : set statistics level"
     },
     {
	  "no-stats", NO_STATS, "--no-stats : disable statistics printing"
     },
     {
	  "alloc-batch", ALLOC_BATCH,
	  "--alloc-batch <n> : set batch length for memory allocator"
     },
     {
	  "yield", YIELD, "--yield : yield time slice when steal fails"
     },
     {
	  "no-yield", NO_YIELD, "--no-yield : don't yield time slice"
     },
     {
	  "with-posix-locks", POSIX_LOCKS, "--with-posix-locks : use posix locks"
     },
     {
	  "with-memory-locks", MEMORY_LOCKS, "--with-memory-locks : use memory locks"
     },
     {
	  "dump-core", DUMP_CORE,
	  "--dump-core : dump core on fatal cilk errors"
     },
     {
	  "no-dump-core", NO_DUMP_CORE,
	  "--no-dump-core : do not dump core on fatal cilk errors"
     },
     {
          "pin-proc", PINNED_PROC,
	  "--pin-proc <n> : pin the program using hex mask (linux only)"
     },
#ifdef CILK_ND
     {
          "first-stop", FIRST_STOP, "-first-stop : stop the program when the first race is found"
     },
     {
          "float-comm", FLOAT_COMM, "-float-comm : assuming floating point operations are commutative"
     },
     {
          "all-sets", ALL_SETS, "-all-sets : use the ALL-SETS algorithm for race detection"
     },
     {
          "brelly", BRELLY, "-brelly : use the BRELLY algorithm for race detection"
     },
     {
          "recycle-mem", RECYCLE_MEM, "-recycle-mem : allow memory reuse from the Cilk heap"
     },
#endif
     {
	  "version", VERSION_,
	  "--version : print information about the cilk runtime system"
     },
     {
	  "help", HELP, "--help : print this message"
     },
     {
	  (char *) 0, NONE, ""
     }
};

static void print_help(void)
{
     struct options *p;

     fprintf(stderr, "Cilk options:\n");

     for (p = optarray; p->string; ++p)
	  if (p->help)
	       fprintf(stderr, "     %s\n", p->help);

     fprintf(stderr, "\n");
     fprintf(stderr, "Cilk web page: http://supertech.lcs.mit.edu/cilk\n");
}

static void print_version(void)
{
     int debug = 0, stats = 0, timing = 0;
     WHEN_CILK_DEBUG(debug = 1);
     WHEN_CILK_STATS(stats = 1);
     WHEN_CILK_TIMING(timing = 1);
     fprintf(stderr, "Cilk version " VERSION "\n");
     fprintf(stderr, "compilation options: ");
     if (debug) fprintf(stderr, "CILK_DEBUG ");
     if (stats) fprintf(stderr, "CILK_STATS ");
     if (timing) fprintf(stderr, "CILK_TIMING ");
     if (!(debug | stats | timing))
	  fprintf(stderr, "none");
     fprintf(stderr, "\n");
}

/* look for a given string in the option table */
static struct options *parse_option(char *s)
{
     struct options *p;

     for (p = optarray; p->string; ++p)
	  if (strcmp(s, p->string) == 0)
	       break;

     return p;
}

#define CHECK(cond, complaint) \
     if (!(cond)) { fprintf(stderr, "Bad option argument for -%s: %s\n", p->string, complaint); return 1; }

int Cilk_parse_command_line(Cilk_options *options, int *argc, char *argv[])
{
     int i, j;
     struct options *p;
     /* gcc allows to write directly into *options, but other compilers
      * only allow you to initialize this way.
      */
     Cilk_options cilk_default_options = CILK_DEFAULT_OPTIONS;

     /* default options */
     *options = cilk_default_options;

     for (i = 1; i < *argc; ++i) {
	  if (argv[i][0] != '-')
	       goto arch_specific;

	  if (argv[i][1] == '-')
	       p = parse_option(argv[i] + 2);
	  else {
	       /* accept deprecated old-style form -X instead of --X */
	       p = parse_option(argv[i] + 1);
	  }

	  switch (p->option) {
	      case NPROC:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->nproc = atoi(argv[i]);
#ifndef CILK_ND
		   if (options->nproc == 0)
			options->nproc = Cilk_partition_size();
		   CHECK(options->nproc > 0,
			 "non-positive number of processors");
#else
		   if (options->nproc != 1)
		     fprintf(stderr, "Warning: the Nondeterminator runs only"
			             " on one processor\n");
		   options->nproc = 1;
#endif
		   break;
	      case STACK:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->stackdepth = atoi(argv[i]);
		   CHECK(options->stackdepth > 0,
			 "non-positive stack depth");
		   break;
	      case PTHREAD_STACKSIZE:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->pthread_stacksize = atoi(argv[i]);
		   CHECK(options->pthread_stacksize > 0,
			 "non-positive stack depth");
		   break;
	      case STATS:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->statlevel = atoi(argv[i]);
		   break;
	      case NO_STATS:
		   options->statlevel = 0;
		   break;
	      case YIELD:
		   options->yieldslice = 1;
		   break;
	      case NO_YIELD:
		   options->yieldslice = 0;
		   break;
	      case POSIX_LOCKS:
		   options->memory_locks = 0;
		   break;
	      case MEMORY_LOCKS:
		   options->memory_locks = 1;
		   break;
	      case DUMP_CORE:
		   options->dump_core = 1;
		   break;
	      case NO_DUMP_CORE:
		   options->dump_core = 0;
		   break;
	      case PINNED_PROC:
	           ++i;
		   CHECK(i < *argc, "argument missing");
		   options->pinned_mask = atol(argv[i]);
		   break;
	      case HELP:
		   print_help();
		   return 1;
		   break;
	      case VERSION_:
		   print_version();
		   return 1;
		   break;
	      case ALLOC_BATCH:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->alloc_batch_size = atoi(argv[i]);
		   if (options->alloc_batch_size < 8)
			options->alloc_batch_size = 8;
		   break;
	      case INFOFILE:
		   ++i;
		   CHECK(i < *argc,
			 "argument missing");
		   options->infofile_name = argv[i];
		   break;
	      WHEN_CILK_ND(
	      case FIRST_STOP:
	           INIT_SHARED_RO(first_stop, 1);
		   break;
	      case FLOAT_COMM:
	           INIT_SHARED_RO(float_comm, 1);
		   break;
	      case ALL_SETS:
	           INIT_SHARED_RO(all_sets, 1);
	           INIT_SHARED_RO(umbrella, 0);
		   break;
	      case BRELLY:
	           INIT_SHARED_RO(umbrella, 1);
		   INIT_SHARED_RO(all_sets, 0);
		   break;
	      case RECYCLE_MEM:
	           INIT_SHARED_RO(recycle_mem, 1);
		   break;
              )
	      case END_OPTIONS:
		   ++i;
		   goto stop;
	      default:
		   goto arch_specific;
	  }
     }

     /* let architecture-specific code get a crack at the command line */
   arch_specific:
     i = Cilk_arch_specific_options(i, *argc, argv);

     /* remove Cilk command-line arguments from argv */
   stop:
     for (j = i; j <= *argc; j++)
	  argv[j - (i - 1)] = argv[j];
     *argc -= i - 1;

     return 0;
}
