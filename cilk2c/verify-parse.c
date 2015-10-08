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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/verify-parse.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


typedef enum verifycontext {
     TopLevel,
     FormalParm,
     StructField,
     Other
} Context;

PRIVATE void VerifyNode(Node *node, Context c);
PRIVATE void VerifyType(Node *type);
PRIVATE void VerifyDeclList(List *list, Context c);
PRIVATE void VerifyExpr(Node *node);
PRIVATE void VerifyExprList(List *list);
PRIVATE void VerifyTypeOrExpr(Node *node);
PRIVATE void VerifyStmt(Node *node);
PRIVATE void VerifyStmtList(List *list);
PRIVATE void VerifyTq(TypeQual tq);
PRIVATE void VerifyScAndDq(TypeQual tq);

GLOBAL void VerifyParse(List *program)
{
     /* a program is a list of declarations and proc definitions */
     VerifyDeclList(program, TopLevel);
}

/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifyConst(UNUSED(Node *node), ConstNode *u, UNUSED(Context c))
{
     VerifyType(u->type);
}

PRIVATE inline void VerifyId(UNUSED(Node *node), idNode *u, UNUSED(Context c))
{
     Node *var;

     assert(u->text != NULL);
     var = u->decl;
     /* var can be NULL for struct/union field names, but VerifyExpr
      * is not called for the right side of -> and . */
     assert(var != NULL);
     assert(var->typ == Decl);
     /* name in decl must match id name */
     assert(var->u.decl.name == u->text);
}

PRIVATE inline void VerifyBinop(UNUSED(Node *node), binopNode *u, UNUSED(Context c))
{
     /* fix: check op */
     if (u->op == ARROW || u->op == '.') {
	  VerifyExpr(u->left);
	  assert(u->right != NULL);
	  assert(u->right->typ == Id);
	  assert(u->right->u.id.decl == NULL);
     } else if (u->op == DESIGNATED_INITIALIZER || u->op==':') {
	  VerifyExpr(u->right);
	  assert(u->left != NULL);
	  assert(u->left->typ == Id);
	  assert(u->left->u.id.decl == NULL);
     } else {
	  VerifyExpr(u->left);
	  VerifyExpr(u->right);
     }
}

PRIVATE inline void VerifyUnary(UNUSED(Node *node), unaryNode *u, UNUSED(Context c))
{
     /* fix: check op */
     if (u->op == SIZEOF || u->op == ALIGNOF)
	  VerifyTypeOrExpr(u->expr);
     else
	  VerifyExpr(u->expr);
}

PRIVATE inline void VerifyCast(UNUSED(Node *node), castNode *u, UNUSED(Context c))
{
     VerifyType(u->type);
     VerifyExpr(u->expr);
}

PRIVATE inline void VerifyComma(UNUSED(Node *node), commaNode *u, UNUSED(Context c))
{
     assert(u->exprs != NULL);
     VerifyExprList(u->exprs);
}

PRIVATE inline void VerifyConstructor(UNUSED(Node *node), constructorNode *u, UNUSED(Context c))
{
     assert(u->initializerlist);
     assert(u->initializerlist->typ==Initializer);
     VerifyType(u->type);
     VerifyExpr(u->initializerlist);
}

PRIVATE inline void VerifyTernary(UNUSED(Node *node), ternaryNode *u, UNUSED(Context c))
{
     VerifyExpr(u->cond);
     if (u->true) VerifyExpr(u->true); /* Allow a null expression*/
     VerifyExpr(u->false);
}

PRIVATE inline void VerifyArray(UNUSED(Node *node), arrayNode *u, UNUSED(Context c))
{
     VerifyExpr(u->name);
     VerifyExprList(u->dims);
}

PRIVATE inline void VerifyCall(UNUSED(Node *node), callNode *u, UNUSED(Context c))
{
     VerifyExpr(u->name);
     /* u->args is NULL if there are no arguments */
     VerifyExprList(u->args);
}

PRIVATE inline void VerifyInitializer(UNUSED(Node *node), initializerNode *u, UNUSED(Context c))
{
     /* This list could be empty. */
     VerifyExprList(u->exprs);
}

