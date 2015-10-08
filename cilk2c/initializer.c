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
 *************************************************************************/
#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/initializer.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include "conversions.h"
#include "initializer.h"

PRIVATE Node *SemCheckInitList(Node *decl, Node *dtype, Node *init, Bool top_p);
PRIVATE void RequireConstantInitializer(Node *node);
PRIVATE Bool IsLinktimeConstant(Node *node);
PRIVATE Bool HasConstantAddress(Node *node);
PRIVATE Node *EnsureInitializerExpr(Node *init, Node *decl);

PRIVATE Bool IsLabeledConstructor(Node *node) {
     return (node->typ==Binop &&
	     (node->u.binop.op==':' || (node->u.binop.op==DESIGNATED_INITIALIZER)));
}

/*
 * SemCheckDeclInit:  requires that decl->u.decl.init has already been 
 * SemCheck'ed.
 */

/* Initialization.  Section 6.5.7 */
GLOBAL void SemCheckDeclInit(Node *decl, Bool blockp)
{
     Node *dtype = NodeDataType(decl->u.decl.type), *init = decl->u.decl.init;

#if 0
     printf("\nSemCheckDeclInit\n");
     PrintNode(stdout, decl,  0); printf("\n");
     PrintNode(stdout, dtype, 0); printf("\n");
     PrintNode(stdout, init,  0); printf("\n");
#endif

     /* 
      * The type of the entity to be initialized shall be an object 
      * type or or an array of unknown size
      */
     if (IsObjectType(dtype) || IsUnsizedArray(dtype));
     else
	  SyntaxErrorCoord(decl->coord,
		       "must be an Object or an incomplete array \"%s\"",
			   decl->u.decl.name);

     /* 
      * The initializer for any variable at top-level or with block scope
      * and static storage duration shall be a constant expression 
      */
     if (!blockp || DeclIsStatic(decl))
	  RequireConstantInitializer(init);

     /*
      * All the expressions in an initializer list for an object that
      * has aggregate or union type shall be constant expressions.
      * (ANSI only.  For gcc, no such restriction.) 
      */
     else if (ANSIOnly
	      && ((IsAggregateType(dtype) || IsUnionType(dtype))
		  && init->typ == Initializer))
	  RequireConstantInitializer(init);

     /*
      * If the declaration of an identifier has block scope, and the
      * identifier has external or internal linkage, the declaration
      * shall have no initializer for the identifier
      */
     if (DeclIsExtern(decl)) {
	  if (blockp)
	       SyntaxErrorCoord(decl->coord,
		      "cannot initialize \"extern\" declaration: \"%s\"",
				decl->u.decl.name);

	  else
	       WarningCoord(2, decl->coord,
	       "inadvisable to initialize \"extern\" declaration: \"%s\"",
			    decl->u.decl.name);
     }

     /* Recursively walk the initialization list */
     if (IsInitializer(init)) {
	  /*printf("\nInitializer is\n"); DPN(init);*/
	  decl->u.decl.init = 
	       SemCheckInitList(decl, dtype, InitializerCopy(init), TRUE);
     } else if (IsLabeledConstructor(init)) {
	  decl->u.decl.init =
	       SemCheckInitList(decl, dtype, NodeCopy(init, NodeOnly), TRUE); /* Why are all these InitializerCopy operations taking palce.  SemCheckInitList must be modifying something...*/
     } else {
	  decl->u.decl.init = SemCheckInitList(decl, dtype, init, TRUE);
     }
}


/****************************************************************************

  SemCheckInitList(Node *decl, Node *dtype, Node *init, Bool top_p)

  Recursively walk the initialization list 
       A) Verify that its shape matches the LHS
       B) Infer the size of unsized arrays
       C) Perform assignment conversions

  Decl is passed (from the root) to provide name/coord for error messages

 ****************************************************************************/

