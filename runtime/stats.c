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

/***********************************************************\
 * Statistics gathering
\***********************************************************/

#include <cilk.h>
#include <cilk-internal.h>

#include <stdio.h>
#include <string.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/stats.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");


void Cilk_stats_init(CilkContext *const context)
{
#if CILK_STATS
     INIT_PARAMETER1(stat_array,
		    (StatsT *) Cilk_malloc_fixed(sizeof(StatsT) *
						 USE_PARAMETER1(active_size)));
#endif

      if (strcmp(USE_PARAMETER1(options->infofile_name),"-") == 0 )
           USE_PARAMETER1(infofile) = stderr;
      else
           USE_PARAMETER1(infofile) =
                fopen(USE_PARAMETER1(options->infofile_name), "w");

      CILK_CHECK(USE_PARAMETER1(infofile),
                 (context, NULL,
                  "can't open %s for writing\n",
                  USE_PARAMETER1(options->infofile_name)));
}

#if CILK_STATS

/*
 * table of description of events. It is printed in
 * the same order.
 */
static const struct event_desc {
     char *name;
     int event;
     int verbosity;   /* if statslevel >= 2 + verbosity this thing is printed */
} event_descriptions[] = {
     {
	  "Threads", EVENT_THREAD,               0
     },
#if 0
     /* this is the same as # of steals, if there are no bugs */
     {
	  "Closures", EVENT_CLOSURE_CREATE,      0
     },
#endif
     {
	  "Steals", EVENT_STEAL,                 0
     },
     {
	  "Attempts", EVENT_STEAL_ATTEMPT,       1
     },
     {
	  "Fail/EmptyQ", EVENT_STEAL_EMPTY_DEQUE,  4
     },
     {
	  "Fail/Dekker", EVENT_STEAL_NO_DEKKER,    4
     },
     {
	  "Fail/Return", EVENT_STEAL_RETURNING,    4
     },
     {
	  "Fail/Abort", EVENT_STEAL_ABORT,         4
     },
     {
	  "Abort/Slow", EVENT_ABORT_SLOW,          0
     },
     {
	  "Abort/Alone", EVENT_ABORT_STANDALONE,   1
     },
     {
	  "Ready->Susp", EVENT_ABORT_READY_SUSPEND,  4
     },
     {
	  "Ready->Ret", EVENT_ABORT_READY_RETURN,    4
     },
     {
	  "Prov. good", EVENT_PROVABLY_GOOD_STEAL,   4
     },
     {
	  "Suspend", EVENT_SUSPEND,                  1
     },
     {
	  "Sync check", EVENT_CILK_SYNC,             1
     },
     {
	  "Exceptions", EVENT_EXCEPTION,             1
     },
     {
	  "Ex/Other", EVENT_EXCEPTION_OTHER,         4
     },
     {
	  "Ex/Steal", EVENT_EXCEPTION_STEAL,         4
     },
     {
	  "Ex/Abort", EVENT_EXCEPTION_ABORT,         4
     },
     {
	  "Ex/Abort_ret", EVENT_EXCEPTION_ABORT_RETURN, 4
     },
     {
	  "Ex/Abort_susp", EVENT_EXCEPTION_ABORT_SUSPEND, 4
     },
     {
	  "Return/slow", EVENT_RETURN_SLOW,          4
     },
     {
	  "Enqueue", EVENT_RETURN_ENQUEUE,           4
     },
     {
	  "Poll", EVENT_POLL_INLETS,                 2
     },
     {
	  "Alloc batch", EVENT_IM_ALLOC_BATCH,       2
     },
     {
	  "Free batch", EVENT_IM_FREE_BATCH,         2
     },
     {
	  "User 0", EVENT_USER0, 0
     },
     {
	  "User 1", EVENT_USER1, 0
     },
     {
	  "User 2", EVENT_USER2, 0
     },
     {
	  "User 3", EVENT_USER3, 0
     },
     {
	  "User 4", EVENT_USER4, 0
     },
     {
	  "User 5", EVENT_USER5, 0
     },
     {
	  "User 6", EVENT_USER7, 0
     },
     {
	  "User 7", EVENT_USER7, 0
     },
     {
	  (char *) 0, 0, 0
     }
};

void Cilk_stats_terminate(CilkContext *const context)
{
     Cilk_free(USE_PARAMETER1(stat_array));

     /* close infofile if it not the default stderr */
     if (strcmp(USE_PARAMETER1(options->infofile_name),"-") != 0 )
     	fclose(USE_PARAMETER1(infofile));
}

void Cilk_event_gathering_init(CilkContext *const context)
{
     int i,j;

     for (i = 0; i < USE_PARAMETER1(active_size); i++)
	  for (j = 0; j < EVENT_NTYPES; j++)
	       USE_PARAMETER1(stat_array)[i].event_count[j] = 0;
}

#define NAME_DESC "%13s"
#define FIELD_DESC "%9ld"
#define TOTAL_DESC "%10s"
#define TOTAL_FIELD_DESC "%10ld"
#define PROC_PER_LINE 5

