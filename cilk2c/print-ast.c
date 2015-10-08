/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *
 *************************************************************************/
/*
 *
 * Copyright (c) 1994 MIT Laboratory for Computer Science
 * Copyright (c) 2002 Massachusetts Institute of Technology
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/print-ast.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include <ctype.h>

/* main debugging entry points -- handy for calling from gdb */

GLOBAL void DPN(Node *n)
{
     fPrintNode(stdout, n, 0);
     putchar('\n');
     fflush(stdout);
}

GLOBAL void DPL(List *list)
{
     fPrintList(stdout, list, 0);
     putchar('\n');
     fflush(stdout);
}

GLOBAL void PrintCoord (GBUF *out, Coord *c) {
#if 0
    static int prevfilenum=-1;
    gbprintf(out, "<");
    if (prevfilenum!=c->file) {
	gbprintf(out, "%s:", FileNames[c->file]);
	prevfilenum=c->file;
    }
#endif
    if (PrintASTcoords)
	gbprintf(out, " <%d.%d>", c->line, c->offset);
}


/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintConst(GBUF *gb, Node *node, UNUSED(ConstNode *u), UNUSED(int offset), UNUSED(Bool norecurse))
{
    gbprintf(gb, "Const: ");
    output_constant_to_GBUF(gb, node, TRUE);
}

PRIVATE inline void PrintId(GBUF *out, Node *node, idNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Id: %s", u->text);
     PrintCoord(out, &node->coord);
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     gbputs("decl: ", out);
     PrintNode(out, u->decl, offset+4);
     /* PrintCRSpaces(out, offset + 2);*/
     /* PrintNode(out, u->decl, offset + 2); KHR */
}

PRIVATE inline void PrintBinop(GBUF *out, Node *node, binopNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Binop: ");
     PrintOp(out, u->op);
     PrintCoord(out, &node->coord);
     if (u->type) {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->type, offset + 2);
     }
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->left, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->right, offset + 2);
}

PRIVATE inline void PrintUnary(GBUF *out, Node *node, unaryNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Unary: ");
     PrintOp(out, u->op);
     PrintCoord(out, &node->coord);
     if (u->type) {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->type, offset + 2);
     }
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
}

PRIVATE inline void PrintCast(GBUF *out, UNUSED(Node *node), castNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("Cast: ", out);
     PrintCoord(out, &node->coord);
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->type, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
}

PRIVATE inline void PrintComma(GBUF *out, Node *node, commaNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Comma: List: exprs <%d.%d>", node->coord.line, node->coord.offset);
     if (node->parenthesized) gbprintf(out, " parenthesized");
     PrintCRSpaces(out, offset + 2);
     PrintList(out, u->exprs, offset + 2);
}

PRIVATE inline void PrintConstructor(GBUF *out, UNUSED(Node *node), constructorNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Constructor: List: exprs");
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->type, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->initializerlist, offset + 2);
}

PRIVATE inline void PrintTernary(GBUF *out, UNUSED(Node *node), ternaryNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("Ternary: ", out);
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->cond, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->true, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->false, offset + 2);
}

PRIVATE inline void PrintArray(GBUF *out, UNUSED(Node *node), arrayNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("Array: ", out);
     if (u->type) {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->type, offset + 2);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->name, offset + 2);
     PrintCRSpaces(out, offset + 2);
     gbputs("List: dims", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->dims, offset + 4);
}

PRIVATE inline void PrintCall(GBUF *out, UNUSED(Node *node), callNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("Call: ", out);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->name, offset + 2);
     PrintCRSpaces(out, offset + 2);
     gbputs("List: args", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->args, offset + 4);
}

PRIVATE inline void PrintInitializer(GBUF *out, UNUSED(Node *node), initializerNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Initializer: List: exprs");
     PrintCRSpaces(out, offset + 2);
     PrintList(out, u->exprs, offset + 2);
}

PRIVATE inline void PrintImplicitCast(GBUF *out, UNUSED(Node *node), implicitcastNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("ImplicitCast: ", out);
     if (u->type) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Type:", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->type, offset + 2);
     }
     if (u->value) {
	  PrintCRSpaces(out, offset + 2);
	  gbputs("Value: ", out);
	  PrintCRSpaces(out, offset + 4);
	  PrintNode(out, u->value, offset + 4);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintLabel(GBUF *out, Node *node, labelNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Label: %s (%d)", u->name, node->print_uid);
#if 0
     gbprintf(out, "Label: %s", u->name);
#endif
}

PRIVATE inline void PrintSwitch(GBUF *out, Node *node, SwitchNode *u, int offset, UNUSED(Bool norecurse))
{
     ListMarker marker;
     Node *cse;

     gbprintf(out, "Switch: (%d)", node->print_uid);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->stmt, offset + 2);
     PrintCRSpaces(out, offset + 2);

     IterateList(&marker, u->cases);
     gbprintf(out, "Cases:");
     while (NextOnList(&marker, (GenericREF) & cse)) {
	  gbprintf(out, " %d", cse->coord.line);
     }
}

