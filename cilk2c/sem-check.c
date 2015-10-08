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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/sem-check.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include "conversions.h"
#include "initializer.h"

PRIVATE void SemCheckIsArithmeticType(Node *type, Node *op);
PRIVATE void SemCheckIsScalarType(Node *type, Node *op);
PRIVATE void SemCheckIsIntegralType(Node *type, Node *op);
PRIVATE void AssignEnumValues(SUEtype *enm);
GLOBAL void StructCheckFields(SUEtype *sue);
GLOBAL void UnionCheckFields(SUEtype *sue);

PRIVATE Node *SemCheckAssignment(Node *node, binopNode *u);
PRIVATE Node *SemCheckDot(Node *node, binopNode *u);
PRIVATE Node *SemCheckDesignatedInitializer(Node *node, binopNode *u);
PRIVATE Node *SemCheckArrayLabeledInitializer(Node *node, binopNode *u);
PRIVATE Node *SemCheckArrow(Node *node, binopNode *u);
PRIVATE Node *SemCheckArithmetic(Node *node, binopNode *u);
PRIVATE Node *SemCheckComparison(Node *node, binopNode *u);

PRIVATE void SemCheckCallArgs(Node *node, List *formals, List *actuals);
PRIVATE void SemCheckInletArgs(Node *node, List *formals, List *actuals);

PRIVATE struct SwitchCheck *NewSwitchCheck(List *cases);
PRIVATE void FreeSwitchCheck(struct SwitchCheck *check);
PRIVATE void SwitchCheckAddCase(struct SwitchCheck *check, Node *expr);
PRIVATE void SwitchCheckAddDefault(struct SwitchCheck *check, Node *node);

PRIVATE Bool in_procedure = 0;
PRIVATE Bool in_inlet = 0;

GLOBAL List *SemanticCheckProgram(List *program)
{
     ListMarker marker;
     Node *item;

     IterateList(&marker, program);
     while (NextOnList(&marker, (GenericREF) & item)) {
	  assert(item);
	  SemCheckNode(item);

	  if (item->typ == Decl && item->u.decl.init == NULL) {
	       Node *type;

	       assert(item->u.decl.type);
	       type = NodeDataType(item->u.decl.type);

	       if (NodeIsConstQual(type) && !DeclIsExtern(item) && !DeclIsTypedef(item))
		    WarningCoord(1, item->coord,
			  "const object should have initializer: \"%s\"",
				 item->u.decl.name);
	  }
     }

     return program;
}

/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *SemCheckConst(Node *node, ConstNode *u)
{
     u->type = SemCheckNode(u->type);
     return node;
}

PRIVATE Node *SemCheckId(Node *node, idNode *u)
{
     if (u->decl) {
	  Node *decl = u->decl;
	  TypeQual tq = NodeTq(decl);

	  /* if id is an enum constant, get its value */
	  if (DeclIsEnumConst(decl)) {
	       assert(decl->u.decl.init);
	       NodeSetConstantValue(node, NodeGetConstantValue(decl->u.decl.init));
	  }

	  if (tq_has_cilkonly(tq)) {
	       if (in_procedure || in_inlet) {
		    /* ok */
	       } else {
		    SyntaxErrorCoord(
			 node->coord,
			 "identifier `%s' can only be used in Cilk procedure or inlet",
			 u->text);
	       }
	  }

     }
     return node;
}

PRIVATE Node *SemCheckBinop(Node *node, binopNode *u)
{
     OpType operator = u->op;

     u->left = SemCheckNode(u->left);
     u->right = SemCheckNode(u->right);

     if (IsAssignmentOp(operator))
	  return SemCheckAssignment(node, u);
     else if (operator == '.')
	  return SemCheckDot(node, u);
     else if (operator == ARROW)
	  return SemCheckArrow(node, u);
     else if (operator == ':' || operator == DESIGNATED_INITIALIZER) 
	 return SemCheckDesignatedInitializer(node, u);
     else if (operator == ARRAY_LABELED_INITIALIZER)
	 return SemCheckArrayLabeledInitializer(node, u);
     else if (IsArithmeticOp(operator))
	  return SemCheckArithmetic(node, u);
     else if (IsComparisonOp(operator))
	  return SemCheckComparison(node, u);
     else {
	  fprintf(stderr, "Internal Error: Unrecognized Binop\n");
	  assert(FALSE);
     }
     UNREACHABLE;
     return NULL;		/* eliminates warning */
}

PRIVATE Node *SemCheckUnary(Node *node, unaryNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->type = NodeDataType(u->expr);

     switch (u->op) {
	      /* Must be arithmetic.  Apply usual conversions */
	 case UMINUS:
	      SemCheckIsArithmeticType(u->type, node);

	      if (NodeIsConstant(u->expr)) {
		   if (NodeTypeIsSint(u->expr)) {
			int eval = NodeConstantSintValue(u->expr);

			NodeSetSintValue(node, 0 - eval);
		   } else if (NodeTypeIsUint(u->expr)) {
			unsigned int eval = NodeConstantUintValue(u->expr);

			NodeSetUintValue(node, 0 - eval);
		   } else if (NodeTypeIsSlong(u->expr)) {
			long eval = NodeConstantSlongValue(u->expr);

			NodeSetSlongValue(node, 0 - eval);
		   } else if (NodeTypeIsUlong(u->expr)) {
			unsigned long eval = NodeConstantUlongValue(u->expr);

			NodeSetUlongValue(node, 0 - eval);
		   } else if (NodeTypeIsFloat(u->expr)) {
			float eval = NodeConstantFloatValue(u->expr);

			NodeSetFloatValue(node, 0 - eval);
		   } else if (NodeTypeIsDouble(u->expr)) {
			double eval = NodeConstantDoubleValue(u->expr);

			NodeSetDoubleValue(node, 0 - eval);
		   }
	      }
	      break;
	 case UPLUS:
	      SemCheckIsArithmeticType(u->type, node);
	      return u->expr;

	 case '!':
	      u->expr = UsualUnaryConversions(u->expr, FALSE);
	      u->type = NodeDataType(u->expr);
	      u->type = PrimSint;

	      if (NodeIsConstant(u->expr)) {
		   if (NodeTypeIsSint(u->expr)) {
			int eval = NodeConstantSintValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   } else if (NodeTypeIsUint(u->expr)) {
			unsigned int eval = NodeConstantUintValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   } else if (NodeTypeIsSlong(u->expr)) {
			long eval = NodeConstantSlongValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   } else if (NodeTypeIsUlong(u->expr)) {
			unsigned long eval = NodeConstantUlongValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   } else if (NodeTypeIsFloat(u->expr)) {
			float eval = NodeConstantFloatValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   } else if (NodeTypeIsDouble(u->expr)) {
			double eval = NodeConstantDoubleValue(u->expr);

			NodeSetSintValue(node, eval == 0);
		   }
	      }
	      break;

	      /* Must be integral.  Apply usual conversions. */
	 case '~':
	      u->expr = UsualUnaryConversions(u->expr, FALSE);
	      u->type = NodeDataType(u->expr);
	      SemCheckIsIntegralType(u->type, node);
	      if (NodeIsConstant(u->expr)) {
		   if (NodeTypeIsSint(u->expr)) {
			int eval = NodeConstantSintValue(u->expr);

			NodeSetSintValue(node, ~eval);
		   } else if (NodeTypeIsUint(u->expr)) {
			unsigned int eval = NodeConstantUintValue(u->expr);

			NodeSetUintValue(node, ~eval);
		   } else if (NodeTypeIsSlong(u->expr)) {
			long eval = NodeConstantSlongValue(u->expr);

			NodeSetSlongValue(node, ~eval);
		   } else if (NodeTypeIsUlong(u->expr)) {
			unsigned long eval = NodeConstantUlongValue(u->expr);

			NodeSetUlongValue(node, ~eval);
		   }
	      }
	      break;

	      /* Must be scalar modifiable lval.  Apply usual conversions */
	 case PREINC:
	 case PREDEC:
	 case POSTINC:
	 case POSTDEC:
	      if (!IsModifiableLvalue(u->expr))
		   SyntaxErrorCoord(node->coord,
			      "operand must be modifiable lvalue: op %s",
				    Operator[u->op].text);

	      u->type = NodeDataType(u->expr);
	      SemCheckIsScalarType(u->type, node);

	      if (IsPointerType(u->type)) {
		   /* try to get size of base type, to ensure that it
		    * is defined (and not void or an incomplete
		    * struct/union type) */
		   (void) NodeSizeof(node, GetShallowBaseType(u->type), FALSE);
	      }
	      break;

	      /* Memory leak */
	 case SIZEOF:
	      NodeSetUintValue(node, NodeSizeof(u->expr, u->type, FALSE));
	      u->type = PrimUint;	/* fix: should really be size_t */
	      break;

	 case ALIGNOF:
	      u->type = PrimUint;	/* fix: should really be size_t */
	      break;

         case IMAG:
         case REAL:
  	      u->type = NodeCopy(NodeDataType(u->expr), NodeOnly);
	      /* Now would be a good time to make sure that it actually is a number of some sort. */
	      NodeUpdateTq(u->type, tq_remove_complex);
	      break;

	      /* Function or object lvalue.  Returns PTR to T */
	 case ADDRESS:
	      if (IsLvalue(u->expr) || IsFunctionType(u->type) || IsArrayType(u->type))
		   u->type = MakePtr(EMPTY_TQ, u->type);
	      else
		   SyntaxErrorCoord(node->coord,
			      "cannot take the address of a non-lvalue");

	      break;

	      /* Must be a pointer or Adcl.  Result type is referenced object */
	 case INDIR:
	      if (u->type->typ == Ptr)
		   u->type = u->type->u.ptr.type;
	      else if (u->type->typ == Adcl)
		   u->type = u->type->u.adcl.type;
	      else if (u->type->typ == Fdcl)
		   /*
		    * Fdcl automatically becomes Ptr(Fdcl), then INDIR
		    * eliminates the Ptr
		    */
		   ;	
	      else
		   SyntaxErrorCoord(node->coord, "cannot dereference non-pointer type");
	      break;

	 default:
	      fprintf(stdout,
		      "Unsupported unary operator \"%s\"\n",
		      Operator[u->op].text);
	      assert(FALSE);
     }

     return node;
}

