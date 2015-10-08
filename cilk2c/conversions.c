/* -*- c-indentation-style: "k&r" -*- */
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/conversions.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include "conversions.h"

#define UCHAR_TO_INT_OVF    0
#define USHORT_TO_INT_OVF   0

PRIVATE Bool IsCoercible(Node *rhs, Node *ltype,
			 Bool ptr2intp, Bool ptr2ptrp);
PRIVATE Bool TypeAssignableModuloConstVolatile(Node *ltype, Node *rtype);
PRIVATE Bool TypeEqualModuloConstVolatile(Node *ltype, Node *rtype);
PRIVATE Bool IsCoercibleToMemberTypeOfUnion(Node *type, Node *unode);

/***************************************************************************/
/*                        C O N V E R S I O N S                            */
/***************************************************************************/

GLOBAL Node *AssignmentConversions(Node *expr, Node *to_type)
{
     Node *from_type = NodeDataType(expr);
     Node *node;

     assert(to_type);
     /*DPN(from_type);*/
     if (TypeEqual(from_type, to_type))
	  return expr;

     if (!IsCoercible(expr, to_type, FALSE, FALSE))
	  return expr;

     node = MakeImplicitCastCoord(to_type, expr, expr->coord);

     ConstFoldCast(node);

     return node;
}

GLOBAL Node *CallConversions(Node *expr, Node *to_type)
{
     switch (to_type->typ) {
	 case Adcl:
	      /* Convert array of X to pointer to X */
	      to_type = MakePtr(EMPTY_TQ, to_type->u.adcl.type);
	      break;

	 case Fdcl:
	      /* Convert function to pointer to function */
	      to_type = MakePtr(EMPTY_TQ, to_type);
	      break;

	 default:
	      break;
     }

     expr = AssignmentConversions(expr, to_type);
     return expr;
}

GLOBAL Node *ReturnConversions(Node *expr, Node *to_type)
{
     return AssignmentConversions(expr, to_type);
}

GLOBAL Node *CastConversions(Node *expr, Node *to_type)
{
     if (!IsVoidType(to_type))
	  (void) IsCoercible(expr, to_type, TRUE, TRUE);

     return expr;
}

GLOBAL Node *ImplicitConversions(Node *expr, Node *to_type)
{
     /*
      * assume that this conversion is valid (since it was chosen by
      * the semantic-checker based on the type of expr -- see
      * UsualUnaryConversions and UsualBinaryConversions, below)
      */
     Node *node = MakeImplicitCastCoord(to_type, expr, expr->coord);

     ConstFoldCast(node);

     return node;
}

GLOBAL Node *UsualUnaryConversions(Node *node, Bool f_to_d)
{
     Node *type;

     assert(node);
#if 0
     printf("UsualUnary\n");
     PrintNode(stdout, node, 0);
     printf("\n");
#endif

     type = NodeDataType(node);
     assert(type);

     /* not sure why type->typ == Const is checked here, since
      * NodeDataType only returns type nodes (Prim, Ptr, S/U/E/F/Adcl) -- rcm */
     if (type->typ == Prim || type->typ == Const) {
	  BasicType btype;

	  if (type->typ == Prim)
	       btype = type->u.prim.basic;
	  else
	       btype = type->u.Const.type->u.prim.basic;

	  switch (btype) {
	      case Char:
	      case Schar:
	      case Sshort:
		   return ImplicitConversions(node, PrimSint);
	      case Uchar:
		   return ImplicitConversions(node,
			       (UCHAR_TO_INT_OVF) ? PrimUint : PrimSint);
	      case Ushort:
		   return ImplicitConversions(node,
			      (USHORT_TO_INT_OVF) ? PrimUint : PrimSint);
	      case Float:
		   return f_to_d ? ImplicitConversions(node, PrimDouble) : node;
	      default:
		   return node;
	  }
     } else if (type->typ == Adcl) {
	  Node *ptype = MakePtr(EMPTY_TQ, type->u.adcl.type);

	  return MakeImplicitCastCoord(ptype, node, node->coord);
     } else if (type->typ == Fdcl) {
	  Node *ptype = MakePtr(EMPTY_TQ, type);

	  return MakeImplicitCastCoord(ptype, node, node->coord);
     } else
	  return node;
}