PRIVATE inline void PrintCase(GBUF *out, UNUSED(Node *node), CaseNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Case: (container = %d)", u->container->print_uid);
#if 0
     gbprintf(out, "Case: ");
#endif
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
     PrintCRSpaces(out, offset + 2);
}

PRIVATE inline void PrintDefault(GBUF *out, UNUSED(Node *node), DefaultNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Default: (container = %d)", u->container->print_uid);
     PrintCRSpaces(out, offset + 2);
}

PRIVATE inline void PrintIf(GBUF *out, UNUSED(Node *node), IfNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("If: ", out);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->stmt, offset + 2);
}

PRIVATE inline void PrintIfElse(GBUF *out, UNUSED(Node *node), IfElseNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("IfElse: ", out);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->true, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->false, offset + 2);
}

PRIVATE inline void PrintWhile(GBUF *out, Node *node, WhileNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "While: (%d) ", node->print_uid);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->stmt, offset + 2);
}

PRIVATE inline void PrintDo(GBUF *out, Node *node, DoNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Do: (%d) ", node->print_uid);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->stmt, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
}

PRIVATE inline void PrintFor(GBUF *out, Node *node, ForNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "For: (%d) ", node->print_uid);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->init, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->cond, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->next, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->stmt, offset + 2);
}

PRIVATE inline void PrintGoto(GBUF *out, UNUSED(Node *node), GotoNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Goto: %s",
	     (u->label ? u->label->u.label.name : "nil"));
}

PRIVATE inline void PrintContinue(GBUF *out, UNUSED(Node *node), ContinueNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Continue: (container = %d)", u->container->print_uid);
#if 0
     gbprintf(out, "Continue: ");
#endif
}

PRIVATE inline void PrintBreak(GBUF *out, UNUSED(Node *node), BreakNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Break: (container = %d)", u->container->print_uid);
#if 0
     gbprintf(out, "Break: ");
#endif
}

PRIVATE inline void PrintReturn(GBUF *out, UNUSED(Node *node), ReturnNode *u, int offset, Bool UNUSED(norecurse))
{
     gbputs("Return: ", out);
#if 0
     if (u->expr) {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, NodeDataType(u->expr), offset + 2);
     }
#endif
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->expr, offset + 2);
}

PRIVATE inline void PrintBlock(GBUF *out, UNUSED(Node *node), BlockNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("Block:", out);
     if (node->parenthesized) gbprintf(out, " parenthesized");
     PrintCRSpaces(out, offset + 2);
     if (u->type) {
	 gbprintf(out, "Type: (%d)", u->type->print_uid);
     } else {
	 gbprintf(out, "Type: nil");
     }
#if 0
     gbputs("Type:", out);
#endif
     PrintCRSpaces(out, offset + 4);
     PrintNode(out, u->type, offset + 4);
     PrintCRSpaces(out, offset + 2);
     gbputs("List: decl", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->decl, offset + 4);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: stmts", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->stmts, offset + 4);
}


/*************************************************************************/
/*                                                                       */
/*                            Type nodes                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintPrim(GBUF *out, Node *node, UNUSED(primNode *u), UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Prim: ");
     output_prim_type_to_GBUF(out, node, 0);
     PrintCoord(out, &node->coord);
}

PRIVATE inline void PrintTdef(GBUF *out, Node *node, tdefNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     /* temp fix for core dump when printing ast  -Angelina */
     if( u->type == NULL )
          gbprintf(out, "Tdef: %s (%d) (type=NULL)  ", u->name, node->print_uid);
     else
          gbprintf(out, "Tdef: %s (%d) (type=%d)  ", u->name, node->print_uid, u->type->print_uid);
     PrintCoord(out, &node->coord);
     PrintTQ(out, u->tq, offset, norecurse);
#if 0
     if (!norecurse) {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->type, offset + 2);
     }
#endif
}

PRIVATE inline void PrintPtr(GBUF *out, UNUSED(Node *node), ptrNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Ptr: ");
     PrintTQ(out, u->tq, offset, norecurse);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->type, offset + 2);
}

