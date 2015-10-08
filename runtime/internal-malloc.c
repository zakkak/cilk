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

#include <cilk.h>
#include <cilk-internal.h>
#include <stdio.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/internal-malloc.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

#define DEFAULT_ALLOC_LIST_SIZE   10

/* compute the length of a free list */
static int free_list_length(void *p)
{
     int l = 0;
     while (p) {
	  ++l;
	  p = ((void **)p)[0];
     }
     return l;
}

/* Free the allocation list */
static void free_alloc_list(CilkContext *const context)
{
     int i;

     for( i = 0; i <= USE_SHARED1(alloc_list_index) ; i++ )
        Cilk_free(USE_SHARED1(alloc_list)[i]);

     USE_SHARED1(alloc_list_index) = -1;
}

/*************************************************************
 * global allocator
 *************************************************************/
#define CILK_INTERNAL_MALLOC_CHUNK (32 * 1024)

static void extend_global_pool(CilkWorkerState *const ws)
{
     USE_SHARED(global_pool_begin) = Cilk_malloc(CILK_INTERNAL_MALLOC_CHUNK);
     CILK_CHECK(USE_SHARED(global_pool_begin),
		(ws->context, ws, "internal malloc() out of memory\n"));
     USE_SHARED(global_pool_end) = USE_SHARED(global_pool_begin) +
	  CILK_INTERNAL_MALLOC_CHUNK;
     USE_SHARED(im_allocated) += 
	  CILK_INTERNAL_MALLOC_CHUNK;
    
    /* Now add this chunk to alloc list */
    USE_SHARED(alloc_list_index)++;

    /* First, enlarge alloc_list if it's already full */
    if( USE_SHARED(alloc_list_index) >= USE_SHARED(alloc_list_size) ) {

        USE_SHARED(alloc_list) = Cilk_realloc(USE_SHARED(alloc_list), (USE_SHARED(alloc_list_size) + DEFAULT_ALLOC_LIST_SIZE ) * sizeof(char*) );

        CILK_CHECK(USE_SHARED(alloc_list),
		(ws->context, ws, "internal malloc() out of memory\n"));

        USE_SHARED(alloc_list_size) += DEFAULT_ALLOC_LIST_SIZE;
   }

   USE_SHARED(alloc_list)[USE_SHARED(alloc_list_index)] = USE_SHARED(global_pool_begin);
}

/* the size is already canonicalized at this point */
static void *global_im_alloc(CilkWorkerState *const ws, 
			     size_t size, int bucket)
{
     void *mem;
     struct Cilk_im_descriptor *d;

     CILK_ASSERT(ws, size <= CILK_INTERNAL_MALLOC_THRESHOLD);
     CILK_ASSERT(ws, bucket < CILK_INTERNAL_MALLOC_BUCKETS);

     WHEN_CILK_DEBUG(USE_SHARED(global_im_info).used += size);
     WHEN_CILK_DEBUG(USE_SHARED(global_im_info).nmalloc += 1);

     d = USE_SHARED(global_im_descriptor) + bucket;

     /* look at the global free list for this bucket */
     if ((mem = d->free_list)) {
	  /* if there is a block in the free list */
	  CILK_ASSERT(ws, mem);
	  d->free_list = ((void **) mem)[0];
	  d->count++;
     } else {
	  /* allocate from the global pool */
	  if (USE_SHARED(global_pool_begin) + size >
	      USE_SHARED(global_pool_end)) {
	       /*
		* For now, we waste the fragment of pool.
		* We could put the fragment in the free list instead...
		*/
	       USE_SHARED(im_wasted) += 
		    (USE_SHARED(global_pool_end) -
		     USE_SHARED(global_pool_begin));
	       extend_global_pool(ws);
	  }
	  mem = USE_SHARED(global_pool_begin);
	  USE_SHARED(global_pool_begin) += size;
     }

     return mem;
}

/*
 * global_free simply returns the block to the free list
 */
static void global_im_free(CilkWorkerState *const ws,
			   void *p, size_t UNUSED(size), int bucket)
{
     struct Cilk_im_descriptor *d;

     CILK_ASSERT(ws, size <= CILK_INTERNAL_MALLOC_THRESHOLD);
     CILK_ASSERT(ws, bucket < CILK_INTERNAL_MALLOC_BUCKETS);
     WHEN_CILK_DEBUG(USE_SHARED(global_im_info).used -= size);
     WHEN_CILK_DEBUG(USE_SHARED(global_im_info).nmalloc -= 1);

     d = USE_SHARED(global_im_descriptor) + bucket;
     ((void **)p)[0] = d->free_list;
     d->free_list = p;
     d->count--;
}

