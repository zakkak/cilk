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
 * Copyright (c) 2002 Matteo Frigo
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

#ifndef _LIST_H_
#define _LIST_H_
FILE_IDENTITY(list_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/list.h $ $LastChangedBy: bradley $ $Rev: 1378 $ $Date: 2004-06-22 00:03:24 -0400 (Tue, 22 Jun 2004) $");

typedef struct liststruct List;

typedef struct {
     List *first, *current, *tail;
} ListMarker;

GLOBAL Generic *FirstItem(List *list);
GLOBAL Generic *LastItem(List *list);
GLOBAL Generic *SetItem(List *list, Generic *element);
GLOBAL List *Rest(List *list);
GLOBAL List *Last(List *list);
GLOBAL int ListLength(List *list);

GLOBAL List *FindItem(List *list, Generic *item);
GLOBAL List *RemoveItem(List *list, Generic *item);

GLOBAL List *MakeNewList(Generic *item);
GLOBAL List *ConsItem(Generic *item, List *list);
GLOBAL List *AppendItem(List *list, Generic *item);
GLOBAL List *JoinLists(List *list1, List *list2);
GLOBAL List *ListCopy(List *list);

#define List1 MakeNewList
GLOBAL List *List2(Generic *x1, Generic *x2);
GLOBAL List *List3(Generic *x1, Generic *x2, Generic *x3);
GLOBAL List *List4(Generic *x1, Generic *x2, Generic *x3, Generic *x4);
GLOBAL List *List5(Generic *x1, Generic *x2, Generic *x3, Generic *x4, Generic *x5);

/* ListMarker passed by reference in the following */
GLOBAL void IterateList(ListMarker *, List *);
GLOBAL Bool NextOnList(ListMarker *, GenericREF itemref);
GLOBAL List *InsertList(ListMarker *marker, List *list);
GLOBAL List *SplitList(ListMarker *marker);
GLOBAL void SetCurrentOnList(ListMarker *marker, Generic *handle);
GLOBAL List *NextChunkOnList(ListMarker *, int chunksize);
GLOBAL Bool EndOfList(ListMarker *marker);

GLOBAL List *Mapc(List *, void(*f)(Generic*)); /* apply f to every element of the list. */
GLOBAL List *DeleteIf(List *, int(*f)(Generic*)); /* modify the list to remove every element for which f() returns non-zero. */

#endif				/* ifndef _LIST_H_ */
