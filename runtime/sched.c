#include "../config.h"

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

/*
 * This file contains the scheduler and associated functions
 */

#include <cilk.h>
#include <cilk-internal.h>
#include <string.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/sched.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

#define CLOSURE_HEAD(cl)  (cl->cache->head)
#define CLOSURE_EXCEPTION(cl)  (cl->cache->exception)
#define CLOSURE_TAIL(cl)  (cl->cache->tail)
#define CLOSURE_STACK(cl) (cl->cache->stack)
#define CLOSURE_ABORT(cl) (cl->abort_status)

#define NOBODY (-1)   /* invalid processor number */

/* 
 * SCHEDULER LOCK DIAGRAM
 * 
 * locks must be grabbed in the following order:
 *
 * ReadyDeque lock
 * Closure lock
 *
 * Closure locks must be acquired downwards
 * (parent first, then children)
 *
 * See also PROTOCOLS for a more verbose description.
 */

/*************************************************************
 * Basic operations on closures
 *************************************************************/
static inline  void Closure_checkmagic(CilkWorkerState *const UNUSED(ws),
				       Closure *UNUSED(t))
{
     CILK_ASSERT(ws, t->magic == CILK_CLOSURE_MAGIC);
}

/* assert that t be locked by ourselves */
static inline void Closure_assert_ownership(CilkWorkerState *const UNUSED(ws),
					    Closure *UNUSED(t))
{
     CILK_ASSERT(ws, t->mutex_owner == ws->self);
}

/* assert that t be not locked */
static inline void Closure_assert_alienation(CilkWorkerState *const UNUSED(ws),
					     Closure *UNUSED(t))
{
     CILK_ASSERT(ws, t->mutex_owner != ws->self);
}

static inline void Closure_clean(CilkContext *const context,
				 Closure *t)
{
     Cilk_mutex_destroy(context, &t->mutex);
}

static inline void Closure_destroy(CilkWorkerState *const ws, Closure *t)
{
     Closure_checkmagic(ws, t);

     WHEN_CILK_DEBUG(t->magic = ~CILK_CLOSURE_MAGIC);
     CILK_ASSERT(ws, ! (t->malloced));
     Closure_clean(ws->context, t);
     Cilk_internal_free(ws, t, sizeof(Closure));
}

static inline void Closure_destroy_malloc(CilkWorkerState *const ws,
					  Closure *t)
{
     Closure_checkmagic(ws, t);

     WHEN_CILK_DEBUG(t->magic = ~CILK_CLOSURE_MAGIC);
     CILK_ASSERT(ws, (t->malloced));
     Closure_clean(ws->context, t);
     Cilk_free(t);
}

static inline void Closure_lock(CilkWorkerState *const ws, Closure *t)
{
     Closure_checkmagic(ws, t);
     Cilk_mutex_wait(ws->context, ws, &(t->mutex));
     WHEN_CILK_DEBUG(t->mutex_owner = ws->self);
}

static inline void Closure_unlock(CilkWorkerState *const ws, Closure *t)
{
     Closure_checkmagic(ws, t);
     Closure_assert_ownership(ws, t);
     WHEN_CILK_DEBUG(t->mutex_owner = -1);
     Cilk_mutex_signal(ws->context, &(t->mutex));
}

/***********************************************************
 *  Internal random number generator.
 ***********************************************************/
static unsigned int rts_rand(CilkWorkerState *const ws)
{
     ws->rand_next = ws->rand_next * 1103515245 + 12345;
     return (ws->rand_next >> 16);
}

static void rts_srand(CilkWorkerState *const ws, unsigned int seed)
{
     ws->rand_next = seed;
}

/*********************************************************
 * Management of ReadyDeques
 *********************************************************/

/* initialize the deques */
static void create_deques(CilkContext *const context)
{
     int i;

     INIT_PARAMETER1(deques,
	            Cilk_malloc_fixed(USE_PARAMETER1(active_size) *
				      sizeof(ReadyDeque)));
     CILK_CHECK(USE_PARAMETER1(deques),
	        (context, NULL, "failed to allocate deques\n"));

     for(i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  Cilk_mutex_init(context, &USE_PARAMETER1(deques)[i].mutex);
     }
}

static void init_deques(CilkContext *const context)
{
     int i;

     for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  USE_PARAMETER1(deques)[i].top = (Closure *) NULL;
	  USE_PARAMETER1(deques)[i].bottom = (Closure *) NULL;
	  WHEN_CILK_DEBUG(USE_PARAMETER1(deques)[i].mutex_owner = NOBODY);
     }
}

static void free_deques(CilkContext *const context)
{
     int i;

     for(i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  Cilk_mutex_destroy(context, &USE_PARAMETER1(deques)[i].mutex);
     }

     Cilk_free(USE_PARAMETER1(deques));
}

/* assert that pn's deque be locked by ourselves */
static inline void deque_assert_ownership(CilkWorkerState *const UNUSED(ws), int UNUSED(pn))
{
     CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].mutex_owner == ws->self);
}

static inline void deque_assert_alienation(CilkWorkerState *const UNUSED(ws), int UNUSED(pn))
{
     CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].mutex_owner != ws->self);
}

static inline void deque_lock(CilkWorkerState *const ws, int pn)
{
     Cilk_mutex_wait(ws->context, ws, &USE_PARAMETER(deques)[pn].mutex);
     WHEN_CILK_DEBUG(USE_PARAMETER(deques)[pn].mutex_owner = ws->self);
}

static inline void deque_unlock(CilkWorkerState *const ws, int pn)
{
     WHEN_CILK_DEBUG(USE_PARAMETER(deques)[pn].mutex_owner = NOBODY);
     Cilk_mutex_signal(ws->context, &USE_PARAMETER(deques)[pn].mutex);
}

/* 
 * functions that add/remove elements from the top/bottom
 * of deques
 */
static Closure *deque_xtract_top(CilkWorkerState *const ws, int pn)
{
     Closure *cl;

     deque_assert_ownership(ws, pn);

     cl = USE_PARAMETER(deques)[pn].top;
     if (cl) {
	  CILK_ASSERT(ws, cl->owner_ready_deque == pn);
	  USE_PARAMETER(deques)[pn].top = cl->next_ready;
	  if (cl == USE_PARAMETER(deques)[pn].bottom) {
	       CILK_ASSERT(ws, cl->next_ready == (Closure *) NULL);
	       USE_PARAMETER(deques)[pn].bottom = (Closure *) NULL;
	  } else {
	       CILK_ASSERT(ws, cl->next_ready);
	       (cl->next_ready)->prev_ready = (Closure *) NULL;
	  }
	  WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
     } else {
	  CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].bottom == (Closure *) NULL);
     }
     
     return cl;
}

