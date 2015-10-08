
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 * 
 *************************************************************************/
/*
 * Copyright (c) 1994-2002 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "basics.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/list.c $ $LastChangedBy: bradley $ $Rev: 1378 $ $Date: 2004-06-22 00:03:24 -0400 (Tue, 22 Jun 2004) $");

/* node definition of struct List only appears in this file;
 * this enforces modularity */

struct liststruct {
     Generic *element;
     struct liststruct *next;
};

GLOBAL Generic *FirstItem(List *list)
{
     assert(list != NULL);
     return (list->element);
}

GLOBAL Generic *LastItem(List *list)
{
     ListMarker marker;
     Node *item;

     assert(list != NULL);

     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & item));

     return item;
}

GLOBAL List *Rest(List *list)
{
     assert(list != NULL);
     return list->next;
}

GLOBAL List *Last(List *list)
{
     if (list == NULL)
	  return list;

     while (list->next != NULL)
	  list = list->next;

     return list;
}

GLOBAL int ListLength(List *list)
{
     ListMarker marker;
     Node *item;
     int length;

     IterateList(&marker, list);
     for (length = 0; NextOnList(&marker, (GenericREF) & item); length++);

     return length;
}

GLOBAL Generic *SetItem(List *list, Generic *element)
{
     assert(list != NULL);
     list->element = element;
     return element;
}

GLOBAL List *MakeNewList(Generic *item)
{
     List *create = HeapNew(List);
     create->element = item;
     create->next = NULL;
     return (create);
}

GLOBAL List *ConsItem(Generic *item, List *list)
{
     List *el = MakeNewList(item);

     el->next = list;
     return el;
}

GLOBAL List *AppendItem(List *list, Generic *item)
{
     List *tail = MakeNewList(item);

     if (list == NULL)
	  return tail;
     else
	  return JoinLists(list, tail);
}

/* fix: this is slow... */
GLOBAL List *JoinLists(List *list1, List *list2)
{
     List *last = Last(list1);

     if (last == NULL)
	  return list2;
     last->next = list2;
     return (list1);
}

GLOBAL List *ListCopy(List *list)
{
     List *new;
     List **plast = &new;
     List *tmp;

     while (list) {
	  tmp = HeapNew(List);
	  tmp->element = list->element;
	  *plast = tmp;

	  list = list->next;
	  plast = &tmp->next;
     }

     *plast = NULL;
     return new;
}

GLOBAL List *List2(Generic *x1, Generic *x2)
{
     return ConsItem(x1, MakeNewList(x2));
}

GLOBAL List *List3(Generic *x1, Generic *x2, Generic *x3)
{
     return ConsItem(x1, ConsItem(x2, MakeNewList(x3)));
}

GLOBAL List *List4(Generic *x1, Generic *x2, Generic *x3, Generic *x4)
{
     return ConsItem(x1, ConsItem(x2, ConsItem(x3, MakeNewList(x4))));
}

GLOBAL List *List5(Generic *x1, Generic *x2, Generic *x3, Generic *x4, Generic *x5)
{
     return ConsItem(x1, ConsItem(x2, ConsItem(x3, ConsItem(x4, MakeNewList(x5)))));
}

GLOBAL void IterateList(ListMarker *marker, List *list)
{
     marker->first = list;	/* useful for when I switch to circular lists */
     marker->current = NULL;
     marker->tail = NULL;
}

GLOBAL Bool NextOnList(ListMarker *marker, GenericREF handle)
{
     if (marker == NULL)
	  return FALSE;
     else if (marker->current == NULL)
	  if (marker->first == NULL)
	       return FALSE;
	  else {
	       marker->current = marker->first;
     } else if (marker->current->next) {
	  marker->current = marker->current->next;
     } else if (marker->tail) {
	  /* reconnect temporary split caused by NextChunkOnList */
	  marker->current->next = marker->tail;
	  marker->tail = NULL;

	  marker->current = marker->current->next;
     } else
	  return FALSE;

     *handle = marker->current->element;
     return TRUE;
}

