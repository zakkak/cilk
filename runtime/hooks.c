/*
 * $Id: debug.c,v 1.1.1.1 2000/06/22 08:04:48 matley Exp $
 */
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

/* hook manager */
#include <cilk.h>
#include <cilk-internal.h>
#include <stdlib.h>

void Cilk_add_hook(HookList **listp, HookT fn)
{
     HookList *new;

     new = (HookList *)malloc(sizeof (HookList));

     new->fn = fn;
     new->next = NULL_HOOK;

     /* traverse the list */
     while (*listp != NULL_HOOK)
	  listp = &((*listp)->next);

     /* append at the end */
     *listp = new;
}

void Cilk_run_hooks(HookList *list)
{
     for ( ; list != NULL_HOOK ; list = list->next) 
	  list->fn();
}
	  