PRIVATE Node *SemCheckInitList(Node *decl, Node *dtype, Node *init, Bool top_p)
{
#if 0
     printf("\nSemCheckInitList(decl dtype init %d)\n", top_p);
     PrintNode(stdout, dtype, 0);
     printf("\n");
     PrintNode(stdout, init, 0);
     printf("\n");
#endif

     if (IsScalarType(dtype)) {
	  if (IsInitializer(init)) {
	       Node *val = InitializerFirstItem(init);

	       /* Swallow the first element of the list */
	       InitializerNext(init);

	       /* If at top-level then complain if list is not empty */
	       if (top_p && !InitializerEmptyList(init))
		    SyntaxErrorCoord(decl->coord,
			       "too many initializers for scalar \"%s\"",
				     decl->u.decl.name);

	       if (IsInitializer(val)) {
		    SyntaxErrorCoord(decl->coord,
		    "initializer for scalar \"%s\" requires one element",
				     decl->u.decl.name);
		    return NULL;
	       } else {
		    return AssignmentConversions(val, dtype);
	       }
	  } else if (IsLabeledConstructor(init)) {
	       return AssignmentConversions(init->u.binop.right, dtype); 
	  } else {
	       return AssignmentConversions(init, dtype);
	  }
     } else if (IsArrayType(dtype)) {
	  if (IsArrayOfCharModuloSign(dtype) &&
	      (IsStringConstant(init) ||
	       (IsInitializer(init) &&
		(InitializerLength(init) == 1 ||
		 (InitializerLength(init) > 0 && !top_p)) &&	/* IL */
		IsStringConstant(InitializerFirstItem(init))))) {
	       Node *sinit = IsInitializer(init) ? InitializerFirstItem(init) : init,
		    *dim = dtype->u.adcl.dimp->dim;
	       const char *string = NodeConstantStringValue(sinit),
		    *ptr = string;
	       List *result = NULL;
	       int count = 0, maxcount = (dim) ? (signed long) NodeConstantIntegralValue(dim) : -1;

	       /* 
		* emit a warning for declarations of the form
		* unsigned char foo[] = "bar";
		* signed char foo[] = "bar";
		* since char and signed/unsigned char are explicitly
		* distinct in ISO C
		*/
	       if (!IsArrayOfChar(dtype)) {
		    WarningCoord(4, decl->coord,
     "implicitly converting plain char into signed/unsigned char");

	       }

	       if (IsInitializer(init))		/* IL */
		    /* Swallow the first element of the list */
		    InitializerNext(init);

	       while ((maxcount == -1 || count < maxcount) && *ptr) {
		    result = AppendItem(result, MakeConstSint(*ptr));
		    count++;
		    ptr++;
	       }


	       /* If the decl is an array of undefined size, then set
                  it now */
	       if (dtype->u.adcl.dimp->dim == NULL) {
		    Node *etype = NodeDataType(dtype->u.adcl.type);
		    result = AppendItem(result, MakeConstSint('\0'));
		    count++;
		    dtype->u.adcl.dimp->dim = MakeImplicitCast(PrimSint, NULL);
		    NodeSetSintValue(dtype->u.adcl.dimp->dim, count);
		    dtype->u.adcl.dimp->size =
			 count * NodeSizeof(dtype, etype, FALSE);
	       } else if (maxcount > count)
		    result = AppendItem(result, MakeConstSint('\0'));
	       else if (*ptr == '\0');
	       else
		    WarningCoord(1, decl->coord,
				 "%d extra byte(s) in string literal initalizer ignored",
				 strlen(ptr));
#if 0
/* don't change the initializer -- memory leak -- rcm */
	       return MakeInitializerCoord(result, init->coord);
#endif
	       return sinit;	/* IL: instead of "init" */
	  } else if (!IsInitializer(init)) {
	       /*
		* initializing an aggregate or union with a single
	        * expression of same type: eg "struct foo f = g;" 
		*/
	       return AssignmentConversions(init, dtype);
	  } else {
	       Node *atype = ArraySubtype(dtype), 
		    *ilist = EnsureInitializerExpr(init, decl),
		    *result = MakeInitializerCoord(NULL, init->coord), 
		    *dim = dtype->u.adcl.dimp->dim;
	       int count = 0,
		    maxcount = (dim) ? 
		    (signed long) NodeConstantIntegralValue(dim) : -1;

	       while ((maxcount == -1 || count < maxcount) && !InitializerEmptyList(ilist)) {
		    Node *item = InitializerFirstItem(ilist);

		    if (IsInitializer(item)) {
			 Node *val = SemCheckInitList(decl, atype, InitializerCopy(item), TRUE);

			 InitializerAppendItem(result, val);
			 InitializerNext(ilist);
		    } else {
			 Node *val = SemCheckInitList(decl, atype, ilist, FALSE);

			 InitializerAppendItem(result, val);
		    }

		    count++;
	       }

	       /* If the decl is an array of undefined size, then set it now */
	       if (dtype->u.adcl.dimp->dim == NULL) {
		    Node *etype = NodeDataType(dtype->u.adcl.type);
		    dtype->u.adcl.dimp->dim = MakeImplicitCast(PrimSint, NULL);
		    NodeSetSintValue(dtype->u.adcl.dimp->dim, count);
		    dtype->u.adcl.dimp->size =
			 count * NodeSizeof(dtype, etype, FALSE);
	       } else if (top_p && !InitializerEmptyList(ilist))
		    SyntaxErrorCoord(decl->coord,
				     "too many array initializers: \"%s\"",
				     decl->u.decl.name);

	       return result;
	  }
     } else if (IsStructType(dtype)) {
	  if (!IsInitializer(init))
	       return AssignmentConversions(init, dtype);
	  else
	       return SUE_MatchInitList(StructUnionFields(dtype),
					decl,
					EnsureInitializerExpr(init, decl),
					top_p);
     
     } else if (IsUnionType(dtype)) {
	  if (!IsInitializer(init))
	       return AssignmentConversions(init, dtype);
	  else {
	       Node *uinit = EnsureInitializerExpr(init, decl);
	       List *initl;
	       Node *first;
	       Node *fixed_up_init;
	       /*printf("%s:%d: uinit\n", __FILE__, __LINE__);*/
	       /*PrintNode(stdout, uinit, 0);*/
	       assert(uinit->typ==Initializer);
	       initl=uinit->u.initializer.exprs;
	       if (ListLength(initl)!=1) 
		    SyntaxErrorCoord(uinit->coord, "Union initializers must contain exactly one value");
	       first = FirstItem(initl);
	       if (first->typ==Binop && (first->u.binop.op==':' || first->u.binop.op==DESIGNATED_INITIALIZER)) {
		    Node *f;
		    /*printf("\nis designated binop:\n");*/
		    /*PrintNode(stdout, first, 0);*/
		    /*printf("\nFinding field:\n"); PrintNode(stdout, first->u.binop.left, 0); printf("\n");*/
		    f = SUE_FindField(dtype->u.udcl.type,first->u.binop.left);
		    assert(f);
		    /*printf("%s:%d Got field\n", __FILE__,__LINE__);*/
		    /*PrintNode(stdout, f, 0);*/
		    fixed_up_init = SemCheckInitList(decl,
						   NodeDataType(f),
						   uinit,
						   top_p);
	       } else {
		    fixed_up_init = SemCheckInitList(decl,
						     NodeDataType(UnionFirstField(dtype)),
						     first,
						     top_p);
	       }
	       /*printf("%s:%d fixed up init\n", __FILE__,__LINE__);*/
	       /*PrintNode(stdout, fixed_up_init, 0);*/
	       return MakeInitializerCoord(MakeNewList(fixed_up_init), uinit->coord);
	  }
     } else {
	  SyntaxErrorCoord(decl->coord,
			   "Expression cannot have an initializer \"%s\"",
			   decl->u.decl.name);
	  DPN(dtype);
	  printf("\n");
	  return NULL;
     }
}

