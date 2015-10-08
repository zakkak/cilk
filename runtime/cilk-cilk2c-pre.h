/* config.h and compiler-dep.h are included by the includer of this file. */
FILE_IDENTITY(ident_cilk_cilk2c_pre,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-cilk2c-pre.h $ $LastChangedBy: bradley $ $Rev: 1465 $ $Date: 2004-08-02 06:31:06 -0400 (Mon, 02 Aug 2004) $");

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


/*************************************************************
 * internal malloc 
 *************************************************************/

static inline void *Cilk_internal_malloc_fast(CilkWorkerState *const ws,
						  size_t size)
{
     int bucket;
     void *mem;
     struct Cilk_im_descriptor *d;

     CILK_ASSERT(ws, ws->self >= 0);

     if (size > CILK_INTERNAL_MALLOC_THRESHOLD)
	  return Cilk_internal_malloc(ws, size);

     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;

     /* look at the free list */
     if ((mem = d->free_list)) {
	  d->free_list = ((void **) mem)[0];
	  d->count++;
	  WHEN_CILK_DEBUG(
	  {
	       USE_PARAMETER(im_info)[ws->self].used += size;
	       USE_PARAMETER(im_info)[ws->self].nmalloc += 1;
	  })
     } else {
	  mem = Cilk_internal_malloc(ws, size);
     }

     return mem;
}

static inline void Cilk_internal_free_fast(CilkWorkerState *const ws,
					       void *p, size_t size)
{
     int bucket;
     struct Cilk_im_descriptor *d;

     if (size > CILK_INTERNAL_MALLOC_THRESHOLD) {
	  Cilk_internal_free(ws, p, size);
	  return;
     }

     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;

     if (d->count <= 0)
	  Cilk_internal_free(ws, p, size);
     else {
	  ((void **) p)[0] = d->free_list;
	  d->free_list = p;
	  d->count--;

	  WHEN_CILK_DEBUG({
	       USE_PARAMETER(im_info)[ws->self].used -= size;
	       USE_PARAMETER(im_info)[ws->self].nmalloc -= 1;
	  });
     }
}

static inline void Cilk_destroy_frame_fast(CilkWorkerState *const ws, 
					       CilkStackFrame *f, size_t size)
{
     WHEN_CILK_ALLOCA( 
     {
	  if (f->alloca_h)
	       Cilk_unalloca_internal(ws, f);
     });

     Cilk_internal_free_fast(ws, f, size);
}

static inline void *Cilk_create_frame(CilkWorkerState *const ws,
					  size_t size, CilkProcInfo *sig)
{
     CilkStackFrame *f = Cilk_internal_malloc_fast(ws, size);
     f->sig = sig;
     WHEN_CILK_ALLOCA(f->alloca_h = (struct cilk_alloca_header *) 0);
     WHEN_CILK_DEBUG(f->magic = CILK_STACKFRAME_MAGIC);

     return (void *) f;
}

static inline void Cilk_cilk2c_push_frame(CilkWorkerState *const UNUSED(ws),
					      CilkStackFrame *UNUSED(frame))
{
     USE_UNUSED(ws); USE_UNUSED(frame);
     CILK_ASSERT(ws, frame->magic == CILK_STACKFRAME_MAGIC);
     Cilk_membar_StoreStore();
}

static inline void *Cilk_cilk2c_init_frame(CilkWorkerState *const ws,
					       size_t s, CilkProcInfo *signat)
{
     volatile CilkStackFrame **t;
     void *f;

     f = Cilk_create_frame(ws, s, signat);
     t = ws->cache.tail;
     CILK_COMPLAIN((CilkStackFrame **) t < ws->cache.stack + ws->stackdepth,
		   (ws->context, ws, USE_PARAMETER(stack_overflow_msg)));
     *t = (CilkStackFrame *) f;
     Cilk_membar_StoreStore();
     ws->cache.tail = t + 1;
     return f;
}

/*************************************************************
 * functions used by cilk2c's output
 *************************************************************/
static inline int Cilk_cilk2c_pop_check(CilkWorkerState *const ws)
{
     volatile CilkStackFrame **t;
     t = ws->cache.tail;
     Cilk_membar_StoreLoad();
     return (ws->cache.exception >= t);
}

static inline void Cilk_cilk2c_pop(CilkWorkerState *const ws)
{
     --ws->cache.tail;
}

static inline void
Cilk_cilk2c_event_new_thread_maybe(CilkWorkerState *const UNUSED(ws))
{
     USE_UNUSED(ws);
     WHEN_CILK_STATS(Cilk_event_new_thread(ws));
}

static inline void Cilk_cilk2c_start_thread_slow(CilkWorkerState *const UNUSED(ws),
						     CilkStackFrame *UNUSED(frame))
{
     USE_UNUSED(ws);
     USE_UNUSED(frame);
     CILK_ASSERT(ws, frame->magic == CILK_STACKFRAME_MAGIC);
}

static inline void Cilk_cilk2c_before_return_fast(CilkWorkerState *const ws,
						  CilkStackFrame *frame,
						      size_t size)
{
     CILK_ASSERT(ws, frame->magic == CILK_STACKFRAME_MAGIC);
     WHEN_CILK_DEBUG(frame->magic = ~CILK_STACKFRAME_MAGIC);
     Cilk_destroy_frame_fast(ws, (CilkStackFrame *) frame, size);
     --ws->cache.tail;
}

static inline void Cilk_cilk2c_before_return_slow(CilkWorkerState *const ws,
						  CilkStackFrame *frame,
						      size_t size)
{
     Cilk_cilk2c_before_return_fast(ws, frame, size);
}

#if CILK_CRITICAL_PATH
/*
 * Critical-path measurements. Each procedure has two variables,
 * CP and MYCP.
 *
 * Let a "nice node" be the beginning of the thread, or any sync
 * point. CP is less than or equal to MYCP at nice nodes, and MYCP
 * is the actual critical path up to that node.
 *
 * On non-nice nodes, CP and MYCP have different meanings.
 * MYCP is the critical path up to the last nice node, plus the
 * execution time (of the procedure only, not its children) since
 * then. CP is the maximum critical path of all children spawned
 * and finished since last nice node.
 *
 * Therefore, when a child is spawned, it starts with the current
 * value of MYCP; moreover, when sync occurs, CP <= MYCP = max(CP, MYCP).
 *
 * The variable cp_hack is used to pass CP from parent to
 * child, and vice versa.
 *
 * CP and MYCP for the fast version are local variables, saved on the
 * frame before a spawn. The slow version always operates on the frame.
 *
 * We also take advantage of the fact the CP measurements are slow
 * to collect a fine-grained timing of the total work at no appreciable
 * cost. Every thread as a variable WORK that collects the relevant
 * quantities.  The variable work_hack passes this value
 * from child to parent (the vice versa is not needed).
 */

/* at begin of thread, set my critical path to the one of the parent */
static inline void
Cilk_cilk2c_start_thread_fast_cp(CilkWorkerState *const ws,
				 CilkStackFrame *frame)
{
     frame->cp = 0;
     frame->mycp = ws->cp_hack;
     frame->work = 0;
     WHEN_CILK_USER_TIMERS(ws->user_work = frame->work);
     WHEN_CILK_USER_TIMERS(ws->user_critical_path = frame->mycp);
     ws->last_cp_time = Cilk_get_time();
}

/* 
 * but the slow version by definition has a valid CP and MYCP, so
 * no initialization is needed.
 */
static inline void
Cilk_cilk2c_start_thread_slow_cp(CilkWorkerState *const ws,
				 CilkStackFrame *frame)
{
     WHEN_CILK_USER_TIMERS(ws->user_work = frame->work);
     WHEN_CILK_USER_TIMERS(ws->user_critical_path = frame->mycp);
}

static inline void
Cilk_cilk2c_at_thread_boundary_slow_cp(CilkWorkerState *const ws,
				       CilkStackFrame *UNUSED(frame))
{
     ws->last_cp_time = Cilk_get_time();
}

/*
 * before spawn, adjust mycp and pass it to the child (implicitly) 
 */
static inline void 
Cilk_cilk2c_before_spawn_fast_cp(CilkWorkerState *const ws,
				 CilkStackFrame *frame)
{
     Cilk_time Cilk_elapsed = Cilk_get_elapsed_time(ws);
     ws->cp_hack = (frame->mycp += Cilk_elapsed);
     frame->work += Cilk_elapsed;
}

static inline void
Cilk_cilk2c_before_spawn_slow_cp(CilkWorkerState *const ws,
				 CilkStackFrame *frame)
{
     Cilk_cilk2c_before_spawn_fast_cp(ws, frame);
}

/* after a spawn, maximize CP and accumulate WORK */
static inline void 
Cilk_cilk2c_after_spawn_fast_cp(CilkWorkerState *const ws,
				CilkStackFrame *frame)
{
     if (ws->cp_hack > frame->cp)
	  frame->cp = ws->cp_hack;
     frame->work += ws->work_hack;
     WHEN_CILK_USER_TIMERS(ws->user_work = frame->work);
     WHEN_CILK_USER_TIMERS(ws->user_critical_path = frame->mycp);
}

static inline void 
Cilk_cilk2c_after_spawn_slow_cp(CilkWorkerState *const ws,
				CilkStackFrame *frame)
{
     Cilk_cilk2c_after_spawn_fast_cp(ws, frame);
}

/* at a sync, set MYCP = CP = max(CP, MYCP) and accumulate work */
static inline void 
Cilk_cilk2c_at_sync_fast_cp(CilkWorkerState *const ws,
			    CilkStackFrame *frame)
{
     Cilk_time Cilk_elapsed = Cilk_get_elapsed_time(ws);
     frame->mycp += Cilk_elapsed;
     frame->work += Cilk_elapsed;
     if (frame->cp > frame->mycp)
	  frame->mycp = frame->cp;
     WHEN_CILK_USER_TIMERS(ws->user_work = frame->work);
     WHEN_CILK_USER_TIMERS(ws->user_critical_path = frame->mycp);
}

/*
 * the slow case is trickier. Before the check for sync,
 * we update CP/MYCP and work.
 *
 * *After* the check we maximize CP with all CP's of children
 * executed by other processors. We call a special procedure to
 * do that.
 */
static inline void Cilk_cilk2c_before_sync_slow_cp(CilkWorkerState *const ws,
						       CilkStackFrame *frame)
{
     Cilk_time Cilk_elapsed = Cilk_get_elapsed_time(ws);
     frame->mycp += Cilk_elapsed;
     frame->work += Cilk_elapsed;
     if (frame->cp > frame->mycp)
	  frame->mycp = frame->cp;
}

static inline void Cilk_cilk2c_after_sync_slow_cp(CilkWorkerState *const ws,
						      CilkStackFrame *frame)
{
     Cilk_after_sync_slow_cp(ws, &frame->work, &frame->mycp);
     WHEN_CILK_USER_TIMERS(ws->user_work = frame->work);
     WHEN_CILK_USER_TIMERS(ws->user_critical_path = frame->mycp);
}

/* return is like sync, but also sets CP for the parent */
/* we should be sync'd at a return, so just use mycp */
static inline void Cilk_cilk2c_before_return_fast_cp(
     CilkWorkerState *const ws, CilkStackFrame *frame)
{
     Cilk_time Cilk_elapsed = Cilk_get_elapsed_time(ws);

     ws->cp_hack = (frame->mycp + Cilk_elapsed);
     ws->work_hack = (frame->work + Cilk_elapsed);
}

static inline void Cilk_cilk2c_before_return_slow_cp(
     CilkWorkerState *const ws, CilkStackFrame *frame)
{
     Cilk_cilk2c_before_return_fast_cp(ws, frame);
}

#else

static inline void Cilk_cilk2c_start_thread_fast_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_start_thread_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_at_thread_boundary_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_before_spawn_fast_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_before_spawn_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_after_spawn_fast_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_after_spawn_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_at_sync_fast_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_before_sync_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_after_sync_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_before_return_fast_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}
static inline void Cilk_cilk2c_before_return_slow_cp(
     CilkWorkerState *const UNUSED(ws), CilkStackFrame *UNUSED(frame))
{
    USE_UNUSED(ws);
    USE_UNUSED(frame);
}

#endif
