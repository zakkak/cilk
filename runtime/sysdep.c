/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
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

#include <stdarg.h>
#include <stdio.h>
#include <cilk.h>
#include <cilk-internal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>

#ifdef HAVE_SYS_TIME_H
#include <unistd.h>
#endif

#if HAVE_SCHED_H
#include <sched.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if HAVE_SYS_PROCFS_H
#include <sys/procfs.h>
#endif

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/sysdep.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

/***********************************************************\
 * Architecture-specific options
\***********************************************************/

/*
 * Parse any architecture-specific command-line options.  <argc>
 * and <argv> are the original arguments passed on the command line,
 * and <i> is the index of the first architecture-specific option (if any).
 * This function returns the index of the first user option, or <argc> if
 * there aren't any.
 */
int Cilk_arch_specific_options(int i, int UNUSED(argc), char UNUSED(*argv[]))
{
     return i;
}

/**********************************************************
 * Priority control
 **********************************************************/

void Cilk_lower_priority(CilkWorkerState *const ws)
{
#if defined(HAVE_PTHREAD_SETSCHEDPARAM) && defined(PRI_OTHER_MIN)
     {
	  struct sched_param param;
	  param.sched_priority = PRI_OTHER_MIN;
	  pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
     }
#endif
     Cilk_yield(ws);
}

void Cilk_raise_priority(CilkWorkerState *const UNUSED(ws))
{
#if defined(HAVE_PTHREAD_SETSCHEDPARAM) && defined(PRI_OTHER_MAX)
     {
	  struct sched_param param;
	  param.sched_priority = PRI_OTHER_MAX;
	  pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
     }
#endif
}

void Cilk_yield(CilkWorkerState *const UNUSED(ws))
{
    usleep(1);  /* This seems simpler and better than all the code below.
		 * The problem is that even with sched_yield() I can incur a significant
                 * slowdown on serial code with lots of spawns.  E.g., if you writen
		 *    for (i=0; i<10; i++) {spawn f(x); sync;}
		 * why would you want to do that?  If f is a cilk procedure, you cannot call
                 * f without using spawn.  Canary runs into this.
                 */ 
    return;
#if 0
#if CILK_WITH_POSIX_THREADS
#  if HAVE_SCHED_YIELD
     sched_yield();
#  elif HAVE_SGINAP
     sginap(1);
#  endif
#else
#  if HAVE_SCHED_YIELD
     sched_yield();
#  elif HAVE_YIELD
     yield();
#  endif
#endif
#endif
}

/***********************************************************\
 * Machine size
\***********************************************************/
int Cilk_partition_size(void)
{
#ifdef _SC_NPROCESSORS_ONLN
     /* 
      * This is only really right for an unloaded machine.
      */
     return sysconf(_SC_NPROCESSORS_ONLN);
#else
     return 1;
#endif
}

/***************************************************
 * Timer control
 ***************************************************/
#if defined(PIOCSET) && defined(PR_MSACCT)  /* true on solaris */
static int openslashproc(int pid)
{
     char buf[64];
     int  fd;

     sprintf(buf, "/proc/%d", pid);
     if ( (fd = open(buf, 2)) < 0 )
	  perror("open /proc");

     return fd;
}

static void start_microaccounting(void)
{
     int fd, flags;

     fd = openslashproc(getpid());

     flags = PR_MSACCT;
     if (ioctl(fd, PIOCSET, &flags, sizeof (flags)) < 0) {
	  perror("ioctl set microstate acct");
     }
}
#else
static void start_microaccounting(void) 
{
}
#endif

/**********************************************************
 * Initialization of this module
 **********************************************************/

void Cilk_arch_specific_init(void)
{
     start_microaccounting();
}

void Cilk_arch_specific_per_worker_init(void)
{
     start_microaccounting();
}