static Closure *deque_peek_top(CilkWorkerState *const ws, int pn)
{
     Closure *cl;

     deque_assert_ownership(ws, pn);

     cl = USE_PARAMETER(deques)[pn].top;
     if (cl) {
	  CILK_ASSERT(ws, cl->owner_ready_deque == pn);
     } else {
	  CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].bottom == (Closure *) NULL);
     }

     return cl;
}

static Closure *deque_xtract_bottom(CilkWorkerState *const ws, int pn)
{
     Closure *cl;

     deque_assert_ownership(ws, pn);

     cl = USE_PARAMETER(deques)[pn].bottom;
     if (cl) {
	  CILK_ASSERT(ws, cl->owner_ready_deque == pn);
	  USE_PARAMETER(deques)[pn].bottom = cl->prev_ready;
	  if (cl == USE_PARAMETER(deques)[pn].top) {
	       CILK_ASSERT(ws, cl->prev_ready == (Closure *) NULL);
	       USE_PARAMETER(deques)[pn].top = (Closure *) NULL;
	  } else {
	       CILK_ASSERT(ws, cl->prev_ready);
	       (cl->prev_ready)->next_ready = (Closure *) NULL;
	  }

	  WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
     } else {
	  CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].top == (Closure *) NULL);
     }
     
     return cl;
}

static Closure *deque_peek_bottom(CilkWorkerState *const ws, int pn)
{
     Closure *cl;

     deque_assert_ownership(ws, pn);

     cl = USE_PARAMETER(deques)[pn].bottom;
     if (cl) {
	  CILK_ASSERT(ws, cl->owner_ready_deque == pn);
     } else {
	  CILK_ASSERT(ws, USE_PARAMETER(deques)[pn].top == (Closure *) NULL);
     }
     
     return cl;
}

#if 0   
/*
 * Unused code, which may be useful if you hack the scheduler.
 */
static void deque_add_top(CilkWorkerState *const ws, Closure *cl, int pn)
{
     deque_assert_ownership(ws, pn);
     CILK_ASSERT(ws, cl->owner_ready_deque == NOBODY);

     cl->next_ready = USE_PARAMETER(deques)[pn].top;
     cl->prev_ready = (Closure *)NULL;
     USE_PARAMETER(deques)[pn].top = cl;
     WHEN_CILK_DEBUG(cl->owner_ready_deque = pn);
     
     if (USE_PARAMETER(deques)[pn].bottom) {
	  CILK_ASSERT(ws, cl->next_ready);
	  (cl->next_ready)->prev_ready = cl;
     } else {
	  USE_PARAMETER(deques)[pn].bottom = cl;
     }
}
#endif

static void deque_add_bottom(CilkWorkerState *const ws, Closure *cl, int pn)
{
     deque_assert_ownership(ws, pn);
     CILK_ASSERT(ws, cl->owner_ready_deque == NOBODY);

     cl->prev_ready = USE_PARAMETER(deques)[pn].bottom;
     cl->next_ready = (Closure *)NULL;
     USE_PARAMETER(deques)[pn].bottom = cl;
     WHEN_CILK_DEBUG(cl->owner_ready_deque = pn);
     
     if (USE_PARAMETER(deques)[pn].top) {
	  CILK_ASSERT(ws, cl->prev_ready);
	  (cl->prev_ready)->next_ready = cl;
     } else {
	  USE_PARAMETER(deques)[pn].top = cl;
     }
}

static inline void deque_assert_is_bottom(CilkWorkerState *const ws, Closure *UNUSED(t))
{
     deque_assert_ownership(ws, ws->self);
     CILK_ASSERT(ws, t == deque_peek_bottom(ws, ws->self));
}

/* Remove closure for frame f from bottom of pn's deque and free them */
void Cilk_remove_and_free_closure_and_frame(CilkWorkerState *const ws,
					    CilkStackFrame *f, int pn)
{
     Closure *t;

     deque_lock(ws, pn);
     t = deque_xtract_bottom(ws, pn);
     CILK_ASSERT(ws, t->frame == f);
     deque_unlock(ws, pn);
     Cilk_free(f);
     Closure_destroy_malloc(ws, t);
}

/**************************************************
 * Management of CilkProcInfo's
 **************************************************/
/*
 * Returns a pointer to the CilkProcInfoT structure describing entry <entry>
 * of procedure whose signature is p.
 */
static inline CilkProcInfo *get_CilkProcInfo(CilkWorkerState *const UNUSED(ws),
					     CilkProcInfo * p, int entry)
{
     CILK_ASSERT(ws, entry > 0);
     return p + entry;
}

/*
 * Returns a pointer to the slow version for a procedure
 * whose signature is p.
 */
static void (*get_proc_slow(CilkProcInfo *p)) () {
     return p[0].inlet;
}

/********************************************
 * Closure management
 ********************************************/
static inline int Closure_at_top_of_stack(Closure *cl)
{
     return (CLOSURE_HEAD(cl) + 1 == CLOSURE_TAIL(cl));
}

static inline int Closure_has_children(Closure *cl)
{
     return (cl->join_counter != 0);
}

static inline void Closure_init(CilkContext *const context, CilkWorkerState *const UNUSED(ws), Closure *new)
{
     Cilk_mutex_init(context, &new->mutex);

     new->cp = (Cilk_time) 0;
     new->work = (Cilk_time) 0;

     new->complete_inlets = (struct InletClosure *) NULL;
     new->incomplete_inlets = (struct InletClosure *) NULL;
     new->next_ready = (Closure *) NULL;
     new->prev_ready = (Closure *) NULL;
     CLOSURE_ABORT(new) = NO_ABORT;
     new->return_size = 0;
     new->return_value = (void *)0;

     WHEN_CILK_DEBUG({
	  new->magic = CILK_CLOSURE_MAGIC;
	  new->owner_ready_deque = NOBODY;
	  new->mutex_owner = NOBODY;
     });

     Cilk_event(ws, EVENT_CLOSURE_CREATE);
}

static Closure *Closure_create(CilkWorkerState *const ws)
{
     Closure *new = Cilk_internal_malloc(ws, sizeof(Closure));

     CILK_CHECK(new, (ws->context, ws, "can't allocate closure\n"));
     WHEN_CILK_DEBUG(new->malloced = 0;)

     Closure_init(ws->context,ws, new);

     return new;
}

Closure *Cilk_Closure_create_malloc(CilkContext *const context, CilkWorkerState *const ws)
{
     Closure *new = Cilk_malloc(sizeof(Closure));

     CILK_CHECK(new, (context, ws, "can't allocate closure\n"));
     WHEN_CILK_DEBUG(new->malloced = 1;)

     Closure_init(context, ws, new);

     return new;
}

/* 
 * if (entry >= 0)
 *      get the inlet according to the entry
 * else
 *      get the inlet from the frame
 */