PRIVATE inline void PrintAdcl(GBUF *out, Node *node, adclNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Adcl: (%d)", node->print_uid);
     PrintTQ(out, u->tq, offset, norecurse);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->type, offset + 2);
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->dimp->dim, offset + 2);
     if (u->dimp->size > 0) {
	  PrintCRSpaces(out, offset + 2);
	  gbprintf(out, "%d", u->dimp->size);
     }
}

PRIVATE inline void PrintFdcl(GBUF *out, Node *node, fdclNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Fdcl:");
     PrintCoord(out, &node->coord);
     gbprintf(out, " ");
     PrintTQ(out, u->tq, offset, norecurse);
     PrintCRSpaces(out, offset + 2);
     gbputs("List: Args: ", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->args, offset + 4);
     PrintCRSpaces(out, offset + 2);
     gbputs("Returns: ", out);
     PrintCRSpaces(out, offset + 4);
     PrintNode(out, u->returns, offset + 4);
}

PRIVATE inline void PrintSdcl(GBUF *out, Node *node, sdclNode *u, int offset, Bool norecurse)
{
     gbprintf(out, "Sdcl: (%d) ", node->print_uid);

     if (norecurse)
	  gbprintf(out, "%s\n", u->type->name);
     else {
	  PrintCRSpaces(out, offset + 2);
	  PrintTQ(out, u->tq, offset+2, norecurse);
	  PrintSUE(out, u->type, offset + 4, TRUE);
     }
}

PRIVATE inline void PrintUdcl(GBUF *out, Node *node, udclNode *u, int offset, Bool norecurse)
{
     gbprintf(out, "Udcl: (%d) ", node->print_uid);

     if (norecurse)
	  gbprintf(out, "%s\n", u->type->name);
     else {
	  PrintCRSpaces(out, offset + 2);
	  PrintTQ(out, u->tq, offset+2, norecurse);
	  PrintSUE(out, u->type, offset + 4, TRUE);
     }
}

PRIVATE inline void PrintEdcl(GBUF *out, Node *node, edclNode *u, int offset, Bool norecurse)
{
     gbprintf(out, "Edcl: (%d) ", node->print_uid);

     if (norecurse)
	  gbprintf(out, "%s\n", u->type->name);
     else {
	  PrintCRSpaces(out, offset + 2);
	  PrintTQ(out, u->tq, offset+2, norecurse);
	  PrintSUE(out, u->type, offset + 4, TRUE);
     }
}

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintDecl(GBUF *out, Node *node, declNode *u, int offset, Bool norecurse)
{
     gbprintf(out, "Decl: %s (%d)", u->name ? u->name : "", node->print_uid);
     PrintCoord(out, &node->coord);
     gbprintf(out, " ");
     PrintTQ(out, u->tq, offset+2, norecurse);

     if (norecurse)
	  gbprintf(out, "\n");
     else {
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->type, offset + 2);
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->init, offset + 2);
	  PrintCRSpaces(out, offset + 2);
	  PrintNode(out, u->bitsize, offset + 2);
	  if (u->attribs) {
	      PrintCRSpaces(out, offset+2);
	      PrintList(out, u->attribs, offset+2);
	  }
     }
}

PRIVATE inline void PrintAttrib(GBUF *out, UNUSED(Node *node), attribNode *u,
				int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Attrib: %s", u->name);

     PrintCRSpaces(out, offset + 2);
     PrintList(out, u->arglist, offset + 2);
}

PRIVATE inline void PrintProc(GBUF *out, UNUSED(Node *node), procNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbputs("Proc:\n  ", out);
     PrintNode(out, u->decl, 2);
     gbputs("\n  ", out);
     PrintNode(out, u->body, 2);
}

PRIVATE inline void PrintText(GBUF *out, UNUSED(Node *node), textNode *u, UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbputs("Text: ", out);
     if (u->start_new_line)
	  gbputs("(new line) ", out);
     PrintString(out, u->text);
}

