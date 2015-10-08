/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Author: Matteo Frigo
 * 
 *************************************************************************/
/*
 * Copyright (c) 2000-2002 Massachusetts Institute of Technology
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

/*
 * In the long run, this file will print information that can be
 * useful when parallelizing a program. There is not much yet,
 * however.  We only warn against declaration and use of global
 * variables, unless they are declared __cilksafe__.  The idea
 * is that, if you are parallelizing an existing Cilk program,
 * removing global variables is likely to be a good place to
 * start.
 */
#include "ast.h"

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/hint.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


PRIVATE Node *HintNode(Node *node);
PRIVATE List *HintList(List *list);
PRIVATE Bool IsTroublesomeDeclaration(Node *node);

GLOBAL List *HintProgram(List *program)
{
     HintList(program);
     return program;
}

/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

PRIVATE inline Node *HintConst(Node *node, ConstNode *u)
{
     u->type = HintNode(u->type);
     return node;
}

PRIVATE inline Node *HintId(Node *node, idNode *u)
{
     Node *decl = u->decl;

     if (decl) { /* can be null for SUE */
	  if (IsTroublesomeDeclaration(decl)) {
	       HintCoord(node->coord,
			 "use of global variable `%s'", u->text);
	  }
     }
     
     return node;
}

PRIVATE inline Node *HintBinop(Node *node, binopNode *u)
{
     u->left = HintNode(u->left);
     u->right = HintNode(u->right);
     return node;
}

