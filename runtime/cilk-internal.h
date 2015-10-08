FILE_IDENTITY(ident_cilk_internal_h,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-internal.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $");

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

/* common definitions used internally by the rts */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_PTHREAD
#include <pthread.h>
#endif

/******************************************************
 *  Locks and associated operations
 ******************************************************/
typedef union {
     volatile int memory;
     pthread_mutex_t posix;
}  Cilk_mutex;

extern void Cilk_mutex_init(CilkContext *const context, Cilk_mutex *lock);
extern void Cilk_mutex_wait(CilkContext *const context, CilkWorkerState *const ws, Cilk_mutex *lock);
extern void Cilk_mutex_signal(CilkContext *const context, Cilk_mutex *lock);
extern int  Cilk_mutex_try(CilkContext *const context, Cilk_mutex *lock);
extern void Cilk_mutex_destroy(CilkContext *const context, Cilk_mutex *lock);

/******************************************************
 *  Closures and associated operations
 ******************************************************/
/*
 * If a frame is at the top of its processor stack, these additional
 * informations are recorded.
 */
enum ClosureStatus { CLOSURE_RUNNING = 42, CLOSURE_SUSPENDED,
                     CLOSURE_RETURNING, CLOSURE_READY };

/* The order is important here. */
enum AbortStatus { ABORT_ALL = 30 , ALMOST_NO_ABORT, NO_ABORT};

/*
 * This structure contains all the data needed in order to execute an
 * inlet.  Some inlets are "complete", other are not.  "Incomplete"
 * inlets are still waiting for data.
 *
 * Each closure has a list of complete inlets, and a list
 * of incomplete inlets.  The list of incomplete inlets
 * is isomorphic to the list of outstanding children,
 * and is also used for that purpose.
 * Complete inlets are ready to go, but are waiting for
 * atomic access to the frame.
 */
struct InletClosure {
     struct Closure_s *this;                /* child that structure is for */
     struct InletClosure *next;
  
     void *receiver;
     void (*inlet) (CilkWorkerState *const, void *, void *, void *);
     int argsize;
     void *inlet_args;
};

/* 
 * the list of children is not distributed among
 * the children themselves, in order to avoid extra protocols
 * and locking.
 */
struct Closure_s {
     Cilk_mutex mutex;          /* mutual exclusion lock */
     WHEN_CILK_DEBUG(int mutex_owner;)

     int join_counter; /* number of outstanding children */
     enum ClosureStatus status;

     /*
      * return value (and its size), valid only
      * for CLOSURE_RETURNING closures
      */
     void *return_value;
     int return_size;

     struct InletClosure *complete_inlets;  /* runnable inlets */
     struct InletClosure *incomplete_inlets;
     /* also, list of outstanding children */

     struct Closure_s *parent; /* the parent closure */

     CilkStackFrame *frame;       /* rest of the closure */
     enum AbortStatus abort_status;
     CilkClosureCache *cache; /* see cilk.h */

     /*
      * stuff related to ready deque.  These fields
      * must be managed only by the queue manager in sched.c
      */
     struct Closure_s *next_ready;
     struct Closure_s *prev_ready;
     WHEN_CILK_DEBUG(int owner_ready_deque;)

     /* miscellanea */
     WHEN_CILK_DEBUG(unsigned int magic;)
     /* indicates that this closure was allocated with mallec */
     WHEN_CILK_DEBUG(int malloced;)

     /* critical path and work */
     Cilk_time cp;      
     Cilk_time work;

     CILK_CACHE_LINE_PAD;
};

/* internal malloc */
extern void
 Cilk_internal_malloc_per_worker_init(CilkWorkerState *const ws);
extern void
 Cilk_internal_malloc_per_worker_cleanup(CilkWorkerState *const ws);
extern void Cilk_internal_malloc_global_init(CilkContext *const context);
extern void Cilk_internal_malloc_global_cleanup(CilkContext *const context);
extern void Cilk_internal_malloc_global_terminate(CilkContext *const context);
extern void Cilk_internal_malloc_global_init_2(CilkContext *const /*context*/);
extern void Cilk_internal_malloc_global_terminate_2(CilkContext *const /*context*/);

/* barriers */
extern void Cilk_barrier(CilkWorkerState *const ws);
extern void Cilk_barrier_init(CilkContext *const context);
extern void Cilk_barrier_per_worker_init(CilkWorkerState *const ws);
void Cilk_barrier_terminate(CilkContext *const context);

/* scheduler */
extern void Cilk_scheduler_init(CilkContext *const context);
extern void Cilk_scheduler_terminate(CilkContext *const);
extern void Cilk_scheduler_per_worker_init(CilkWorkerState *const ws);
extern Closure *Cilk_create_initial_thread(
     CilkContext *const context,
     void (*import_main)(CilkWorkerState *const ws, void *args),
     void *args,
     int return_size);
extern void Cilk_scheduler(CilkWorkerState *const ws, Closure *t);
void Cilk_remove_and_free_closure_and_frame(
     CilkWorkerState *const ws, CilkStackFrame *f, int pn);
extern void Cilk_destroy_frame(
     CilkWorkerState *const ws, CilkStackFrame *f, size_t size);
extern void Cilk_worker_wait_for_invocation(CilkContext *const /*context*/, long /*self*/, int */*local_terminating*/);
extern void Cilk_worker_is_done(CilkContext *const /*context*/, int */*local_terminating*/);
extern void Cilk_scheduler_per_worker_terminate(CilkWorkerState *const /*ws*/);
extern void Cilk_scheduler_init_2(CilkContext *const /*context*/);
extern void Cilk_scheduler_terminate_2(CilkContext *const /*context*/);
extern void Cilk_wakeup_workers(CilkContext *const /*context*/);

/* exit protocol */
extern void Cilk_exit_from_user_main(
     CilkWorkerState *const ws, Closure *cl, int res);
extern void Cilk_really_exit(CilkWorkerState *const ws, int res);
extern void Cilk_really_exit_1(CilkWorkerState *const ws, int res);
/* Ofra and Sivan: changed Cilk_die to Cilk_die_external, 19 June 2003 */
extern void Cilk_die_external(CilkContext *const context, const char *fmt,...);

/* memory allocator */
extern void *Cilk_malloc(size_t size);
extern void *Cilk_malloc_fixed(size_t size);
extern void *Cilk_memalign(size_t alignment, size_t size);
extern void Cilk_free(void *s);
extern void *Cilk_calloc(size_t nelem, size_t elsize);
extern void *Cilk_realloc(void *s, size_t size);
extern void *Cilk_valloc(size_t size);

/* global scheduler state */
struct CilkGlobalState_s{
     Cilk_time critical_path;
     Cilk_time total_work;
     int done; /* current calculation */
     int terminating; /* Cilk_terminate was called */
     Cilk_mutex barrier_lock;
     volatile int barrier_counter;
     volatile int barrier_release;
     struct Cilk_im_descriptor 
          global_im_descriptor [CILK_INTERNAL_MALLOC_BUCKETS];
     struct Cilk_im_stats global_im_info;
     int im_allocated;
     int im_wasted;
     Cilk_mutex global_im_lock;
     char *global_pool_begin;
     char *global_pool_end;
     char **alloc_list;
     int   alloc_list_index;
     int   alloc_list_size;
     Cilk_mutex dprintf_lock;
     Cilk_mutex die_lock;
    CilkProcInfo invoke_main_sig[3];
     /*Children (threads) handling */
     pthread_t *tid;
     CilkChildParams *thrd_params_array;
     volatile  int   nothing_to_do;
     volatile  int   workers_are_done;
     /*pthread_mutex_t waiting_workers_mutex;*/
     pthread_cond_t  waiting_workers_cond;
     pthread_cond_t  wakeup_first_worker_cond;
     pthread_cond_t  wakeup_other_workers_cond;
     pthread_mutex_t workers_mutex;
     pthread_cond_t  workers_done_cond;
     volatile long   workers_done_counter;
};

/*
 * The ready deque.  Each ready queue contains a list of ready closures;
 * the last element of the deque may also be RUNNING or RETURNING
 */
typedef struct ReadyDeque {
     Cilk_mutex mutex;
     WHEN_CILK_DEBUG(int mutex_owner;)
     Closure *top, *bottom;
     CILK_CACHE_LINE_PAD;
} ReadyDeque;

/* handy syntactic sugar */
#define USE_SHARED(id) (ws->context->Cilk_global_state->id)
#define USE_SHARED1(id) (context->Cilk_global_state->id)


/* external malloc */
extern void Cilk_init_arena(size_t arenasize);

/* statistics */
extern void Cilk_stats_init(CilkContext *const context); /* Cilk_stats_init should be called even if there are no stats*/
#if CILK_STATS

enum {
     EVENT_THREAD,

     EVENT_STEAL_ATTEMPT,
     EVENT_STEAL,
     EVENT_STEAL_NO_DEKKER,
     EVENT_STEAL_EMPTY_DEQUE,
     EVENT_STEAL_RETURNING,
     EVENT_STEAL_ABORT,

     EVENT_ABORT_SLOW,
     EVENT_ABORT_STANDALONE,
     EVENT_ABORT_READY_SUSPEND,
     EVENT_ABORT_READY_RETURN,

     EVENT_SUSPEND,
     EVENT_CILK_SYNC,

     EVENT_EXCEPTION,
     EVENT_EXCEPTION_STEAL,
     EVENT_EXCEPTION_ABORT,
     EVENT_EXCEPTION_ABORT_RETURN,
     EVENT_EXCEPTION_ABORT_SUSPEND,
     EVENT_EXCEPTION_OTHER,

     EVENT_RETURN_SLOW,
     EVENT_PROVABLY_GOOD_STEAL,
     EVENT_RETURN_ENQUEUE,
     EVENT_POLL_INLETS,
     EVENT_CLOSURE_CREATE,

     EVENT_IM_ALLOC_BATCH,
     EVENT_IM_FREE_BATCH,

     EVENT_USER0,
     EVENT_USER1,
     EVENT_USER2,
     EVENT_USER3,
     EVENT_USER4,
     EVENT_USER5,
     EVENT_USER6,
     EVENT_USER7,
     EVENT_NTYPES  		/* this must be last */
};
     
extern void Cilk_event(CilkWorkerState *const ws, int type);
extern void Cilk_event_gathering_init(CilkContext *const context);
extern void Cilk_stats_terminate(CilkContext *const context);
extern void Cilk_print_rts_statistics(CilkContext *const context);
extern void Cilk_internal_malloc_print_statistics(CilkContext *const context);
extern void Cilk_increment_curr_stack_depth(CilkWorkerState *const ws);
extern void Cilk_decrement_curr_stack_depth(CilkWorkerState *const ws);
extern void Cilk_reset_stack_depth_stats(CilkWorkerState *const ws);


typedef struct Stats {
     long event_count[EVENT_NTYPES];
     unsigned long max_stack_depth;
     unsigned long curr_stack_depth;
     CILK_CACHE_LINE_PAD;
} StatsT;

#else

#define Cilk_event(a, b)
#define Cilk_stats_terminate(context)
#define Cilk_event_gathering_init(context)
#define Cilk_print_rts_statistics(context)
#define Cilk_internal_malloc_print_statistics(context)
#define Cilk_increment_curr_stack_depth(ws)
#define Cilk_decrement_curr_stack_depth(ws)
#define Cilk_reset_stack_depth_stats(ws)
#endif

/* timing */
#if CILK_TIMING
extern void Cilk_enter_state(CilkWorkerState *const ws, int state);
extern void Cilk_exit_state(CilkWorkerState *const ws, int state);
extern void Cilk_print_time_statistics(CilkContext *const context);
extern void Cilk_print_detailed_time_statistics(CilkContext *const context);
extern void Cilk_summarize_time_statistics(CilkContext *const context);
extern double Cilk_compute_work(CilkContext *const context);
extern void Cilk_timing_init(CilkContext *const context);
extern void Cilk_timing_terminate(CilkContext *const context);
extern void Cilk_time_gathering_init(CilkContext *const context);

enum {
     STATE_TOTAL,
     STATE_WORKING,
     STATE_WAITING_FOR_LOCK,
     STATE_STEALING,
     STATE_RETURNING,
     STATE_ABORT_RECURSIVE,
     STATE_USER0,                /* for the user */
     STATE_USER1,
     STATE_USER2,
     STATE_USER3,
     STATE_USER4,
     STATE_USER5,
     STATE_USER6,
     STATE_USER7,
     TIME_NSTATES		/* this must be last */
};

#define STATE_DEPTH 256
typedef struct StateInfo {
     volatile int state_stackp;
     volatile int state_stack[STATE_DEPTH];
     volatile Cilk_time state_stack_begintime[STATE_DEPTH];
     volatile Cilk_time statetime[TIME_NSTATES];
     volatile Cilk_time state_stack_begin_wall_time[STATE_DEPTH];
     volatile Cilk_time state_wall_time[TIME_NSTATES];
     volatile int stateentered[TIME_NSTATES];
     CILK_CACHE_LINE_PAD;
} StateInfoT;

#else

#define Cilk_enter_state(ws, state)
#define Cilk_exit_state(ws, state)
#define Cilk_print_time_statistics(context)
#define Cilk_print_detailed_time_statistics(context)
#define Cilk_summarize_time_statistics(context)
#define Cilk_compute_work(context) (0.0)
#define Cilk_timing_init(context)
#define Cilk_timing_terminate(context)
#define Cilk_time_gathering_init(context)

#endif				/* timing */

/*
 * Functions defined in the architecture-specific file and used
 * only by the runtime system.
 */
extern int Cilk_arch_specific_options(int i, int argc, char *argv[]);
extern void Cilk_arch_specific_init(void);
extern void Cilk_debug_init(CilkContext *const context);
extern void Cilk_debug_terminate(CilkContext *const context);
extern void Cilk_arch_specific_per_worker_init(void);
extern void Cilk_create_children(CilkContext *const context,
				 void (*child)(CilkChildParams*));
extern void Cilk_terminate_children(CilkContext *const context);
extern void Cilk_lower_priority(CilkWorkerState *const ws);
extern void Cilk_raise_priority(CilkWorkerState *const ws);
extern void Cilk_yield(CilkWorkerState *const ws);
extern int Cilk_partition_size(void);

/*
 * functions used by the scheduler and by other protocols
 * operating on the Closures tree
 */
extern Closure *Cilk_Closure_create_malloc(CilkContext *const context, CilkWorkerState *const ws);

/******************************************************
 *  Options processing
 ******************************************************/
struct Cilk_options_s
{
     int nproc;
     int stackdepth;
     int statlevel;
     int yieldslice;
     char *infofile_name;  /* this should really be arch-specific */
     int dump_core;
     int alloc_batch_size;
     int memory_locks;
     int pthread_stacksize; /* if positive, set the stacksize. */
     unsigned long pinned_mask;
};

/* command-line parser */
extern int Cilk_parse_command_line(
     Cilk_options *options, int *argc, char *argv[]);

#define CILK_DEFAULT_OPTIONS                    \
{                                               \
     1,                                         \
     CILK_DEFAULT_STACK_DEPTH,                  \
     0,                                         \
     0,                                         \
     "-",                                       \
     0,                                         \
     64,                                        \
     0,                                         \
     0,                                         \
     1024					\
}