/*
 * UsualUnaryConversionType is used by parser to check that
 * old-style function decls contain only "usual unary conversions"
 * types -- in particular, float and char won't be allowed as
 * parameter types. 
 */
GLOBAL Node *UsualUnaryConversionType(Node *type)
{
     assert(type);

     if (type->typ == Prim || type->typ == Const) {
	  BasicType btype;

	  if (type->typ == Prim)
	       btype = type->u.prim.basic;
	  else
	       btype = type->u.Const.type->u.prim.basic;

	  switch (btype) {
	      case Char:
	      case Schar:
		   return PrimSint;
	      case Sshort:
		   return PrimSint;
	      case Uchar:
		   return (UCHAR_TO_INT_OVF) ? PrimUint : PrimSint;
	      case Ushort:
		   return (USHORT_TO_INT_OVF) ? PrimUint : PrimSint;
	      case Float:
		   return PrimDouble;
	      default:
		   return type;
	  }
     } else
	  return type;
}

GLOBAL void UsualBinaryConversions(Node **node1p, Node **node2p)
{
     Node *type1, *type2;

     assert(*node1p);
     assert(*node2p);
     type1 = NodeDataType(*node1p),
	 type2 = NodeDataType(*node2p);

     if ((type1->typ == Prim || type1->typ == Const) &&
	 (type2->typ == Prim || type2->typ == Const)) {
	  BasicType btype1, btype2;

	  if (type1->typ == Prim)
	       btype1 = type1->u.prim.basic;
	  else
	       btype1 = type1->u.Const.type->u.prim.basic;

	  if (type2->typ == Prim)
	       btype2 = type2->u.prim.basic;
	  else
	       btype2 = type2->u.Const.type->u.prim.basic;

	  switch (btype1) {
	      case Sint:
		   switch (btype2) {
		       case Sint:
			    return;
		       case Uint:
			    *node1p = ImplicitConversions(*node1p, PrimUint);
			    return;
		       case Slong:
			    *node1p = ImplicitConversions(*node1p, PrimSlong);
			    return;
		       case Ulong:
			    *node1p = ImplicitConversions(*node1p, PrimUlong);
			    return;
		       case Slonglong:
			    *node1p = ImplicitConversions(*node1p, PrimSlonglong);
			    return;
		       case Ulonglong:
			    *node1p = ImplicitConversions(*node1p, PrimUlonglong);
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Uint:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimUint);
			    return;
		       case Uint:
			    return;
		       case Slong:
			    *node1p = ImplicitConversions(*node1p, PrimSlong);
			    return;
		       case Ulong:
			    *node1p = ImplicitConversions(*node1p, PrimUlong);
			    return;
		       case Slonglong:
			    *node1p = ImplicitConversions(*node1p, PrimSlonglong);
			    return;
		       case Ulonglong:
			    *node1p = ImplicitConversions(*node1p, PrimUlonglong);
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Slong:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimSlong);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimSlong);
			    return;
		       case Slong:
			    return;
		       case Ulong:
			    *node1p = ImplicitConversions(*node1p, PrimUlong);
			    return;
		       case Slonglong:
			    *node1p = ImplicitConversions(*node1p, PrimSlonglong);
			    return;
		       case Ulonglong:
			    *node1p = ImplicitConversions(*node1p, PrimUlonglong);
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Ulong:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimUlong);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimUlong);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimUlong);
			    return;
		       case Ulong:
			    return;
		       case Slonglong:
			    *node1p = ImplicitConversions(*node1p, PrimSlonglong);
			    return;
		       case Ulonglong:
			    *node1p = ImplicitConversions(*node1p, PrimUlonglong);
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Slonglong:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimSlonglong);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimSlonglong);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimSlonglong);
			    return;
		       case Ulong:
			    *node2p = ImplicitConversions(*node2p, PrimSlonglong);
			    return;
		       case Slonglong:
			    return;
		       case Ulonglong:
			    *node1p = ImplicitConversions(*node1p, PrimUlonglong);
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Ulonglong:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimUlonglong);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimUlonglong);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimUlonglong);
			    return;
		       case Ulong:
			    *node2p = ImplicitConversions(*node2p, PrimUlonglong);
			    return;
		       case Slonglong:
			    *node2p = ImplicitConversions(*node2p, PrimUlonglong);
			    return;
		       case Ulonglong:
			    return;
		       case Float:
			    *node1p = ImplicitConversions(*node1p, PrimFloat);
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Float:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Ulong:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Slonglong:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Ulonglong:
			    *node2p = ImplicitConversions(*node2p, PrimFloat);
			    return;
		       case Float:
			    return;
		       case Double:
			    *node1p = ImplicitConversions(*node1p, PrimDouble);
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    FAIL("Unrecognized to type");
		   }
	      case Double:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Ulong:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Slonglong:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Ulonglong:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Float:
			    *node2p = ImplicitConversions(*node2p, PrimDouble);
			    return;
		       case Double:
			    return;
		       case Longdouble:
			    *node1p = ImplicitConversions(*node1p, PrimLongdouble);
			    return;
		       default:
			    printf("Unrecognized to type\n");
			    DPN(*node2p);
			    printf("\n");
			    DPN(type2);
			    printf("\n");
			    assert(FALSE);
		   }
	      case Longdouble:
		   switch (btype2) {
		       case Sint:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Uint:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Slong:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Ulong:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Slonglong:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Ulonglong:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Float:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Double:
			    *node2p = ImplicitConversions(*node2p, PrimLongdouble);
			    return;
		       case Longdouble:
			    return;
		       default:
			    printf("Unrecognized to type\n");
			    DPN(*node2p);
			    printf("\n");
			    DPN(type2);
			    printf("\n");
			    assert(FALSE);
		   }
	      default:
		   printf("Unrecognized from type\n");
		   DPN(*node1p);
		   printf("\n");
		   DPN(type1);
		   printf("\n");
		   assert(FALSE);
	  }
     }
