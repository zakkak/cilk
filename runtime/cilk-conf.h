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

#ifndef _CILK_CONF_H
#define _CILK_CONF_H

FILE_IDENTITY(ident_cilk_conf_h,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-conf.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $");


/* use the __cilksafe__ attribute if running through Cilk */
#ifdef __CILK__
#define __CILKSAFE__  __cilksafe__
#else
#define __CILKSAFE__
#endif

/* Cannot put this into the file because cilk-conf.h is included by cilk2c twice, once expanded, and once not expanded.
 * leave this here, commented out, so that at least the version number will be in the comments.
 * static const char *cilk_conf_h_ident __attribute__((__unused__))
     = "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-conf.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $";
*/
/*
 * Keep track of various statistics 
 */
#ifndef CILK_STATS
#define CILK_STATS 0
#endif

/*
 * Keep track of execution time
 */
#ifndef CILK_TIMING
#define CILK_TIMING 0
#endif

/*
 * Measure critical path. EXPENSIVE in terms of execution time
 */
#ifndef CILK_CRITICAL_PATH
#define CILK_CRITICAL_PATH 0
#endif

/*
 * Provide the current work and critical path in
 * Cilk_user_work and Cilk_user_critical_path
 */
#define CILK_USER_TIMERS CILK_CRITICAL_PATH

/*
 * Enable rts self-consistency checks.
 */
#ifndef CILK_DEBUG
#define CILK_DEBUG 0
#endif

/*
 * enable support for alloca (slows down common case frame
 * allocation by 20%)
 */
#ifndef CILK_ALLOCA
#define CILK_ALLOCA 1
#endif

/* 
 * Default size of Cilk's stack
 */
#define CILK_DEFAULT_STACK_DEPTH 32768  

/*
 * there should be no need to hack below this line
 */
#if CILK_CRITICAL_PATH && (!CILK_TIMING)
#warning Critical path measurements cannot be enabled because timing is not supported by the rts
#undef CILK_CRITICAL_PATH
#define CILK_CRITICAL_PATH 0
#endif

#define CILK_CHECK(cond, complaints) \
     if (!(cond)) Cilk_die_internal complaints;

#if CILK_DEBUG
#define CILK_ASSERT(ws, ex)						 \
      (void)((ex) || 							 \
	     (Cilk_die_internal(ws->context, ws, USE_PARAMETER(assertion_failed_msg), \
             #ex, __LINE__, __FILE__), 0))
#define WHEN_CILK_DEBUG(ex)  ex
#define CILK_COMPLAIN(cond, complaints) \
     if (!(cond)) Cilk_die_internal complaints;
#else
#define CILK_ASSERT(ws, ex)
#define WHEN_CILK_DEBUG(ex)
#define CILK_COMPLAIN(cond, complaints)
#endif

#if CILK_ALLOCA
#define WHEN_CILK_ALLOCA(a) a
#else
#define WHEN_CILK_ALLOCA(a)
#endif

#if CILK_STATS
#define WHEN_CILK_STATS(a) a
#else
#define WHEN_CILK_STATS(a)
#endif

#if CILK_TIMING
#define WHEN_CILK_TIMING(a) a
#else
#define WHEN_CILK_TIMING(a)
#endif

#if CILK_CRITICAL_PATH
#define WHEN_CILK_CRITICAL_PATH(a) a
#else
#define WHEN_CILK_CRITICAL_PATH(a)
#endif

#if CILK_USER_TIMERS
#define WHEN_CILK_USER_TIMERS(a) a
#else
#define WHEN_CILK_USER_TIMERS(a)
#endif

#define CILK_IM_FREE_MAGIC 0x132DEBFA
#define CILK_CLOSURE_MAGIC 0xDEADBEEF
#define CILK_STACKFRAME_MAGIC 0xBEEF1234

#define CILK_MALLOC_NLISTS 32

/* threshold above which we use slow malloc */
#define CILK_INTERNAL_MALLOC_THRESHOLD 4096
#define CILK_INTERNAL_MALLOC_BUCKETS 9

/* tell cilk2c not to emit code for linker segments */
#ifdef __CILK2C__
#pragma segments -
#endif

/*
 * ``parameters'' are set by the global process and they are only read
 * by workers.  It would be nice to map these in a segment which is
 * actually read only.
 */
#define USE_PARAMETER(id) ws->context->Cilk_RO_params->id
#define USE_PARAMETER1(id) context->Cilk_RO_params->id
/* Ofra and Sivan, added USE_PARAMETER2, 19 June 2003 */
#define USE_PARAMETER2(id) _cilk_ws->context->Cilk_RO_params->id
#define INIT_PARAMETER(id, value) USE_PARAMETER(id) = value
#define INIT_PARAMETER1(id, value) USE_PARAMETER1(id) = value

#define EXPORT(cilk_func) mt_##cilk_func

/*
 * macros used to compile codes related to the Nondeterminator
 */
#ifdef CILK_ND
#define WHEN_CILK_ND(a) a
#else
#define WHEN_CILK_ND(a)
#endif
#endif				/* _CILK_CONF_H */
