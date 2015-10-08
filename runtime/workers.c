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
#ifdef CILK_USE_PERFCTR
#include <string.h>
#include <libperfctr.h>
#endif

/***********************************************************\
 * Spawning worker processes
\***********************************************************/
#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif


static void init_sync_variables(CilkContext *const context)
{
    /* pthread_cond_t cond_init = PTHREAD_COND_INITIALIZER; */
     int res;

     USE_SHARED1(terminating) = 0;
     USE_SHARED1(nothing_to_do) = 1;
     USE_SHARED1(workers_done_counter) = 0;
     USE_SHARED1(workers_are_done) = 0;

     /* Create conditions and mutex*/
     res = pthread_cond_init(&USE_SHARED1(waiting_workers_cond), NULL);
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_cond_init: %d returned \n", res));

      res = pthread_mutex_init(&USE_SHARED1(workers_mutex), NULL);
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_init: %d returned \n", res));

      res = pthread_cond_init(&USE_SHARED1(workers_done_cond), NULL);
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_cond_init: %d returned \n", res));

      res = pthread_cond_init(&USE_SHARED1(wakeup_first_worker_cond), NULL);
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_cond_init: %d returned \n", res));

      res = pthread_cond_init(&USE_SHARED1(wakeup_other_workers_cond), NULL);
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_cond_init: %d returned \n", res));
}

/* pthread_setconcurrency appears to be in the library archives, but may not be in the header files.
 * Perhaps we should be using autoconf to determine what to do here.
 */
extern int pthread_setconcurrency (int);

#ifdef CILK_USE_PERFCTR
double __cilk_perfctr_to_second_factor;

struct vperfctr {
     /* XXX: point to &vperfctr_state.cpu_state instead? */
     volatile const struct vperfctr_state *kstate;
     int fd;
     unsigned char have_rdpmc;
};

volatile const struct vperfctr_state *__cilk_vperfctr_init(int set_global_variables) {
      struct vperfctr *vpc;
      if (!(vpc = vperfctr_open())) {
             perror("vperfctr_open");
             exit(1);
      }
      if (set_global_variables) {
             struct perfctr_info info;
             if (vperfctr_info(vpc, &info) < 0) {
                    perror("vperfctr_info");
                    exit(1);
             }
             __cilk_perfctr_to_second_factor = 1e-3 / info.cpu_khz;
      }
      /* turn it on */
      {
             struct vperfctr_control control;
             memset(&control.cpu_control, 0, sizeof control.cpu_control);
             control.cpu_control.tsc_on = 1;
             vperfctr_control(vpc, &control);
      }
      return vpc->kstate;
}

#endif



void Cilk_create_children(CilkContext *const context,
			  void (*child)(CilkChildParams *const childParams))
{
     long i;
     int res;
     pthread_attr_t attr;

#ifdef CILK_USE_PERFCTR
     __cilk_vperfctr_init(1);
#endif
	
     init_sync_variables(context);

     /* Create thread-id array */
     USE_SHARED1(tid) = Cilk_malloc_fixed(USE_PARAMETER1(active_size) * sizeof(pthread_t));
     CILK_CHECK(USE_SHARED1(tid), (context, NULL, "could not malloc tid\n"));

     /* allocate and init the params sent to the children*/
      USE_SHARED1(thrd_params_array) =
     			Cilk_malloc_fixed(USE_PARAMETER1(active_size) * sizeof(CilkChildParams));
     CILK_CHECK(USE_SHARED1(thrd_params_array), (context, NULL, "could not malloc params_array\n"));

     for (i = 0; i < USE_PARAMETER1(active_size); i++)
     {
	USE_SHARED1(thrd_params_array)[i].context = context;
	USE_SHARED1(thrd_params_array)[i].id = i;
     }

     pthread_attr_init(&attr);
     /* initialize attr with default attributes */
     pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
                              /* system-wide contention */
     if (USE_PARAMETER1(pthread_stacksize)>0) {
	 size_t sz = USE_PARAMETER1(pthread_stacksize);
	 int r = pthread_attr_setstacksize(&attr, sz);
	 if (r!=0) {
	     Cilk_die_internal(context, NULL, "Can't set stacksize (maybe be too small or too large)\n");
	 }
     }
     pthread_setconcurrency(USE_PARAMETER1(active_size));

     for (i = 0; i < USE_PARAMETER1(active_size); i++)
     {
	  res = pthread_create(USE_SHARED1(tid) + i,
			       &attr,
			       (void * (*) (void *)) child,
			       (void *) &(USE_SHARED1(thrd_params_array)[i]) );
	  if (res)
	       Cilk_die_internal(context, NULL, "Can't create threads\n");
     }

}