#if 0
     /* Ptr + integral offset */
     else if (type1->typ == Ptr && (type2->typ == Prim || type2->typ == Const)) {
	  BasicType btype2;

	  if (type2->typ == Prim)
	       btype2 = type2->u.prim.basic;
	  else
	       btype2 = type2->u.Const.value->typ;

	  switch (btype2) {
	      case Sint:
		   *node2p = ImplicitConversions(*node2p, PrimSlong);
		   return;
	      case Uint:
		   *node2p = ImplicitConversions(*node2p, PrimUlong);
		   return;
	      case Slong:
		   return;
	      case Ulong:
		   return;
	      default:
		   FAIL("Unrecognized offset type");
	  }
     }
     /* integral offset + Ptr */
     else if ((type1->typ == Prim || type1->typ == Const) && type2->typ == Ptr) {
	  BasicType btype1;

	  if (type1->typ == Prim)
	       btype1 = type1->u.prim.basic;
	  else
	       btype1 = type1->u.Const.value->typ;

	  switch (btype1) {
	      case Sint:
		   *node1p = ImplicitConversions(*node1p, PrimSlong);
		   return;
	      case Uint:
		   *node1p = ImplicitConversions(*node1p, PrimUlong);
		   return;
	      case Slong:
		   return;
	      case Ulong:
		   return;
	      default:
		   FAIL("Unrecognized offset type");
	  }
     }
#endif

     else
	  return;
}

