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
 * This file used to contain a simple malloc()/free() for Cilk. 
 * Now we use the standard implementation on top of posix threads.
 */

#include <cilk.h>
#include <cilk-internal.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/malloc.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

#if HAVE_STRINGS_H
#include <strings.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

/*
 * use posix malloc()  (assumed thread-safe)
 */
void *Cilk_malloc(size_t size)
{
     void *x = malloc(size);
     /*printf("Cilk_malloc - size=%d p=%p\n", size, x);*/
     return x;
}

void *Cilk_malloc_fixed(size_t size)
{
     void *x = malloc(size);
    /*printf("Cilk_malloc_fixed - size=%d p=%p\n", size, x);*/
     return x;
}

#if HAVE_MEMALIGN
void *Cilk_memalign(size_t alignment, size_t size)
{
     /*printf("memalign\n");*/
     return memalign(alignment, size);
}
#endif

void Cilk_free(void *s)
{
    /*printf("Cilk_free - p=%p\n", s);*/
     free(s);

}

void *Cilk_calloc(size_t nelem, size_t elsize)
{
     /*printf("calloc\n");*/
     return calloc(nelem, elsize);
}

void *Cilk_realloc(void *s, size_t size)
{
     void* x = realloc(s, size);
     /*printf("Cilk_reaslloc newsize = %s p=%p newp= %p\n", size, s,x);*/
     return x;
}

void *Cilk_valloc(size_t size)
{
     /*printf("valloc\n");*/
     return valloc(size);
}