static struct InletClosure 
*make_incomplete_inlet_closure(CilkWorkerState *const ws,
			       Closure *parent, 
			       int entry)
{
     struct InletClosure *p;
     CilkStackFrame *f;
     CilkProcInfo *info;

     p = (struct InletClosure *)
	 Cilk_internal_malloc(ws, sizeof(struct InletClosure));

     f = parent->frame;

     if (entry >= 0) 
	  /* get the special exit closure */
	  info = get_CilkProcInfo(ws, f->sig, entry);
     else
	  info = get_CilkProcInfo(ws, f->sig, f->entry);
     
     /* if no index is statically specified, get it from the frame */
     if (info->index == -1)
	  p->receiver = f->receiver;
     else
          p->receiver = ((char *)f) + info->index;

     /* take care of inlets */
     if (info->inlet != NULL) {
	  /* get arguments of inlet out of frame and into closure */
          p->inlet = info->inlet;
          p->argsize = info->argsize;
	  p->inlet_args = Cilk_internal_malloc(ws, p->argsize);

	  memcpy((void *) p->inlet_args,
		 (char *) f + info->argindex, p->argsize);
     } else {
	  /*
	   * in the non-inlet case, we use the inlet_args field
	   * to hold the return value
	   */
	  p->inlet = NULL;
	  p->argsize = info->size;
	  p->inlet_args = Cilk_internal_malloc(ws, p->argsize);
     }
     
     return p;
}

/* 
 * only the scheduler is allowed to alter the closure
 * tree.  Consequently, these operations are private
 */
static void Closure_add_child(CilkWorkerState *const ws,
			      Closure *parent,
			      Closure *child)
{
     struct InletClosure *p;

     Closure_checkmagic(ws, parent);
     Closure_assert_ownership(ws, parent);
     Closure_checkmagic(ws, child);
     Closure_assert_alienation(ws, child);

     p = make_incomplete_inlet_closure(ws, parent, -1);

     /*
      * semantic overloading: the inlet closure also contains
      * the list of children.
      */
     p->this = child;

     /* splice into list of unreturned children in parent */
     p->next = parent->incomplete_inlets;
     parent->incomplete_inlets = p;
}

static struct InletClosure *Closure_remove_child(CilkWorkerState *const ws,
						 Closure *parent,
						 Closure *child)
{
     struct InletClosure **q;
     struct InletClosure *p;

     Closure_checkmagic(ws, parent);
     Closure_checkmagic(ws, child);
     Closure_assert_ownership(ws, parent);

     for (q = &parent->incomplete_inlets; *q; q = &((*q)->next)) {
	  p = *q;
	  if (p->this == child) {
	       *q = p->next;
	       goto found;
	  }
     }

     Cilk_die_internal(ws->context, ws, "BUG in Closure_remove_child\n");
     p = NULL;

   found:
     return p;
}

/*******************************************
 * Exceptions
 *******************************************/
/*
 * Cilk supports a clever way to send an interruption/exception to a
 * running worker, without interrupting its normal way of working. The
 * idea is as follows.
 *
 * The local worker operates at the bottom of the frame stack, using a
 * stack pointer T (tail).  The exception mechanism maintains another
 * pointer E (exception).  The worker polls the value of E when
 * decrementing T.  Whenever T becomes less or equal than E, the
 * worker calls the exception handler of the runtime system, that does
 * all the necessary bookkeeping.
 *
 * Exceptions can be sent only to running closures.  The exception
 * pointer is always greater or equal than the head pointer (the index
 * of the first non-stolen frame on the stack)
 */

/* value of E bigger than any possible value of T */
#ifdef __GNUC__
#define EXCEPTION_INFINITY (volatile CilkStackFrame**)(-1LL)
#else
 #if defined(__mips)
 /* must be mipspro */
 /* for o64 ABI (uses the LP memory model) */
 #define EXCEPTION_INFINITY (volatile CilkStackFrame**)(-1L)
 #else
   #error ohops
#endif
#endif

static inline void increment_exception_pointer(CilkWorkerState *const ws,
						Closure *cl)
{
     Closure_assert_ownership(ws, cl);

     CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

     if (CLOSURE_EXCEPTION(cl) != EXCEPTION_INFINITY) {
	  ++CLOSURE_EXCEPTION(cl);
	  
	  /* make sure the exception is visible, before we continue */
	  Cilk_fence();
     }
}

static inline void decrement_exception_pointer(CilkWorkerState *const ws,
					       Closure *cl)
{
     Closure_assert_ownership(ws, cl);

     CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

     if (CLOSURE_EXCEPTION(cl) != EXCEPTION_INFINITY)
	  --CLOSURE_EXCEPTION(cl);
}

static inline void reset_exception_pointer(CilkWorkerState *const ws,
					   Closure *cl)
{
     Closure_assert_ownership(ws, cl);

     CLOSURE_EXCEPTION(cl) = CLOSURE_HEAD(cl);
}

static inline void signal_immediate_exception(CilkWorkerState *const ws,
					      Closure *cl)
{
     Closure_assert_ownership(ws, cl);

     CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

     CLOSURE_EXCEPTION(cl) = EXCEPTION_INFINITY;

     /* make sure the exception is visible, before we continue */
     Cilk_fence();
}

/********************************************************
 * Abort-related stuff 
 ********************************************************/

/* 
 * Abort is implemented by adding an abort_status in the closure
 * structure.  
 *    NO_ABORT:         the closure is not aborted;
 *    ALMOST_NO_ABORT:  the closure is being aborted as parent;
 *    ABORT_ALL:        the closure is being aborted as child;
 *
 * When abort happens, we want to make sure that already spawned
 * but not returned subcomputations stop working, and "return" as 
 * quickly as possible.  This is handled in the exception handler.
 * Also,  the inlets of a closure that is being aborted, either as
 * parent or child, should not be executed.  We handle this in
 * function apply_inlets.
 *
 * Abort is signaled either in running slow code, or in apply_inlets.
 * To signal an abort, the process that discovers the abort grabs
 * the parent lock and recursively goes down the tree to signal 
 * it's children by setting their abort_status to ABORT_ALL, and 
 * signal immediate exception to all running children.  The parent
 * is set to ALMOST_NO_ABORT, so that the complete inlets would not
 * be executed.  
 *
 * Before we signal an abort from the parent, we check to see that, if
 * the parent is running and the worker is not on the top of the stack,
 * then, we promote the frame second to the top to be a complete closure,
 * put the parent back to the ready deque, and signal abort from the parent.
 * In this way, the parent will start execution at the right entry point.
 * 
 * The stealing protocol does not steal a closure if it is being 
 * aborted.  Inlets are also not applied to closures that are aborting.
 *  
 * Once the abort is signaled, we let the normal protocol to handle
 * the abort:
 *
 *    We let the exception handler do the work to abort running closure.
 *    (see comments in exception_handler).
 *
 *    For a ready closure, we check whether it's being aborted before 
 *    set up to execute it.  If it is being aborted, we free the last 
 *    frame and let it return.
 *
 *    For a returning closure, if it is being aborted, we do not append 
 *    the corresponding inlet to the complete inlet list, but discard
 *    the return value.
 *
 *    For a suspended closure, we do the same provably_good_steal.  
 *    If it is being aborted, we will discover it we it's state is ready.  
 *    
 *
 */

