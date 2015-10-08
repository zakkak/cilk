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

/***********************************************************
 * Time-gathering stuff
 ***********************************************************/

#include <cilk.h>
#include <cilk-internal.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/timing.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");


#if CILK_TIMING
#include <stdio.h>

/*
 * The model for time gathering is the following:
 * there is a certain set of states in which the system can
 * be. Throughout the code, ENTER_STATE(something) signals
 * that a given state is entered. EXIT_STATE(state) reverts back to
 * the previous state, and checks that the right state is being
 * exited. The time is accumulated on a per-state
 * basis. Time is cumulative, i.e. it is measured from ENTER_STATE
 * to EXIT_STATE, regardless of other states entered in between.
 * 
 * No more than STATE_DEPTH states can be nested.
 * The stack is really used only for checking purposes, but it's
 * probably a good idea to keep it.
 *
 */

/*
 * table of description of states. It is printed in
 * the same order.
 */
static const struct state_desc {
     char *name;
     int state;
} state_descriptions[] = {

     {
	  "Total execution time", STATE_TOTAL
     },
     {
	  "Working time", STATE_WORKING
     },
     {
	  "Waiting for locks", STATE_WAITING_FOR_LOCK
     },
     {
	  "Stealing", STATE_STEALING
     },
     {
	  "Returning", STATE_RETURNING
     },
     {
	  "Aborting", STATE_ABORT_RECURSIVE
     },
     {
	  "User state 0", STATE_USER0
     },
     {
	  "User state 1", STATE_USER1
     },
     {
	  "User state 2", STATE_USER2
     },
     {
	  "User state 3", STATE_USER3
     },
     {
	  "User state 4", STATE_USER4
     },
     {
	  "User state 5", STATE_USER5
     },
     {
	  "User state 6", STATE_USER6
     },
     {
	  "User state 7", STATE_USER7
     },
     {
	  (char *) 0, 0
     }
};

void Cilk_enter_state(CilkWorkerState *const ws, 
		      int state)
{
     int stackp;
     Cilk_time now = Cilk_get_time();
     Cilk_time now_wall = Cilk_get_wall_time();
     StateInfoT *info;
     if (USE_PARAMETER(timer_state_info) == NULL || ws == NULL)
	  return;
     info = USE_PARAMETER(timer_state_info) + ws->self;
     stackp = info->state_stackp;
     CILK_ASSERT(ws, stackp < STATE_DEPTH);
     info->state_stack[stackp] = state;
     info->state_stack_begintime[stackp] = now;
     info->state_stack_begin_wall_time[stackp] = now_wall;
     info->state_stackp = stackp + 1;
     ++(info->stateentered[state]);
}

void Cilk_exit_state(CilkWorkerState *const ws, 
		     int state)
{
     int stackp;
     Cilk_time now = Cilk_get_time();
     Cilk_time now_wall = Cilk_get_wall_time();
     StateInfoT *info;
     if (USE_PARAMETER(timer_state_info) == NULL || ws == NULL)
	  return;
     info = USE_PARAMETER(timer_state_info) + ws->self;
     stackp = info->state_stackp - 1;
     info->state_stackp = stackp;
     CILK_ASSERT(ws, stackp >= 0);

     CILK_ASSERT(ws, info->state_stack[stackp] == state);
     info->statetime[state] += now - info->state_stack_begintime[stackp];
     info->state_wall_time[state] +=
	  now_wall - info->state_stack_begin_wall_time[stackp];
}

