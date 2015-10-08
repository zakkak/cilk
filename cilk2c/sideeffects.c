/* Determine whether an expression has side effects in it.
 *
 * Copyright (C) 2003 Bradley C. Kuszmaul
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
	      "$HeadURL: https://bradley.lcs.mit.edu/svn/repos/cilk/current.Bug66/cilk2c/simplify.c $ $LastChangedBy: bradley $ $Rev: 387 $ $Date: 2003-05-01 14:39:50 -0400 (Thu, 01 May 2003) $");


PRIVATE Bool SideeffectsList(List *list);

#define SideeffectsNop(kind,utype) PRIVATE Bool Sideeffects##kind(utype##Node *UNUSED(u)) { return FALSE; }


SideeffectsNop(Id,id)
SideeffectsNop(Label,label)
SideeffectsNop(Goto,Goto)
SideeffectsNop(Default,Default)
SideeffectsNop(Continue,Continue)
SideeffectsNop(Break,Break)
SideeffectsNop(Prim,prim)
SideeffectsNop(Sync,sync)
SideeffectsNop(Abort,abort)
SideeffectsNop(Synched,synched)
SideeffectsNop(Attrib,attrib)
SideeffectsNop(Text,text)
SideeffectsNop(Const,Const)
SideeffectsNop(Tdef,tdef)
SideeffectsNop(Ptr,ptr)
SideeffectsNop(Adcl,adcl)
SideeffectsNop(Fdcl,fdcl)
SideeffectsNop(Sdcl,sdcl)
SideeffectsNop(Udcl,udcl)
SideeffectsNop(Edcl,edcl)
SideeffectsNop(Proc,proc)

PRIVATE Bool SideeffectsUnary (unaryNode *u) {
    if (IsIncDecOp(u->op)) return TRUE;
    else if (u->op==SIZEOF)     return FALSE;
    else if (u->op==ALIGNOF)    return FALSE;
    else return SideeffectsNode(u->expr);
}

PRIVATE Bool SideeffectsBinop (binopNode *u) {
    if (IsAssignmentOp(u->op)) return TRUE;
    else {
	switch (u->op) {
	case Cast:
	case ':':
	case DESIGNATED_INITIALIZER:
	    return SideeffectsNode(u->right);
	case '.':
	case ARROW:
	    return SideeffectsNode(u->left);
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case LS:
	case RS:
	case '<':
	case '>':
	case LE:
	case GE:
	case EQ:
	case NE:
	case '&':
	case '^':
	case '|':
	case ANDAND:
	case OROR:
	    return SideeffectsNode(u->left) || SideeffectsNode(u->right);
	default:
	    printf("%s:%d Unhandled case %d\n", __FILE__, __LINE__, u->op);
	    abort();
	}
    }
}

PRIVATE Bool SideeffectsCast (castNode *u) {
    return SideeffectsNode(u->expr);
}

PRIVATE Bool SideeffectsComma (commaNode *u) {
    return SideeffectsList(u->exprs);
}

PRIVATE Bool SideeffectsConstructor (constructorNode *u) {
    return SideeffectsNode(u->initializerlist);
}
PRIVATE Bool SideeffectsTernary (ternaryNode *u) {
    return (SideeffectsNode(u->cond)
	    || SideeffectsNode(u->true)
	    || SideeffectsNode(u->false));
}

PRIVATE Bool SideeffectsArray (arrayNode *u) {
    return SideeffectsNode(u->name) || SideeffectsList(u->dims);
}

PRIVATE Bool SideeffectsCall (UNUSED(callNode *u)) {
    return TRUE; /* assume that evaluating a function call will cause side effects.*/
}

PRIVATE Bool SideeffectsInitializer (initializerNode *u) {
    return SideeffectsList(u->exprs);
}

PRIVATE Bool SideeffectsImplicitCast (implicitcastNode *u) {
    return SideeffectsNode(u->expr);
}

PRIVATE Bool SideeffectsSwitch (SwitchNode *u) {
    return SideeffectsNode(u->expr) || SideeffectsNode(u->stmt);
}

PRIVATE Bool SideeffectsCase (CaseNode *u) {
    assert(!SideeffectsNode(u->expr)); /* It is supposed to be a constant*/
    return FALSE;
}


PRIVATE Bool SideeffectsIf (IfNode *u) {
    return SideeffectsNode(u->expr)
	|| SideeffectsNode(u->stmt);
}

PRIVATE Bool SideeffectsIfElse (IfElseNode *u) {
    return  SideeffectsNode(u->expr)
	||  SideeffectsNode(u->true)
	||  SideeffectsNode(u->false);
}

PRIVATE Bool SideeffectsWhile (WhileNode *u) {
    return SideeffectsNode(u->expr)
	|| SideeffectsNode(u->stmt);
}

PRIVATE Bool SideeffectsDo (DoNode *u) {
    return SideeffectsNode(u->stmt)
	|| SideeffectsNode(u->expr);
}

PRIVATE Bool SideeffectsFor (ForNode *u) {
    return SideeffectsNode(u->init)
	|| SideeffectsNode(u->cond)
	||  SideeffectsNode(u->next)
	|| SideeffectsNode(u->stmt);
}

PRIVATE Bool SideeffectsReturn (ReturnNode *u) {
    return SideeffectsNode(u->expr);
}

PRIVATE Bool SideeffectsBlock (BlockNode *u) {
    return SideeffectsList(u->decl)
	|| SideeffectsList(u->stmts);
}

PRIVATE Bool SideeffectsAsm(asmNode *u) {
    return (u->output!=NULL)
	|| SideeffectsList(u->input);
}

PRIVATE Bool SideeffectsAsmArg(asmargNode *u) {
    return SideeffectsNode(u->expr);
}    

PRIVATE Bool SideeffectsBuiltinVaArg(builtinvaargNode *u) {
    return SideeffectsNode(u->expr);
}    

PRIVATE Bool SideeffectsSpawn(UNUSED(spawnNode *u)) {
    return TRUE;
}

PRIVATE Bool SideeffectsInletCall(UNUSED(inletcallNode *u)) {
    return TRUE;
}


PRIVATE Bool SideeffectsXBlock (XBlockNode *u) {
    return SideeffectsList(u->decl)
	|| SideeffectsList(u->stmts);
}


PRIVATE Bool SideeffectsDecl(declNode *u) {
    return SideeffectsNode(u->init);
}

GLOBAL Bool SideeffectsNode(Node *node) {
    if (node == NULL)
	return FALSE;
#define CODE(name, node, union) return Sideeffects##name(union)
     ASTSWITCH(node, CODE)
#undef CODE
	 UNREACHABLE;
}
PRIVATE Bool SideeffectsList(List *list) {
    List *aptr;
    for (aptr = list; aptr; aptr = Rest(aptr)) {
	Node *item = FirstItem(aptr);
	
	if (SideeffectsNode(item)) return TRUE;
    }
    return FALSE;
}
