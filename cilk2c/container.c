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

#include "ast.h"
FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/container.c $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

typedef struct {
     NodeType typ;
     List *cases;
     List *exits;
} Container;

PRIVATE Container stack[MAX_SCOPE_DEPTH];
PRIVATE Container *top = NULL;

GLOBAL void PushContainer(NodeType typ)
{
     if (top == NULL)
	  top = stack;
     else
	  top++;

     top->typ = typ;
     top->cases = top->exits = NULL;
}

GLOBAL Node *PopContainer(Node *n)
{
     ListMarker marker;
     Node *containee;

     assert(n->typ == top->typ);
     assert(top != NULL);

     IterateList(&marker, top->exits);
     while (NextOnList(&marker, (GenericREF) & containee)) {
	  switch (containee->typ) {
	      case Break:
		   containee->u.Break.container = n;
		   break;
	      case Continue:
		   assert(n->typ != Switch);
		   containee->u.Continue.container = n;
		   break;
	      default:
		   FAIL("unexpected containee type");
	  }
     }

     IterateList(&marker, top->cases);
     while (NextOnList(&marker, (GenericREF) & containee)) {
	  assert(n->typ == Switch);
	  switch (containee->typ) {
	      case Case:
		   containee->u.Case.container = n;
		   break;
	      case Default:
		   containee->u.Default.container = n;
		   n->u.Switch.has_default = TRUE;
		   break;
	      default:
		   FAIL("unexpected containee type");
	  }
     }

     /* store cases list into enclosing switch node */
     if (n->typ == Switch)
	  n->u.Switch.cases = top->cases;

     /* memory leak: exits list */

     if (top == stack)
	  top = NULL;
     else
	  top--;

     return (n);
}

GLOBAL Node *AddContainee(Node *c)
{
     Container *container;

     if (top == NULL) {
	  switch (c->typ) {
	      case Case:
	      case Default:
		   goto NoSwitch;
	      case Break:
		   SyntaxErrorCoord(c->coord,
			  "no enclosing loop or switch statement found");
		   return (c);
	      case Continue:
		   goto NoLoop;
	      default:
		   break;
	  }
     }
     switch (c->typ) {
	 case Case:
	 case Default:
	      for (container = top; container >= stack; container--) {
		   if (container->typ == Switch) {
			if (container->cases == NULL)
			     container->cases = MakeNewList(c);
			else
			     AppendItem(container->cases, c);
			return (c);
		   }
	      }
	    NoSwitch:
	      /* didn't find an enclosing switch... */
	      SyntaxErrorCoord(c->coord, "no enclosing switch statement");
	      break;
	 case Break:
	      if (top->exits == NULL)
		   top->exits = MakeNewList(c);
	      else
		   AppendItem(top->exits, c);
	      break;
	 case Continue:
	      for (container = top; container >= stack; container--) {
		   if (container->typ != Switch) {
			if (container->exits == NULL)
			     container->exits = MakeNewList(c);
			else
			     AppendItem(container->exits, c);
			return (c);
		   }
	      }
	    NoLoop:
	      /* didn't find an enclosing loop... */
	      SyntaxErrorCoord(c->coord, "no enclosing loop");
	      break;
	 default:
	      FAIL("unexpected node type");
     }
     return (c);
}
