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
#include <stdlib.h>
#include <cilk.h>
#include <cilk-internal.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/debug.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");


/***********************************************************\
 * Debugging & tracing
\***********************************************************/

void Cilk_dprintf(CilkWorkerState *const ws, const char *fmt,...)
{
     va_list l;

     /* ensure nice interleaving of debugging messages */
     Cilk_mutex_wait(ws->context, ws, &USE_SHARED(dprintf_lock));

     if (ws != 0)
	  fprintf(stderr, "%d: ", ws->self);
     else
	  fprintf(stderr, "global: ");

     va_start(l, fmt);
     vfprintf(stderr, fmt, l);
     fflush(stderr);
     va_end(l);
     Cilk_mutex_signal(ws->context, &USE_SHARED(dprintf_lock));
}

void Cilk_die_internal(CilkContext *const context, CilkWorkerState *const ws, const char *fmt,...)
{
     va_list l;

     /* Use USE_SHARED1 not USE_SHARED because ws may be NULL */
     Cilk_mutex_wait(context, ws, &USE_SHARED1(dprintf_lock));

     if (ws)
	  fprintf(stderr, "%d: fatal error: ", ws->self);
     else
	  fprintf(stderr, "global: fatal error: ");

     va_start(l, fmt);
     vfprintf(stderr, fmt, l);
     va_end(l);
     Cilk_mutex_signal(context, &USE_SHARED1(dprintf_lock));

     /* ensure only the first fatal error produces death of the program */
     if (USE_PARAMETER(options->dump_core) && Cilk_mutex_try(context, &USE_SHARED1(die_lock)))
	  abort();		/* dump core */
     else
          exit(1);              /* don't dump core */
}

void Cilk_die_external(CilkContext *const context, const char *fmt,...)
{
     va_list l;

     Cilk_mutex_wait(context, NULL, &USE_SHARED1(dprintf_lock));

     fprintf(stderr, "fatal error: ");

     va_start(l, fmt);
     vfprintf(stderr, fmt, l);
     va_end(l);
     Cilk_mutex_signal(context, &USE_SHARED1(dprintf_lock));

     /* ensure only the first fatal error produces death of the program */
     if (USE_PARAMETER1(options->dump_core) && Cilk_mutex_try(context, &USE_SHARED1(die_lock)))
	  abort();		/* dump core */
     else
          exit(1);              /* don't dump core */
}

void Cilk_debug_init(CilkContext *const context)
{
     Cilk_mutex_init(context, &USE_SHARED1(dprintf_lock));
     Cilk_mutex_init(context, &USE_SHARED1(die_lock));
}

void Cilk_debug_terminate(CilkContext *const context)
{
     Cilk_mutex_destroy(context, &USE_SHARED1(dprintf_lock));
     Cilk_mutex_destroy(context, &USE_SHARED1(die_lock));
}
