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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/constexpr.c $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");


PRIVATE BasicType BasicTypeOfConstantValue(Node *type);

/* NodeIsConstant returns true iff node is a constant expression.
 * Requires SemCheck(node) to be called first. */
GLOBAL Bool NodeIsConstant(Node *node)
{
     assert(node);
     return (NodeGetConstantValue(node) != NULL);
}

GLOBAL Node *NodeGetConstantValue(Node *node)
{
     Node *value;

     switch (node->typ) {
	 case Const:
	      value = node;
	      break;
	 case Id:
	      value = node->u.id.value;
	      break;
	 case Binop:
	      value = node->u.binop.value;
	      break;
	 case Unary:
	      value = node->u.unary.value;
	      break;
	 case Cast:
	      value = node->u.cast.value;
	      break;
	 case ImplicitCast:
	      value = node->u.implicitcast.value;
	      break;
	 case Comma:
	      value = NULL;
	      break;
	 case Ternary:
	      value = node->u.ternary.value;
	      break;
	 case Array:
	 case Call:
	      value = NULL;
	      break;
	 case Initializer:
	      value = NULL;
	      break;
	 default:		/* non-expression nodes */
	      value = NULL;
	      break;
     }

     /* if value is present, must be a Const node */
     assert(!value || value->typ == Const);
     return value;

}

GLOBAL void NodeSetConstantValue(Node *node, Node *value)
{
     assert(node);
     assert(value);
     assert(value->typ == Const);

     switch (node->typ) {
	 case Const:
	      node->u.Const.value = value->u.Const.value;
	      break;
	 case Id:
	      node->u.id.value = value;
	      break;
	 case Binop:
	      node->u.binop.value = value;
	      break;
	 case Unary:
	      node->u.unary.value = value;
	      break;
	 case Cast:
	      node->u.cast.value = value;
	      break;
	 case ImplicitCast:
	      node->u.implicitcast.value = value;
	      break;
	 case Ternary:
	      node->u.ternary.value = value;
	      break;
	 default:
	      /* Comma, Array, Call, Initializer, and non-expression nodes */
	      UNREACHABLE;	/* should not set constant value on these types of nodes */
     }
}

GLOBAL int NodeConstantCharValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsChar(val));

     return val->u.Const.value.i;
}

GLOBAL int NodeConstantSintValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsSint(val));

     return val->u.Const.value.i;
}

GLOBAL unsigned int NodeConstantUintValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsUint(val));

     return val->u.Const.value.u;
}

GLOBAL long NodeConstantSlongValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsSlong(val));

     return val->u.Const.value.l;
}

GLOBAL unsigned long NodeConstantUlongValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsUlong(val));

     return val->u.Const.value.ul;
}

GLOBAL float NodeConstantFloatValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsFloat(val));

     return val->u.Const.value.f;
}

GLOBAL double NodeConstantDoubleValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsDouble(val));

     return val->u.Const.value.d;
}

GLOBAL const char *NodeConstantStringValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);

     assert(val);
     assert(NodeTypeIsString(val));

     return val->u.Const.value.s;
}

GLOBAL unsigned long NodeConstantIntegralValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);
     Node *type = NodeDataType(val);

     assert(val);
     assert(type);

     if (type->typ == Prim) {
	  switch (type->u.prim.basic) {
	      case Sint:
		   return val->u.Const.value.i;
	      case Uint:
		   return val->u.Const.value.u;
	      case Slong:
		   return val->u.Const.value.l;
	      case Ulong:
		   return val->u.Const.value.ul;
#if 0
	      case Float:
		   return val->u.Const.value.f;
	      case Double:
		   return val->u.Const.value.d;
#endif
    default:     break;
    }
  }
  FAIL("Unexpected constant type");
}

GLOBAL Bool NodeConstantBooleanValue(Node *node)
{
     Node *val = NodeGetConstantValue(node);
     Node *type = NodeDataType(val);

     assert(val);
     assert(type);

  if (type->typ == Prim) {
    switch (type->u.prim.basic) {
    case Sint:   return val->u.Const.value.i;
    case Uint:   return val->u.Const.value.u;
    case Slong:  return val->u.Const.value.l;
    case Ulong:  return val->u.Const.value.ul;
    case Float:  return val->u.Const.value.f;
    case Double: return val->u.Const.value.d;
    default: break;
    }
  }
  else if (type->typ == Adcl)
    return TRUE;  /* constant string, always true */

  FAIL("Unexpected constant type");
}

GLOBAL void NodeSetSintValue(Node *node, int i)
{
     NodeSetConstantValue(node, MakeConstSint(i));
}

GLOBAL void NodeSetUintValue(Node *node, unsigned u)
{
     NodeSetConstantValue(node, MakeConstUint(u));
}

GLOBAL void NodeSetSlongValue(Node *node, long l)
{
     NodeSetConstantValue(node, MakeConstSlong(l));
}

GLOBAL void NodeSetUlongValue(Node *node, unsigned long ul)
{
     NodeSetConstantValue(node, MakeConstUlong(ul));
}

GLOBAL void NodeSetFloatValue(Node *node, float f)
{
     NodeSetConstantValue(node, MakeConstFloat(f));
}