PRIVATE Node *SemCheckCast(Node *node, castNode *u)
{
     u->type = SemCheckNode(u->type);
     u->expr = SemCheckNode(u->expr);

     u->expr = CastConversions(u->expr, u->type);
     ConstFoldCast(node);
     return node;
}

PRIVATE Node *SemCheckComma(Node *node, commaNode *u)
{
     u->exprs = SemCheckList(u->exprs);
     return node;
}

PRIVATE Node *SemCheckConstructor(Node *node, constructorNode *u)
{
     u->type            = SemCheckNode(u->type);
     u->initializerlist = SemCheckNode(u->initializerlist);
     return node;
}

PRIVATE Node *SemCheckTernary(Node *node, ternaryNode *u)
{
     Node *cond = u->cond, *true = u->true, *false = u->false, *ctype,
     *ttype, *ftype;

     cond = SemCheckNode(cond);
     true = SemCheckNode(true);
     false = SemCheckNode(false);

     cond = UsualUnaryConversions(cond, FALSE);

     assert(cond);
     /* Allow the true operand to be omitted. */
     /*assert(true);*/
     assert(false);

     ctype = NodeDataType(cond);
     if (true) ttype = NodeDataType(true);       
     ftype = NodeDataType(false);

     /* Apply the standard unary conversions */
     if (true) true = UsualUnaryConversions(true, FALSE);
     false = UsualUnaryConversions(false, FALSE);

     /* Unify the true/false branches */
     if (true) {
       u->type = ConditionalConversions(&true, &false);
       u->true = true;
       u->false = false;
     } else {
       u->type = ConditionalConversions(&cond, &false);
       u->cond = cond;
       u->false = false;
     }
     ConstFoldTernary(node);

     return node;
}

PRIVATE Node *SemCheckArray(Node *node, arrayNode *u)
{
     Node *type;

     assert(u->name);
     assert(u->dims);

     u->name = SemCheckNode(u->name);
     SemCheckList(u->dims);

     assert(u->name);
     type = NodeDataType(u->name);

     /* if array.name is not an Adcl or a Ptr then check first dimension */
     if (type->typ != Adcl && type->typ != Ptr) {
	  Node *first = FirstItem(u->dims), *ftype;

	  /* Canonicalize A[i] and i[A] */
	  assert(first);
	  ftype = NodeDataType(first);

	  if (ftype->typ == Adcl || ftype->typ == Ptr) {
	       SetItem(u->dims, u->name);
	       u->name = first;
	       type = ftype;
	  } else {
	       SyntaxErrorCoord(node->coord,
				"cannot dereference non-pointer type");
	       u->type = PrimVoid;
	       return node;
	  }
     }
     /* try to get size of base type, to ensure that it is defined
      * (and not void or an incomplete struct/union type) */
     NodeSizeof(u->name, GetShallowBaseType(type), FALSE);

     /*printf("->name type is\n"); DPN(NodeDataType(u->name));*/
     /*printf("->name is\n"); DPN(u->name);*/

     u->type = ArrayType(node);

     return node;
}

PRIVATE Node *SemCheckCall1(Node *node, callNode *u, Node *call_type)
{
     if (call_type->typ == Fdcl) {
	  SemCheckCallArgs(node, call_type->u.fdcl.args, u->args);
	  if (tq_has_procedure(call_type->u.fdcl.tq))
	       SyntaxErrorCoord(node->coord, "use spawn to call a cilk procedure");
	  else if (tq_has_inlet(call_type->u.fdcl.tq))
	       SyntaxErrorCoord(node->coord, "inlets must have a spawned argument");
     } else if (call_type->typ == Ptr) {
	  return SemCheckCall1(node, u, NodeDataType(call_type->u.ptr.type));
     } else
	  SyntaxErrorCoord(node->coord, "called object is not a function");

     return node;
}

PRIVATE Node *SemCheckCall(Node *node, callNode *u)
{
     Node *call_type;

     u->name = SemCheckNode(u->name);
     u->args = SemCheckList(u->args);

     call_type = NodeDataType(u->name);

     return SemCheckCall1(node, u, call_type);
}

PRIVATE Node *SemCheckInitializer(Node *node, initializerNode *u)
{
     u->exprs = SemCheckList(u->exprs);
     return node;
}