PRIVATE inline Node *HintUnary(Node *node, unaryNode *u)
{
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintCast(Node *node, castNode *u)
{
     u->type = HintNode(u->type);
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintComma(Node *node, commaNode *u)
{
     u->exprs = HintList(u->exprs);
     return node;
}


PRIVATE inline Node *HintConstructor(Node *node, constructorNode *u)
{
     u->type = HintNode(u->type);
     u->initializerlist = HintNode(u->initializerlist);
     return node;
}

PRIVATE inline Node *HintTernary(Node *node, ternaryNode *u)
{
     u->cond = HintNode(u->cond);
     u->true = HintNode(u->true);
     u->false = HintNode(u->false);
     return node;
}

PRIVATE inline Node *HintArray(Node *node, arrayNode *u)
{
     u->name = HintNode(u->name);
     u->dims = HintList(u->dims);
     return node;
}

PRIVATE inline Node *HintCall(Node *node, callNode *u)
{
     u->name = HintNode(u->name);
     u->args = HintList(u->args);
     return node;
}

PRIVATE inline Node *HintInitializer(Node *node, initializerNode *u)
{
     u->exprs = HintList(u->exprs);
     return node;
}

PRIVATE inline Node *HintImplicitCast(Node *node, implicitcastNode *u)
{
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintLabel(Node *node, UNUSED(labelNode *u))
{
     return node;
}

PRIVATE inline Node *HintSwitch(Node *node, SwitchNode *u)
{
     u->expr = HintNode(u->expr);
     u->stmt = HintNode(u->stmt);
     return node;
}

PRIVATE inline Node *HintCase(Node *node, CaseNode *u)
{
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintDefault(Node *node, UNUSED(DefaultNode *u))
{
     return node;
}

PRIVATE inline Node *HintIf(Node *node, IfNode *u)
{
     u->expr = HintNode(u->expr);
     u->stmt = HintNode(u->stmt);
     return node;
}

PRIVATE inline Node *HintIfElse(Node *node, IfElseNode *u)
{
     u->expr = HintNode(u->expr);
     u->true = HintNode(u->true);
     u->false = HintNode(u->false);
     return node;
}

PRIVATE inline Node *HintWhile(Node *node, WhileNode *u)
{
     u->expr = HintNode(u->expr);
     u->stmt = HintNode(u->stmt);
     return node;
}

PRIVATE inline Node *HintDo(Node *node, DoNode *u)
{
     u->stmt = HintNode(u->stmt);
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintFor(Node *node, ForNode *u)
{
     u->init = HintNode(u->init);
     u->cond = HintNode(u->cond);
     u->next = HintNode(u->next);
     u->stmt = HintNode(u->stmt);
     return node;
}

PRIVATE inline Node *HintGoto(Node *node, UNUSED(GotoNode *u))
{
     return node;
}

PRIVATE inline Node *HintContinue(Node *node, UNUSED(ContinueNode *u))
{
     return node;
}

PRIVATE inline Node *HintBreak(Node *node, UNUSED(BreakNode *u))
{
     return node;
}

PRIVATE inline Node *HintReturn(Node *node, ReturnNode *u)
{
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintBlock(Node *node, BlockNode *u)
{
     u->decl = HintList(u->decl);
     u->stmts = HintList(u->stmts);
     return node;
}


PRIVATE inline Node *HintXBlock(Node *node, XBlockNode *u)
{
     u->decl = HintList(u->decl);
     u->stmts = HintList(u->stmts);
     return node;
}


PRIVATE inline Node *HintPrim(Node *node, UNUSED(primNode *u))
{
     return node;
}

PRIVATE inline Node *HintTdef(Node *node, UNUSED(tdefNode *u))
{
     return node;
}

PRIVATE inline Node *HintPtr(Node *node, ptrNode *u)
{
     u->type = HintNode(u->type);
     return node;
}

PRIVATE inline Node *HintAdcl(Node *node, adclNode *u)
{
     u->type = HintNode(u->type);
     return node;
}

PRIVATE inline Node *HintFdcl(Node *node, fdclNode *u)
{
     u->args = HintList(u->args);
     u->returns = HintNode(u->returns);
     return node;
}

PRIVATE inline Node *HintSdcl(Node *node, UNUSED(sdclNode *u))
{
     return node;
}

PRIVATE inline Node *HintUdcl(Node *node, UNUSED(udclNode *u))
{
     return node;
}

PRIVATE inline Node *HintEdcl(Node *node, UNUSED(edclNode *u))
{
     return node;
}

PRIVATE inline Node *HintAsm(Node *node, asmNode *u)
{
     u->template = HintNode(u->template);
     u->output = HintList(u->output);
     u->input = HintList(u->input);
     u->clobbered = HintList(u->clobbered);
     return node;
}

PRIVATE inline Node *HintAsmArg(Node *node, asmargNode *u)
{
     u->constraint = HintNode(u->constraint);
     u->expr = HintNode(u->expr);
     return node;
}

PRIVATE inline Node *HintBuiltinVaArg(Node *node, builtinvaargNode *u)
{
     u->expr = HintNode(u->expr);
     u->type = HintNode(u->type);
     return node;
}


PRIVATE inline Node *HintSpawn(Node *node, spawnNode *u)
{
     u->receiver = HintNode(u->receiver);
     u->name = HintNode(u->name);
     u->args = HintList(u->args);
     return node;
}

PRIVATE inline Node *HintSync(Node *node, UNUSED(syncNode *u))
{
     return node;
}

PRIVATE inline Node *HintInletCall(Node *node, inletcallNode *u)
{
     u->name = HintNode(u->name);
     u->args = HintList(u->args);
     return node;
}

PRIVATE inline Node *HintAbort(Node *node, UNUSED(abortNode *u))
{
     return node;
}

PRIVATE inline Node *HintSynched(Node *node, UNUSED(synchedNode *u))
{
     return node;
}

PRIVATE inline Node *HintDecl(Node *node, declNode *u)
{
     u->type = HintNode(u->type);
     u->init = HintNode(u->init);
     u->bitsize = HintNode(u->bitsize);
     u->asmdecl = HintNode(u->asmdecl);

     /* global variables are a bad idea, especially in Cilk */
     if (!tq_has_extern(NodeStorageClass(node)) &&
	 IsTroublesomeDeclaration(node)) {
	  HintCoord(node->coord,
		    "declaration of global variable `%s'", u->name);
     }
     return node;
}

PRIVATE inline Node *HintAttrib(Node *node, attribNode *u)
{
     u->arglist = HintList(u->arglist);
     return node;
}

PRIVATE inline Node *HintProc(Node *node, procNode *u)
{
     u->decl = HintNode(u->decl);
     u->body = HintNode(u->body);
     return node;
}

PRIVATE inline Node *HintText(Node *node, UNUSED(textNode *u))
{
     return node;
}

/*************************************************************************/
/*                                                                       */
/*                  HintNode and HintList                                */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *HintNode(Node *node)
{
     if (node == NULL)
	  return node;

#define CODE(name, node, union) return Hint##name(node, union)
     ASTSWITCH(node, CODE)
#undef CODE

	 UNREACHABLE;
     return NULL;
}

PRIVATE List *HintList(List *list)
{
     List *aptr;

     for (aptr = list; aptr; aptr = Rest(aptr)) {
	  Node *item = FirstItem(aptr);

	  if (item)
	       SetItem(aptr, HintNode(item));
     }

     return list;
}

/*************************************************************
 * Ancillary functions
 *************************************************************/
PRIVATE Bool IsTroublesomeDeclaration(Node *node)
{
     Node *type;
     assert(node);
     assert(node->typ = Decl);

     type = NodeDataTypeSuperior(node);
     return ((tq_has_top_decl(NodeDeclLocation(node))
	      || tq_has_static(NodeTq(node))) 
	     &&
	     !tq_has_cilksafe(NodeTq(node)) &&
	     !(NodeIsConstQual(type)) &&
	     !(NodeIsConstQual(GetShallowBaseType(type))) &&
	     !DeclIsTypedef(node) && 
	     (node->u.decl.type->typ != Fdcl));
}
