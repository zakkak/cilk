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

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/mutex.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");


/***********************************************************\
 * Support for mutexen 
\***********************************************************/

/* implementation over shared memory */
#define ACQUIRE(lock) Cilk_xchg(lock, 1)
#define RELEASE(lock) *(lock) = 0

/* initialize lock */
static inline void Cilk_mutex_init_memory(Cilk_mutex *lock)
{
     RELEASE(&(lock->memory));
}

/* destroy mutex - not sure about the implementation... */
static inline void Cilk_mutex_destroy_memory(Cilk_mutex *lock)
{
     RELEASE(&(lock->memory));
}
/*
 *  Obtain lock <lock>.  Waits until lock is available.
 */
static inline void Cilk_mutex_wait_memory(CilkWorkerState *const UNUSED(ws),
					  Cilk_mutex *lock)
{
     if (ACQUIRE(&(lock->memory)) == 0)  /* fast case */
          return;

     Cilk_enter_state(ws, STATE_WAITING_FOR_LOCK);

     CILK_WMB();

     /*
      * This hackery should reduce bus traffic, if the
      * cache is snooped correctly.
      */
     do {
	  while (*&(lock->memory) != 0);
     } while (ACQUIRE(&(lock->memory)) != 0);

     CILK_RMB();

     Cilk_exit_state(ws, STATE_WAITING_FOR_LOCK);
}

/*
 *  Try to obtain lock <lock>.  Returns 1 upon success and 0 upon failure.
 *  Do not wait.
 */
static inline int Cilk_mutex_try_memory(Cilk_mutex *lock)
{
     CILK_WMB();
     if (ACQUIRE(&(lock->memory)) == 0) {
	  CILK_RMB();
	  return 1;
     } else
	  return 0;
}

/*
 *  Give up lock <lock>.
 */
static inline void Cilk_mutex_signal_memory(Cilk_mutex *lock)
{
     CILK_WMB();
     RELEASE(&(lock->memory));
}


/*******************************
 * Posix-threads locks
 *******************************/
/*
 * Initialize lock
 */
static inline void Cilk_mutex_init_posix(Cilk_mutex *lock)
{
    pthread_mutex_init(&lock->posix, NULL);
}

/*
 *  Obtain lock <lock>.  Waits until lock is available.
 */
static inline void Cilk_mutex_wait_posix(CilkContext *const UNUSED(context),
					 CilkWorkerState *const UNUSED(ws),
					 Cilk_mutex *lock)
{
     Cilk_enter_state(ws, STATE_WAITING_FOR_LOCK);

     pthread_mutex_lock(&lock->posix);

     Cilk_exit_state(ws, STATE_WAITING_FOR_LOCK);
}

/*
 *  Give up lock <lock>.
 */
static inline void Cilk_mutex_signal_posix(Cilk_mutex *lock)
{
     pthread_mutex_unlock(&lock->posix);
}

/*
 *  Try to obtain lock <lock>.  Returns 1 if it succeeds and 0 if it fails.
 *  Does not wait.
 */
static inline int Cilk_mutex_try_posix(Cilk_mutex *lock)
{
     if (pthread_mutex_trylock(&lock->posix) == 0) {
	  return 1;
     } else {
	  return 0;
     }
}

/*
 * Destroy the mutex
 */
static inline void Cilk_mutex_destroy_posix(Cilk_mutex* lock)
{
    pthread_mutex_destroy(&lock->posix);
}

/*********************
 * Lock glue
 *********************/
void Cilk_mutex_init(CilkContext *const context,
		     Cilk_mutex *lock)
{
     if (USE_PARAMETER1(options->memory_locks)) {
	  Cilk_mutex_init_memory(lock);
     } else {
	  Cilk_mutex_init_posix(lock);
     }
}

void Cilk_mutex_wait(CilkContext *const context,
                     CilkWorkerState *const ws,
		     Cilk_mutex *lock)
{
     if (USE_PARAMETER1(options->memory_locks)) {
	  Cilk_mutex_wait_memory(ws, lock);
     } else {
	  Cilk_mutex_wait_posix(context, ws, lock);
     }
}

void Cilk_mutex_signal(CilkContext *const context,
		       Cilk_mutex *lock)
{
     if (USE_PARAMETER1(options->memory_locks)) {
	  Cilk_mutex_signal_memory(lock);
     } else {
	  Cilk_mutex_signal_posix(lock);
     }
}


int Cilk_mutex_try(CilkContext *const context,
		   Cilk_mutex *lock)
{
     if (USE_PARAMETER1(options->memory_locks)) {
	  return Cilk_mutex_try_memory(lock);
     } else {
	  return Cilk_mutex_try_posix(lock);
     }
}

void Cilk_mutex_destroy(CilkContext *const context,
			Cilk_mutex *lock)
{
    if(USE_PARAMETER1(options->memory_locks)) {
	Cilk_mutex_destroy_memory(lock);
    } else {
	Cilk_mutex_destroy_posix(lock);
    }
}