PRIVATE Node *SemCheckImplicitCast(Node *node, implicitcastNode *u)
{
     u->expr = SemCheckNode(u->expr);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *SemCheckLabel(Node *node, UNUSED(labelNode *u))
{
     return node;
}

PRIVATE Node *SemCheckSwitch(Node *node, SwitchNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->expr = UsualUnaryConversions(u->expr, FALSE);

     if (!NodeTypeIsIntegral(u->expr))
	  SyntaxErrorCoord(u->expr->coord,
		       "controlling expression must have integral type");

     u->check = NewSwitchCheck(u->cases);
     u->stmt = SemCheckNode(u->stmt);

     /*
      * fix: if u->expr is enumerated type, look at u->check and issue
      * warning if not all cases covered
      */
     FreeSwitchCheck(u->check);
     u->check = NULL;

     return node;
}

PRIVATE Node *SemCheckCase(Node *node, CaseNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->expr = UsualUnaryConversions(u->expr, FALSE);

     if (!IsIntegralConstant(u->expr))
	  SyntaxErrorCoord(u->expr->coord,
			   "case expression must be integer constant");
     else {
	  assert(u->container->typ == Switch);
	  SwitchCheckAddCase(u->container->u.Switch.check, u->expr);
     }

     return node;
}

PRIVATE Node *SemCheckDefault(Node *node, DefaultNode *u)
{
     assert(u->container->typ == Switch);
     SwitchCheckAddDefault(u->container->u.Switch.check, node);

     return node;
}

PRIVATE Node *EnsureScalarType(Node *node)
{
     Node *type;
     if (node) {
	  node = UsualUnaryConversions(node, FALSE);
	  type = NodeDataType(node);

	  if (!(IsScalarType(type)))
	       SyntaxErrorCoord(node->coord,
				"controlling expressions must have scalar type");
     }
     
     return node;
}

PRIVATE Node *SemCheckIf(Node *node, IfNode *u)
{
     u->expr = SemCheckNode(u->expr);
     assert(u->expr);
     u->expr = EnsureScalarType(u->expr);

     if (u->stmt)
	  u->stmt = SemCheckNode(u->stmt);
     return node;
}

PRIVATE Node *SemCheckIfElse(Node *node, IfElseNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->expr = UsualUnaryConversions(u->expr, FALSE);
     assert(u->expr);
     u->expr = EnsureScalarType(u->expr);

     if (u->true)
	  u->true = SemCheckNode(u->true);
     if (u->false)
	  u->false = SemCheckNode(u->false);
     return node;
}

PRIVATE Node *SemCheckWhile(Node *node, WhileNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->expr = EnsureScalarType(u->expr);

     u->stmt = SemCheckNode(u->stmt);
     return node;
}

PRIVATE Node *SemCheckDo(Node *node, DoNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->expr = EnsureScalarType(u->expr);

     u->stmt = SemCheckNode(u->stmt);

     return node;
}

PRIVATE Node *SemCheckFor(Node *node, ForNode *u)
{
     u->cond = SemCheckNode(u->cond);
     u->cond = EnsureScalarType(u->cond);

     u->init = SemCheckNode(u->init);
     u->next = SemCheckNode(u->next);
     u->stmt = SemCheckNode(u->stmt);

     return node;
}

PRIVATE Node *SemCheckGoto(Node *node, GotoNode * UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckContinue(Node *node, ContinueNode *UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckBreak(Node *node, BreakNode *UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckReturn(Node *node, ReturnNode *u)
{
     Node *FunctionRetType = FunctionReturnType(u->proc);

     u->expr = SemCheckNode(u->expr);

     if (u->expr) {
	  if (IsVoidType(FunctionRetType)) {
	       if (ANSIOnly)
		    SyntaxErrorCoord(u->expr->coord, "void function cannot return value");
	       else if (IsVoidType(NodeDataType(u->expr))) {
		   /*nothing*/; /* For gcc, void functions may return void values.*/
	       } else {
		    SyntaxErrorCoord(u->expr->coord, "`return ' with a value, in a function returning void");
	       }
	  } else
	       u->expr = ReturnConversions(u->expr, FunctionRetType);
     } else if (!IsVoidType(FunctionRetType))
	  SyntaxErrorCoord(node->coord, "non-void function must return value");

     return node;
}

PRIVATE Node *SemCheckBlock(Node *node, BlockNode *u)
{
     /* Check that any Adcl's in the declaration list are complete */
     ListMarker decl_marker;
     Node *decl;

     IterateList(&decl_marker, u->decl);
     while (NextOnList(&decl_marker, (GenericREF) & decl)) {
	  if (decl)
	       decl = SemCheckNode(decl);

	  if (decl->typ == Decl && decl->u.decl.init == NULL) {
	       Node *type;

	       assert(decl->u.decl.type);
	       type = NodeDataType(decl->u.decl.type);

	       if (NodeIsConstQual(type) && !DeclIsExtern(decl) && !DeclIsTypedef(decl)) {
		    WarningCoord(1, decl->coord,
			  "const object should have initializer: \"%s\"",
				 decl->u.decl.name);
	       }
	  }
     }

     /* Now walk the statements */
     u->stmts = SemCheckList(u->stmts);

     if (u->type == NULL) {
	  Node *item = LastItem(u->stmts);

	  assert(item);
	  u->type = NodeDataType(item);
	  /* PrintNode(stdout, u->type, 2); */
     }
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                             Type nodes                                */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *SemCheckPrim(Node *node, primNode *UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckTdef(Node *node, tdefNode *UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckPtr(Node *node, ptrNode *u)
{
     u->type = SemCheckNode(u->type);
     return node;
}

PRIVATE Node *SemCheckAdcl(Node *node, adclNode *u)
{
     ARRAYtype *dimp;
     u->type = SemCheckNode(u->type);
     dimp = u->dimp;

     if (dimp->dim) {
	  Node *dim = SemCheckNode(dimp->dim);
	  dimp->dim = dim;

	  if (!NodeIsConstant(dim)) {
	       SyntaxErrorCoord(dim->coord, "array dimension must be constant");
	       dimp->size = 0;
	  } else if (!IsIntegralType(NodeDataType(dim))) {
	       SyntaxErrorCoord(dim->coord,
			      "array dimension must be an integer type");
	       dimp->size = 0;
	  } else {
	       int val = NodeConstantIntegralValue(dim);

	       assert(u->type);

	       /* check the array bound */
	       if (val < 0) {
		    SyntaxErrorCoord(dim->coord, "negative array dimension");
		    dimp->size = 0;
	       } else {
		    if (val == 0 && ANSIOnly)
			 WarningCoord(1, dim->coord, "array dimension is zero");

		    dimp->size =
			val * NodeSizeof(node, NodeDataType(u->type), FALSE);
	       }
	  }
     } else
	  dimp->size = 0;

     return node;
}

PRIVATE Node *SemCheckFdcl(Node *node, fdclNode *u)
{
     u->args = SemCheckList(u->args);
     u->returns = SemCheckNode(u->returns);
     return node;
}

PRIVATE Node *SemCheckSdcl(Node *node, sdclNode *u)
{
     if (tq_has_sue_elaborated(u->tq))
	  StructCheckFields(u->type);
     return node;
}

PRIVATE Node *SemCheckUdcl(Node *node, udclNode *u)
{
     if (tq_has_sue_elaborated(u->tq))
	  UnionCheckFields(u->type);
     return node;
}

PRIVATE Node *SemCheckEdcl(Node *node, edclNode *u)
{
     if (tq_has_sue_elaborated(u->tq))
	  AssignEnumValues(u->type);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *SemCheckDecl(Node *node, declNode *u)
{
     u->type = SemCheckNode(u->type);
     u->init = SemCheckNode(u->init);
     u->bitsize = SemCheckNode(u->bitsize);

     if (u->init)
	  SemCheckDeclInit(node, tq_has_block_decl(NodeDeclLocation(node)));
     else {
	  /* catch unsized arrays in blocks */
	  if (DeclIsBlock(node) 
	      && !DeclIsTypedef(node) && u->type->typ != Fdcl)
	       NodeSizeof(node, u->type, FALSE);
     }

     if (tq_has_shared(u->tq)) {
	  if (tq_has_private(u->tq))
	       SyntaxErrorCoord(node->coord,
				"variable `%s' cannot be both shared and private", u->name);
	  if (!tq_has_top_decl(NodeDeclLocation(node)))
	       SyntaxErrorCoord(node->coord,
				"only global variables can be shared");
	  if (u->type->typ == Fdcl)
	       SyntaxErrorCoord(node->coord,
			 "functions cannot be declared shared", u->name);
     } else if (tq_has_private(u->tq)) {
	 if (!tq_has_top_decl(NodeDeclLocation(node)))
	       SyntaxErrorCoord(node->coord,
				"only global variables can be private");
	  if (u->type->typ == Fdcl)
	       SyntaxErrorCoord(node->coord,
				"functions cannot be declared private");
	  if (u->init)
	       WarningCoord(1, node->coord,
		  "Initialization has no meaning for private variables");
     }
     return node;
}

PRIVATE Node *SemCheckAttrib(Node *node, attribNode *UNUSED(u))
{
     return node;
}

PRIVATE Node *SemCheckProc(Node *node, procNode *u)
{
     Node *type;
     List *args;
     Bool save_in_procedure, save_in_inlet;

     u->decl = SemCheckNode(u->decl);

     assert(u->decl->typ == Decl);
     type = u->decl->u.decl.type;

     args = type->u.fdcl.args;

     /* Verify that none of the parameters have initializers */
     {
	  ListMarker marker;
	  Node *item;

	  if (!IsVoidArglist(args)) {
	       IterateList(&marker, args);
	       while (NextOnList(&marker, (GenericREF) & item))
		    if (item->typ == Decl) {
			 if (item->u.decl.init)
			      SyntaxErrorCoord(item->coord,
					"cannot initialize parameter %s",
					       item->u.decl.name);
		    } else if (IsEllipsis(item));
		    else {
			 fprintf(stderr, "Unrecognized parameter\n");
			 fPrintNode(stderr, item, 0);
			 fprintf(stderr, "\n");
			 assert(FALSE);
		    }
	  }
     }

     save_in_procedure = in_procedure;
     save_in_inlet = in_inlet;

     if (tq_has_procedure(tq_type_quals(u->decl->u.decl.type->u.fdcl.tq))) {
	 if (!tq_has_top_decl(tq_decl_location(u->decl->u.decl.tq)))
	       SyntaxErrorCoord(node->coord, "cilk procedure must be defined at top level");
	  if (tq_has_inlet(tq_type_quals(u->decl->u.decl.type->u.fdcl.tq)))
	       SyntaxErrorCoord(node->coord, "cilk procedure cannot be an inlet");
	  in_procedure = 1;
     } else if (tq_has_inlet(tq_type_quals(u->decl->u.decl.type->u.fdcl.tq))) {
	  if (!in_procedure)
	       SyntaxErrorCoord(node->coord,
			"inlet must be defined inside a cilk procedure");
	  if (in_inlet)
	       SyntaxErrorCoord(node->coord,
			     "inlet cannot be defined in another inlet");
	  in_inlet = 1;
     }
     u->body = SemCheckNode(u->body);

     /* save pointer to proc node from fdcl node -KHR */
     type->u.fdcl.proc_node = node;

     in_procedure = save_in_procedure;
     in_inlet = save_in_inlet;

     return node;
}

PRIVATE Node *SemCheckText(Node *node, textNode *UNUSED(u))
{
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                            GCC asm Extensions                         */
/*                                                                       */
/*************************************************************************/

PRIVATE Node *SemCheckAsm(Node *node, asmNode *u)
{
     u->template = SemCheckNode(u->template);
     u->output = SemCheckList(u->output);
     u->input = SemCheckList(u->input);
     u->clobbered = SemCheckList(u->clobbered);

     /* make sure template is a string */
     /* make sure output arguments are lvalues */

     return node;
}

PRIVATE Node *SemCheckAsmArg(Node *node, asmargNode *u)
{
     u->constraint = SemCheckNode(u->constraint);
     u->expr = SemCheckNode(u->expr);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
PRIVATE Node *SemCheckBuiltinVaArg(Node *node, builtinvaargNode *u)
{
     u->expr = SemCheckNode(u->expr);
     u->type = SemCheckNode(u->type);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE Bool IsSchedulableType(Node *type)
{
     assert(type);
     type = NodeDataType(type);
     if (type->typ == Fdcl) {
	  TypeQual tq = NodeTq(type);
	  return tq_has_procedure(tq);
     } else
	  return FALSE;
}

PRIVATE Bool IsInletType(Node *type)
{
     assert(type);
     type = NodeDataType(type);
     if (type->typ == Fdcl) {
	  TypeQual tq = NodeTq(type);
	  return tq_has_inlet(tq);
     } else
	  return FALSE;
}

PRIVATE Node *SemCheckSpawn(Node *node, spawnNode *u)
{
     Node *calltype;

     if (!in_procedure || in_inlet)
	  SyntaxErrorCoord(node->coord,
			   "spawn must be in a cilk procedure");

     u->receiver = SemCheckNode(u->receiver);
     u->name = SemCheckNode(u->name);
     u->args = SemCheckList(u->args);

     if (u->receiver) {
	  if (!IsModifiableLvalue(u->receiver))
	       SyntaxErrorCoord(node->coord,
			     "spawn receiver must be modifiable lvalue");
     }
     calltype = NodeDataType(u->name);
     if (IsPtrToFunction(calltype))
	  calltype = calltype->u.ptr.type;
     if (!IsSchedulableType(calltype))
	  SyntaxErrorCoord(u->name->coord, "cannot spawn non-procedure");
     else {
	  /* check return type */
	  /* leverage off of AssignmentConversions - KHR */
	  if (!u->in_inlet) {
	       if (!u->receiver) {
		    if (!TypeEqual(calltype->u.fdcl.returns, PrimVoid))
			 SyntaxErrorCoord(node->coord,
			  "spawn must be assigned to a receiver lvalue");
	       } else {
		    if ((u->assign_op == PLUSassign || u->assign_op == MINUSassign) &&
			IsPointerType(NodeDataType(u->receiver)) &&
			IsIntegralType(calltype->u.fdcl.returns));
		    else if (!TypeEqualQualified(calltype->u.fdcl.returns, NodeDataType(u->receiver), FALSE, FALSE))
			 SyntaxErrorCoord(node->coord,
					  "spawn receiver incompatible with procedure return type");
	       }
	  }
	  /* compare the modified formals/actuals lists */
	  SemCheckCallArgs(node, calltype->u.fdcl.args, u->args);
     }

     return node;
}

PRIVATE Node *SemCheckSync(Node *node, syncNode *UNUSED(u))
{
     if (!in_procedure || in_inlet)
	  SyntaxErrorCoord(node->coord,
			   "sync must be in a cilk procedure");

     return node;
}

PRIVATE Node *SemCheckInletCall(Node *node, inletcallNode *u)
{
     Node *calltype;
     ListMarker marker;
     Node *actual;
     Node *spawn_return_type;

     if (!in_procedure || in_inlet)
	  SyntaxErrorCoord(node->coord,
			   "inlet call must be in a cilk procedure");

     u->name = SemCheckNode(u->name);
     u->args = SemCheckList(u->args);

     calltype = NodeDataType(u->name);
#if 0
     /* uncomment this when inlets are allowed to be called by pointer */
     if (IsPtrToFunction(calltype))
	  calltype = calltype->u.ptr.type;
#else
     if (IsPtrToFunction(calltype) && IsInletType(calltype->u.ptr.type)) {
	  SyntaxErrorCoord(u->name->coord, "cilk2c cannot compile inlet calls via a pointer");
	  return node;
     }
#endif
     if (!IsInletType(calltype)) {
	  SyntaxErrorCoord(u->name->coord, "cannot call a non-inlet at inlet call");
	  return node;
     }
     /* check return type */
     /* leverage off of AssignmentConversions - KHR */
     if (!TypeEqual(calltype->u.fdcl.returns, PrimVoid))
	 SyntaxErrorCoord(node->coord,
			  "inlets must have void return type");

     /* make sure first actual is a spawn */
     if (ListLength(u->args) == 0) {
	  SyntaxErrorCoord(node->coord,
			   "inlet call must have a spawn argument");
	  return node;
     }
     if (((Node *) FirstItem(u->args))->typ != Spawn) {
	  SyntaxErrorCoord(node->coord, "first argument to inlet call must be a spawn");
	  spawn_return_type = NULL;
     } else {
	  Node *spawn = FirstItem(u->args);
	  spawn_return_type = spawn->u.spawn.name->u.id.decl->u.decl.type->u.fdcl.returns;
     }

     /* make sure none of the rest of the actuals are spawns */
     IterateList(&marker, Rest(u->args));
     while (NextOnList(&marker, (GenericREF) & actual)) {
	  if (actual->typ == Spawn)
	       SyntaxErrorCoord(actual->coord,
			"only first inlet call argument can be a spawn");
     }

     if (u->implicit_inlet) {
	  /* this is a call to an implicit_inlet, we need check the type of
	   * lhs (which is contained within the second argument) matches with
	   * the one of rhs (which could be found in calltype->u.fdcl.args) */
	  Node *lhs, *rhs;
	  Node *ltype;

	  assert(u->args);
	  assert(Rest(u->args));
	  assert(FirstItem(Rest(u->args)));
	  assert(((Node *) FirstItem(Rest(u->args)))->typ == Unary);
	  lhs = FirstItem(Rest(u->args));

	  assert(calltype->u.fdcl.args);
	  assert(Rest(calltype->u.fdcl.args));
	  assert(FirstItem(Rest(calltype->u.fdcl.args)));
	  assert(((Node *) FirstItem(Rest(calltype->u.fdcl.args)))->typ == Decl);
	  rhs = FirstItem(Rest(calltype->u.fdcl.args));

	  /* the type of lhs is the actual type with one additional Ptr level
	   * because of & is added in front of lhs */
	  ltype = NodeDataType(lhs);
	  assert(ltype->typ == Ptr);

	  if (!TypeEqualQualified(NodeDataType(lhs),
				  NodeDataType(rhs),
				  FALSE,
				  FALSE))
	      SyntaxErrorCoord(node->coord,
			       "implicit inlet call receiver incompatible with spawn return type");
     } else {
	  /* compare the modified formals/actuals lists */
	  if (spawn_return_type && IsVoidType(spawn_return_type))
	       SemCheckInletArgs(node, calltype->u.fdcl.args, Rest(u->args));
	  else
	       SemCheckInletArgs(node, calltype->u.fdcl.args, u->args);
     }

     return node;
}

PRIVATE Node *SemCheckAbort(Node *node, abortNode *UNUSED(u))
{
     if (!in_procedure)
	  SyntaxErrorCoord(node->coord,
			   "abort must be in a cilk procedure or inlet");

     return node;
}

PRIVATE Node *SemCheckSynched(Node *node, synchedNode *UNUSED(u))
{
     if (!in_procedure || in_inlet)
	  SyntaxErrorCoord(node->coord,
			   "SYNCHED must be in a cilk procedure");

     return node;
}


PRIVATE Node *SemCheckXBlock(Node *node, XBlockNode *u)
{
     /* Check that any Adcl's in the declaration list are complete */
     ListMarker decl_marker;
     Node *decl;

     IterateList(&decl_marker, u->decl);
     while (NextOnList(&decl_marker, (GenericREF) & decl)) {
	  if (decl)
	       decl = SemCheckNode(decl);

	  if (decl->typ == Decl && decl->u.decl.init == NULL) {
	       Node *type;

	       assert(decl->u.decl.type);
	       type = NodeDataType(decl->u.decl.type);

	       if (NodeIsConstQual(type) && !DeclIsExtern(decl) && !DeclIsTypedef(decl)) {
		    WarningCoord(1, decl->coord,
			  "const object should have initializer: \"%s\"",
				 decl->u.decl.name);
	       }
	  }
     }

     /* Now walk the statements */
     u->stmts = SemCheckList(u->stmts);

     if (u->type == NULL) {
	  Node *item = LastItem(u->stmts);

	  assert(item);
	  u->type = NodeDataType(item);
	  /* PrintNode(stdout, u->type, 2); */
     }
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                  SemCheckNode and SemCheckList                        */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *SemCheckNode(Node *node)
{
#if 0
     printf("\nSemCheckNode 0x%08x\n", node);
#endif

     if (node == NULL)
	  return node;

#if 0
     PrintNode(stdout, node, 0);
     printf("\n");
#endif

#define CODE(name, node, union) return SemCheck##name(node, union)
     ASTSWITCH(node, CODE)
#undef CODE

	 UNREACHABLE;
     return NULL;
}

GLOBAL List *SemCheckList(List *list)
{
     List *aptr;

     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  if (item)
	       SetItem(aptr, SemCheckNode(item));
     }

     return list;
}

/*************************************************************************/
/*                  Auxilliary routines for SemCheck                     */
/*************************************************************************/

PRIVATE void SemCheckIsArithmeticType(Node *type, Node *op)
{
     assert(type);
     assert(op);
     if (!IsArithmeticType(type)) {
	  OpType opcode = (op->typ == Unary) ? op->u.unary.op : op->u.binop.op;

	  SyntaxErrorCoord(op->coord,
			   "operand must have arithmetic type: op \"%s\"",
			   Operator[opcode].text);
     }
}

PRIVATE void SemCheckIsScalarType(Node *type, Node *op)
{
     if (!IsScalarType(type)) {
	  OpType opcode = (op->typ == Unary) ? op->u.unary.op : op->u.binop.op;

	  SyntaxErrorCoord(op->coord,
			   "operand must have scalar type: op \"%s\"",
			   Operator[opcode].text);
     }
}

PRIVATE void SemCheckIsIntegralType(Node *type, Node *op)
{
     if (!IsIntegralType(type)) {
	  OpType opcode = (op->typ == Unary) ? op->u.unary.op : op->u.binop.op;

	  SyntaxErrorCoord(op->coord,
			   "operand must have integral type: op \"%s\"",
			   Operator[opcode].text);
     }
}

PRIVATE void AssignEnumValues(SUEtype *enm)
{
     ListMarker marker;
     Node *c;
     TARGET_INT current_value = 0;

     assert(enm->typ == Edcl);
     if (enm->fields == NULL)
	  return;

     IterateList(&marker, enm->fields);
     while (NextOnList(&marker, (GenericREF) & c)) {
	  assert(c->typ == Decl);
	  if (c->u.decl.init == NULL) {
	       c->u.decl.init = MakeImplicitCast(PrimSint, NULL);
	       NodeSetSintValue(c->u.decl.init, current_value);
	  } else {
	       Node *value = SemCheckNode(c->u.decl.init);
	       c->u.decl.init = value;

	       if (!NodeIsConstant(value)) {
		    SyntaxErrorCoord(value->coord, "enum initializer must be constant");
	       } else if (NodeTypeIsSint(value))
		    current_value = NodeConstantSintValue(value);
	       else if (NodeTypeIsUint(value)) {
		    current_value = NodeConstantUintValue(value);
		    c->u.decl.init = AssignmentConversions(value, PrimSint);
	       } else if (NodeTypeIsSlong(value))
		    current_value = NodeConstantSlongValue(value);
	       else if (NodeTypeIsUlong(value)) {
		    current_value = NodeConstantUlongValue(value);
		    c->u.decl.init = AssignmentConversions(value, PrimSint);
	       } else {
		    SyntaxErrorCoord(value->coord, "enum initializer must be type int");
	       }
	  }
	  /* constants are already in the symbol table (by BuildEnumConst) */
	  current_value++;
	  /*
	   *it would be nice to check for values used twice, which is
	   * legal but usually a mistake 
	   */
     }
}

GLOBAL void StructCheckFields(SUEtype *sue)
{
     int cnt = 0;

     if (sue->visited == FALSE) {
	  int currentbit = 0, max_bitalign = 8;
	  ListMarker marker;
	  Node *field;

	  /* To stop infinite recursion */
	  sue->visited = TRUE;

	  /* Loop over the fields of the SDCL */
	  IterateList(&marker, sue->fields);
	  while (NextOnList(&marker, (GenericREF) & field)) {
	       int bitsize, bitalign;
	       declNode *decl;
	       Node *type;

	       assert(field->typ == Decl);
	       decl = &(field->u.decl);
	       type = NodeDataType(decl->type);
	       ++cnt;

	       if (decl->bitsize != NULL) {

		    /*
		     * Harbison&Steele is ambigous regarding which
		     * types are valid in a bitfield declaration, in
		     * particular regarding enumeration types. S5.6.5
		     * says that the only valid types are unsigned
		     * int, signed int, or int.  S5.5 says that, for
		     * the purpose of type checking, ``an enumerated
		     * type is just one of the integer types.''  ``One
		     * of the integer types'' could be ``long'',
		     * however.
		     *
		     * gcc accepts enumeration types even with -ansi
		     * -pedantic.  gcc also accepts ``char'',
		     * ``short'', etc.
		     *
		     * Here, we accept enumeration types but not the
		     * other gcc extensions.
		     *
		     * As of Mon Apr 21 2003, we are now going to accept the other gcc extensions, such as char and short, because the linux kernel actually uses it.
		     *  (See Bug 63)
		     */
		    if ((type->typ != Edcl) &&
			((type->typ != Prim) || ((type->u.prim.basic != Schar) &&
						 (type->u.prim.basic != Uchar) &&
						 (type->u.prim.basic != Sshort) &&
						 (type->u.prim.basic != Ushort) &&
						 (type->u.prim.basic != Sint) &&
						 (type->u.prim.basic != Uint)
						 ))) {
			 SyntaxErrorCoord(field->coord,
					  "bitfield must be of type "
					  "enum; or signed or unsigned char, short, int, long");
		    }
		    /* resolve bitsize */
		    decl->bitsize = SemCheckNode(decl->bitsize);
		    bitalign = BYTE_LENGTH;
		    if ((!NodeIsConstant(decl->bitsize)) ||
			(!IsIntegralType(NodeDataType(decl->bitsize)))) {
			 SyntaxErrorCoord(field->coord,
					  "bitfield size must be a positive "
					  "integral constant");
			 bitsize = 8;
		    } else
			 bitsize = NodeConstantIntegralValue(decl->bitsize);

		    if (bitsize < 0) {
			 SyntaxErrorCoord(field->coord,
				       "bitfield size must be positive");
		    } else if (bitsize == 0) {
			 if (decl->name == NULL) {
			      /* realign and go on */
			      if (currentbit % WORD_LENGTH != 0) {
				   currentbit = currentbit + WORD_LENGTH -
				       (currentbit % WORD_LENGTH);
			      }
			 } else {
			      SyntaxErrorCoord(field->coord,
					   "zero width for bit-field %s",
					       decl->name);
			 }
		    } else {
			 if (((currentbit + bitsize - 1) / WORD_LENGTH) !=
			     (currentbit / WORD_LENGTH)) {
			      /*
			       * bitfield crosses a word boundary
			       * realign to next word
			       */
			      currentbit =
				  WORD_LENGTH * ((currentbit / WORD_LENGTH) + 1);
			 }

			 currentbit += bitsize;
		    }
	       } else {		/* not a bitfield */
		    field = SemCheckNode(field);
		    SetCurrentOnList(&marker, field);

		    /*
		     * allow flexible arrays in last position, but only
		     * if there are other fields in the struct
		     */
		    bitsize = BYTE_LENGTH * 
			 NodeSizeof(field, NULL, 
				    (EndOfList(&marker) && cnt > 1));
		    bitalign = NodeAlignment(field, NULL) * BYTE_LENGTH;

		    /* realign field */
		    if (currentbit % bitalign != 0)
			 currentbit = currentbit + bitalign -
			     (currentbit % bitalign);

		    currentbit += bitsize;
	       }

	       if (bitalign > max_bitalign)
		    max_bitalign = bitalign;
	  }

	  /* realign struct */
	  if (currentbit % max_bitalign != 0)
	       currentbit = currentbit + max_bitalign - (currentbit % max_bitalign);

	  sue->size = currentbit / BYTE_LENGTH;
	  sue->align = max_bitalign / BYTE_LENGTH;
     }
}

GLOBAL void UnionCheckFields(SUEtype *sue)
{
     if (sue->visited == FALSE) {
	  int max_align = 0, max_size = 0;
	  ListMarker marker;
	  Node *field;

	  /* To stop infinite recursion */
	  sue->visited = TRUE;

	  /* Loop over the fields of the SDCL */
	  IterateList(&marker, sue->fields);
	  while (NextOnList(&marker, (GenericREF) & field)) {
	       Node *type;
	       int size, align;

	       assert(field->typ == Decl);
	       type = NodeDataType(field->u.decl.type);

	       if (IsStructType(type) ||
		   IsArrayType(type) ||
		   IsUnionType(type)
		   ) {
		    field = SemCheckNode(field);
		    SetCurrentOnList(&marker, field);
	       }
	       size = NodeSizeof(field, NULL, FALSE);
	       align = NodeAlignment(field, NULL);

#if 0
/* offsets and bitlengths not kept in c-to-c -- rcm */
	       field->u.decl.init = SintZero;
	       field->u.decl.bitlength = -1;	/* no bit length */
#endif

	       if (size > max_size)
		    max_size = size;
	       if (align > max_align)
		    max_align = align;
	  }

	  sue->size = max_size;
#ifndef AUTOPILOT
	  sue->align = max_align;
#else
	  /*
	   * for Autopilot, short-align all unions so that
	   * internal structs are never character-aligned
	   */
	  sue->align = (max_align < 2) ? 2 : max_align;
#endif
     }
}

PRIVATE void SemCheckCallArgs_Aux(Node *node, List *formals, List *actuals,
				  Bool apply_conversions)
{
     ListMarker fm, am;
     Node *formal, *actual;
     Node *formaltype, *actualtype;
     int formals_len, actuals_len;
     Bool traditional = (formals == NULL);
     Bool variable_length = traditional || IsEllipsis(LastItem(formals));

     formals_len = IsVoidArglist(formals) ? 0 : ListLength(formals);
     actuals_len = ListLength(actuals);

     if (!variable_length && formals_len != actuals_len) {
	  SyntaxErrorCoord(node->coord,
			"argument mismatch: %d args passed, %d expected",
			   actuals_len, formals_len);
     } else if (!IsVoidArglist(formals)) {
	  IterateList(&am, actuals);
	  if (!traditional)
	       IterateList(&fm, formals);
	  while (NextOnList(&am, (GenericREF) & actual)) {
	       if (!traditional) {
		    NextOnList(&fm, (GenericREF) & formal);
		    if (IsEllipsis(formal))
			 traditional = TRUE;
	       }
	       
	       if (apply_conversions) {
		    if (traditional)
			 actual = UsualUnaryConversions(actual, TRUE);
		    else {
			 formaltype = NodeDataType(formal);
			 actual = CallConversions(actual, formaltype);
		    }
		    SetCurrentOnList(&am, actual);
	       } else {
		    /*
		     * do not apply conversions, and check that
		     * types are the same
		     */
		    formaltype = NodeDataType(formal);
		    actualtype = NodeDataType(actual);
		    if (!TypeEqual(formaltype, actualtype))
			 SyntaxErrorCoord(actual->coord,
					  "argument type mismatch in inlet call");
	       }
	  }
     }
}

PRIVATE void SemCheckCallArgs(Node *node, List *formals, List *actuals)
{
     SemCheckCallArgs_Aux(node, formals, actuals, TRUE);
}

PRIVATE void SemCheckInletArgs(Node *node, List *formals, List *actuals)
{
     SemCheckCallArgs_Aux(node, formals, actuals, FALSE);
}

/***************************************************************************/
/*                    B I N A R Y    O P E R A T O R S                     */
/***************************************************************************/

/* Binop assignment */
PRIVATE Node *SemCheckAssignment(Node *node, binopNode *u)
{
     Node *left, *right, *ltype, *rtype;
     OpType opcode;

     assert(node);
     left = u->left;
     right = u->right;
     assert(left);
     assert(right);
     ltype = NodeDataType(left);
     rtype = NodeDataType(right);
     assert(ltype);
     assert(rtype);
     opcode = u->op;

     assert(left);
     assert(right);
     assert(ltype);
     assert(rtype);

     /* The left hand side must be a modifiable lvalue */
     if (!IsModifiableLvalue(left))
	  SyntaxErrorCoord(node->coord,
			 "left operand must be modifiable lvalue: op %s",
			   OperatorText(opcode));

     /*
      * assignment conversions don't work for ptr+=int or ptr-=int,
      * so treat them as special cases 
      */
     if ((opcode == PLUSassign || opcode == MINUSassign) &&
	 IsPointerType(ltype) &&
	 IsIntegralType(UsualUnaryConversionType(rtype)))
	  /* make sure we can calculate size of base type */
	  NodeSizeof(node, GetDeepBaseType(ltype), FALSE);
     else
	  u->right = AssignmentConversions(right, ltype);

     /* The type of the expression is the type of the LHS */
     u->type = ltype;

     return node;
}

PRIVATE Node *SemCheckDot(Node *node, binopNode *u)
{
     Node *field = NULL, *left, *right, *ltype, *rtype, *type;

     assert(node);

     left = u->left;
     right = u->right;
     assert(left);
     assert(right);
     ltype = NodeDataType(left);
     rtype = NodeDataType(right);
     assert(ltype);
     type = NodeDataType(ltype);
     assert(type);

     /* The lhs must be a struct or union */
     if (!(type->typ == Sdcl || type->typ == Udcl)) {
	  SyntaxErrorCoord(node->coord,
		  "left operand of \".\" must be a struct/union object");
	  u->type = type;
	  return node;
     }
     field = SdclFindField(type, right);

     /* Check that the field is valid */
     if (field == NULL) {
	  SyntaxErrorCoord(node->coord,
			   "undefined struct/union member: \"%s\"",
			   rtype->u.id.text);
	  u->type = type;
	  return node;
     }
     assert(field->u.decl.type);
     u->type = NodeDataType(field->u.decl.type);
     return node;
}

PRIVATE Node *SemCheckArrayLabeledInitializer(Node *node, binopNode *u) {
    /* Really should check that everything is a constant...*/
    Node *left,*right,*ltype,*rtype, *tmptype;
    assert(node);

    left  = u->left;
    right = u->right;

    assert(left);
    assert(right);

    ltype = NodeDataType(left);
    rtype = NodeDataType(right);

    assert(ltype);
    assert(rtype);
    
    tmptype = NodeDataType(rtype);
    assert(tmptype);

    u->type = rtype;
    return node;
}


PRIVATE Node *SemCheckDesignatedInitializer(Node *node, binopNode *u) {
    Node *left,*right,*ltype,*rtype, *tmptype;
    assert(node);

    left  = u->left;   /* this should be an identifier. */
    right = u->right;

    assert(left);
    assert(right);

    ltype = NodeDataType(left);
    rtype = NodeDataType(right);

    assert(ltype);
    assert(rtype);
    
    tmptype = NodeDataType(rtype);
    assert(tmptype);

#if 0
    printf("u->type was:\n");
    PrintNode(stdout, u->type, 0);
    printf("\nu->type becomes:\n");
    PrintNode(stdout, rtype, 0);
#endif
    u->type = rtype;
    return node;
}

PRIVATE Node *SemCheckArrow(Node *node, binopNode *u)
{
     ListMarker marker;
     Node *decl, *field = NULL, *right, *left, *ltype, *rtype, *type;

     assert(node);
     right = u->right;
     /* perform UnaryConversions on left node, to ensure that arrays
      * are converted to pointers */
     left = UsualUnaryConversions(u->left, FALSE);

     assert(right);
     assert(left);
     ltype = NodeDataType(left);
     rtype = NodeDataType(right);
     /*printf("%s:%d ltype is\n", __FILE__, __LINE__); DPN(ltype);*/

     assert(ltype);
     assert(rtype);
     type = NodeDataType(ltype);
     /*printf("%s:%d type is\n", __FILE__, __LINE__); DPN(type);*/
     assert(type);

     /* The lhs must be a pointer to a struct or union */
     if (ltype->typ != Ptr) {
	  SyntaxErrorCoord(node->coord,
	    "left operand of \"%s\" must be a pointer to a struct/union",
			   OperatorText(u->op));
	  u->type = ltype;
	  return node;
     }
     assert(ltype->u.ptr.type);
     /*printf("%s:%d ptr.type was\n", __FILE__, __LINE__); DPN(ltype->u.ptr.type);*/
     type = NodeDataType(ltype->u.ptr.type);
     /*printf("%s:%d type is\n", __FILE__, __LINE__); DPN(type);*/

     if (!(type->typ == Sdcl || type->typ == Udcl)) {
	  SyntaxErrorCoord(node->coord,
		  "left operand of \"%s\" must be a struct/union object",
			   OperatorText(u->op));
	  u->type = type;
	  return node;
     }
     /* Find the field in the struct/union fields */
     field = SdclFindField(type, right);

     /* Check that the field is valid */
     if (field == NULL) {
	  SyntaxErrorCoord(node->coord,
			   "undefined struct/union member: \"%s\"",
			   rtype->u.id.text);
	  u->type = type;
	  return node;
     }
     assert(field->u.decl.type);
     /* if we have const on the typedef, then we should get const on the field.*/
     /* Hence we merge the type qualfiiers into the result type.*/
     u->type = MakeMergedType(NodeDataType(field->u.decl.type),
			      ltype->u.ptr.type);
     /*printf("Merged type is\n");  DPN(u->type);*/
     return node;
}

PRIVATE Node *SemCheckArithmetic(Node *node, binopNode *u)
{
     Node *left, *right, *ltype, *rtype;
     OpType opcode;

     assert(node);

     left = UsualUnaryConversions(u->left, FALSE),
	 right = UsualUnaryConversions(u->right, FALSE),
	 opcode = u->op;

     assert(left);
     assert(right);

     switch (opcode) {
	 case LS:
	 case RS:
	      break;
	 default:
	      UsualBinaryConversions(&left, &right);
     }

     assert(left);
     assert(right);

     ltype = NodeDataType(left);
     rtype = NodeDataType(right);

     if (ltype == NULL) {
	  fPrintNode(stdout, left, 0);
	  printf("\n");
     }
     assert(left);
     assert(right);
     assert(ltype);
     assert(rtype);

     switch (opcode) {
	 case '+':
	      /* Canonicalize PTR + INT expressions */
	      if (IsIntegralType(ltype) && IsPointerType(rtype)) {
		   Node *tnode = left, *ttype = ltype;

		   left = right;
		   ltype = rtype;
		   right = tnode;
		   rtype = ttype;
	      }
	      if (!((IsArithmeticType(ltype) && IsArithmeticType(rtype)) ||
		    (IsPointerType(ltype) && IsIntegralType(rtype))))
		   SyntaxErrorCoord(node->coord,
				    "operands must have arithmetic type or ptr/int: op \"+\"");
	      u->type = ltype;

	      if (IsPointerType(ltype)) {
		   /* try to get size of base type, to ensure that it
		    * is defined (and not void or an incomplete
		    * struct/union type) */
		   (void) NodeSizeof(left, GetShallowBaseType(ltype), FALSE);
	      }
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval + rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval + rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval + rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval + rval);
		   } else if (NodeTypeIsFloat(left)) {
			float lval = NodeConstantFloatValue(left), rval = NodeConstantFloatValue(right);

			NodeSetFloatValue(node, lval + rval);
		   } else if (NodeTypeIsDouble(left)) {
			double lval = NodeConstantDoubleValue(left),
			 rval = NodeConstantDoubleValue(right);

			NodeSetDoubleValue(node, lval + rval);
		   }
	      }
	      break;

	 case '-':
	      if (!((IsArithmeticType(ltype) && IsArithmeticType(rtype)) ||
		    (IsPointerType(ltype) && IsIntegralType(rtype)) ||
		    (IsPointerType(ltype) && IsPointerType(rtype)))) {
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"-\"");
	      }
	      if (ltype->typ == Ptr && rtype->typ == Ptr)
		   u->type = PrimSint;	/* fix: should really be ptrdiff_t */
	      else
		   u->type = ltype;

	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval - rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval - rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval - rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval - rval);
		   } else if (NodeTypeIsFloat(left)) {
			float lval = NodeConstantFloatValue(left), rval = NodeConstantFloatValue(right);

			NodeSetFloatValue(node, lval - rval);
		   } else if (NodeTypeIsDouble(left)) {
			double lval = NodeConstantDoubleValue(left),
			 rval = NodeConstantDoubleValue(right);

			NodeSetDoubleValue(node, lval - rval);
		   }
	      }
	      break;

	 case '*':
	      if (!(IsArithmeticType(ltype) && IsArithmeticType(rtype)))
		   SyntaxErrorCoord(node->coord,
			 "operands must have arithmetic type: op \"%s\"",
				    Operator[opcode].text);

	      u->type = ltype;

	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval * rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval * rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval * rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval * rval);
		   } else if (NodeTypeIsFloat(left)) {
			float lval = NodeConstantFloatValue(left), rval = NodeConstantFloatValue(right);

			NodeSetFloatValue(node, lval * rval);
		   } else if (NodeTypeIsDouble(left)) {
			double lval = NodeConstantDoubleValue(left),
			 rval = NodeConstantDoubleValue(right);

			NodeSetDoubleValue(node, lval * rval);
		   }
	      }
	      break;

	 case '/':
	      if (!(IsArithmeticType(ltype) && IsArithmeticType(rtype)))
		   SyntaxErrorCoord(node->coord,
			 "operands must have arithmetic type: op \"%s\"",
				    Operator[opcode].text);

	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetSintValue(node, lval / rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetUintValue(node, lval / rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetSlongValue(node, lval / rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetUlongValue(node, lval / rval);
		   } else if (NodeTypeIsFloat(left)) {
			float lval = NodeConstantFloatValue(left), rval = NodeConstantFloatValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetFloatValue(node, lval / rval);
		   } else if (NodeTypeIsDouble(left)) {
			double lval = NodeConstantDoubleValue(left),
			 rval = NodeConstantDoubleValue(right);

			if (rval == 0)
			     SyntaxErrorCoord(node->coord,
				      "attempt to divide constant by 0");
			else
			     NodeSetDoubleValue(node, lval / rval);
		   }
	      }
	      u->type = ltype;
	      break;

	 case '%':
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval % rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval % rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval % rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval % rval);
		   }
	      }
	      u->type = ltype;
	      break;
	 case LS:
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   unsigned long rval = NodeConstantIntegralValue(right);

		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left);

			NodeSetSintValue(node, lval << rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left);

			NodeSetUintValue(node, lval << rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left);

			NodeSetSlongValue(node, lval << rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left);

			NodeSetUlongValue(node, lval << rval);
		   }
	      }
	      u->type = ltype;
	      break;
	 case RS:
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   unsigned long rval = NodeConstantIntegralValue(right);

		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left);

			NodeSetSintValue(node, lval >> rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left);

			NodeSetUintValue(node, lval >> rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left);

			NodeSetSlongValue(node, lval >> rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left);

			NodeSetUlongValue(node, lval >> rval);
		   }
	      }
	      u->type = ltype;
	      break;
	 case '&':
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval & rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval & rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval & rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval & rval);
		   }
	      }
	      u->type = ltype;
	      break;
	 case '^':
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval ^ rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval ^ rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval ^ rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval ^ rval);
		   }
	      }
	      u->type = ltype;
	      break;
	 case '|':
	      if (!(IsIntegralType(ltype) && IsIntegralType(rtype)))
		   SyntaxErrorCoord(node->coord,
			   "operands must have integral type: op \"%s\"",
				    Operator[opcode].text);
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   if (NodeTypeIsSint(left)) {
			int lval = NodeConstantSintValue(left), rval = NodeConstantSintValue(right);

			NodeSetSintValue(node, lval | rval);
		   } else if (NodeTypeIsUint(left)) {
			unsigned int lval = NodeConstantUintValue(left),
			 rval = NodeConstantUintValue(right);

			NodeSetUintValue(node, lval | rval);
		   } else if (NodeTypeIsSlong(left)) {
			long lval = NodeConstantSlongValue(left), rval = NodeConstantSlongValue(right);

			NodeSetSlongValue(node, lval | rval);
		   } else if (NodeTypeIsUlong(left)) {
			unsigned long lval = NodeConstantUlongValue(left),
			 rval = NodeConstantUlongValue(right);

			NodeSetUlongValue(node, lval | rval);
		   }
	      }
	      u->type = ltype;
	      break;

	 case ANDAND:
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   int lval = IsConstantZero(left), rval = IsConstantZero(right);

		   NodeSetSintValue(node, !lval && !rval);
	      }
	      u->type = PrimSint;
	      break;
	 case OROR:
	      if (NodeIsConstant(left) && NodeIsConstant(right)) {
		   int lval = IsConstantZero(left), rval = IsConstantZero(right);

		   NodeSetSintValue(node, !lval || !rval);
	      }
	      u->type = PrimSint;
	      break;
	 default:
	      fprintf(stderr, "Internal Error! Unrecognized arithmetic operator\n");
	      assert(FALSE);
     }

     u->left = left;
     u->right = right;
     return node;
}