static inline int get_frame_size(CilkStackFrame *f) {
     return (f->sig)[0].index; /* compiler put the frame size here. */
}

static inline int Closure_being_aborted(CilkWorkerState *const ws, Closure *cl)
{
     Closure_assert_ownership(ws, cl);
     return (CLOSURE_ABORT(cl) != NO_ABORT);
}

static inline void signal_abort(Closure *cl, enum AbortStatus s)
{
     if (s < CLOSURE_ABORT(cl))
	  CLOSURE_ABORT(cl) = s;
}

static inline void reset_abort(Closure *cl)
{
     CLOSURE_ABORT(cl) = NO_ABORT;
}

static inline void maybe_reset_abort(Closure *cl)
{
     if (CLOSURE_ABORT(cl) != ABORT_ALL)
	  reset_abort(cl);
}


static void recursively_signal_abort(CilkWorkerState *const ws,
				     Closure *cl, enum AbortStatus s)
{
     struct InletClosure *p;

     Closure_assert_ownership(ws, cl);
     signal_abort(cl, s);
     if (cl->status == CLOSURE_RUNNING)
	  signal_immediate_exception(ws, cl);

     /* walk the list of children */
     for (p = cl->incomplete_inlets; p; p = p->next) {
	  Closure_lock(ws, p->this);
	  recursively_signal_abort(ws, p->this, ABORT_ALL);
	  Closure_unlock(ws, p->this);
     }
}


static Closure *abort_ready_closure(CilkWorkerState *const ws, Closure *cl)
{
     /*
      * abort a ready closure. There are two cases: if it has
      * children, suspend it.  Else free the frame associated
      * with the closure and make the closure return
      */
     CILK_ASSERT(ws, CLOSURE_ABORT(cl) == ABORT_ALL);

     if (Closure_has_children(cl)) {
	  cl->status = CLOSURE_SUSPENDED;
	  Cilk_event(ws, EVENT_ABORT_READY_SUSPEND);
	  return (Closure *)0;
     } else {
	  Cilk_destroy_frame(ws, cl->frame, get_frame_size(cl->frame));
	  Cilk_event(ws, EVENT_ABORT_READY_RETURN);
	  cl->status = CLOSURE_RETURNING;
	  return cl;
     }
}

/***********************************************************
 *         Work-stealing and related functions
 ***********************************************************/
/* 
 * For stealing we use a Dekker-like protocol, that achieves
 * mutual exclusion using shared memory.
 *
 * Thief:
 *   lock
 *   E++          ; signal intention to steal, sending an exception
 *   (implicit MEMBAR)
 *   If (H >= T)  ; if we can steal
 *     E--        ; retract the exception
 *     give up
 *   steal (that is, H++, etc)
 *   unlock
 *
 * The victim does the normal exception-handling mechanism:
 *
 *   T--
 *   --MEMBAR-- ensure T write occurs before E is read.
 *   if (E>=T)
 *     
 *
 */

/*
 * Do the thief part of Dekker's protocol.  Return 1 upon success,
 * 0 otherwise.  The protocol fails when the victim already popped
 * T so that E=T.
 */
static int do_dekker_on(CilkWorkerState *const ws, Closure *cl)
{
     Closure_assert_ownership(ws, cl);

     increment_exception_pointer(ws, cl);
     Cilk_membar_StoreLoad(); 

     if ((CLOSURE_HEAD(cl) + 1) >= CLOSURE_TAIL(cl)) {
	  decrement_exception_pointer(ws, cl);
	  return 0;
     }

     return 1;
}

static void Closure_make_ready(Closure *cl)
{
     cl->cache = (CilkClosureCache *)NULL;
     cl->status = CLOSURE_READY;
}

/*
 * promote the child frame of parent to a full closure.
 * Detach the parent and return it.
 *
 * Assumptions: the parent is running on victim, and we own
 * the locks of both parent and deque[victim].
 * The child keeps running on the same cache of the parent.
 * The parent's join counter is incremented.
 *
 * In order to promote a child frame to a closure,
 * the parent's frame must be the last in its ready queue.
 *
 * Returns the child.
 */
static Closure *promote_child(CilkWorkerState *const ws,
			      Closure *parent, int victim)
{
     Closure *child = Closure_create(ws);

     Closure_assert_ownership(ws, parent);
     deque_assert_ownership(ws, victim);
     CILK_ASSERT(ws, parent->status == CLOSURE_RUNNING);
     CILK_ASSERT(ws, parent->owner_ready_deque == victim);
     CILK_ASSERT(ws, parent->next_ready == (Closure *) NULL);

     /* can't promote until we are sure nobody works on the frame */
     CILK_ASSERT(ws, CLOSURE_HEAD(parent) <= CLOSURE_EXCEPTION(parent));

     /* update the various fields */
     child->parent = parent;
     child->join_counter = 0;
     child->cache = parent->cache;
     child->status = CLOSURE_RUNNING;

     /* setup the frame of the child closure */
     CLOSURE_HEAD(child)++;
     child->frame = (CilkStackFrame*)*CLOSURE_HEAD(child);

     /* insert the closure on the victim processor's deque */
     deque_add_bottom(ws, child, victim);

     /* at this point the child can be freely executed */
     return child;
}

/*
 * Finishes the promotion process.  The child is already fully promoted
 * and requires no more work (we only use the given pointer to identify
 * the child).  This function does some more work on the parent to make
 * the promotion complete.
 */
void finish_promote(CilkWorkerState *const ws, 
		    Closure *parent, Closure *child)
{
     Closure_assert_ownership(ws, parent);
     Closure_assert_alienation(ws, child);
     
     /* the parent is still locked; we still need to update it */
     /* join counter update */
     parent->join_counter++;

     /* register this child */
     Closure_add_child(ws, parent, child);

     /* Make the parent ready (also invalidates the cache) */
     Closure_make_ready(parent);

     return;
}

/*
 * stealing protocol.  Tries to steal from the victim; returns a
 * stolen closure, or NULL if none.
 */
