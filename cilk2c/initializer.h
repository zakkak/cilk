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

#ifndef _INITIALIZER_H_
#define _INITIALIZER_H_
FILE_IDENTITY(initializer_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/initializer.h $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

GLOBAL void SemCheckDeclInit(Node *decl, Bool blockp);
GLOBAL Bool IsInitializer(Node *node);
GLOBAL Node *InitializerCopy(Node *node);
GLOBAL int InitializerLength(Node *node);
GLOBAL Node *InitializerFirstItem(Node *node);
GLOBAL List *InitializerExprs(Node *node);
GLOBAL Bool InitializerEmptyList(Node *node);
GLOBAL void InitializerNext(Node *node);
GLOBAL Node *InitializerAppendItem(Node *initializer, Node *element);
GLOBAL Node *ArraySubtype(Node *node);
GLOBAL int ArrayNumberElements(Node *node);
GLOBAL SUEtype *StructUnionFields(Node *node);
GLOBAL Node *UnionFirstField(Node *node);
GLOBAL Node *SUE_MatchInitList(SUEtype *sue, Node *decl, Node *initializer, Bool top_p);

#endif				/* _INITIALIZER_H_ */