/* Conditional Expression conversions.    6.3.15 */
GLOBAL Node *ConditionalConversions(Node **truep, Node **falsep)
{
     Node *type1, *type2;

     assert(*truep);
     assert(*falsep);

     type1 = NodeDataType(*truep), type2 = NodeDataType(*falsep);

     /* One of the following shall hold for second and third operands */

     /* both operands have arithmetic types */
     assert(type1);
     assert(type2);
     if (IsArithmeticType(type1) && IsArithmeticType(type2))
	  UsualBinaryConversions(truep, falsep);

     /* both operands have compatible structure or union types */
     else if (((IsUnionType(type1) && IsUnionType(type2)) ||
	       (IsStructType(type1) && IsStructType(type2)))
	      && TypeEqualQualified(type1, type2, FALSE, FALSE));

     /* both operands have void type */
     else if (IsVoidType(type1) && IsVoidType(type2));

     /* 
      * both operands are compatible pointers (or constant zero)
      */
     else if (IsPointerType(type1) || IsPointerType(type2))
	  UsualPointerConversions(truep, falsep, TRUE);

     /*
      * For gcc, we allow one side to be void, in which case the whole thing is of void type. */
     else if ((IsVoidType(type1) || IsVoidType(type2))) {
	  if (ANSIOnly) {
	       SyntaxErrorCoord((*truep)->coord, "ISO C forbids conditional expr with only one void side" );
	  } else if (IsVoidType(type1)) {
	       Coord coord = (*falsep)->coord;
	       *falsep = MakeImplicitCastCoord(MakePrimCoord(EMPTY_TQ, Void, coord),
					       *falsep, coord);
	       ConstFoldCast(*falsep);
	  } else {
	       Coord coord = (*truep)->coord;
	       *truep = MakeImplicitCastCoord(MakePrimCoord(EMPTY_TQ, Void, coord),
					      *truep, coord);
	       ConstFoldCast(*truep);
	  }
     } else {
	  SyntaxErrorCoord((*truep)->coord,
		      "cons and alt clauses must have compatible types");
     }

     return NodeDataType(*truep);
}

GLOBAL void UsualPointerConversions(Node **node1p, Node **node2p,
				    Bool allow_void_or_zero)
{
     Node *type1, *type2;
     Node *basetype1, *basetype2;

     assert(*node1p);
     assert(*node2p);
     type1 = NodeDataType(*node1p);
     type2 = NodeDataType(*node2p);

     if (IsPointerType(type1) && IsPointerType(type2)) {
	  basetype1 = NodeDataType(PtrSubtype(type1));
	  basetype2 = NodeDataType(PtrSubtype(type2));

	  if (TypeEqual(basetype1, basetype2));
	  else if (TypeEqualQualified(basetype1, basetype2, FALSE, FALSE)) {
	       /* K&R A7.16: "any type qualifiers in the type to which the
	        * pointer points are insignificant, but the result type inherits
	        * qualifiers from both arms of the conditional."
	        * K&R A7.2: "pointers to objects of the same type (ignoring 
	        * qualifiers) may be compared; ..." */
	       goto Merge;
	  } else if (!ANSIOnly &&
		     TypeEqualQualified(basetype1 = SansSign(basetype1),
			basetype2 = SansSign(basetype2), FALSE, FALSE)) {
	       /* common non-ANSI extension: allow comparing (e.g.) "int *" and
	        * "unsigned *", with warning. */
	       WarningCoord(4, (*node1p)->coord, "pointer base types have different sign");
	       goto Merge;
	  } else if (allow_void_or_zero && IsVoidType(basetype1))
	       goto Convert2;
	  else if (allow_void_or_zero && IsVoidType(basetype2))
	       goto Convert1;
	  else
	       SyntaxErrorCoord((*node1p)->coord,
			     "operands have incompatible pointer types");

     } else if (allow_void_or_zero &&
		IsPointerType(type1) && IsConstantZero(*node2p))
	  goto Convert2;
     else if (allow_void_or_zero &&
	      IsPointerType(type2) && IsConstantZero(*node1p))
	  goto Convert1;
     else
	  SyntaxErrorCoord((*node1p)->coord,
			   "operands have incompatible types");
     return;

   Merge:
     {
	  Node *newtype = MakePtr(EMPTY_TQ, MakeMergedType(basetype1, basetype2));
	  *node1p = ImplicitConversions(*node1p, newtype);
	  *node2p = ImplicitConversions(*node2p, newtype);
     }
     return;

   Convert1:
     *node1p = ImplicitConversions(*node1p, type2);
     return;

   Convert2:
     *node2p = ImplicitConversions(*node2p, type1);
     return;
}