PRIVATE Node *SemCheckComparison(Node *node, binopNode *u)
{
     Node *left = UsualUnaryConversions(u->left, FALSE), *right = UsualUnaryConversions(u->right, FALSE),
     *ltype, *rtype;
     OpType opcode = u->op;

     assert(left);
     assert(right);

     UsualBinaryConversions(&left, &right);

     assert(left);
     assert(right);

     ltype = NodeDataType(left);
     rtype = NodeDataType(right);

     assert(ltype);
     assert(rtype);

     switch (opcode) {
	 case '<':
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval < rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval < rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval < rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval < rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval < rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval < rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, FALSE);
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;
	 case LE:
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval <= rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval <= rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval <= rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval <= rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval <= rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval <= rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, FALSE);
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;
	 case '>':
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval > rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval > rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval > rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval > rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval > rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval > rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, FALSE);
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;
	 case GE:
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval >= rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval >= rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval >= rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval >= rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval >= rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval >= rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, FALSE);
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;

	 case EQ:
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval == rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval == rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval == rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval == rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval == rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval == rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, TRUE);
	      else if ((IsPointerType(ltype) && IsConstantZero(right)) ||
		       (IsConstantZero(left) && IsPointerType(rtype)));
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;
	 case NE:
	      u->type = PrimSint;

	      if (IsArithmeticType(ltype) && IsArithmeticType(rtype)) {
		   if (NodeIsConstant(left) && NodeIsConstant(right)) {
			if (NodeTypeIsSint(left)) {
			     int lval = NodeConstantSintValue(left),
			      rval = NodeConstantSintValue(right);

			     NodeSetSintValue(node, lval != rval);
			} else if (NodeTypeIsUint(left)) {
			     unsigned int lval = NodeConstantUintValue(left),
			      rval = NodeConstantUintValue(right);

			     NodeSetSintValue(node, lval != rval);
			} else if (NodeTypeIsSlong(left)) {
			     long lval = NodeConstantSlongValue(left),
			      rval = NodeConstantSlongValue(right);

			     NodeSetSintValue(node, lval != rval);
			} else if (NodeTypeIsUlong(left)) {
			     unsigned long lval = NodeConstantUlongValue(left),
			      rval = NodeConstantUlongValue(right);

			     NodeSetSintValue(node, lval != rval);
			} else if (NodeTypeIsFloat(left)) {
			     float lval = NodeConstantFloatValue(left),
			      rval = NodeConstantFloatValue(right);

			     NodeSetSintValue(node, lval != rval);
			} else if (NodeTypeIsDouble(left)) {
			     double lval = NodeConstantDoubleValue(left),
			      rval = NodeConstantDoubleValue(right);

			     NodeSetSintValue(node, lval != rval);
			}
		   }
	      } else if (IsPointerType(ltype) && IsPointerType(rtype))
		   UsualPointerConversions(&left, &right, TRUE);
	      else if ((IsPointerType(ltype) && IsConstantZero(right)) ||
		       (IsConstantZero(left) && IsPointerType(rtype)));
	      else
		   SyntaxErrorCoord(node->coord,
			   "operands have incompatible types: op \"%s\"",
				    OperatorText(opcode));
	      break;
	 default:
	      fprintf(stdout, "Internal Error: Unrecognized comparison operator\n");
	      assert(FALSE);
     }

     u->left = left;
     u->right = right;
     return node;
}