static Closure *Closure_steal(CilkWorkerState *const ws, int victim)
{
     Closure *res = (Closure *) NULL;
     Closure *cl, *child;

     Cilk_event(ws, EVENT_STEAL_ATTEMPT);

     deque_lock(ws, victim);

     cl = deque_peek_top(ws, victim);

     if (cl) {
	  Closure_lock(ws, cl);
	  
	  /* do not steal aborting closures */
	  if (Closure_being_aborted(ws, cl)) {
	       Cilk_event(ws, EVENT_STEAL_ABORT);
	       goto give_up;
	  }

	  switch (cl->status) {
	      case CLOSURE_READY:
		   Cilk_event(ws, EVENT_STEAL);
		   res = deque_xtract_top(ws, victim);
		   CILK_ASSERT(ws, cl == res);

		   /* it is ok to swap these two */
		   Closure_unlock(ws, cl);
		   deque_unlock(ws, victim);
		   break;

	      case CLOSURE_RUNNING:
		   /* send the exception to the worker */
		   if (do_dekker_on(ws, cl)) {
			/* 
			 * if we could send the exception, promote
			 * the child to a full closure, and steal
			 * the parent
			 */
		        child = promote_child(ws, cl, victim);

			/* detach the parent */
			res = deque_xtract_top(ws, victim);
			CILK_ASSERT(ws, cl == res);
			deque_unlock(ws, victim);

			/* 
			 * at this point, more steals can happen
			 * from the victim.
			 */
			finish_promote(ws, cl, child);
		        Closure_unlock(ws, cl);
			Cilk_event(ws, EVENT_STEAL);
		   } else {
			Cilk_event(ws, EVENT_STEAL_NO_DEKKER);
			goto give_up;
		   }
		   break;
			
	      case CLOSURE_SUSPENDED:
		   Cilk_die_internal(ws->context, ws, "Bug: suspended closure in ready deque\n");
		   break;

	      case CLOSURE_RETURNING:
		   /* ok, let it leave alone */
		   Cilk_event(ws, EVENT_STEAL_RETURNING);

		   /* 
		    * MUST unlock the closure before the queue;
		    * see rule D in the file PROTOCOLS
		    */
	  give_up:
		   Closure_unlock(ws, cl);
		   deque_unlock(ws, victim);
		   break;

	      default:
		   Cilk_die_internal(ws->context, ws, "Bug: unknown closure status\n");
	  }

     } else {
          deque_unlock(ws, victim);
	  Cilk_event(ws, EVENT_STEAL_EMPTY_DEQUE);
     }

     return res;
}

/********************************************************
 * Functions that take care of remote argument sending.
 ********************************************************/

/*
 * signal_abort_from_inlet:
 *
 * Signal all closures that need to be aborted from the parent <cl>.
 * If the parent is running and not working on the top of the 
 * stackframe, then, the current work is a spawn and should be aborted
 * as well.  In this case, we promote the child frame to a closure,
 * detach the parent, put the parent on the ready deque, and abort from
 * the parent.
 */
static void signal_abort_from_inlet(CilkWorkerState *const ws, Closure *cl)
{
     Closure *child;
     Closure_assert_ownership(ws, cl);

     if (cl->status == CLOSURE_RUNNING &&
	 !Closure_at_top_of_stack(cl)) {
	  /* 
	   * if closure is running and the frame is not on the top,
	   * then, poll_inlets is called in exception_handler.  So,
	   * the worker "ws->self" should own ready deque.  
	   */
	  deque_assert_ownership(ws, ws->self);

	  /* promote the child frame, and detach the parent from deque */
	  child = promote_child(ws, cl, ws->self);
	  finish_promote(ws, cl, child);
	  
	  /*
	   * Now, the stackframe the exception handler is working on
	   * belongs to the child, not the parent.  Therefore, the
	   * exception handler should get the child from the deque
	   * after calling poll_inlets 
	   */
     }

     /* do not abort the frame at the top of the stack */
     Cilk_enter_state(ws, STATE_ABORT_RECURSIVE);
     recursively_signal_abort(ws, cl, ALMOST_NO_ABORT);
     Cilk_exit_state(ws, STATE_ABORT_RECURSIVE);
}

/*
 * apply_inlet:
 *
 * Call inlet function only if the closure is not being aborted.  Use
 * abort_flag to check if abort happens when inlet function is
 * executed.  If abort occurs, signal all closures that need to be
 * aborted.
 */
static void apply_inlet(CilkWorkerState *const ws,
			Closure *cl, struct InletClosure *i)
{
     void *receiver = i->receiver;
     int argsize = i->argsize;
     void (*inlet) (CilkWorkerState *const, void *, void *, void *) = i->inlet;
     CilkStackFrame *frame = cl->frame;
     void *inlet_args = i->inlet_args;

     Closure_assert_ownership(ws, cl);

     /* 
      * do not apply inlets to aborting closures --- these inlets
      * are supposed to be killed
      */
     if (!Closure_being_aborted(ws, cl)) {
	  ws->abort_flag = 0;

	  if (inlet) {
	       /* run the inlet */
	       inlet(ws, frame, inlet_args, receiver);
	  } else {
	       /* 
		* default inlet: copy just-returned value into frame 
		*/
	       if (argsize) {
		    memcpy(receiver, inlet_args, argsize);
	       }

	       /* 
		* TODO: make a little stub that does the memcpy.
		* (Probably the resulting code will be messier than
		*  just doing the check here)
		*/
	  }

	  if (ws->abort_flag) 
	       signal_abort_from_inlet(ws, cl);
     }

     /* free the arguments to the inlet, and the inlet structure itself */
     Cilk_internal_free(ws, inlet_args, argsize);
     Cilk_internal_free(ws, i, sizeof(struct InletClosure));
}

/* walk the list of inlets and call apply_inlets on all of them */
static void poll_inlets(CilkWorkerState *const ws, Closure *cl)
{
     struct InletClosure *i;

     Closure_assert_ownership(ws, cl);
     if (cl->status == CLOSURE_RUNNING &&
	 !Closure_at_top_of_stack(cl))
	  /* 
	   * If we get here, poll_inlets has been called by
	   * the exception handler 
	   */
	  deque_assert_ownership(ws, ws->self);

     Cilk_event(ws, EVENT_POLL_INLETS);

     for (i = cl->complete_inlets; i; i = cl->complete_inlets) {
	  cl->complete_inlets = i->next;

	  /*
	   * apply_inlets may change the ready deque. See comments in
	   * apply_inlet for details 
	   */

	  apply_inlet(ws, cl, i); 
     }
     
     /* 
      * Reset the abort status after all incoming inlets have been
      * executed/killed
      */
     if (CLOSURE_ABORT(cl) == ALMOST_NO_ABORT)
	  reset_abort(cl);
}

static void complete_and_enque_inlet(CilkWorkerState *const ws,
				     Closure *parent, Closure *child, 
				     struct InletClosure *i)
{
     Closure_assert_ownership(ws, parent);
     Cilk_event(ws, EVENT_RETURN_ENQUEUE);