/*************************************************************************/
/*                                                                       */
/*                            GCC asm Extensions                         */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintAsm(GBUF *out, UNUSED(Node *node), asmNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Asm: ");

     if (tq_has_anything(u->tq)) {
	  PrintCRSpaces(out, offset + 2);
	  PrintTQ(out, u->tq, offset, norecurse);
     }
     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->template, offset + 2);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: output operands", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->output, offset + 4);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: input operands", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->input, offset + 4);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: clobbered registers", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->clobbered, offset + 4);
}
PRIVATE inline void PrintAsmArg(GBUF *out, UNUSED(Node *node), asmargNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "AsmArg: ");
     PrintCRSpaces(out, offset + 2);
     gbputs("Constraint:", out);
     PrintNode(out, u->constraint, offset + 2);
     PrintCRSpaces(out, offset + 2);
     gbputs("Expr:", out);
     PrintNode(out, u->expr, offset + 2);
}

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
PRIVATE inline void PrintBuiltinVaArg(GBUF *out, UNUSED(Node *node), builtinvaargNode *u, int offset, UNUSED(Bool norecurse))
{
    gbprintf(out, "builtinvaarg: ");
    PrintCRSpaces(out, offset + 2);
    gbputs("Expr:", out);
    PrintNode(out, u->expr, offset + 2);
    PrintCRSpaces(out, offset + 2);
    gbputs("Type:", out);
    PrintNode(out, u->type, offset + 2);
}

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE inline void PrintSpawn(GBUF *out, UNUSED(Node *node), spawnNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "Spawn: ");

     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->receiver, offset + 2);

     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->name, offset + 2);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: args", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->args, offset + 4);
}

PRIVATE inline void PrintSync(GBUF *out, UNUSED(Node *node), UNUSED(syncNode *u), UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Sync");
}

PRIVATE inline void PrintInletCall(GBUF *out, UNUSED(Node *node), inletcallNode *u, int offset, UNUSED(Bool norecurse))
{
     gbprintf(out, "InletCall: ");

     PrintCRSpaces(out, offset + 2);
     PrintNode(out, u->name, offset + 2);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: args", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->args, offset + 4);
}

PRIVATE inline void PrintAbort(GBUF *out, UNUSED(Node *node), UNUSED(abortNode *u), UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Abort");
}

PRIVATE inline void PrintSynched(GBUF *out, UNUSED(Node *node), UNUSED(synchedNode *u), UNUSED(int offset), UNUSED(Bool norecurse))
{
     gbprintf(out, "Synched");
}


PRIVATE inline void PrintXBlock(GBUF *out, UNUSED(Node *node), XBlockNode *u, int offset, UNUSED(Bool norecurse))
{
     gbputs("XBlock:", out);
     if (node->parenthesized) gbprintf(out, " parenthesized");
     PrintCRSpaces(out, offset + 2);
     if (u->type) {
	 gbprintf(out, "Type: (%d)", u->type->print_uid);
     } else {
	 gbprintf(out, "Type: nil");
     }
#if 0
     gbputs("Type:", out);
#endif
     PrintCRSpaces(out, offset + 4);
     PrintNode(out, u->type, offset + 4);
     PrintCRSpaces(out, offset + 2);
     gbputs("List: decl", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->decl, offset + 4);

     PrintCRSpaces(out, offset + 2);
     gbputs("List: stmts", out);
     PrintCRSpaces(out, offset + 4);
     PrintList(out, u->stmts, offset + 4);
}


/*************************************************************************/
/*                                                                       */
/*                      PrintNode and PrintList                          */
/*                                                                       */
/*************************************************************************/

GLOBAL short PassCounter = 0;
GLOBAL int PrintInvocations = 0;	/* number of pending PrintNodes on call
					 * stack */

static void kill_print_uid (Node *n) {
    n->print_uid = -1;
    ASTWALK(n, kill_print_uid);
}

static int print_uid = 0;
static void assign_uids (Node *n) {
    if (n->print_uid==-1) {
	n->print_uid = print_uid++;
	ASTWALK(n, assign_uids);
    }
}


GLOBAL void PrintWildcard(GBUF *out, Node *node, int offset) {

  PrintCRSpaces(out, offset);
  switch (node->wTyp) {
  case WildcardE:
    gbprintf(out, "WildcardE: ");
    break;
  case WildcardD:
    gbprintf(out, "WildcardD: ");
    break;
  case WildcardT:
    gbprintf(out, "WildcardT: ");
    break;
  default:
    gbprintf(out, "Unknown wildcard.");
  }
}



GLOBAL void PrintNode(GBUF *out, Node *node, int offset)
{
  Bool norecurse;

  if (node == NULL) {
    gbprintf(out, "nil");
    return;
  }
  if (PrintInvocations++ == 0) {
    /* then we're the first invocation for this pass over the tree */
    ++PassCounter;
    print_uid=MaxBasicType; /* The first MaxBasicType values are used to uniquely identify prims.  Ugly...*/
    kill_print_uid(node);
    assign_uids(node);
  }
  norecurse = (node->pass == PassCounter);
  node->pass = PassCounter;

  if (IsWildcard(node)) {
    PrintWildcard(out, node, offset);
  }

#define CODE(name, node, union) Print##name(out,node,union,offset,norecurse)
  ASTSWITCH(node, CODE)
#undef CODE

    if (node->analysis.livevars) {
      PrintCRSpaces(out, offset + 2);
      PrintAnalysis(out, node);
    }
  --PrintInvocations;
}