/* print time statistics */
void Cilk_print_time_statistics(CilkContext *const context)
{
     int i;
     StateInfoT *info = USE_PARAMETER1(timer_state_info);
     double worktime = 0.0;
     double totaltime = 0.0;

     fprintf(USE_PARAMETER1(infofile),
	     "\nSUMMARY OF TIME SPENT BY EACH PROCESSOR:\n\n"
	     " PN     Working       Total\n"
	     "---------------------------\n");
     for (i = 0; i < USE_PARAMETER1(active_size); i++) {
	  fprintf(USE_PARAMETER1(infofile), "%3d %11f %11f\n", i,
	      Cilk_time_to_sec(info[i].statetime[STATE_WORKING]),
	       Cilk_time_to_sec(info[i].statetime[STATE_TOTAL]));
	  worktime +=
	      Cilk_time_to_sec(info[i].statetime[STATE_WORKING]);
	  totaltime +=
	      Cilk_time_to_sec(info[i].statetime[STATE_TOTAL]);
     }
     fprintf(USE_PARAMETER1(infofile),
	     "---------------------------\n"
	     "TOT %11f %11f\n", worktime, totaltime);
}

void Cilk_summarize_time_statistics(CilkContext *const context)
{
     int i;
     struct state_desc *p;
     StateInfoT *info = USE_PARAMETER1(timer_state_info);

     fprintf(USE_PARAMETER1(infofile),
	     "\nSUMMARY OF TIME STATISTICS:\n\n");

     for (p = (struct state_desc *) state_descriptions; p->name; ++p) {
	  Cilk_time virtual = 0;
	  Cilk_time wall = 0;
	  int entered = 0;

	  for (i = 0; i < USE_PARAMETER1(active_size); i++) {
	       virtual += info[i].statetime[p->state];
	       wall += info[i].state_wall_time[p->state];
	       entered += info[i].stateentered[p->state];
	  }

	  if (entered > 0)
	       fprintf(USE_PARAMETER1(infofile),
		       "%s: %fs / wall %fs (entered %d times)\n",
		       p->name,
		       Cilk_time_to_sec(virtual),
		       Cilk_wall_time_to_sec(wall),
		       entered);
     }
}

void Cilk_print_detailed_time_statistics(CilkContext *const context)
{
     int i;
     struct state_desc *p;
     StateInfoT *info = USE_PARAMETER1(timer_state_info);

     fprintf(USE_PARAMETER1(infofile),
	     "\nDETAILS OF TIME SPENT BY EACH PROCESSOR:\n");

     for (i = 0; i < USE_PARAMETER1(active_size); i++) {
	  fprintf(USE_PARAMETER1(infofile),
		  "\n--- Time statistics for processor %d ---\n", i);

	  for (p = (struct state_desc *) state_descriptions; p->name; ++p) {
	       if (info[i].stateentered[p->state] > 0)
		    fprintf(USE_PARAMETER1(infofile),
			    "%s: %fs / wall %fs (entered %d times)\n",
			    p->name,
			    Cilk_time_to_sec(info[i].statetime
						     [p->state]),
			    Cilk_wall_time_to_sec(info[i].state_wall_time
						  [p->state]),
			    info[i].stateentered[p->state]);
	  }
     }
}

double Cilk_compute_work(CilkContext *const context)
{
     int i;
     double work = 0.0;
     StateInfoT *info = USE_PARAMETER1(timer_state_info);

     for (i = 0; i < USE_PARAMETER1(active_size); i++)
	  work += Cilk_time_to_sec(info[i].statetime[STATE_WORKING]);

     return work;
}

void Cilk_timing_init(CilkContext *const context)
{
     INIT_PARAMETER1(timer_state_info ,
		    (StateInfoT *) Cilk_malloc_fixed(sizeof(StateInfoT) *
					   USE_PARAMETER1(active_size)));
}

void Cilk_timing_terminate(CilkContext *const context)
{
     Cilk_free(USE_PARAMETER1(timer_state_info));
}

void Cilk_time_gathering_init(CilkContext *const context)
{
     int i, j;
     StateInfoT *info;

     info = USE_PARAMETER1(timer_state_info);

     for (i = 0; i < USE_PARAMETER1(active_size); i++) {
	  for (j = 0; j < TIME_NSTATES; ++j) {
	       info[i].statetime[j] = (Cilk_time) 0;
	       info[i].state_wall_time[j] = (Cilk_time) 0;
	       info[i].stateentered[j] = 0;
	  }
	  info[i].state_stackp = 0;
     }
}

#endif