     /*
      * Install value child is returning into inlet structure.
      */
     CILK_COMPLAIN
	  (child->return_size <= i->argsize, 
	   (ws->context, ws,
	    "Cilk runtime system: invalid size of a return-ed value.\n"
	    "Either some internal Cilk data structure is corrupted,\n"
	    "or you have inconsistent prototypes across different files.\n"
	    "(E.g., cilk int foo(...)  and  cilk double foo(...))\n"));

     if (child->return_size) {
          CILK_ASSERT(ws, i->inlet_args != NULL);
          memcpy(i->inlet_args, child->return_value, child->return_size);
	  Cilk_internal_free(ws, child->return_value, child->return_size);
	  WHEN_CILK_DEBUG(child->return_value = NULL);
	  WHEN_CILK_DEBUG(child->return_size = 0);
     }

     if (Closure_being_aborted(ws, child)) {
	  Cilk_internal_free(ws, i->inlet_args, i->argsize);
	  Cilk_internal_free(ws, i, sizeof(struct InletClosure));
     } else {
	  i->next = parent->complete_inlets;
	  parent->complete_inlets = i;
     }
}

static Closure *provably_good_steal_maybe(CilkWorkerState *const ws, Closure *parent)
{
     Closure *res = (Closure *) NULL;

     Closure_assert_ownership(ws, parent);

     if (!Closure_has_children(parent) &&
	 parent->status == CLOSURE_SUSPENDED) {
	  /* do a provably-good steal; this is *really* simple */
	  res = parent;

	  poll_inlets(ws, parent);

	  Cilk_event(ws, EVENT_PROVABLY_GOOD_STEAL);

	  /* debugging stuff */
	  CILK_ASSERT(ws, parent->owner_ready_deque == NOBODY);
	  Closure_make_ready(parent);
     }

     return res;
}

/*
 * Return protocol.
 *
 * This protocol deposits the return value of a subcomputation (child)
 * into the appropriate place (parent).  This function returns a
 * closure to be executed next, or NULL if none.
 * The child must not be locked by ourselves, and be in no deque.
 */
static Closure *Closure_return(CilkWorkerState *const ws, Closure *child)
{
     Closure *parent;
     Closure *res = (Closure *) NULL;
     struct InletClosure *inlet;

     CILK_ASSERT(ws, child);
     CILK_ASSERT(ws, child->join_counter == 0);
     CILK_ASSERT(ws, child->status == CLOSURE_RETURNING);
     CILK_ASSERT(ws, child->owner_ready_deque == NOBODY);
     Closure_assert_alienation(ws, child);

     parent = child->parent;
     CILK_ASSERT(ws, parent);

     /* 
      * at this point the status is as follows: the child is in
      * no deque and unlocked.  However, the child is still
      * in the children list of the parent.  Since the child
      * is not locked, we can safely grab the parent's lock.
      */
     Closure_lock(ws, parent);

     CILK_ASSERT(ws, parent->status != CLOSURE_RETURNING);
     CILK_ASSERT(ws, parent->frame);
     CILK_ASSERT(ws, parent->frame->magic == CILK_STACKFRAME_MAGIC);

     inlet = Closure_remove_child(ws, parent, child);

     /*
      * the lock is not really necessary, since if rule D
      * is obeyed no one can have a pointer to child at this
      * point.  But, better be paranoid...
      */
     Closure_lock(ws, child);

     complete_and_enque_inlet(ws, parent, child, inlet);

     /* tell the parent to poll this inlet (it may be aborting) */
     if (parent->status == CLOSURE_RUNNING)
	  signal_immediate_exception(ws, parent);
     else {
	  /* run them yourself */
	  poll_inlets(ws, parent);
     }

     /* update critical path and work */
     WHEN_CILK_TIMING({
	  if (ws->cp_hack > parent->cp)
	       parent->cp = ws->cp_hack;
	  parent->work += ws->work_hack;
     });
	
     /* 
      * the two fences ensure dag consistency (Backer)
      */
     CILK_ASSERT(ws, parent->join_counter > 0);
     Cilk_fence();
     --parent->join_counter;
     Cilk_fence();

     res = provably_good_steal_maybe(ws, parent);

     Closure_unlock(ws, parent);

     /* now the child is no longer needed */
     Closure_unlock(ws, child);
     Closure_destroy(ws, child);

     return res;
}

/*
 * suspend protocol 
 */
static void Closure_suspend(CilkWorkerState *const ws, Closure *cl)
{
     Closure *cl1;

     Closure_checkmagic(ws, cl);
     Closure_assert_ownership(ws, cl);
     deque_assert_ownership(ws, ws->self);

     CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);
     cl->status = CLOSURE_SUSPENDED;

     Cilk_event(ws, EVENT_SUSPEND);
     cl1 = deque_xtract_bottom(ws, ws->self);
     USE_UNUSED(cl1); /* prevent warning when ASSERT is defined as nothing */

     CILK_ASSERT(ws, cl == cl1);
}

/*************************************************************
 * Various interactions with the cilk2c output
 *************************************************************/
/* destruction of a slow frame */
void Cilk_destroy_frame(CilkWorkerState *const ws,
			 CilkStackFrame *f, size_t size)
{
     WHEN_CILK_ALLOCA( 
     {
	  if (f->alloca_h)
	       Cilk_unalloca_internal(ws, f);
     });

     Cilk_internal_free(ws, f, size);
}

/* at a slow sync; return 0 if the sync succeeds, and 1 if suspended */
int Cilk_sync(CilkWorkerState *const ws)
{
     Closure *t;
     int res = 0;

     Cilk_event(ws, EVENT_CILK_SYNC);

     deque_lock(ws, ws->self);

     t = deque_peek_bottom(ws, ws->self);

     Closure_lock(ws, t);
     CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
     CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

     /* assert we are really at the top of the stack */
     CILK_ASSERT(ws, Closure_at_top_of_stack(t));

     poll_inlets(ws, t);
     deque_assert_is_bottom(ws, t);

     maybe_reset_abort(t);

     if (Closure_has_children(t)) {
	  Closure_suspend(ws, t);
	  res = 1;
     } 

     Closure_unlock(ws, t);

     deque_unlock(ws, ws->self);
     return res;
}

#if CILK_TIMING
/* update work and CP after a slow sync */
void Cilk_after_sync_slow_cp(CilkWorkerState *const ws,
			     Cilk_time *work, Cilk_time *cp)
{
     Closure *t;

     deque_lock(ws, ws->self);
     t = deque_peek_bottom(ws, ws->self);

     Closure_lock(ws, t);

     CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
     CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

     /* assert we are really at the top of the stack */
     CILK_ASSERT(ws, Closure_at_top_of_stack(t));

     *work += t->work;
     if (t->cp > *cp)
	  *cp = t->cp;
     t->cp = 0;
     t->work = 0;

     Closure_unlock(ws, t);
     deque_unlock(ws, ws->self);
}
#endif

