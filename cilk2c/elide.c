/* Cilk elision.  Remove cilk keywords. */
/* Bradley C. Kuszmaul.  Thu Oct 16 2003 */
/* Copyright (c) 2003 Bradley C. Kuszmaul
 * Copyright (c) 2003 Massachusetts Institute of Technology
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
#include <ast.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/elide.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

    
PRIVATE Node *ElideNode(Node *node);

PRIVATE List *ElideList(List *list)
{
     List *aptr;

     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  SetItem(aptr, ElideNode(item));
     }

     return list;
}


PRIVATE inline Node *ElideConst(Node *node, UNUSED(ConstNode *u))
{
     return node;
}

PRIVATE inline Node *ElideId(Node *node, UNUSED(idNode *u))
{
     return node;
}

PRIVATE inline Node *ElideBinop(Node *node, binopNode *u) {
    u->left = ElideNode(u->left);
    u->right = ElideNode(u->right);
    return node;
}

PRIVATE inline Node *ElideUnary(Node *node, unaryNode *u) {
    u->expr = ElideNode(u->expr);
    return node;
}

PRIVATE inline Node *ElideCast(Node *node, castNode *u)
{
     u->type = ElideNode(u->type);
     u->expr = ElideNode(u->expr);

     return node;
}

PRIVATE inline Node *ElideComma(Node *node, commaNode *u)
{
     u->exprs = ElideList(u->exprs);
     return node;
}

PRIVATE inline Node *ElideConstructor(Node *node, constructorNode *u)
{
     u->type  = ElideNode(u->type);
     u->initializerlist = ElideNode(u->initializerlist);
     return node;
}

PRIVATE inline Node *ElideTernary(Node *node, ternaryNode *u)
{
     u->cond = ElideNode(u->cond);
     u->true = ElideNode(u->true);
     u->false = ElideNode(u->false);

     return node;
}

PRIVATE inline Node *ElideArray(Node *node, arrayNode *u)
{
     u->name = ElideNode(u->name);
     u->dims = ElideList(u->dims);

     return node;
}


PRIVATE inline Node *ElideCall(Node *node, callNode *u)
{
     u->name = ElideNode(u->name);
     u->args = ElideList(u->args);

     return node;
}

PRIVATE inline Node *ElideInitializer(Node *node, initializerNode *u)
{
     u->exprs = ElideList(u->exprs);
     return node;
}

PRIVATE inline Node *ElideImplicitCast(Node *node, implicitcastNode *u)
{
     u->expr = ElideNode(u->expr);
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE inline Node *ElideLabel(Node *node, UNUSED(labelNode *u))
{
     return node;
}

PRIVATE inline Node *ElideSwitch(Node *node, SwitchNode *u)
{
     u->expr = ElideNode(u->expr);
     u->stmt = ElideNode(u->stmt);

     return node;
}

PRIVATE inline Node *ElideCase(Node *node, UNUSED(CaseNode *u))
{
     return node;
}

PRIVATE inline Node *ElideDefault(Node *node, UNUSED(DefaultNode *u))
{
     return node;
}

PRIVATE inline Node *ElideIf(Node *node, IfNode *u)
{
     u->expr = ElideNode(u->expr);
     u->stmt = ElideNode(u->stmt);
     return node;
}

PRIVATE inline Node *ElideIfElse(Node *node, IfElseNode *u)
{
     u->expr = ElideNode(u->expr);
     u->true = ElideNode(u->true);
     u->false = ElideNode(u->false);
     return node;
}

PRIVATE inline Node *ElideWhile(Node *node, WhileNode *u)
{
     u->expr = ElideNode(u->expr);
     u->stmt = ElideNode(u->stmt);
     return node;
}

PRIVATE inline Node *ElideDo(Node *node, DoNode *u)
{
     u->expr = ElideNode(u->expr);
     u->stmt = ElideNode(u->stmt);
     return node;
}

PRIVATE inline Node *ElideFor(Node *node, ForNode *u)
{
     u->cond = ElideNode(u->cond);
     u->init = ElideNode(u->init);
     u->next = ElideNode(u->next);
     u->stmt = ElideNode(u->stmt);

     return node;
}

PRIVATE inline Node *ElideGoto(Node *node, UNUSED(GotoNode *u))
{
     return node;
}

PRIVATE inline Node *ElideContinue(Node *node, UNUSED(ContinueNode *u))
{
     return node;
}

PRIVATE inline Node *ElideBreak(Node *node, UNUSED(BreakNode *u))
{
     return node;
}

PRIVATE inline Node *ElideReturn(Node *node, ReturnNode *u)
{
    u->expr = ElideNode(u->expr);
    return node;
}

PRIVATE inline Node *ElideBlock(Node *node, BlockNode *u)
{
     u->decl = ElideList(u->decl);
     u->stmts = ElideList(u->stmts);
     return node;
}

PRIVATE inline Node *ElidePrim(Node *node, UNUSED(primNode *u))
{
     return node;
}

PRIVATE inline Node *ElideTdef(Node *node, UNUSED(tdefNode *u))
{
     return node;
}

PRIVATE inline Node *ElidePtr(Node *node, ptrNode *u)
{
     u->type = ElideNode(u->type);
     return node;
}

PRIVATE inline Node *ElideAdcl(Node *node, adclNode *u)
{
     u->type = ElideNode(u->type);
     u->tq = NodeTq(u->type);
     u->dimp->dim = ElideNode(u->dimp->dim);
     return node;
}

PRIVATE inline Node *ElideFdcl(Node *node, fdclNode *u)
{
     u->args = ElideList(u->args);
     u->returns = ElideNode(u->returns);

     u->tq = tq_remove_procedure(u->tq); /* Get rid of the cilk keyword*/

     return node;
}