PRIVATE void RequireConstantInitializer(Node *node)
{
     assert(node);

     if (node->typ == Initializer) {
	  ListMarker marker;
	  Node *n;

	  IterateList(&marker, node->u.initializer.exprs);
	  while (NextOnList(&marker, (GenericREF) & n))
	       RequireConstantInitializer(n);
     } else if (node->typ == Constructor) {
	  assert(node->u.constructor.initializerlist->typ==Initializer);
	  RequireConstantInitializer(node->u.constructor.initializerlist);
     } else if (node->typ == Binop && (node->u.binop.op == ':' || node->u.binop.op == DESIGNATED_INITIALIZER)) {
	 RequireConstantInitializer(node->u.binop.right);
     } else if (node->typ == Binop && ((node->u.binop.op == ELLIPSIS) || node->u.binop.op == ARRAY_LABELED_INITIALIZER)) {
	  RequireConstantInitializer(node->u.binop.left);
	  RequireConstantInitializer(node->u.binop.right);
     } else if (!IsLinktimeConstant(node)) {
	  SyntaxErrorCoord(node->coord, "initializer must be constant");
     }
}

PRIVATE Bool IsLinktimeConstant(Node *node)
{
     /* if node is compile-time constant, then certainly it's
      * link-time */
     if (NodeIsConstant(node))
	  return TRUE;

     /* otherwise check special cases */
     switch (node->typ) {
	 case Id:{
		   Node *decl = node->u.id.decl;
		   Node *declType = NodeDataType(decl);

		   /* global/static array and function name are link-time constants */
		   return (IsArrayType(declType) && HasConstantAddress(node))
		       || IsFunctionType(declType);
	      }

	 case Unary:
	      if (node->u.unary.op == ADDRESS)
		   return HasConstantAddress(node->u.unary.expr);

	 case Binop:{
		   OpType op = node->u.binop.op;
		   Node *left = node->u.binop.left;
		   Node *right = node->u.binop.right;

		   /* check for pointer arithmetic involving constant addresses */
		   if (op == '+' || op == '-') {
			return IsLinktimeConstant(left) && IsLinktimeConstant(right);
		   } else if (op == '.') {
			return IsArrayType(node->u.binop.type) && HasConstantAddress(left);
		   } else {
			return FALSE;
		   }
	      }

	 case Cast:
	      /* casts are ignored */
	      return IsLinktimeConstant(node->u.cast.expr);

	 case ImplicitCast:
	      return IsLinktimeConstant(node->u.implicitcast.expr);

	 default:
	      return FALSE;
     }
}