static void im_allocate_batch(CilkWorkerState *const ws,
			      size_t size, int bucket)
{
     int i;
     void *p;

     Cilk_event(ws, EVENT_IM_ALLOC_BATCH);
     Cilk_mutex_wait(ws->context, ws, &USE_SHARED(global_im_lock));
     for (i = 0; i < USE_PARAMETER(options->alloc_batch_size) / 2; ++i) {
	  p = global_im_alloc(ws, size, bucket);
	  Cilk_internal_free(ws, p, size);
     }
     Cilk_mutex_signal(ws->context, &USE_SHARED(global_im_lock));
}

static void im_free_batch(CilkWorkerState *const ws,
			  size_t size, int bucket)
{
     int i;
     void *p;

     Cilk_event(ws, EVENT_IM_FREE_BATCH);
     Cilk_mutex_wait(ws->context, ws, &USE_SHARED(global_im_lock));
     for (i = 0; i < USE_PARAMETER(options->alloc_batch_size) / 2; ++i) {
	  p = Cilk_internal_malloc(ws, size);
	  global_im_free(ws, p, size, bucket);
     }
     Cilk_mutex_signal(ws->context, &USE_SHARED(global_im_lock));
}
 

void Cilk_internal_malloc_global_init_2(CilkContext *const context)
{
     int i;
     struct Cilk_im_descriptor *d;

     USE_SHARED1(global_im_info).used = 0;
     USE_SHARED1(global_im_info).nmalloc = 0;

     for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  USE_PARAMETER1(im_info)[i].used = 0;
	  USE_PARAMETER1(im_info)[i].nmalloc = 0;

	  /*
	   * bogus value.
	   * Will be updated by Cilk_internal_malloc_per_worker_cleanup
	   */
	  USE_PARAMETER1(im_info)[i].in_free_lists = -1;
     }

     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  d = USE_SHARED1(global_im_descriptor) + i;
	  d->free_list = (void *) 0;
	  d->count = USE_PARAMETER1(options->alloc_batch_size);
     }

     USE_SHARED1(alloc_list_index)= -1;
     USE_SHARED1(global_pool_begin) = (char*)0;
     USE_SHARED1(global_pool_end) = (char*)0;
     USE_SHARED1(im_allocated) = 0;
     USE_SHARED1(im_wasted) = 0;
}

void Cilk_internal_malloc_global_init(CilkContext *const context)
{
     INIT_PARAMETER1(im_info,
		    Cilk_malloc_fixed(USE_PARAMETER1(active_size) *
				      sizeof(struct Cilk_im_stats)));
     CILK_CHECK(USE_PARAMETER1(im_info),
		(context, NULL, "Can't allocate Cilk_im_info\n"));

     Cilk_mutex_init(context, &USE_SHARED1(global_im_lock));

     USE_SHARED1(alloc_list) = Cilk_malloc_fixed( DEFAULT_ALLOC_LIST_SIZE *
						  sizeof(char*));
     CILK_CHECK(USE_SHARED1(alloc_list),
	       (context, NULL, "Can't allocate alloc_list\n"));

     USE_SHARED1(alloc_list_size) = DEFAULT_ALLOC_LIST_SIZE;
}

void Cilk_internal_malloc_global_terminate(CilkContext *const context)
{
     Cilk_mutex_destroy(context, &USE_SHARED1(global_im_lock));
     Cilk_free(USE_PARAMETER1(im_info));
     Cilk_free(USE_SHARED1(alloc_list));
}

void Cilk_internal_malloc_global_terminate_2(CilkContext *const context)
{
    free_alloc_list(context);
}


/*************************************************************
 * Processor-level allocator
 *************************************************************/