GLOBAL void PrintList(GBUF *out, List *list, int offset)
{
     ListMarker marker;
     Node *item;
     Bool firstp = TRUE;

     if (PrintInvocations++ == 0) {
	  /* then we're the first invocation for this pass over the tree */
	  ++PassCounter;
	  print_uid=MaxBasicType;
	  LISTWALK(list, kill_print_uid);
	  LISTWALK(list, assign_uids);
     }
     IterateList(&marker, list);
     while (NextOnList(&marker, (GenericREF) & item)) {
	  if (firstp == TRUE)
	       firstp = FALSE;
	  else if (offset < 0)
	       gbputs("\n\n", out);
	  else
	       PrintCRSpaces(out, offset);

	  PrintNode(out, item, offset);
     }

     if (firstp == TRUE)
	  gbputs("nil", out);

     --PrintInvocations;
}

/*************************************************************************/
/*                                                                       */
/*                      Low-level output routines                        */
/*                                                                       */
/*************************************************************************/

GLOBAL int print_float(GBUF *fd, float val)
{
     int i;
     char fmt[8];
     char buf[64];
     float tmp;

     i = 7;
     while (1) {
	  sprintf(fmt, "%%.%dg", i);
	  sprintf(buf, fmt, val);
	  sscanf(buf, "%f", &tmp);
	  if (tmp == val)
	       break;
	  i += 1;
	  assert(i < 20);
     }

     return gbprintf(fd, "%s", buf);
}

GLOBAL int print_double(GBUF *fd, double val)
{
     int i;
     char fmt[8];
     char buf[64];
     double tmp;

     i = 16;
     while (1) {
	  sprintf(fmt, "%%.%dlg", i);
	  sprintf(buf, fmt, val);
	  sscanf(buf, "%lf", &tmp);
	  if (tmp == val)
	       break;
	  i += 1;
	  assert(i < 20);
     }

     return gbprintf(fd, "%s", buf);
}

void PrintCRSpaces(GBUF *out, int spaces)
{
     gbputs("\n", out);
     PrintSpaces(out, spaces);
}

void PrintSpaces(GBUF *out, int spaces)
{
     while (spaces--)
	 gbputs(" ", out);
}

GLOBAL void CharToText(char *array, unsigned char value)
{
     if (value < ' ') {
	  static const char *names[32] =
	  {
	       "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
	       "\\b", "\\t", "\\n", "\\v", "ff", "cr", "so", "si",
	       "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
	       "can", "em", "sub", "esc", "fs", "gs", "rs", "us"};
	  sprintf(array, "0x%02x (%s)", value, names[value]);
     } else if (value < 0x7f) {
	  sprintf(array, "'%c'", value);
     } else if (value == 0x7f) {
	  strcpy(array, "0x7f (del)");
     } else {			/* value >= 0x80 */
	  sprintf(array, "0x%x", value);
     }
}

GLOBAL int PrintChar(GBUF *out, int value)
{
     switch (value) {
	 case '\n':
	      return gbprintf(out, "\\n");
	 case '\t':
	      return gbprintf(out, "\\t");
	 case '\v':
	      return gbprintf(out, "\\v");
	 case '\b':
	      return gbprintf(out, "\\b");
	 case '\r':
	      return gbprintf(out, "\\r");
	 case '\f':
	      return gbprintf(out, "\\f");
	 case '\a':
	      return gbprintf(out, "\\a");
	 case '\\':
	      return gbprintf(out, "\\\\");
	 case '\?':
	      return gbprintf(out, "\\\?");
	 case '\"':
	      return gbprintf(out, "\\\"");
	 case '\'':
	      return gbprintf(out, "\\\'");
	 default:
	      if (isprint(value)) {
		   gbprintf(out, "%c", value);
		   return 1;
	      } else {
		   return gbprintf(out, "\\%o", value);
	      }
     }
}

GLOBAL int PrintString(GBUF *out, const char *s)
{
     int len = 0;

     gbputs("\"", out);
     while (*s != 0) {
	  len += PrintChar(out, *s++);
     }
     gbputs("\"", out);

     return len + 2;
}