PRIVATE Bool HasConstantAddress(Node *node)
{
     assert(node);

     switch (node->typ) {
	 case Id:{
		   Node *decl = node->u.id.decl;

		   /* global/static variable always has constant address */
		   return (DeclIsExternal(decl) || DeclIsStatic(decl));
	      }

	 case Binop:{
		   OpType op = node->u.binop.op;

		   /*
		    * structure/union field reference has constant address iff
		    * structure does 
		    */
		   if (op == '.')
			return HasConstantAddress(node->u.binop.left);
		   else if (op == ARROW)
			return IsLinktimeConstant(node->u.binop.left);
		   else
			return FALSE;
	      }

	 case Array:{
		   ListMarker marker;
		   Node *n;

		   /* array reference has constant address iff name
		    * and all dimensions are linktime constants
		    */
		   if (!IsLinktimeConstant(node->u.array.name))
			return FALSE;

		   IterateList(&marker, node->u.array.dims);
		   while (NextOnList(&marker, (GenericREF) & n))
			if (!IsLinktimeConstant(n))
			     return FALSE;
		   return TRUE;
	      }

	 case Cast:
	      /* casts are ignored */
	      return HasConstantAddress(node->u.cast.expr);

	 case ImplicitCast:
	      return HasConstantAddress(node->u.implicitcast.expr);

	 default:
	      return FALSE;
     }
}

PRIVATE Node *EnsureInitializerExpr(Node *init, Node *decl)
{
     if (IsInitializer(init)) {
	  return init;
     }

     WarningCoord(1, decl->coord,
		  "{}-enclosed initializer required \"%s\"",
		  decl->u.decl.name);
     return MakeInitializerCoord(MakeNewList(init), init->coord);
}

GLOBAL Bool IsInitializer(Node *node)
{
     assert(node);
     return node->typ == Initializer;
}

GLOBAL Node *InitializerCopy(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);
     return NodeCopy(node, NodeOnly);
}

GLOBAL int InitializerLength(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);
     return (ListLength(node->u.initializer.exprs));
}

GLOBAL Node *InitializerFirstItem(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);

     return FirstItem(node->u.initializer.exprs);
}

GLOBAL List *InitializerExprs(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);
     return node->u.initializer.exprs;
}

GLOBAL Bool InitializerEmptyList(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);
     return node->u.initializer.exprs == NULL;
}

GLOBAL void InitializerNext(Node *node)
{
     assert(node);
     assert(node->typ == Initializer);
     node->u.initializer.exprs = Rest(node->u.initializer.exprs);
}

GLOBAL Node *InitializerAppendItem(Node *initializer, Node *element)
{
     assert(initializer);
     assert(initializer->typ == Initializer);
     initializer->u.initializer.exprs = AppendItem(initializer->u.initializer.exprs, element);
     return initializer;
}

GLOBAL Node *ArraySubtype(Node *node)
{
     assert(node);
     assert(node->typ == Adcl);
     return NodeDataType(node->u.adcl.type);
}

GLOBAL int ArrayNumberElements(Node *node)
{
     int total;

     assert(node);
     assert(node->typ == Adcl);

     for (total = 1; node->typ == Adcl; node = node->u.adcl.type) {
	  assert(node->u.adcl.dimp->dim);
	  total = total * NodeConstantIntegralValue(node->u.adcl.dimp->dim);
     }

     return total;
}