/***************************************************************************/
/*                        I S    C O E R C I B L E                         */
/***************************************************************************/

/*
 * IsCoercible() tests whether assigning rhs to an object of ltype
 * is valid. 
 * 
 * Pointer/pointer and int/pointer coercions (except for constant 0) 
 * are disallowed unless the caller explicitly specifies that they are
 * allowable:
 * 
 * ptr2intp == true  implies that pointers can be coerced to integral types
 * and back.
 * ptr2ptrp == true  implies that pointers can be coerced regardless of
 * their base type.
 * 
 * ltype is restricted to types which make sense in a cast.  In particular,
 * ltype may not be an array or function type (though it may be a pointer
 * to an array or function).
 */
PRIVATE Bool IsCoercible(Node *rhs, Node *ltype,
			 Bool ptr2intp, Bool ptr2ptrp)
{
     Node *rtype = NodeDataType(rhs);

     /* First we massage rtype and ltype slightly to make them comparable */

     switch (rtype->typ) {
	 case Tdef:
	      /* Get rid of top-level typedef indirections */
	      rtype = NodeDataType(rtype);
	      break;

	 case Edcl:
	      /* Treat enums as signed int */
	      rtype = PrimSint;
	      break;

	 case Adcl:
	      /* Convert array of X to pointer to X */
	      rtype = MakePtr(EMPTY_TQ, rtype->u.adcl.type);
	      break;

	 case Fdcl:
	      /* Convert function objects to pointer to function */
	      rtype = MakePtr(EMPTY_TQ, rtype);
	      break;

	 default:
	      break;
     }

     switch (ltype->typ) {
	 case Tdef:
	      /* Get rid of top-level typedef indirections */
	      ltype = NodeDataType(ltype);
	      break;

	 case Edcl:
	      /* Treat enums as signed int */
	      ltype = PrimSint;
	      break;

	 default:
	      break;
     }


     /* Now check for legality of coercion */

     if (IsArithmeticType(rtype) && IsArithmeticType(ltype))
	  return TRUE;

     if (IsPointerType(rtype) && IsPointerType(ltype)) {
	  if (ptr2ptrp)
	       return TRUE;
	  else {
	       Node *lbasetype = NodeDataType(PtrSubtype(ltype)),
		    *rbasetype = NodeDataType(PtrSubtype(rtype));

	       if (IsVoidType(lbasetype) || IsVoidType(rbasetype)) {
		    return TRUE;
	       } else if (TypeEqual(lbasetype, rbasetype)) {
		    return TRUE;
	       } else if (TypeAssignableModuloConstVolatile(lbasetype, rbasetype)) {
		    return TRUE;
	       } else if (NodeIsCilkProcedure(lbasetype) && !NodeIsCilkProcedure(rbasetype)) {
		    SyntaxErrorCoord(rhs->coord,
				     "cannot assign non-cilk pointer to cilk pointer");
		    return FALSE;
	       } else if (!NodeIsCilkProcedure(lbasetype) && NodeIsCilkProcedure(rbasetype)) {
		    SyntaxErrorCoord(rhs->coord,
				     "cannot assign cilk pointer to non-cilk pointer");
		    return FALSE;
	       } else if (!ANSIOnly &&
			TypeEqualModuloConstVolatile(lbasetype, rbasetype)) {
		    WarningCoord(4, rhs->coord, 
				 "discarding type qualifiers in pointer assignment");
		    return TRUE;
	       } else if (!ANSIOnly &&
			TypeAssignableModuloConstVolatile(SansSign(lbasetype),
						  SansSign(rbasetype))) {
		    WarningCoord(4, rhs->coord, 
				 "pointer base types have different sign");
		    return TRUE;
	       } else if (!ANSIOnly) {
		    /* just assign these damn pointers anyway, what the hell --- athena */
		    WarningCoord(3, rhs->coord, 
				 "assignment of pointers with different base types");
		    return TRUE;
	       } else {
		    SyntaxErrorCoord(rhs->coord,
		     "cannot assign pointers with different base types");
		    return FALSE;
	       }
	  }
     } 

     if (IsIntegralType(rtype) && IsPointerType(ltype)) {
	  if (ptr2intp || IsConstantZero(rhs)) {
	       return TRUE;
	  } else {
	       if (ANSIOnly) {
		    SyntaxErrorCoord(rhs->coord,
				     "cannot assign integer value to pointer");
		    return FALSE;
	       } else {
		    WarningCoord(4, rhs->coord, 
				 "assignment of integer value to pointer");
		    return TRUE;
	       }
	  }
     }
     
     if (IsPointerType(rtype) && IsIntegralType(ltype)) {
	  if (ptr2intp)
	       return TRUE;
	  else {
	       if (ANSIOnly) {
		    SyntaxErrorCoord(rhs->coord,
				     "cannot assign pointer value to integer");
		    return FALSE;
	       } else {
		    WarningCoord(4, rhs->coord, 
				 "assignment of pointer value to integer");
		    return TRUE;
	       }
	  }
     }

     /* handles structures and unions */
     if (IsUnionType(ltype) || IsStructType(ltype)) {
	  if (TypeEqualQualified(rtype, ltype, FALSE, FALSE)) {
	       return TRUE;
	  } else if (IsUnionType(ltype) &&
		     IsCoercibleToMemberTypeOfUnion(rtype, ltype)
		     && !ANSIOnly /* casting to union is a GNU extension */ ) {
	       WarningCoord(5, rhs->coord, "coercing to union member");
	       return TRUE;
	  } else {
	       SyntaxErrorCoord(rhs->coord,
			   "cannot assign to incompatible struct/union");
	       return FALSE;
	  }
     }
	
     /* default error message */
     SyntaxErrorCoord(rhs->coord, "operands have incompatible types");
     return FALSE;
}