void Cilk_print_rts_statistics(CilkContext *const context)
{
     int i, j;
     int start, end, line;
     StatsT total;
     struct event_desc *p;

     USE_PARAMETER1(max_stack_depth) = 0;

     for (j = 0; j < EVENT_NTYPES; j++)
	  total.event_count[j] = 0;

     /* compute totals and max */
     for (i = 0; i < USE_PARAMETER1(active_size); ++i){
          if(USE_PARAMETER1(stat_array)[i].max_stack_depth > USE_PARAMETER1(max_stack_depth) )
	  	USE_PARAMETER1(max_stack_depth) = USE_PARAMETER1(stat_array)[i].max_stack_depth;

	  for (j = 0; j < EVENT_NTYPES; j++)
	       total.event_count[j] +=
		    USE_PARAMETER1(stat_array)[i].event_count[j];
     }

     USE_PARAMETER1(num_threads) = total.event_count[EVENT_THREAD];
     USE_PARAMETER1(num_steals) = total.event_count[EVENT_STEAL] +
                       total.event_count[EVENT_PROVABLY_GOOD_STEAL];

     for (line = 0;
	  line <= (USE_PARAMETER1(active_size) - 1) / PROC_PER_LINE;
	  ++line) {
	  start = line * PROC_PER_LINE;
	  end = (line + 1) * PROC_PER_LINE;
	  if (end > USE_PARAMETER1(active_size))
	       end = USE_PARAMETER1(active_size);

	  fprintf(USE_PARAMETER1(infofile), NAME_DESC, "PN");
	  for (i = start; i < end; ++i)
	       fprintf(USE_PARAMETER1(infofile), FIELD_DESC, (long) i);

	  if (i == USE_PARAMETER1(active_size))
	       fprintf(USE_PARAMETER1(infofile), TOTAL_DESC, "Total");

	  fprintf(USE_PARAMETER1(infofile), "\n");

	  for (p = (struct event_desc *) event_descriptions; p->name; p++) {
	       if (!total.event_count[p->event])
		    continue;  /* skip if total is 0 */

	       if (p->verbosity > USE_PARAMETER1(options->statlevel) - 2)
		    continue;  /* skip if stat level too low */

	       fprintf(USE_PARAMETER1(infofile), NAME_DESC, p->name);
	       for (i = start; i < end; ++i)
		    fprintf(USE_PARAMETER1(infofile), FIELD_DESC,
			    USE_PARAMETER1(stat_array)[i].event_count[p->event]);
	       if (i == USE_PARAMETER1(active_size))
		    fprintf(USE_PARAMETER1(infofile), TOTAL_FIELD_DESC,
			    total.event_count[p->event]);
	       fprintf(USE_PARAMETER1(infofile), "\n");
	  }

	  fprintf(USE_PARAMETER1(infofile), "\n");

     }
}

void Cilk_event(CilkWorkerState *const ws, int type)
{
     if (ws)
	  ++USE_PARAMETER(stat_array)[ws->self].event_count[type];
}

void Cilk_event_new_thread(CilkWorkerState *const ws)
{
     if (ws)
	  ++USE_PARAMETER(stat_array)[ws->self].event_count[EVENT_THREAD];
}

void Cilk_increment_curr_stack_depth(CilkWorkerState *const ws)
{
     if (ws) {
	++USE_PARAMETER(stat_array)[ws->self].curr_stack_depth;
	if(USE_PARAMETER(stat_array)[ws->self].curr_stack_depth >
		USE_PARAMETER(stat_array)[ws->self].max_stack_depth )
			USE_PARAMETER(stat_array)[ws->self].max_stack_depth =
				USE_PARAMETER(stat_array)[ws->self].curr_stack_depth;
     /*printf("increment - %d curr=%d max=%d\n", ws->self,
     USE_PARAMETER(stat_array)[ws->self].curr_stack_depth,
     USE_PARAMETER(stat_array)[ws->self].max_stack_depth );*/

     }
}

void Cilk_decrement_curr_stack_depth(CilkWorkerState *const ws)
{
     if (ws){
        if( USE_PARAMETER(stat_array)[ws->self].curr_stack_depth > 0 )
		--USE_PARAMETER(stat_array)[ws->self].curr_stack_depth;
	/*else
		printf("decrement - %d trying to dec from 0 \n", ws->self);*/
    /*printf("decrement - %d curr=%d max=%d\n", ws->self,
     USE_PARAMETER(stat_array)[ws->self].curr_stack_depth,
     USE_PARAMETER(stat_array)[ws->self].max_stack_depth );*/
     }
}

void Cilk_reset_stack_depth_stats(CilkWorkerState *const ws)
{
     if (ws) {
	USE_PARAMETER(stat_array)[ws->self].curr_stack_depth = 1;
	USE_PARAMETER(stat_array)[ws->self].max_stack_depth = 1;
    }
}


#endif				/* CILK_STATS */