GLOBAL void NodeSetDoubleValue(Node *node, double d)
{
     NodeSetConstantValue(node, MakeConstDouble(d));
}

GLOBAL void NodeSetStringValue(Node *node, const char *s)
{
     NodeSetConstantValue(node, MakeString(s));
}

/* Return TRUE if this is a constant string */
GLOBAL Bool IsConstantString(Node *node)
{
     return (NodeIsConstant(node) && NodeTypeIsString(node));
}

GLOBAL Bool IsIntegralConstant(Node *node)
{
     return (NodeIsConstant(node) && NodeTypeIsIntegral(node));
}

GLOBAL Bool IsConstantZero(Node *node)
{
  Node *val = NodeGetConstantValue(node);
  Node *type;

  if (val == NULL)
    return FALSE;

  type = NodeDataType(val);
  assert(type);

  if (type->typ == Prim) {
    switch (type->u.prim.basic) {
    case Sint:   return val->u.Const.value.i == 0;
    case Uint:   return val->u.Const.value.u == 0;
    case Slong:  return val->u.Const.value.l == 0;
    case Ulong:  return val->u.Const.value.ul == 0;
    case Float:  return val->u.Const.value.f == 0;
    case Double: return val->u.Const.value.d == 0;
    default:     break;
    }
  }
  else if (type->typ == Adcl)
    return FALSE;  /* constant string, never 0 */

  FAIL("Unexpected constant type");
}

GLOBAL Bool IntegralConstEqual(Node *node1, Node *node2)
{
     return (NodeConstantIntegralValue(node1) == NodeConstantIntegralValue(node2));
}

/***************************************************************************/
/*                    C O N S T A N T     F O L D I N G                    */
/***************************************************************************/

GLOBAL void ConstFoldTernary(Node *node)
{
     Node *cond, *true, *false;

     assert(node->typ == Ternary);

     cond = node->u.ternary.cond;
     true = node->u.ternary.true;
     false = node->u.ternary.false;
     
     if (NodeIsConstant(cond) && (true ? NodeIsConstant(true) : 1) && NodeIsConstant(false)) {
       Node *value;
       
       value = (NodeConstantBooleanValue(cond))
	   ? (true ? NodeGetConstantValue(true) : NodeGetConstantValue(cond)) /* more hackery to deal with the gcc missiong-operand extension*/
	 : NodeGetConstantValue(false);
       
       NodeSetConstantValue(node, value);
     }
}

GLOBAL void ConstFoldCast(Node *node)
{
  Node *expr;
  Node *from_type, *to_type;
  BasicType from_basic, to_basic;

  /* this function works on both casts and implicitcasts */
  switch (node->typ) {
  case Cast:
    expr = node->u.cast.expr;
    break;
  case ImplicitCast:
    expr = node->u.implicitcast.expr;
    if (expr == NULL)
      return;
    break;
  default:
    UNREACHABLE;
  }

  if (!NodeIsConstant(expr))
    return;

  to_type = NodeDataType(node);
  from_type = NodeDataType(expr);


  /* can only constant-fold scalar expressions (integral, floating,
     and pointer) */

  if (IsScalarType(to_type) && IsScalarType(from_type)) {
    from_basic = BasicTypeOfConstantValue(from_type);
    to_basic = BasicTypeOfConstantValue(to_type);

    switch (to_basic) {
    case Slonglong:
    case Ulonglong:
    case Longdouble:
      /* fix: cannot represent these types internally, so no constant-folding
	 occurs. */
      return;
    default:
      break;
    }

    switch (from_basic) {
    case Sint:
      { int eval = NodeConstantSintValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, eval);                  return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    case Uint:
      { unsigned eval = NodeConstantUintValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, (signed int)eval);      return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    case Slong:
      { long eval = NodeConstantSlongValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, (signed int)eval);      return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    case Ulong:
      { unsigned long eval = NodeConstantUlongValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, (signed int)eval);      return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    case Float:
      { float eval = NodeConstantFloatValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, (signed int)eval);      return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    case Double:
      { double eval = NodeConstantDoubleValue(expr);
	switch (to_basic) {
	case Sint:
	  NodeSetSintValue(node, (signed int)eval);      return;
	case Uint:
	  NodeSetUintValue(node, (unsigned int)eval);    return;
	case Slong:
	  NodeSetSlongValue(node, (signed long)eval);    return;
	case Ulong:
	  NodeSetUlongValue(node, (unsigned long)eval);  return;
	case Float:
	  NodeSetFloatValue(node, (float)eval);          return;
	case Double:
	  NodeSetDoubleValue(node, (double)eval);        return;
	default:
	  UNREACHABLE;
	}
      }
    default:
      UNREACHABLE;
    }
  }
}

PRIVATE BasicType BasicTypeOfConstantValue(Node *type)
{
     if (type->typ == Ptr)
	  return Uint;

     if (type->typ == Edcl)
	  return Sint;

     assert(type->typ == Prim);
     switch (type->u.prim.basic) {
	 case Char:
	 case Schar:
	 case Uchar:
	 case Sshort:
	 case Ushort:
	      return Sint;
	 default:
	      return type->u.prim.basic;
     }
}
