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

/* startup routines. You are not expected to understand this file */

#include <stdio.h>
#include <string.h>
#include <cilk.h>
#include <cilk-internal.h>
#include <cilk-cilk2c-pre.h>
#include <cilk-cilk2c.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/invoke-main.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

/************************************************************
 *  Initial thread
 ************************************************************/

/* This is a hand-compiled procedure that calls cilk_main */
struct invoke_main_frame {
     CilkStackFrame header;
     void *args;
     int  return_size;
     void (*import_main)(CilkWorkerState *const ws, void *args);
     int retval;
};


struct invoke_main_catch_inlet_args {int res;};

/*
 * This inlet is created for the purpose of Cilk_exit().
 * It sets retval to the exit value, and calls abort.
 * So, when this inlet is excuted, the user's cilk_main
 * should be aborted as a child of invoke_main.
 */
void
invoke_main_catch_inlet(CilkWorkerState *const _cilk_ws,
			struct invoke_main_frame *_cilk_frame,
			void *inletargs,
			void* UNUSED(inletresult))
{

     memcpy(_cilk_frame->args, inletargs, _cilk_frame->return_size);
     CILK2C_ABORT_STANDALONE();
}

static void invoke_main_slow(CilkWorkerState *const _cilk_ws,
			     struct invoke_main_frame *_cilk_frame)
{
     void *args;
     
     CilkWorkerState *const ws = _cilk_ws; /*for the USE_SHARED macro at the end of the func.*/

     CILK2C_START_THREAD_SLOW();
     switch (_cilk_frame->header.entry) {
     case 1:
	  goto _sync1;
     case 2:
	  goto _sync2;
     }

     _cilk_ws->cp_hack = 0;
     _cilk_ws->work_hack = 0;
     _cilk_ws->user_work = 0;
     _cilk_ws->user_critical_path = 0;
     WHEN_CILK_TIMING(_cilk_frame->header.cp = (Cilk_time) 0);
     WHEN_CILK_TIMING(_cilk_frame->header.work = (Cilk_time) 0);
     WHEN_CILK_TIMING(_cilk_frame->header.mycp = (Cilk_time) 0);
     WHEN_CILK_TIMING(_cilk_ws->last_cp_time = Cilk_get_time());

     args = _cilk_frame->args;

     _cilk_frame->header.receiver = (void *) &_cilk_frame->retval;
     _cilk_frame->header.entry=1;
     CILK2C_BEFORE_SPAWN_SLOW();
     CILK2C_PUSH_FRAME(_cilk_frame);

     _cilk_frame->import_main(_cilk_ws, args);
     CILK2C_XPOP_FRAME_NORESULT(_cilk_frame,/* return nothing */);
     CILK2C_AFTER_SPAWN_SLOW();


     if (0) {
     _sync1:
	  ;
     }
     CILK2C_AT_THREAD_BOUNDARY_SLOW();

     CILK2C_BEFORE_SYNC_SLOW();
     _cilk_frame->header.entry=2;
     if (CILK2C_SYNC) {
	  return;
     _sync2:
	  ;
     }
     CILK2C_AFTER_SYNC_SLOW();
     CILK2C_AT_THREAD_BOUNDARY_SLOW();

     WHEN_CILK_TIMING(USE_SHARED(critical_path) = _cilk_frame->header.mycp);
     WHEN_CILK_TIMING(USE_SHARED(total_work) = _cilk_frame->header.work);

     CILK_WMB();
     USE_SHARED(done) = 1;

     Cilk_remove_and_free_closure_and_frame(_cilk_ws,
					    &_cilk_frame->header,
					    _cilk_ws->self);

     return;
}




Closure *Cilk_create_initial_thread(
     CilkContext *const context,
     void (*import_main)(CilkWorkerState *const ws, void *args),
     void *args,
     int return_size)
{
     Closure *t;
     struct invoke_main_frame *f;

     /* create a frame for invoke_cilk_main */
     t = Cilk_Closure_create_malloc(context, NULL);
     t->parent = (Closure *) NULL;
     t->join_counter = 0;
     t->status = CLOSURE_READY;

     f = Cilk_malloc(sizeof(struct invoke_main_frame));
     f->header.entry = 0;
     f->header.sig = USE_SHARED1(invoke_main_sig);
     WHEN_CILK_DEBUG(f->header.magic = CILK_STACKFRAME_MAGIC);

     f->args = args;
     f->return_size = return_size;
     f->import_main = import_main;

     t->frame = &f->header;

     /* Initialize the signature of Cilk_main */
     memset( USE_SHARED1(invoke_main_sig), 0 , 3*sizeof(CilkProcInfo) );
     USE_SHARED1(invoke_main_sig)[0].size = sizeof(int);
     USE_SHARED1(invoke_main_sig)[0].index = sizeof(struct invoke_main_frame);
     USE_SHARED1(invoke_main_sig)[0].inlet = invoke_main_slow;
     USE_SHARED1(invoke_main_sig)[1].size = sizeof(int);
     USE_SHARED1(invoke_main_sig)[1].inlet = invoke_main_catch_inlet;
     USE_SHARED1(invoke_main_sig)[1].argsize = return_size;

     return t;
}

/*************************************************************
 * Exit-related routines
 *************************************************************/
struct _Cilk_really_exit_frame {
     CilkStackFrame header;
     struct {
	  int val;
     } scope0;
};

static CilkProcInfo _Cilk_really_exit_sig[] =
{
     {0, sizeof(struct _Cilk_really_exit_frame), /* no slow version */ 0, 0, 0}
};

void Cilk_really_exit(CilkWorkerState *const _cilk_ws,
		      int val)
{
     struct _Cilk_really_exit_frame *_cilk_frame;
     CILK2C_INIT_FRAME(_cilk_frame, sizeof(struct _Cilk_really_exit_frame),
		       _Cilk_really_exit_sig);
     CILK2C_START_THREAD_FAST();

     {
	  Cilk_really_exit_1(_cilk_ws, val);
	  {
	       CILK2C_BEFORE_RETURN_FAST();
	       return;
	  }
     }
}