PRIVATE inline void VerifyImplicitCast(UNUSED(Node *node), implicitcastNode *u, UNUSED(Context c))
{
     if (u->expr != NULL)
	  VerifyExpr(u->expr);
     if (u->type != NULL)
	  VerifyType(u->type);
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifyLabel(UNUSED(Node *node), labelNode *u, UNUSED(Context c))
{
     assert(u->name != NULL);
}

PRIVATE inline void VerifySwitch(UNUSED(Node *node), SwitchNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     if (u->stmt)
	  VerifyStmt(u->stmt);
}

PRIVATE inline void VerifyCase(UNUSED(Node *node), CaseNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
}

PRIVATE inline void VerifyDefault(UNUSED(Node *node), UNUSED(DefaultNode *u), UNUSED(Context c))
{
}

PRIVATE inline void VerifyIf(UNUSED(Node *node), IfNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     /* `true' field may be NULL, e.g.,  if (1) {} */
     if (u->stmt)
	  VerifyStmt(u->stmt);
}

PRIVATE inline void VerifyIfElse(UNUSED(Node *node), IfElseNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     /* `true' field may be NULL, e.g.,  if (1) {} */
     if (u->true)
	  VerifyStmt(u->true);
     /* `false' field may be NULL */
     if (u->false)
	  VerifyStmt(u->false);
}

PRIVATE inline void VerifyWhile(UNUSED(Node *node), WhileNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     if (u->stmt)
	  VerifyStmt(u->stmt);
}

PRIVATE inline void VerifyDo(UNUSED(Node *node), DoNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     if (u->stmt)
	  VerifyStmt(u->stmt);
}

PRIVATE inline void VerifyFor(UNUSED(Node *node), ForNode *u, UNUSED(Context c))
{
     /* any field may be NULL */
     if (u->init)
	  VerifyExpr(u->init);
     if (u->cond)
	  VerifyExpr(u->cond);
     if (u->next)
	  VerifyExpr(u->next);
     if (u->stmt)
	  VerifyStmt(u->stmt);
}

PRIVATE inline void VerifyGoto(UNUSED(Node *node), GotoNode *u, UNUSED(Context c))
{
     Node *var;

     var = u->label;
     assert(var != NULL);
     assert(var->typ == Label);
}

PRIVATE inline void VerifyContinue(UNUSED(Node *node), UNUSED(ContinueNode *u), UNUSED(Context c))
{
}

PRIVATE inline void VerifyBreak(UNUSED(Node *node), UNUSED(BreakNode *u), UNUSED(Context c))
{
}

PRIVATE inline void VerifyReturn(UNUSED(Node *node), ReturnNode *u, UNUSED(Context c))
{
     /* expr may be NULL */
     if (u->expr)
	  VerifyExpr(u->expr);
     assert(u->proc);
     assert(u->proc->typ == Proc);
     assert(u->proc->typ == Proc);
}

PRIVATE inline void VerifyBlock(UNUSED(Node *node), BlockNode *u, UNUSED(Context c))
{
     VerifyDeclList(u->decl, Other);
     VerifyStmtList(u->stmts);
}

/*************************************************************************/
/*                                                                       */
/*                             Type nodes                                */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifyPrim(UNUSED(Node *node), primNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     assert(u->basic > 0 && u->basic < MaxBasicType && u->basic != Int_ParseOnly);
}

PRIVATE inline void VerifyTdef(UNUSED(Node *node), tdefNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     assert(u->name != NULL);
     VerifyType(u->type);
}

PRIVATE inline void VerifyPtr(UNUSED(Node *node), ptrNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     VerifyType(u->type);
}

PRIVATE inline void VerifyAdcl(UNUSED(Node *node), adclNode *u, UNUSED(Context c))
{
     assert(!tq_has_anything(u->tq));
     VerifyType(u->type);
     if (u->dimp->dim)
	  VerifyExpr(u->dimp->dim);	/* can be null */
}

PRIVATE inline void VerifyFdcl(UNUSED(Node *node), fdclNode *u, UNUSED(Context c))
{
     /* allow inlet --- commented by fengmd */
     assert(!tq_has_anything(u->tq) || tq_equal(u->tq, TQ_INLINE) || tq_equal(u->tq, TQ_PROCEDURE) ||
	    tq_equal(u->tq, tq_add_inline(TQ_PROCEDURE)) || tq_equal(u->tq, TQ_INLET) ||
	    tq_equal(u->tq, tq_add_inline(TQ_INLET)));
     VerifyType(u->returns);
     VerifyDeclList(u->args, FormalParm);
}

PRIVATE inline void VerifySdcl(UNUSED(Node *node), sdclNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     assert(u->type != NULL);
     assert(u->type->typ == Sdcl);
     /* u->type->name could be NULL */

     if (tq_has_sue_elaborated(u->tq)) {
	  VerifyDeclList(u->type->fields, StructField);
     }
}

PRIVATE inline void VerifyUdcl(UNUSED(Node *node), udclNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     assert(u->type != NULL);
     assert(u->type->typ == Udcl);
     /* u->type->name could be NULL */

     if (tq_has_sue_elaborated(u->tq)) {
	  VerifyDeclList(u->type->fields, StructField);
     }
}

PRIVATE inline void VerifyEdcl(UNUSED(Node *node), edclNode *u, UNUSED(Context c))
{
     VerifyTq(u->tq);
     assert(u->type != NULL);
     /* fix: verify list? */
#if 0
     /* unlike struct/union, there are no empty enum declarations */
     fprintf(stderr, "unexpected node type %d\n", decl->typ);
     assert(FALSE);
#endif
}

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifyDecl(UNUSED(Node *node), declNode *u, Context c)
{
     if (c != StructField)
	  assert(u->name != NULL);
     VerifyScAndDq(u->tq);
     VerifyType(u->type);
     if (u->init != NULL) {
	  VerifyExpr(u->init);
	  assert(u->bitsize == NULL);
     }
     if (u->bitsize != NULL) {
	  VerifyExpr(u->bitsize);
	  assert(u->init == NULL);
     }
}

PRIVATE inline void VerifyAttrib(UNUSED(Node *node), UNUSED(attribNode *u), UNUSED(Context c))
{
}

PRIVATE inline void VerifyProc(UNUSED(Node *node), procNode *u, UNUSED(Context c))
{
     assert(u->decl != NULL);
     assert(u->decl->typ == Decl);
     assert(u->decl->u.decl.type != NULL);
     assert(u->decl->u.decl.type->typ == Fdcl);
     VerifyNode(u->decl, Other);

     /* body must be a Block, but could be NULL */
     if (u->body != NULL) {
	  assert(u->body->typ == Block);
	  VerifyStmt(u->body);
     }
}

PRIVATE inline void VerifyText(UNUSED(Node *node), UNUSED(textNode *u), UNUSED(Context c))
{
     /* anything goes */
}

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifyAsmArg(UNUSED(Node *node), asmargNode *u, UNUSED(Context c))
{
     VerifyExpr(u->constraint);
     VerifyExpr(u->expr);
}

PRIVATE void VerifyAsmArgList(List *list)
{
     ListMarker marker;
     Node *item;

     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & item)) {
	  assert(item);
	  assert(item->typ == AsmArg);
	  VerifyAsmArg(item, &item->u.AsmArg, Other);
     }
}

