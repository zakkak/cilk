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

/*
 * This file contains the code for barriers
 */

#include <cilk.h>
#include <cilk-internal.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/barrier.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");


/***********************************************************\
 * Code for barriers
\***********************************************************/
/* sense-reversing barrier, see Hennessy & Patterson page 702 */
/* this barrier can be safely reused */

/*
 * Volker pointed out that the original code in H&P has a bug,
 * and provided a fix, which is used here
 */
void Cilk_barrier(CilkWorkerState *const ws)
{
     int local_counter;

     ws->barrier_direction = !ws->barrier_direction;

     Cilk_mutex_wait(ws->context, ws, &USE_SHARED(barrier_lock));
     local_counter = ++USE_SHARED(barrier_counter);
     Cilk_mutex_signal(ws->context, &USE_SHARED(barrier_lock));

     if (local_counter == USE_PARAMETER(active_size)) {
	  USE_SHARED(barrier_counter) = 0;
	  USE_SHARED(barrier_release) = ws->barrier_direction;
     } else
	  while (USE_SHARED(barrier_release) != ws->barrier_direction)
	       if (USE_PARAMETER(options->yieldslice))
		    Cilk_yield(ws);
}

void Cilk_barrier_init(CilkContext *const context)
{
     /* initialize the barrier code */
     Cilk_mutex_init(context, &USE_SHARED1(barrier_lock));
     USE_SHARED1(barrier_counter) = 0;
     USE_SHARED1(barrier_release) = 0;
}

void Cilk_barrier_per_worker_init(CilkWorkerState *const ws)
{
     ws->barrier_direction = 0;
}

void Cilk_barrier_terminate(CilkContext *const context)
{
	/* Destroy the mutex*/
	Cilk_mutex_destroy(context, &USE_SHARED1(barrier_lock));
}