GLOBAL SUEtype *StructUnionFields(Node *node)
{
     assert(node);

     if (node->typ == Sdcl)
	  return node->u.sdcl.type;
     else if (node->typ == Udcl)
	  return node->u.udcl.type;
     else
	  assert(FALSE);
     UNREACHABLE;
     return NULL;		/* eliminates warning */
}

GLOBAL Node *UnionFirstField(Node *node)
{
     assert(node);
     assert(node->typ == Udcl);
     return FirstItem(node->u.udcl.type->fields);
}

/* Rewritten by IL to fix c2c bugs */
GLOBAL Node *SUE_MatchInitList(SUEtype *sue, Node *decl, Node *initializer, Bool top_p)
{
     ListMarker marker;
     Node *field, *result = MakeInitializerCoord(NULL, initializer->coord);

     /*printf("SUE_MatchInitList top=%d initializer is\n", top_p); DPN(initializer);  printf("\nsue is\n"); DPL(sue->fields);*/

     /* Loop over the fields of the SDCL */
     IterateList(&marker, sue->fields);
     while (!InitializerEmptyList(initializer)) {
	  Node *item = InitializerFirstItem(initializer);

	  if (IsLabeledConstructor(item)) {
	       /* If we have a particular label, then we need to find the corresponding ftype */
	       IterateList(&marker, sue->fields);
	       while (NextOnList(&marker, (GenericREF) & field)) {
		    const char *fname = field->u.decl.name;
		    Node *ftype = NodeDataType(field->u.decl.type);
		    assert(item->u.binop.left->typ==Id);
		    if (strcmp(fname, item->u.binop.left->u.id.text)==0) {
			 Node *val = SemCheckInitList(decl, ftype, NodeCopy(item->u.binop.right,NodeOnly), TRUE);
			 item->u.binop.right = val;
			 InitializerAppendItem(result, item);
			 /* This is a screwy way to do this, to actually modify the initializer, and to have to create a new initializer. -Bradley*/
			 InitializerNext(initializer);
			 goto next_item;  /* If only C had labeled "continue" statements....*/
		    }
	       }
	       /* fell through, meaning we didn't filed the field. */
	       SyntaxErrorCoord(decl->coord, "Field \"%s\" mentioned in initializer of \"%s\" doesn't exist in the declaration",
				item->u.binop.left->u.id.text, decl->u.decl.name);
	       return result;
	  } else {
	       {
		    Bool isanother = NextOnList(&marker, (GenericREF) & field);
		    if (!isanother) {
			 if (top_p) {
			      /* If at top level, then complain if the list is not empty when we run out of fiels*/
			      SyntaxErrorCoord(decl->coord,
					       "too many struct/union initializers for \"%s\"",
					       decl->u.decl.name);
			 } else {
			      /* Otherwise we return the result, and have modified the initializer, consuming all its fields*/
			      return result;
			 }
		    }
	       }
	       /*printf("The field is this:\n"); DPN(field);*/
	       /*printf("The item is this: \n"); DPN(item);*/

	       {
		    Node *ftype  = NodeDataType(field->u.decl.type);

		    if (IsInitializer(item)) {
			 Node *val = SemCheckInitList(decl, ftype, InitializerCopy(item), TRUE);

			 InitializerAppendItem(result, val);
			 /* This is a screwy way to do this, to actually modify the initializer, and to have to create a new initializer. -Bradley*/
			 InitializerNext(initializer);
		    } else if (item->typ==Constructor) {
			 Node *val;
			 assert(item->u.constructor.initializerlist->typ==Initializer);
			 val = SemCheckInitList(decl, ftype, InitializerCopy(item->u.constructor.initializerlist), TRUE);
			 item->u.constructor.initializerlist = val;
			 InitializerAppendItem(result, item);
			 InitializerNext(initializer);
		    } else {
			 /* The screwyness is partly explained by this.  We consume the items we need for the subfield.*/
			 Node *val = SemCheckInitList(decl, ftype, initializer, FALSE);

			 InitializerAppendItem(result, val);
		    }
	       }
	  }
     next_item: ; /* gcc 3.2 20020903 complains if you have a label at the end of a compound statement */
     }

     /* Ran out of initializer fields, so return the result. */ 

     return result;
}
