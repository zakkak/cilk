/* -*- c-basic-offset: 5 -*- */
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/type2.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


PRIVATE Node *NodeDataTypeBase(Node *node, Bool TdefIndir);

/***************************************************************************/
/*                       N O D E  D A T A  T Y P E                         */
/***************************************************************************/

/*
 * These routines extract type information from nodes that have
 * already been type-checked with SemCheckNode().
 */

GLOBAL Node *NodeDataType(Node *node)
{
     Node *result;

     assert(node);

     result = NodeDataTypeBase(node, TRUE);

     if (result == NULL) {
	  printf("NodeDataType().  Trying to return NULL\n");
	  fPrintNode(stdout, node, 0);
	  printf("\n");
	  assert(FALSE);
     }
     return result;
}

GLOBAL Node *NodeDataTypeSuperior(Node *node)
{
     Node *result;

     assert(node);

     result = NodeDataTypeBase(node, FALSE);

     if (result == NULL) {
	  printf("NodeDataTypeSuperior().  Trying to return NULL\n");
	  fPrintNode(stdout, node, 0);
	  printf("\n");
	  assert(FALSE);
     }
     return result;
}

PRIVATE Node *NodeDataTypeBase(Node *node, Bool TdefIndir)
{
     assert(node);

     switch (node->typ) {
	 case Proc:
	      assert(node->u.proc.decl);
	      return NodeDataTypeBase(node->u.proc.decl, TdefIndir);
	 case Decl:
	      assert(node->u.decl.type);
	      return NodeDataTypeBase(node->u.decl.type, TdefIndir);
	 case Prim:
	      return node;
	 case Tdef:
	      if (TdefIndir && node->u.tdef.type)
		   return NodeDataTypeBase(node->u.tdef.type, TdefIndir);
	      else
		   return node;
	 case Ptr:
	      return node;
	 case Adcl:
	      return node;
	 case Sdcl:
	 case Udcl:
	 case Edcl:
	      return node;
	 case Fdcl:
	      return node;
	 case Call:
	      {
		   Node *atype;
		   atype = NodeDataTypeBase(node->u.call.name, TRUE);
		   while (atype->typ == Ptr)
			atype = NodeDataTypeBase(atype->u.ptr.type, TRUE);

		   assert(atype->typ == Fdcl);
		   return NodeDataTypeBase(atype->u.fdcl.returns, TdefIndir);
	      }
	 case Spawn:
	      {
		   Node *proc;
		   proc = NodeDataTypeBase(node->u.spawn.name, TRUE);
		   while (proc->typ == Ptr)
			proc = NodeDataTypeBase(proc->u.ptr.type, TRUE);
		   assert(proc->typ == Fdcl);
		   return NodeDataTypeBase(proc->u.fdcl.returns, TdefIndir);
	      }
	 case InletCall:
	      {
		   Node *proc;
		   proc = NodeDataTypeBase(node->u.inletcall.name, TRUE);
		   if (proc->typ == Ptr)
			proc = proc->u.ptr.type;
		   assert(proc->typ == Fdcl);
		   return NodeDataTypeBase(proc->u.fdcl.returns, TdefIndir);
	      }
	 case Return:
	      return PrimVoid;
	 case Cast:
	      assert(node->u.cast.type);
	      return NodeDataTypeBase(node->u.cast.type, TdefIndir);
	 case Comma:
	      {
		   Node *last = LastItem(node->u.comma.exprs);

		   assert(last);
		   return NodeDataTypeBase(last, TdefIndir);
	      }
	 case Constructor:
	      assert(node->u.constructor.type);
	      return NodeDataTypeBase(node->u.constructor.type, TdefIndir);
	     
	 case Ternary:
	      assert(node->u.ternary.type);
	      return NodeDataTypeBase(node->u.ternary.type, TdefIndir);
	 case Array:
	      assert(node->u.array.type);
	      return NodeDataTypeBase(node->u.array.type, TdefIndir);
	 case Initializer:
	      return node;
	 case ImplicitCast:
	      assert(node->u.implicitcast.type);
	      return NodeDataTypeBase(node->u.implicitcast.type, TdefIndir);
	 case Label:
	      return node;
	 case Goto:
	      return node;
	 case Unary:
	      assert(node->u.unary.type);
	      return NodeDataTypeBase(node->u.unary.type, TdefIndir);
	 case Binop:
	      assert(node->u.binop.type);
	      return NodeDataTypeBase(node->u.binop.type, TdefIndir);
	 case Const:
	      assert(node->u.Const.type);
	      return NodeDataTypeBase(node->u.Const.type, TdefIndir);
	 case Id:
	      if (node->u.id.decl) {
		   assert(node->u.id.decl);
		   assert(node->u.id.decl->u.decl.type);
		   return NodeDataTypeBase(node->u.id.decl->u.decl.type, TdefIndir);
	      } else
		   return node;

	 case Block:
	      assert(node->u.Block.type);
	      return NodeDataTypeBase(node->u.Block.type, TdefIndir);

	 case If:
	 case IfElse:
	 case For:
	 case While:
	 case Do:
	 case Continue:
	 case Break:
	 case Switch:
	 case Case:
	 case Default:
	      return node;

         case BuiltinVaArg:
	      return NodeDataTypeBase(node->u.builtinvaarg.type, TdefIndir);

	 case Synched:
	      return PrimSint;

	 default:
	      fprintf(stderr, "Internal Error! NodeDataType: Unknown node type\n");
	      fPrintNode(stderr, node, 0);
	      fprintf(stderr, "\n");
	      return node;
     }
}