/***************************************************************************/
/*                  S W I T C H   S T A T E M E N T S                      */
/***************************************************************************/

struct SwitchCheck {
     Node *defaultcase;		/* Default node, or NULL if not found yet */
     unsigned tablesize;	/* number of entries in table */
     Node *table[1];		/* Case expr nodes checked so far, hashed by value */
};

#define HASH_FACTOR   2

PRIVATE struct SwitchCheck *NewSwitchCheck(List *cases)
{
     int i;
     int count = ListLength(cases);
     int tablesize = count * HASH_FACTOR;
     struct SwitchCheck *check =
     HeapAllocate(sizeof(struct SwitchCheck) + sizeof(Node *) * (tablesize - 1),
		  1);
     assert(check);

     check->defaultcase = NULL;
     check->tablesize = tablesize;
     for (i = 0; i < tablesize; ++i)
	  check->table[i] = NULL;

     return check;
}

PRIVATE void FreeSwitchCheck(struct SwitchCheck *check)
{
     /* invalidate some fields in check before freeing it */
     check->tablesize = 0;
     HeapFree(check);
}

PRIVATE void SwitchCheckAddCase(struct SwitchCheck *check, Node *expr)
{
     unsigned long val;
     unsigned h;

     assert(check);
     assert(expr);

     val = NodeConstantIntegralValue(expr);

     /* starting at hash value, probe linearly for empty slot or
      * matching value (indicating a redundant case).  Since hash table
      * HASH_FACTOR times larger than the number of cases, this loop will 
      * always eventually find a NULL slot, so it will terminate.  */
     for (h = (unsigned) (val % check->tablesize);
	  check->table[h] != NULL;
	  h = (h + 1) % check->tablesize)
	  if (NodeConstantIntegralValue(check->table[h]) == val) {
	       SyntaxErrorCoord(expr->coord, "duplicate case label");
	       fprintf(stderr, "\tOriginal case: ");
	       PRINT_COORD(stderr, check->table[h]->coord);
	       fputc('\n', stderr);
	       return;
	  }
     check->table[h] = expr;
}

PRIVATE void SwitchCheckAddDefault(struct SwitchCheck *check, Node *node)
{
     assert(check);
     assert(node);

     if (check->defaultcase != NULL)
	  SyntaxErrorCoord(node->coord, "multiple default cases");
     else
	  check->defaultcase = node;
}