PRIVATE void ElideSUE(SUEtype *sue)
{
     if (sue->transformed == FALSE) {
	  ListMarker marker;
	  Node *field;

	  /* To stop infinite recursion */
	  sue->transformed = TRUE;

	  /* Loop over the fields of the SDCL */
	  IterateList(&marker, sue->fields);
	  while (NextOnList(&marker, (GenericREF) & field)) {
	       assert(field->typ == Decl);
	       field->u.decl.type = ElideNode(field->u.decl.type);
	  }
     }
}

PRIVATE inline Node *ElideSdcl(Node *node, sdclNode *u)
{
     ElideSUE(u->type);
     return node;
}

PRIVATE inline Node *ElideUdcl(Node *node, udclNode *u)
{
     ElideSUE(u->type);
     return node;
}

PRIVATE inline Node *ElideEdcl(Node *node, edclNode *u)
{
     ElideSUE(u->type);
     return node;
}

PRIVATE inline Node *ElideAsm(Node *node, asmNode *u)
{
     u->template = ElideNode(u->template);
     u->output = ElideList(u->output);
     u->input = ElideList(u->input);
     u->clobbered = ElideList(u->clobbered);
     return node;
}

PRIVATE inline Node *ElideAsmArg(Node *node, asmargNode *u)
{
     u->constraint = ElideNode(u->constraint);
     u->expr = ElideNode(u->expr);
     return node;
}

PRIVATE inline Node *ElideBuiltinVaArg(Node *node, builtinvaargNode *u)
{
     u->expr = ElideNode(u->expr);
     u->type = ElideNode(u->type);
     return node;
}

PRIVATE inline Node *ElideSpawn(Node *node, spawnNode *u)
{
    u->receiver = ElideNode(u->receiver);
    u->name     = ElideNode(u->name);
    u->args     = ElideList(u->args);
    if (u->receiver == NULL ) {
	/* spawn with no receiver */
	return MakeCallCoord(u->name, u->args, node->coord);
    } else
	return MakeBinopCoord(u->assign_op, u->receiver, MakeCallCoord(u->name, u->args, node->coord), u->receiver->coord);
}

PRIVATE Node *ElideInletCall(Node *node, inletcallNode *u)
{
    u->name     = ElideNode(u->name);
    u->args     = ElideList(u->args);
    return MakeCallCoord(u->name, u->args, node->coord);
}

PRIVATE inline Node *ElideSync(UNUSED(Node *node), UNUSED(syncNode *u))
{
    return NULL;
}

PRIVATE inline Node *ElideAbort(UNUSED(Node *node), UNUSED(abortNode *u))
{
    return NULL;
#if 0
    /* Old broken stuff: */
    MakeTextCoord(UniqueString("/* abort */"), FALSE, node->coord);
#endif
}

PRIVATE inline Node *ElideSynched(UNUSED(Node *node), UNUSED(synchedNode *u))
{
    return MakeConstSint(1);
}
PRIVATE inline Node *ElideXBlock(Node *node, XBlockNode *u)
{
    return MakeBlockCoord(u->type, u->decl, u->stmts, node->coord, u->right_coord);
}


PRIVATE inline Node *ElideDecl(Node *node, declNode *u)
{
     u->type = ElideNode(u->type);
     u->init = ElideNode(u->init);
     u->bitsize = ElideNode(u->bitsize);
     return node;
}

PRIVATE inline Node *ElideAttrib(Node *node, UNUSED(attribNode *u))
{
     return node;
}

PRIVATE inline Node *ElideProc(Node *node, procNode *u)
{
    Node *decl, *fdcl;
    decl = u->decl;
    assert(decl->typ == Decl);
    fdcl = decl->u.decl.type;
    assert(fdcl->typ == Fdcl);
    fdcl->u.fdcl.tq = tq_remove_inlet(tq_remove_procedure(tq_remove_cilk_where_am_i_decl_quals(fdcl->u.fdcl.tq)));
    
    u->decl = ElideNode(u->decl);
    u->body = ElideNode(u->body);

    return node;
}

PRIVATE inline Node *ElideText(Node *node, UNUSED(textNode *u))
{
     return node;
}

PRIVATE Node *ElideNode(Node *node)
{
    if (node == NULL)
	return NULL;
#define CODE(name, node, union) return Elide##name(node, union)
    ASTSWITCH(node, CODE)
#undef CODE
	UNREACHABLE;
    return NULL;
}

PRIVATE List *ElideTopDecl(Node *node) 
{
    List *result;

    if (!node)
	return NULL;
    
    /* Get rid of cilk keyword. */
    if (node->typ == Proc &&
	node->u.proc.decl->u.decl.type->typ == Fdcl) {
	node->u.proc.decl->u.decl.type->u.fdcl.tq = tq_remove_procedure(node->u.proc.decl->u.decl.type->u.fdcl.tq);
    }
    result = List1(ElideNode(node));
    
    return result;
}

GLOBAL List *ElideProgram(List *program)
{
    ListMarker marker;
    Node *node;
    List *newprogram = NULL;
    
    IterateList(&marker, program);
    while (NextOnList(&marker, (GenericREF) & node)) {
	newprogram = JoinLists(newprogram, ElideTopDecl(node));
    }
    
    return newprogram;

}