GLOBAL void SetNodeDataType(Node *node, Node *type)
{
     if (node == NULL)
	  return;

     switch (node->typ) {
	 case Decl:
	      node->u.decl.type = type;
	      return;
	 case Id:
	      SetNodeDataType(node->u.id.decl, type);
	      return;
	 default:
	      FAIL("SetNodeDataType: Unknown node type");
     }
}

GLOBAL Node *ArrayType(Node *array)
{
     Node *atype;

     assert(array->typ == Array);
     assert(array->u.array.name);

     atype = NodeDataType(array->u.array.name);

     if (atype->typ == Adcl) {
	  Node *btype = atype, *item;
	  ListMarker marker;

	  /* Loop down the index operations to find the type */
	  IterateList(&marker, array->u.array.dims);
	  while (NextOnList(&marker, (GenericREF) & item))
	       if (btype->typ == Adcl)
		    btype = NodeDataType(btype->u.adcl.type);
	       else if (btype->typ == Ptr)
		    btype = NodeDataType(btype->u.ptr.type);
	       else {
		    SyntaxErrorCoord(array->coord,
				"3 cannot dereference non-pointer type");
		    return PrimVoid;
	       }

	  return btype;
     } else if (atype->typ == Ptr) {
	  Node *btype = atype, *item;
	  ListMarker marker;

	  /* Loop down the index operations to find the type */
	  IterateList(&marker, array->u.array.dims);
	  while (NextOnList(&marker, (GenericREF) & item))
	       if (btype->typ == Adcl)
		    btype = NodeDataType(btype->u.adcl.type);
	       else if (btype->typ == Ptr)
		    btype = NodeDataType(btype->u.ptr.type);
	       else {
		    SyntaxErrorCoord(array->coord,
				     "cannot dereference non-pointer type");
		    return PrimVoid;
	       }

	  return btype;
     } else {
	  fprintf(stderr, "ArrayType: Node at ");
	  PRINT_COORD(stderr, array->coord);
	  fputc('\n', stderr);
	  fPrintNode(stderr, array, 0);
	  fprintf(stderr, "\n");
	  assert(FALSE);
	  return (NULL);
     }
}

GLOBAL Node *SdclFindField(Node *sdcl, Node *field_name)
{
     if (sdcl->typ == Sdcl)
	  return SUE_FindField(sdcl->u.sdcl.type, field_name);
     else if (sdcl->typ == Udcl)
	  return SUE_FindField(sdcl->u.udcl.type, field_name);
     else if (sdcl->typ == Ptr) {
	  assert(sdcl->u.ptr.type);
	  return SdclFindField(NodeDataType(sdcl->u.ptr.type), field_name);
     } else if (sdcl->typ == Binop)
	  if (sdcl->u.binop.op == '.' || sdcl->u.binop.op == ARROW)
	       return (SdclFindField(sdcl->u.binop.type, field_name));
	  else {
	       printf("SdclFindField(): not a supported binop\n");
	       fPrintNode(stdout, sdcl, 0);
	       printf("\n");
	       fPrintNode(stdout, field_name, 0);
	       printf("\n");
	       assert(FALSE);
     } else {
	  printf("SdclFindField(): not a recognized type\n");
	  fPrintNode(stdout, sdcl, 0);
	  printf("\n");
	  fPrintNode(stdout, field_name, 0);
	  printf("\n");
	  assert(FALSE);
     }
     UNREACHABLE;
     return NULL;		/* eliminates warning */
}