void *Cilk_internal_malloc(CilkWorkerState *const ws, size_t size)
{
     int bucket;
     void *mem;
     struct Cilk_im_descriptor *d;

     CILK_ASSERT(ws, ws->self >= 0);

     WHEN_CILK_DEBUG(USE_PARAMETER(im_info)[ws->self].used += size);
     WHEN_CILK_DEBUG(USE_PARAMETER(im_info)[ws->self].nmalloc += 1);

     if (size > CILK_INTERNAL_MALLOC_THRESHOLD)
	  return Cilk_malloc(size);

     size = Cilk_internal_malloc_canonicalize(size);
     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;
     while (!((mem = d->free_list))) {
	  im_allocate_batch(ws, size, bucket);
     }

     /* if there is a block in the free list */
     CILK_ASSERT(ws, mem);
     d->free_list = ((void **) mem)[0];
     d->count++;

     return mem;
}

/*
 * FREE simply returns to the free list 
 */
void Cilk_internal_free(CilkWorkerState *const ws, void *p, size_t size)
{
     int bucket;
     struct Cilk_im_descriptor *d;

     WHEN_CILK_DEBUG(USE_PARAMETER(im_info)[ws->self].used -= size);
     WHEN_CILK_DEBUG(USE_PARAMETER(im_info)[ws->self].nmalloc -= 1);

     if (size > CILK_INTERNAL_MALLOC_THRESHOLD) {
	  Cilk_free(p);
	  return;
     }

     bucket = Cilk_internal_malloc_size_to_bucket(size);
     d = ws->im_descriptor + bucket;

     while (d->count <= 0)
	  im_free_batch(ws, size, bucket);

     ((void **)p)[0] = d->free_list;
     d->free_list = p;
     d->count--;
}

void Cilk_internal_malloc_per_worker_init(CilkWorkerState *const ws)
{
     int i;
     struct Cilk_im_descriptor *d;

     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  d = ws->im_descriptor + i;
	  d->free_list = (void *) 0;
	  d->count = USE_PARAMETER(options->alloc_batch_size);
     }
}

void Cilk_internal_malloc_per_worker_cleanup(CilkWorkerState *const ws)
{
     int in_free_lists;
     struct Cilk_im_descriptor *d;
     int i, k;

     in_free_lists = 0;
     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  d = ws->im_descriptor + i;
	  k = free_list_length(d->free_list);
	  CILK_COMPLAIN(d->count + k == USE_PARAMETER(options->alloc_batch_size),
	       (ws->context, ws,
		"Cilk runtime system: Cilk detected an inconsistency in\n"
		"the free lists used internally for memory management.\n"
		"Either this is a cilk bug (unlikely), or your program\n"
		"corrupted some Cilk data structures.\n"));
	  d->length = k;
	  in_free_lists += Cilk_internal_malloc_bucket_to_size(i) * k;
	  USE_PARAMETER(im_info)[ws->self].length[i] = k;
     }

     USE_PARAMETER(im_info)[ws->self].in_free_lists = in_free_lists;
}

/*
 * support for alloca
 */
#if CILK_ALLOCA
void *Cilk_alloca_internal(CilkWorkerState *const ws, size_t size) 
{
     CilkStackFrame *f = (CilkStackFrame *)ws->cache.tail[-1];
     struct cilk_alloca_header *p;

     size += sizeof(struct cilk_alloca_header);
     p = Cilk_internal_malloc(ws, size);
     p->size = size;
     p->next = f->alloca_h;
     f->alloca_h = p;
     return (void *)(p+1);
}

void Cilk_unalloca_internal(CilkWorkerState *const ws, 
			    CilkStackFrame *f)
{
     struct cilk_alloca_header *p;
     struct cilk_alloca_header *q;
     for (p = f->alloca_h; p; p = q) {
	  q = p->next; Cilk_internal_free(ws, p, p->size);
     }
}
#endif /* CILK_ALLOCA */

/*************************************************************
 * Debugging and statistics
 *************************************************************/

