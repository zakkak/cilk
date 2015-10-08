/* getting FILE_IDENTITY to work here is a pain. The trick is to avoid including config.h at the wrong time. */
/*      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilk-cilk2c.h $ $LastChangedBy: bradley $ $Rev: 1389 $ $Date: 2004-06-22 10:46:28 -0400 (Tue, 22 Jun 2004) $"); */

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

#define Self _cilk_ws->self
#define Cilk_user_work _cilk_ws->user_work
#define Cilk_user_critical_path _cilk_ws->user_critical_path
#define Cilk_alloca(size) Cilk_alloca_internal(_cilk_ws, size)

#define CILK_OFFSETOF(type, mem) \
     ((size_t) ((char *)&((type *) 0)->mem - (char *)((type *) 0)))

/* add current frame to deque */
#define CILK2C_PUSH_FRAME(f) {				\
     Cilk_cilk2c_push_frame(_cilk_ws, &(f->header));	\
}

#define CILK2C_INIT_FRAME(f,s,signat) {			\
     f = Cilk_cilk2c_init_frame(_cilk_ws, s, signat);	\
}

#define CILK2C_SYNC Cilk_sync(_cilk_ws)

#define CILK2C_SYNCHED 0

#define CILK2C_ABORT_STANDALONE() Cilk_abort_standalone(_cilk_ws)

#define CILK2C_ABORT_SLOW() Cilk_abort_slow(_cilk_ws)

#if 0
/* This is now done directly by the compiler, to avoid the "typeof" non-ansi construction (Bug 125) */
#define CILK2C_SET_RESULT(x) {				\
     typeof(x) __tmp = x;				\
     Cilk_set_result(_cilk_ws, &__tmp, sizeof(__tmp));	\
}
#endif

#define CILK2C_SET_NORESULT() {			\
     Cilk_set_result(_cilk_ws, (void *)0, 0);	\
}

/* __tmp must be declared in the surrounding code.  We used to do '{ typeof(result) __tmp = result; }", now we simply assume that __tmp is declared in the surrounding code. */
#define CILK2C_XPOP_FRAME_RESULT(f, r, result)				 \
{									 \
     if (Cilk_cilk2c_pop_check(_cilk_ws)) {				 \
	  __tmp = result;				 \
	  if (Cilk_exception_handler(_cilk_ws, &__tmp, sizeof(__tmp))) { \
	       Cilk_cilk2c_pop(_cilk_ws);				 \
	       return r;						 \
          }								 \
     }									 \
}

#define CILK2C_XPOP_FRAME_NORESULT(f, r)			\
{								\
     if (Cilk_cilk2c_pop_check(_cilk_ws)) {			\
 	  if (Cilk_exception_handler(_cilk_ws, (void *)0, 0)) {	\
	       Cilk_cilk2c_pop(_cilk_ws);			\
	       return r;					\
	  }							\
     }								\
}

#define CILK2C_BEFORE_SPAWN_FAST()					\
  Cilk_cilk2c_before_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header))

#define CILK2C_BEFORE_SPAWN_SLOW()					\
  Cilk_cilk2c_before_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header))

#define CILK2C_AT_THREAD_BOUNDARY_SLOW() {		\
     Cilk_cilk2c_at_thread_boundary_slow_cp(		\
	  _cilk_ws, &(_cilk_frame->header));		\
     Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);	\
}

#define CILK2C_AFTER_SPAWN_FAST() {					\
     Cilk_cilk2c_after_spawn_fast_cp(_cilk_ws, &(_cilk_frame->header));	\
     Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);			\
}

#define CILK2C_AFTER_SPAWN_SLOW() \
  Cilk_cilk2c_after_spawn_slow_cp(_cilk_ws, &(_cilk_frame->header))

#define CILK2C_START_THREAD_FAST() {					 \
     Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header)); \
     Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);			 \
}

#define CILK2C_START_THREAD_SLOW() {					 \
     Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header)); \
     Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header));	 \
}

#define CILK2C_BEFORE_RETURN_FAST() {					  \
     Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header)); \
     Cilk_cilk2c_before_return_fast(					  \
       _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));		  \
}

#define CILK2C_BEFORE_RETURN_SLOW() {					  \
     Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header)); \
     Cilk_cilk2c_before_return_slow(					  \
       _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));		  \
}

#define CILK2C_BEFORE_RETURN_INLET()

#define CILK2C_BEFORE_SYNC_SLOW()					\
  Cilk_cilk2c_before_sync_slow_cp(_cilk_ws, &(_cilk_frame->header))

#define CILK2C_AFTER_SYNC_SLOW()					\
  Cilk_cilk2c_after_sync_slow_cp(_cilk_ws, &(_cilk_frame->header)) 

#define CILK2C_AT_SYNC_FAST() {						\
     Cilk_cilk2c_at_sync_fast_cp(_cilk_ws, &(_cilk_frame->header));	\
     Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);			\
}