GLOBAL Bool EndOfList(ListMarker *marker)
{
     if (!marker)
	  return TRUE;
     if (!marker->current)
	  return (!marker->first);
     
     if (marker->current->next) 
	  return FALSE;

     return (!marker->tail);
}


/* Requires that previous NextOnList(marker) returned true. */
GLOBAL void SetCurrentOnList(ListMarker *marker, Generic *handle)
{
     assert(marker && marker->current);
     marker->current->element = handle;
}

/* Terminate the current list after the point and return the tail.
 * Requires that previous NextOnList(marker) returned true. */
GLOBAL List *SplitList(ListMarker *marker)
{
     List *tail;

     assert(marker && marker->current);
     tail = (marker->current)->next;
     (marker->current)->next = NULL;
     return tail;
}

/* Insert sublist before the point. Returns beginning of whole list
 * being iterated. */
GLOBAL List *InsertList(ListMarker *marker, List *sublist)
{
     if (!marker->current) {
	  /* point is before beginning of list */
	  marker->current = Last(sublist);
	  marker->first = JoinLists(sublist, marker->first);
     } else {
	  List *prev;

	  for (prev = marker->first;
	       prev != NULL && prev->next != marker->current;
	       prev = prev->next);
	  if (prev == NULL)
	       marker->first = JoinLists(sublist, marker->first);
	  else
	       prev->next = JoinLists(sublist, prev->next);
     }

     return marker->first;
}

/* Inserts sublist after point (so sublist will be iterated in subsequent
 * calls to NextOnList).  Returns whole list being iterated. */
GLOBAL List *SpliceList(ListMarker *marker, List *sublist)
{
     if (!marker->current) {
	  /* point is before beginning of list */
	  marker->first = JoinLists(sublist, marker->first);
     } else {
	  marker->current->next = JoinLists(sublist, marker->current->next);
     }

     return marker->first;
}

/* FindItem returns sublist whose head is item, or NULL if item not
 * found in list. */
GLOBAL List *FindItem(List *list, Generic *item)
{
     while (list) {
	  if (item == list->element)
	       return list;
	  list = list->next;
     }
     return NULL;
}

/* RemoveItem mutates list to remove first item matching <item>.  Does nothing
 * if <item> not found. */
GLOBAL List *RemoveItem(List *list, Generic *item)
{
     List *prev = NULL;
     List *curr = list;

     while (curr) {
	  if (item == curr->element) {
	       if (prev)
		    prev->next = curr->next;
	       else
		    list = curr->next;
	       break;
	  }
	  prev = curr;
	  curr = curr->next;
     }
     return list;
}

/* Splits off at most <chunksize> elements starting at point into a temporarily
 * NULL-terminated list.  (Subsequent NextOnList or NextChunkOnList call will
 * re-connect the separated list.)  If fewer than chunksize elements remain
 * to be iterated, will return all of them. */
GLOBAL List *NextChunkOnList(ListMarker *m, int chunksize)
{
     Generic *trash;
     List *chunk_begin = m->current ? m->tail : m->first;
     int i;

     if (chunksize == 0)
	  return NULL;

     i = 0;
     while (i < chunksize && NextOnList(m, &trash))
	  ++i;

     /* Terminate list after point */
     if (m->current) {
	  m->tail = (m->current)->next;
	  (m->current)->next = NULL;
     }
     assert(ListLength(chunk_begin) <= chunksize);
     return chunk_begin;
}

GLOBAL List *Mapc(List *l, void(*f)(Generic*)) /* apply f to every element of the list. */
{
    ListMarker marker;
    Generic *item;
    IterateList(&marker, l);
    while (NextOnList(&marker, (GenericREF) &item)) {
	f(item);
    }
    return l;
} /*  */

GLOBAL List *DeleteIf(List *l, int(*f)(Generic*)) /* modify the list to remove every element for which f() returns non-zero. */
{
     if (l) {
	 if (f(l->element)) {
	     return DeleteIf(l->next, f);
	 } else {
	     l->next = DeleteIf(l->next, f);
	     return l;
	 }
     } else {
	 return l;
     }
}