static void move_result_into_closure(CilkWorkerState *const ws,
				     Closure *t, void *resultp,
				     int size)
{
     Closure_assert_ownership(ws, t);
     t->return_size = size;

     if (size) {
          t->return_value = Cilk_internal_malloc(ws, size);
	  memcpy (t->return_value, (void *)resultp, size);
     } else {
	  WHEN_CILK_DEBUG(t->return_value = NULL);
     }
}

/*
 * This is the exception handler.
 *
 * The handler returns 1 if the worker must return to the runtime 
 * system, 0 otherwise.  This is what it does:
 *  
 * We need to call poll_inlets first. 
 * But poll_inlets may change the closure that owns the stack that 
 * is currently running.  So, we need to get the closure from the 
 * ready deque.  Then, we follow the protocol below:
 *
 *   reset exception pointer
 *   if ( H>=T )
 *       it is a steal
 *       change state into CLOSRUE_RETURNING
 *       res = 1  
 *
 *   else if (current frame is being aborted)
 *       res = 1
 *       signal another exception for parent frame
 *       if (current frame is the last frame)
 *            do a sync
 *            {suspend if needed; o.w. destroy frame and return}
 *       else
 *            destroy current frame
 *
 *   else { not steal, not abort}
 *       reset abort pointer to NO_ABORT
 *
 *   return res;
 */
int Cilk_exception_handler(CilkWorkerState *const ws, void *resultp, int size)
{
     Closure *t, *t1;
     int res = 0;

     Cilk_event(ws, EVENT_EXCEPTION);

     deque_lock(ws, ws->self);
     t = deque_peek_bottom(ws, ws->self);

     CILK_ASSERT(ws, t);
     Closure_lock(ws, t);
     poll_inlets(ws, t);

     t1 = deque_peek_bottom(ws, ws->self);
     CILK_ASSERT(ws, t1);

     /*
      * poll_inlets may change the bottom of the ready deque.
      * (see comments at signal_abort_from_inlet)
      */
     if (t != t1) {
	  Closure_unlock(ws, t);
	  t = t1;
	  Closure_lock(ws, t);
     }

     reset_exception_pointer(ws, t);

     CILK_ASSERT(ws, t->status == CLOSURE_RUNNING ||
		 t->status == CLOSURE_RETURNING);

     if (CLOSURE_HEAD(t) >= CLOSURE_TAIL(t)) {
	  Cilk_event(ws, EVENT_EXCEPTION_STEAL);
	  res = 1;

	  if (t->status == CLOSURE_RUNNING) {
	       t->status = CLOSURE_RETURNING;
	       move_result_into_closure(ws, t, resultp, size);
	  }
     } else {
	  CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);
	  if (Closure_being_aborted(ws, t)) {
	       CilkStackFrame *f;
	       res = 1;

	       Cilk_event(ws, EVENT_EXCEPTION_ABORT);
	       signal_immediate_exception(ws, t);

	       if (Closure_at_top_of_stack(t)) {
		    if (Closure_has_children(t)) {
			 Cilk_event(ws, EVENT_EXCEPTION_ABORT_SUSPEND);
			 /* pretend to sync */
			 maybe_reset_abort(t);
			 Closure_suspend(ws, t);
			 goto skip_free;
		    } else {
			 /* pretend we are returning */
			 t->status = CLOSURE_RETURNING;
			 Cilk_event(ws, EVENT_EXCEPTION_ABORT_RETURN);
		    }
	       }

	       f = (CilkStackFrame *) CLOSURE_TAIL(t)[-1];
	       Cilk_destroy_frame(ws, f, get_frame_size(f));
	  skip_free: ;
	  } else {
	       /* frame not being aborted */
	       maybe_reset_abort(t);
	       Cilk_event(ws, EVENT_EXCEPTION_OTHER);
	  }
     }

     Closure_unlock(ws, t);
     deque_unlock(ws, ws->self);
     return res;
}

/*
 * before a return in the slow code.  This function marks the
 * closure as returning, so that it won't be stolen.
 */
void Cilk_set_result(CilkWorkerState *const ws, void *resultp, int size)
{
     Closure *t;

     Cilk_event(ws, EVENT_RETURN_SLOW);

     deque_lock(ws, ws->self);
     t = deque_peek_bottom(ws, ws->self);
     Closure_lock(ws, t);

     CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
     CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

     /* 
      * this must happen after a sync; hence, either there is
      * no abort at all, or the parent is trying to kill us
      */
     CILK_ASSERT(ws, CLOSURE_ABORT(t) == NO_ABORT ||
		 CLOSURE_ABORT(t) == ABORT_ALL);

     t->status = CLOSURE_RETURNING;
     t->frame = (CilkStackFrame *) NULL;

     move_result_into_closure(ws, t, resultp, size);

     Closure_unlock(ws, t);
     deque_unlock(ws, ws->self);
}


void Cilk_abort_slow(CilkWorkerState *const ws) 
{
     Closure *cl;
     deque_lock(ws, ws->self);
     cl = deque_peek_bottom(ws, ws->self);
     Closure_lock(ws, cl);

     CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);
     CILK_ASSERT(ws, Closure_at_top_of_stack(cl));
     Cilk_event(ws, EVENT_ABORT_SLOW);

     Cilk_enter_state(ws, STATE_ABORT_RECURSIVE);
     recursively_signal_abort(ws, cl, ALMOST_NO_ABORT);
     Cilk_exit_state(ws, STATE_ABORT_RECURSIVE);

     poll_inlets(ws, cl);

     Closure_unlock(ws, cl);
     deque_unlock(ws, ws->self);
}

void Cilk_abort_standalone(CilkWorkerState *const ws) 
{
     Cilk_event(ws, EVENT_ABORT_STANDALONE);
     ws->abort_flag = 1;
}
  
/*********************************************************
 * Execution
 *********************************************************/
static Closure *setup_for_execution(CilkWorkerState *const ws, Closure *t)
{
     Closure_assert_ownership(ws, t);

     CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

     t->status = CLOSURE_RUNNING;
     t->cache = &ws->cache;

     CLOSURE_STACK(t)[0] = t->frame;	/* push the first frame on the stack */
     CLOSURE_HEAD(t) = (volatile CilkStackFrame**)CLOSURE_STACK(t);
     CLOSURE_TAIL(t) = (volatile CilkStackFrame**)CLOSURE_STACK(t)+1;
     reset_exception_pointer(ws, t);
     reset_abort(t);

     return t;
}

static Closure *return_value(CilkWorkerState *const ws, Closure *t)
{
     Closure *res = NULL;

     Cilk_enter_state(ws, STATE_RETURNING);

     CILK_ASSERT(ws, t->status == CLOSURE_RETURNING);

     res = Closure_return(ws, t);

     Cilk_exit_state(ws, STATE_RETURNING);
     return res;
}