PRIVATE Bool TypeAssignableModuloConstVolatile(Node *ltype, Node *rtype)
{
     /*
      * K&R2 A7.17: "... or both operands are pointers to functions
      * or objects whose types are the same except for the possible
      * absence of const or volatile in the right operand."  So
      * qualifiers on left must include all qualifiers on right.  
      */
    if ((!tq_has_const(NodeTq(ltype)))    && tq_has_const(NodeTq(rtype))) return FALSE;
    if ((!tq_has_volatile(NodeTq(ltype))) && tq_has_volatile(NodeTq(rtype))) return FALSE;
    if ((!tq_has_restrict(NodeTq(ltype))) && tq_has_restrict(NodeTq(rtype))) return FALSE;
/* OLD CODE TO DO THAT:  I Personally find the new code (above) to be much easier to read.  -Bradley
 *    TypeQual lcv = NodeTypeQuals(ltype) & (T_CONST | T_VOLATILE | T_RESTRICT),
 *             rcv = NodeTypeQuals(rtype) & (T_CONST | T_VOLATILE | T_RESTRICT);
 *
 *    if ((lcv | rcv) != lcv)
 *       return FALSE;
*/

     /*
      * type qualifiers okay at top-level; remove them and test
      * entire type
      */
     return TypeEqualQualified(ltype, rtype, FALSE, TRUE);
}

PRIVATE Bool TypeEqualModuloConstVolatile(Node *ltype, Node *rtype)
{
     /* ignore top-level type qualifiers and test the type */
     return TypeEqualQualified(ltype, rtype, FALSE, TRUE);
}

PRIVATE Bool IsCoercibleToMemberTypeOfUnion(Node *type, Node *unode)
{
     ListMarker marker;
     Node *field;

     assert(IsUnionType(unode));
     assert(IsType(type));

     IterateList(&marker, unode->u.udcl.type->fields);
     while (NextOnList(&marker, (GenericREF) & field)) {
	  if (IsCoercible(type, NodeDataType(field), TRUE, TRUE))
	       return TRUE;
     }

     return FALSE;
}
