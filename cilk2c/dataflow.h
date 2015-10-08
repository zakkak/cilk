/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Rob Miller
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

#ifndef _DATAFLOW_H_
#define _DATAFLOW_H_

FILE_IDENTITY(dataflow_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/dataflow.h $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

typedef unsigned long BitVector;
#define MAX_BITVECTOR_LENGTH    (sizeof(BitVector) * CHAR_BIT)

typedef struct {
     Bool undefined;
     union {
	  BitVector bitvector;
	  List *list;
	  Generic *ptr;
     } u;
} FlowValue;

typedef struct {
     /* Structures used in analysis */
     FlowValue gen, kill;

     /* Analysis results */
     List *livevars;
     List *dirtyvars;
} Analysis;

typedef enum {
     Backwards,
     Forwards
} Direction;

typedef enum {
     EntryPoint,
     ExitPoint
} Point;

typedef FlowValue (*MeetOp) (FlowValue, FlowValue);
typedef FlowValue (*TransOp) (Node *, FlowValue, Point, Bool);
typedef Bool (*EqualOp) (FlowValue, FlowValue);

/* from dataflow.c */
GLOBAL void IterateDataFlow(
				Node *root,	/* root node */
				FlowValue init,		/* input value for root node */
				Direction dir,	/* direction */
				MeetOp meet,	/* meet operation */
				EqualOp equal,	/* equality operation */
				TransOp trans	/* transfer function */
);

/* from analyze.c */
GLOBAL List *RegisterVariables(Node *node, List *vars);

GLOBAL void PV(List *vars);
GLOBAL int PrintVariables(GBUF *,List *vars);
GLOBAL int PrintAnalysis(GBUF *,Node *node);

GLOBAL void AnalyzeLiveVariables(Node *root, List *vars);
GLOBAL void AnalyzeReturnFlow(Node *root);

GLOBAL void AnalyzeProgram(List *program);

#endif
