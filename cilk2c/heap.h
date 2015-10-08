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

#ifndef _HEAP_H_
#define _HEAP_H_
FILE_IDENTITY(heap_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/heap.h $ $LastChangedBy: bradley $ $Rev: 1475 $ $Date: 2004-08-02 07:58:58 -0400 (Mon, 02 Aug 2004) $");

#define HeapNew(T) ((T *)HeapAllocate(1, sizeof(T)))
#define HeapNewArray(T, c) ((T *)HeapAllocate(c, sizeof(T)))

GLOBAL void *HeapAllocate(unsigned int number, unsigned int size);
GLOBAL void HeapFree(void *ptr);

#endif				/* ifndef _HEAP_H_ */