void Cilk_wakeup_workers(CilkContext *const context)
{
   int res;

   /* Reset to 0 nothing_to_do */
   res = pthread_mutex_lock(&USE_SHARED1(workers_mutex));
   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_lock: %d returned \n", res));

   USE_SHARED1(workers_done_counter) = 0;
   USE_SHARED1(workers_are_done) = 0;
   USE_SHARED1(nothing_to_do) = 0;

   res = pthread_mutex_unlock(&USE_SHARED1(workers_mutex));
   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_unlock: %d returned \n", res));

   /* Wake up the workers who are sleeping */
   res = pthread_cond_broadcast(&USE_SHARED1(wakeup_first_worker_cond));
   if (res)
       Cilk_die_internal(context, NULL, "Failed to broadcast\n");

   /* Wait till they are done */
   res = pthread_mutex_lock(&USE_SHARED1(workers_mutex));
   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_lock: %d returned \n", res));

   while(! USE_SHARED1(workers_are_done) ) {
	res = pthread_cond_wait(&USE_SHARED1(workers_done_cond), &USE_SHARED1(workers_mutex));
   	CILK_CHECK(res == 0,(context, NULL,"error in pthreads_cond_wait: %d returned \n", res));
   }

   res = pthread_mutex_unlock(&USE_SHARED1(workers_mutex));
   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_unlock: %d returned \n", res));
}

void Cilk_worker_wait_for_invocation(CilkContext *const context, long self, int *local_terminating)
{
   int res;
   pthread_cond_t *my_cond = ( self != 0 ? &USE_SHARED1(wakeup_other_workers_cond)
   					: &USE_SHARED1(wakeup_first_worker_cond));

   res = pthread_mutex_lock(&USE_SHARED1(workers_mutex));
   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_lock: %d returned \n", res));

   while((!USE_SHARED1(terminating)) && USE_SHARED1(nothing_to_do)){
   	res = pthread_cond_wait(my_cond, &USE_SHARED1(workers_mutex));
       	CILK_CHECK((res == 0), (context, NULL, "error in pthread_cond_wait: %d returned \n", res));
   }

   USE_SHARED1(workers_done_counter)++;

   *local_terminating = USE_SHARED1(terminating);
   res = pthread_mutex_unlock(&USE_SHARED1(workers_mutex));

   if( self == 0 ) {
   	/* Wakeup the other workers */
   	res = pthread_cond_broadcast(&USE_SHARED1(wakeup_other_workers_cond));
   	if (res)
       		Cilk_die_internal(context, NULL, "Failed to broadcast\n");

   }

   CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_unlock: %d returned \n", res));
}

/* Assumption:  terminating = false */
void Cilk_worker_is_done(CilkContext *const context, int *local_terminating)
{
   int res;

   /* Incrementing done_counter */
   res = pthread_mutex_lock(&USE_SHARED1(workers_mutex));
   CILK_CHECK(res == 0,(context, NULL,"error in pthread_mutex_lock: %d returned \n", res));

   USE_SHARED1(nothing_to_do) = 1;
   USE_SHARED1(workers_done_counter)--;

   /* Signal if all worker threads are done */
   if(USE_SHARED1(workers_done_counter) <= 0 ) {
        USE_SHARED1(workers_are_done) = 1;
   	res = pthread_cond_signal(&USE_SHARED1(workers_done_cond));
   	CILK_CHECK(res == 0,(context, NULL,"error in pthread_cond_signal: %d returned \n", res));
   }

   *local_terminating = USE_SHARED1(terminating);
   res = pthread_mutex_unlock(&USE_SHARED1(workers_mutex));
   CILK_CHECK(res == 0,(context, NULL,"error in pthread_mutex_unlock: %d returned \n", res));
}

void Cilk_terminate_children(CilkContext *const context)
{
     long i;
     int res;

     /* wake up only those who are sleeping */
     res = pthread_mutex_lock(&USE_SHARED1(workers_mutex));
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_lock: %d returned \n", res));

     USE_SHARED1(terminating) = 1;

     res = pthread_mutex_unlock(&USE_SHARED1(workers_mutex));
     CILK_CHECK((res == 0), (context, NULL, "error in pthread_mutex_unlock: %d returned \n", res));

     res = pthread_cond_broadcast(&USE_SHARED1(wakeup_first_worker_cond));
     if (res)
       Cilk_die_internal(context, NULL, "Failed to broadcast\n");

     res = pthread_cond_broadcast(&USE_SHARED1(wakeup_other_workers_cond));
     if (res)
       Cilk_die_internal(context, NULL, "Failed to broadcast\n");

     /* Now join the threads */
     for (i = 0; i < USE_PARAMETER1(active_size); i++)
     {
	  res = pthread_join(USE_SHARED1(tid[i]), NULL);
	  CILK_CHECK(res == 0,
		     (context, NULL,
		      "error in pthread_join: %d returned (worker %d)\n",
		      res, i));
     }


     Cilk_free(USE_SHARED1(thrd_params_array));
     Cilk_free(USE_SHARED1(tid));
}