PRIVATE inline void VerifyAsm(UNUSED(Node *node), asmNode *u, UNUSED(Context c))
{
     VerifyExpr(u->template);
     if (u->output)
	  VerifyAsmArgList(u->output);
     if (u->input)
	  VerifyAsmArgList(u->input);
     if (u->clobbered)
	  VerifyExprList(u->clobbered);
}

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
PRIVATE inline void VerifyBuiltinVaArg(UNUSED(Node *node), builtinvaargNode *u, UNUSED(Context c))
{
     VerifyExpr(u->expr);
     VerifyType(u->type);
}

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void VerifySpawn(UNUSED(Node *node), spawnNode *u, UNUSED(Context c))
{
     if (u->receiver)
	  VerifyExpr(u->receiver);

     VerifyExpr(u->name);
     VerifyExprList(u->args);
}

PRIVATE inline void VerifySync(UNUSED(Node *node), UNUSED(syncNode *u), UNUSED(Context c))
{
}

#if 0
PRIVATE inline void VerifyInlet(Node *node, inletNode * u, Context c)
{
     Node *type;
     assert(u->decl != NULL);
     assert(u->decl->typ == Decl);
     type = u->decl->u.decl.type;
     assert(type != NULL);
     assert(type->typ == Fdcl);
     VerifyNode(u->decl, Other);

     /* body must be a Block, but could be NULL */
     if (u->body != NULL) {
	  assert(u->body->typ == Block);
	  VerifyStmt(u->body);
     }
}
#endif