/*
 * execute the closure. If the closure is returning, take care of the
 * result.
 */
static Closure *do_what_it_says(CilkWorkerState *const ws, Closure *t)
{
     Closure *res = NULL;
     CilkStackFrame *f;

     /* t must not be locked */
     Closure_assert_alienation(ws, t);
     Closure_lock(ws, t);

     switch (t->status) {
	 case CLOSURE_READY:
	      if (Closure_being_aborted(ws, t)) {
		   res = abort_ready_closure(ws, t);
		   Closure_unlock(ws, t);
	      } else {
		   /* just execute it */
		   setup_for_execution(ws, t);
		   f = t->frame;
		   CILK_ASSERT(ws, f);
		   CILK_ASSERT(ws, f->sig);
		   CILK_ASSERT(ws, f->magic == CILK_STACKFRAME_MAGIC);
		   poll_inlets(ws, t);
		   Closure_unlock(ws, t);
		   /* 
		    * MUST unlock the closure before locking the queue 
		    * (rule A in file PROTOCOLS)
		    */
		   deque_lock(ws, ws->self);
		   deque_add_bottom(ws, t, ws->self);
		   deque_unlock(ws, ws->self);

		   /* now execute it */
		   Cilk_enter_state(ws, STATE_WORKING);
		   (get_proc_slow(f->sig)) (ws, f);
		   Cilk_exit_state(ws, STATE_WORKING);
	      }

	      break;

	 case CLOSURE_RETURNING:
	      /*
	       * the return protocol assumes t is not locked,
	       * and everybody will respect the fact that t is
	       * returning
	       */
	      Closure_unlock(ws, t);
	      res = return_value(ws, t);
	      break;

	 default:
	      Cilk_die_internal(ws->context, ws,
		       "BUG in do_what_it_says(), t->status = %d\n", 
		       t->status);
	      break;
     }

     return res;
}

/*********************************************************
 * The scheduler itself
 *********************************************************/
void Cilk_scheduler(CilkWorkerState *const ws, Closure *t)
{
     /* 
      * t contains 'the next thing to do'.  Initially, the
      * scheduler on proc 0 executes the main closure.
      */
     int victim;

     /*Cilk_internal_malloc_per_worker_init(context, ws); - moved to child_main*/
     CILK_ASSERT(ws, ws->self >= 0);
     rts_srand(ws, ws->self * 162347);

     Cilk_enter_state(ws, STATE_TOTAL);

     while (!USE_SHARED(done)) {
	  if (!t) {
	       /* try to get work from our local queue */
	       deque_lock(ws, ws->self);
	       t = deque_xtract_bottom(ws, ws->self);
	       deque_unlock(ws, ws->self);
	  }

	  while (!t && !USE_SHARED(done)) {
	       /* otherwise, steal */
	       Cilk_enter_state(ws, STATE_STEALING);
	       victim = rts_rand(ws) % USE_PARAMETER(active_size);
	       if (victim != ws->self) {
		    t = Closure_steal(ws, victim);
		    if (!t && USE_PARAMETER(options->yieldslice) &&
			!USE_SHARED(done)) {
			 Cilk_lower_priority(ws);
		    }
	       }
	       /* Cilk_fence(); */
	       Cilk_exit_state(ws, STATE_STEALING);
	  }

	  if (USE_PARAMETER(options->yieldslice))
	       Cilk_raise_priority(ws);

	  if (!USE_SHARED(done))
	       t = do_what_it_says(ws, t);

	  /* 
	   * if provably-good steals happened, t will contain
	   * the next closure to execute
	   */
     }

     Cilk_exit_state(ws, STATE_TOTAL);

     return;
}

/*
 * initialization of the scheduler. 
 */
void Cilk_scheduler_init(CilkContext *const context)
{
    CILK_CHECK(USE_PARAMETER1(active_size) > 0,
		(context, NULL, "Partition size must be positive\n"));
     create_deques(context);
     Cilk_internal_malloc_global_init(context);
     Cilk_internal_malloc_global_init_2(context);
}

void Cilk_scheduler_terminate(CilkContext *const context)
{
   Cilk_internal_malloc_global_terminate_2(context);
   Cilk_internal_malloc_global_terminate(context);
   free_deques(context);
}

void Cilk_scheduler_init_2(CilkContext *const context)
{
     USE_SHARED1(done) = 0;
     init_deques(context);
     /*Cilk_internal_malloc_global_init_2(context); */
}

void Cilk_scheduler_terminate_2(CilkContext *const UNUSED(context))
{
     /*Cilk_internal_malloc_global_terminate_2(context); */
}

void Cilk_scheduler_per_worker_init(CilkWorkerState *const ws)
{
     WHEN_CILK_TIMING(ws->cp_hack = 0);
     WHEN_CILK_TIMING(ws->work_hack = 0);

     ws->cache.stack = 
	  Cilk_malloc_fixed(USE_PARAMETER(options->stackdepth) *
			    sizeof(CilkStackFrame *));
     ws->stackdepth = USE_PARAMETER(options->stackdepth);
     Cilk_reset_stack_depth_stats(ws);

     CILK_CHECK(ws->cache.stack, (ws->context, ws, "failed to allocate stack\n"));
     
}

void Cilk_scheduler_per_worker_terminate(CilkWorkerState *const ws)
{
     Cilk_free(ws->cache.stack);
}

/* 
 * From the frame of <cl>, which should be the invoke_main closure,
 * get the inlet that should be executed when cilk_main returns.
 * (All the inlet does, is to set the proper return value and call
 * abort).
 * Pass <res>, the exit value, to the inlet, and enque it as 
 * a complete inlet.
 */
static void make_exit_inlet_closure(CilkWorkerState *const ws, 
				    Closure *cl, int res)
{
     struct InletClosure *p;

     p = make_incomplete_inlet_closure(ws, cl, 1);

     memcpy(p->inlet_args, &res, sizeof(int));     
     p->next = cl->complete_inlets;
     cl->complete_inlets = p;
}

/* 
 * This procedure is called by Cilk_exit().
 * <cl> is the invoke_main closure, and <res> is the return
 * value passed in by Cilk_exit.  
 * 
 * The real work is done by first enqueing an inlet that calls
 * abort, and then either signal exception or polls the inlet.
 * So, cilk_main, which is spawned by invoke_main will be 
 * aborted.  
 * (Refer to "invoke_main.c" for details.)
 */
void Cilk_exit_from_user_main(CilkWorkerState *const ws, Closure *cl, int res)
{
     Closure_assert_alienation(ws, cl);

     Closure_lock(ws, cl);
     make_exit_inlet_closure(ws, cl, res);
     
     if (cl->status == CLOSURE_RUNNING)
	  signal_immediate_exception(ws, cl);
     else
	  poll_inlets(ws, cl);
     
     Closure_unlock(ws, cl);
}