void Cilk_internal_malloc_global_cleanup(CilkContext *const context)
{
     int i, total, tm, k;
     int in_free_lists;
     struct Cilk_im_descriptor *d;

     total = USE_SHARED1(global_im_info).used;
     tm = USE_SHARED1(global_im_info).nmalloc;
     for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  total += USE_PARAMETER1(im_info)[i].used;
	  tm += USE_PARAMETER1(im_info)[i].nmalloc;
     }

     CILK_COMPLAIN(total == 0 && tm == 0,
      	   (context, NULL,
	    "Cilk runtime system: Cilk detected a memory leak.\n"
      	    "Either this is a cilk bug (unlikely), or your program\n"
      	    "exited without giving Cilk a chance to free its own\n"
	    "memory.  Make sure you use Cilk_exit() and not exit().\n"));

     in_free_lists = 0;
     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  d = USE_SHARED1(global_im_descriptor) + i;
	  k = free_list_length(d->free_list);
	  CILK_COMPLAIN(d->count + k == USE_PARAMETER1(options->alloc_batch_size),
	       (context, NULL,
		"Cilk runtime system: Cilk detected an inconsistency in\n"
		"the free lists used internally for memory management.\n"
		"Either this is a cilk bug (unlikely), or your program\n"
		"corrupted some Cilk data structures.\n"));
	  d->length = k;
	  in_free_lists += Cilk_internal_malloc_bucket_to_size(i) * k;
	  USE_SHARED1(global_im_info).length[i] = k;
     }
     USE_SHARED1(global_im_info).in_free_lists = in_free_lists;
}

#if CILK_STATS
#define FIELD_DESC "%6d"
#define PN_DESC "%7d"
#define PN_NAME_DESC "%7s"

static void print_detailed_im_stats(CilkContext *const context)
{
     int i, j;

     fprintf(USE_PARAMETER1(infofile),
	     "\n"
	     "Detailed count of elements in allocator buckets:\n"
	     "\n");

     fprintf(USE_PARAMETER1(infofile), PN_NAME_DESC, "PN\\size");
     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  fprintf(USE_PARAMETER1(infofile), FIELD_DESC,
		  Cilk_internal_malloc_bucket_to_size(i));
     }
     fprintf(USE_PARAMETER1(infofile), "\n");

     fprintf(USE_PARAMETER1(infofile), PN_NAME_DESC, "Global");
     for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	  fprintf(USE_PARAMETER1(infofile), FIELD_DESC,
		  USE_SHARED1(global_im_info).length[i]);
     }
     fprintf(USE_PARAMETER1(infofile), "\n");

     for (j = 0; j < USE_PARAMETER1(active_size); ++j) {
	  fprintf(USE_PARAMETER1(infofile), PN_DESC, j);
	  for (i = 0; i < CILK_INTERNAL_MALLOC_BUCKETS; ++i) {
	       fprintf(USE_PARAMETER1(infofile), FIELD_DESC,
		       USE_PARAMETER1(im_info)[j].length[i]);
	  }
	  fprintf(USE_PARAMETER1(infofile), "\n");
     }

     fprintf(USE_PARAMETER1(infofile), "\n");
}

#define FORMAT "%14d\n"
void Cilk_internal_malloc_print_statistics(CilkContext *const context)
{
     int i;
     int in_free_lists = 0;

     fprintf(USE_PARAMETER1(infofile),
	     "\nSCHEDULER MEMORY STATISTICS:\n"
	     "\n"
	     "Total memory allocated for the scheduler: %d bytes\n"
	     "Unfragmented memory in global pool: %ld bytes\n"
	     "Memory wasted because of fragmentation: %d bytes\n"
	     "Memory in free lists at end of execution (in bytes):\n",
	     USE_SHARED1(im_allocated),
	     (long) (USE_SHARED1(global_pool_end) -
		     USE_SHARED1(global_pool_begin)),
	     USE_SHARED1(im_wasted));

     fprintf(USE_PARAMETER1(infofile),
	     "      PN     free-list\n"
	     "  --------------------\n");

     fprintf(USE_PARAMETER1(infofile), "  Global" FORMAT,
	     USE_SHARED1(global_im_info).in_free_lists);
     in_free_lists += USE_SHARED1(global_im_info).in_free_lists;

     for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
	  fprintf(USE_PARAMETER1(infofile), "     %3d" FORMAT, i,
		  USE_PARAMETER1(im_info)[i].in_free_lists);
	  in_free_lists += USE_PARAMETER1(im_info)[i].in_free_lists;
     }

     fprintf(USE_PARAMETER1(infofile),
	     "  --------------------\n");

     fprintf(USE_PARAMETER1(infofile), "        " FORMAT, in_free_lists);

     if (USE_PARAMETER1(options->statlevel) >= 4)
	  print_detailed_im_stats(context);
}
#endif /* CILK_STATS */