PRIVATE void VerifyInletCallArg(Node *node)
{
     assert(node);
     assert(IsExpr(node) || (node->typ == Spawn));
     VerifyNode(node, Other);
}

PRIVATE inline void VerifyInletCall(UNUSED(Node *node), inletcallNode *u, UNUSED(Context c))
{
     ListMarker marker;
     Node *item;

     VerifyExpr(u->name);
     assert(u->name->typ == Id);

     IterateList(&marker, u->args);
     while (NextOnList(&marker, (GenericREF) & item)) {
	  VerifyInletCallArg(item);
     }
}


PRIVATE inline void VerifyAbort(UNUSED(Node *node), UNUSED(abortNode *u), UNUSED(Context c))
{
}

PRIVATE inline void VerifySynched(UNUSED(Node *node), UNUSED(synchedNode *u), UNUSED(Context c))
{
}


PRIVATE inline void VerifyXBlock(UNUSED(Node *node), XBlockNode *u, UNUSED(Context c))
{
     VerifyDeclList(u->decl, Other);
     VerifyStmtList(u->stmts);
}

/*************************************************************************/
/*                                                                       */
/*                        Verify et al.                              */
/*                                                                       */
/*************************************************************************/

PRIVATE void VerifyNode(Node *node, Context c)
{
     assert(node);

#define CODE(name, node, union) Verify##name(node, union, c);
     ASTSWITCH(node, CODE)
#undef CODE
}

PRIVATE void VerifyTq(TypeQual tq)
{
     assert(tq_equal(tq, tq_type_quals(tq)));
}

PRIVATE void VerifyScAndDq(TypeQual tq)
{
     assert(!tq_has_some_storageclass(tq) || tq_has_typedef(tq) || tq_has_extern(tq)
	    || tq_has_static(tq) || tq_has_auto(tq) || tq_has_register(tq));
     assert(tq_has_top_decl(tq) || tq_has_block_decl(tq) || tq_has_formal_decl(tq) ||
	    tq_has_su_decl(tq) || tq_has_enum_decl(tq));
     assert(!tq_has_anything(tq_type_quals(tq)));
}

PRIVATE void VerifyType(Node *type)
{
     assert(type);
     assert(IsType(type));
     VerifyNode(type, Other);
}

PRIVATE void VerifyDeclList(List *list, Context c)
{
     ListMarker marker;
     Node *decl;

     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & decl)) {
	  assert(decl);

	  switch (c) {
	      case TopLevel:
		   assert(IsDecl(decl));
		   break;

	      case FormalParm:
		   assert(decl->typ == Decl ||
			  IsType(decl));
		   break;

	      default:
		   /* allow local proc decls!!! KHR */
		   /* assert(IsDecl(decl) && decl->typ != Proc); */
		   break;
	  }

	  VerifyNode(decl, c);
     }
}

PRIVATE void VerifyExpr(Node *node)
{
     assert(node);
     assert(IsExpr(node));
     VerifyNode(node, Other);
}

PRIVATE void VerifyExprList(List *list)
{
     ListMarker marker;
     Node *item;

     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & item))
	  VerifyExpr(item);
}

PRIVATE void VerifyTypeOrExpr(Node *node)
{
     assert(node);
     assert(IsType(node) || IsExpr(node));
     VerifyNode(node, Other);
}

PRIVATE void VerifyStmt(Node *node)
{
     assert(node);
     assert(IsStmt(node));
     VerifyNode(node, Other);
}

PRIVATE void VerifyStmtList(List *list)
{
     ListMarker marker;
     Node *item;

     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & item))
	  if (item)		/* NULL statements are legal */
	       VerifyStmt(item);
}
