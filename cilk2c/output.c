/* -*- c-basic-offset: 5 -*- */
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Rob Miller
 *  
 *  $Log: output.c,v $
 *  Revision 1.1.1.1  1997/02/14 23:06:55  strumpen
 *  Create Cilk Repository
 *
 *  Revision 1.1.1.1  1997/02/14 22:04:03  strumpen
 *  Create cilk repository
 *
 *  Revision 1.3  1997/02/02 22:24:05  randall
 *  Added code to allow spawning via a function pointer.
 *  Made cilk2c accept multi-dimensional incomplete arrays (like int a[][])
 *  in declarations.
 *  Allowed type match between elaborated and unelaborated Sdcls.
 *  Added some BUGS.
 *
 *  Revision 1.2  1996/12/12 23:27:44  athena
 *  Cilk2c now understands `register int foo asm("r0")' and the like.
 *  Cilk2c understands #pragma segments [+-].
 *  Added flexibility to the rts to accomodate the above.
 *
 *  Revision 1.1  1996/12/10 17:11:18  athena
 *  Got rid of the Cilk_result hack --- required modification
 *  of cilk2c.  Consequently, added cilk2c to the Cilk-5 repository.
 *  Polished Keith's hacks (changed promote into promote_and_detach,
 *  and separated the victim part from the thief part).
 *
 *  Revision 1.14  1996/09/04 04:08:31  fengmd
 *  add the preliminary version of cilk debugger
 *
 *  Revision 1.13  1996/08/24 21:18:15  randall
 *  Added support for shared and private declarations of variables.
 *
 *  Revision 1.12  1996/08/23 21:46:43  randall
 *  Make gcc __attribute__ command appear before a variable initializer.
 *
 *  Revision 1.11  1996/06/06 03:52:28  fengmd
 *  Output IN_FAST_INLET, IN_SLOW_INLET and IN_STANDALONE_INLET.
 *
 *  Revision 1.10  1996/06/06 01:02:17  fengmd
 *  Output preprocessing directives for CILK_WHERE_AM_I.
 *
 *  Revision 1.9  1996/05/06 22:31:19  randall
 *  Modified cilk2c: changed the way cilk function declarations are handled.
 *  Started adding code to migrate struct and typedefs to the top level.
 *
 *  Revision 1.8  1996/05/04 01:44:01  randall
 *  Major overhaul.  Made an inlet a type of Proc instead of its own ast
 *  node.  Reorganized dataflow and other files to account for local
 *  procedure nodes.  Changed grammar to allow local function definitions
 *  (including inlets).  Moved most syntax errors into sem-check.c from
 *  transform.c.
 *
 *  Revision 1.7  1996/04/21 04:20:17  fengmd
 *  Add implicit inlet, crlf, on_abort, SYNCHED, SPAWN_COUNTER.
 *
 *  Revision 1.6  1996/03/24 20:08:28  randall
 *  Fixed up ASM directive.
 *
 *  Revision 1.5  1996/03/08 17:07:31  randall
 *  Allowed spawn with void return type in inlet_call.  Started working on
 *  resume and abort.
 *
 *  Revision 1.4  1996/02/29 00:37:25  athena
 *  Updated cilk2c for Charles' algorithm (not implemented yet)
 *
 *  Revision 1.3  1996/02/27 15:48:49  athena
 *  Hacked cilk2c to accept 'asm' (fengmd)
 *
 * Revision 1.2  1996/02/19  22:15:56  randall
 * Moved _PUSH_FRAME/_POP_FRAME calls around, and reorganized rts macros
 * a bit.  Removed #include inlining, and added EnterC and ExitC calls to
 * prevent C headers from being parsed as Cilk.
 *
 *  Revision 1.1  1996/02/14 23:05:34  athena
 *  Added cilk2c
 *
 *  Revision 1.1.1.1  1996/02/14 22:56:33  randall
 *  Added cilk2stack to repository
 *
 * Revision 1.1.1.1  1996/01/28  09:13:36  cilk
 * Added cilk2stack to repository.
 *
 * Revision 1.22  1995/08/01  18:08:22  bradley
 * Get rid of most warnings from -Wconversion
 * There are no errors from -Wpointer-arith
 *
 * Revision 1.21  1995/05/11  18:54:22  rcm
 * Added gcc extension __attribute__.
 *
 * Revision 1.20  1995/05/08  04:30:39  randall
 * Cleaned up the #include-induced suppression of output generation.  Now,
 * #includes are only generated at the top level.  If they occur in a scope,
 * the file will be written to the output and no #include will be generated.
 *
 * Revision 1.19  1995/05/05  19:18:30  randall
 * Added #include reconstruction.
 *
 * Revision 1.18  1995/04/21  05:44:33  rcm
 * Cleaned up data-flow analysis, and separated into two files, dataflow.c
 * and analyze.c.  Fixed void pointer arithmetic bug (void *p; p+=5).
 * Moved CVS Id after comment header of each file.
 *
 * Revision 1.17  1995/04/09  21:30:50  rcm
 * Added Analysis phase to perform all analysis at one place in pipeline.
 * Also added checking for functions without return values and unreachable
 * code.  Added tests of live-variable analysis.
 *
 * Revision 1.16  1995/03/23  15:31:17  rcm
 * Dataflow analysis; removed IsCompatible; replaced SUN4 compile-time symbol
 * with more specific symbols; minor bug fixes.
 *
 * Revision 1.15  1995/02/13  02:00:16  rcm
 * Added ASTWALK macro; fixed some small bugs.
 *
 * Revision 1.14  1995/02/07  21:24:24  rcm
 * Fixed SetOutputCoord problem and allowed Text.text to be null
 *
 * Revision 1.13  1995/02/01  23:01:25  rcm
 * Added Text node and #pragma collection
 *
 * Revision 1.12  1995/02/01  21:07:20  rcm
 * New AST constructors convention: MakeFoo makes a foo with unknown coordinates,
 * whereas MakeFooCoord takes an explicit Coord argument.
 *
 * Revision 1.11  1995/02/01  07:37:45  rcm
 * Renamed list primitives consistently from '...Element' to '...Item'
 *
 * Revision 1.10  1995/01/27  01:39:01  rcm
 * Redesigned type qualifiers and storage classes;  introduced "declaration
 * qualifier."
 *
 * Revision 1.9  1995/01/20  03:38:09  rcm
 * Added some GNU extensions (long long, zero-length arrays, cast to union).
 * Moved all scope manipulation out of lexer.
 *
 * Revision 1.8  1995/01/06  16:48:57  rcm
 * added copyright message
 *
 * Revision 1.7  1994/12/23  09:18:33  rcm
 * Added struct packing rules from wchsieh.  Fixed some initializer problems.
 *
 * Revision 1.6  1994/12/20  09:24:09  rcm
 * Added ASTSWITCH, made other changes to simplify extensions
 *
 * Revision 1.5  1994/11/22  01:54:37  rcm
 * No longer folds constant expressions.
 *
 * Revision 1.4  1994/11/10  03:13:22  rcm
 * Fixed line numbers on AST nodes.
 *
 * Revision 1.3  1994/11/03  07:38:47  rcm
 * Added code to output C from the parse tree.
 *
 * Revision 1.2  1994/10/28  18:52:39  rcm
 * Removed ALEWIFE-isms.
 *
 *
 *  Created: Wed Apr 28 16:07:13 EDT 1993
 *
 *
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/output.c $ $LastChangedBy: bradley $ $Rev: 3383 $ $Date: 2007-01-20 21:59:03 -0500 (Sat, 20 Jan 2007) $");

#include <ctype.h>


typedef enum {
     Left,
     Right
} Context;

typedef enum {
     TopDecl,			/* toplevel decls: procs, global variables, typedefs, SUE defs */
     BlockDecl,			/* local decls at beginning of a block */
     SUFieldDecl,		/* structure/union field decls */
     EnumConstDecl,		/* enumerated constant decls */
     FormalDecl			/* formal args to a procedure */
} DeclKind;

typedef struct {
     char prev_char;            /* what is the previous character written?  Use this to determine if whitespace may need to be added. */
     Coord curr;
     int block;
     int block_indents[MAX_NESTED_SCOPES];
     GBUF *gb;
} OutputContext;

PRIVATE void SetOutputCoord(OutputContext *out, Coord *pcoord);
PRIVATE void SetOutputCoordStmt(OutputContext *out, Coord *pcoord);
PRIVATE void ForceNewLine(OutputContext *out, Coord *pcoord);
PRIVATE inline void OutNewLine(OutputContext *out);
PRIVATE inline void OutCh(OutputContext *out, int ch);
PRIVATE inline void OutS(OutputContext *out, const char *s);
PRIVATE inline void OutS_embedded_newlines(OutputContext *out, const char *s);
PRIVATE inline void OutOffset(OutputContext *out, int len);

PRIVATE void StartBlockIndent(OutputContext *out, List *block_items);
PRIVATE void EndBlockIndent(OutputContext *out);
PRIVATE inline int BlockIndent(OutputContext *out);

PRIVATE Bool IsSourceExpression(Node *node);
PRIVATE int Precedence(Node *node, Bool *left_assoc);

PRIVATE void output_prim_type (OutputContext *out, Node *type, char *lastc, Bool print_coords);
PRIVATE void OutputTQ (OutputContext *out, TypeQual tq, Coord *coord);

PRIVATE void OutputExpr(OutputContext *out, Node *node);
PRIVATE void OutputInnerExpr(OutputContext *out, Node *node, int enclosing_precedence, Context context);
PRIVATE void OutputPartialType(OutputContext *out, Context context,
			       Node *node, TypeQual sc);
PRIVATE void OutputType(OutputContext *out, Node *node);
PRIVATE void OutputStatement(OutputContext *out, Node *node);
PRIVATE void OutputStatementList(OutputContext *out, List *lst);
PRIVATE void OutputAttribs(OutputContext *out, List *attribs);
PRIVATE void OutputAsmDecl(OutputContext *out, Node *a);
PRIVATE int OutputDecl(OutputContext *out, DeclKind k, Node *node);
PRIVATE void OutputDeclList(OutputContext *out, DeclKind k, List *lst);
PRIVATE void OutputSUE(OutputContext *out, SUEtype *sue, Bool elaboratedp, List *attributes);
PRIVATE void OutputTextNode(OutputContext *out, Node *node);

GLOBAL void OutputProgram(FILE *outfile, List *program)
{
     OutputContext out;

     out.gb = make_file_gbuf(outfile);
     out.curr = UnknownCoord;
     out.block = 0;
     out.block_indents[0] = 0;
     out.prev_char = 0;

     OutputDeclList(&out, TopDecl, program);
     OutCh(&out, '\n');
     free_gbuf(out.gb);
}

/** This was added to work with parsing from a string.
 *    -- Jim , 7/03
 */
GLOBAL void OutputStmtList(FILE *outfile, List *myStmtList)
{
     OutputContext out;
     out.gb = make_file_gbuf(outfile);
     out.curr = UnknownCoord;
     out.block = 0;
     out.block_indents[0] = 0;

     OutputStatementList(&out, myStmtList);
     OutCh(&out, '\n');
}

/* If SetOutputCoord() needs to adjust the output line number, it can
 * insert at most MAX_INSERTED_NEWLINES newline characters.  If more
 * adjustment is needed, a #line directive is emitted instead.  This
 * limits the number of #line directives needed.
 */
#define MAX_INSERTED_NEWLINES      3

PRIVATE void SetOutputCoord(OutputContext *out, Coord *pcoord)
{
     if (IsUnknownCoord(*pcoord) || IsDontPrintCoord(out->curr))
	  return;		/* *pcoord == UnknownCoord, so ignore */

     /* First, set line */
     if (pcoord->file != out->curr.file) {
	  if (out->curr.offset != 0)
	       OutNewLine(out);
	  if (!FormatReadably)
	       gbprintf(out->gb, "# %d \"%s\"\n", pcoord->line,
			FileNames[pcoord->file]);
	  out->curr.file = pcoord->file;
	  out->curr.line = pcoord->line;
	  out->curr.offset = 0;
     } else if (pcoord->line == out->curr.line) {
	  /* do nothing */
     } else if (pcoord->line > out->curr.line &&
		 pcoord->line <= out->curr.line + MAX_INSERTED_NEWLINES) {
	  while (out->curr.line < pcoord->line)
	       OutNewLine(out);
     } else {
	  if (out->curr.offset != 0)
	       OutNewLine(out);
	  if (!FormatReadably)
	       gbprintf(out->gb, "# %d\n", pcoord->line);
	  out->curr.line = pcoord->line;
	  out->curr.offset = 0;
     }

     /* Now, set offset */
     while (out->curr.offset < pcoord->offset) {
	 OutCh(out, ' ');
     }
}

/* SetOutputCoordStmt is like SetOutputCoord, but handles
 * UnknownCoord differently: if -N option was supplied to
 * c2c, insert a line break and indent to current block indentation */
PRIVATE void SetOutputCoordStmt(OutputContext *out, Coord *pcoord)
{
     if (FormatReadably && IsUnknownCoord(*pcoord)) {
	  Coord new = out->curr;
	  ++new.line;		/* force line break */
	  new.offset = BlockIndent(out);
	  pcoord = &new;
     }
     SetOutputCoord(out, pcoord);
}

/* ForceNewLine ensures that only whitespace appears before
 * current position on output line */
PRIVATE void ForceNewLine(OutputContext *out, Coord *pcoord)
{
     /* if current offset is > 0, need to start a new line */
     if (out->curr.offset > 0) {
	  OutNewLine(out);
     }
     SetOutputCoord(out, pcoord);
}

PRIVATE inline void OutNewLine(OutputContext *out) {
     gbputs("\n", out->gb);
     ++out->curr.line;
     out->curr.offset=0;
     out->prev_char = '\n';
}

PRIVATE inline void OutCh(OutputContext *out, int ch)
{
     gbprintf(out->gb, "%c", ch);
     ++out->curr.offset;
     out->prev_char = ch;
}

PRIVATE inline void OutS(OutputContext *out, const char *s)
{
     int len = strlen(s);
     int r = gbputs(s, out->gb);
     assert(r>=0);
     out->curr.offset += len;
     out->prev_char = s[len-1];
}

PRIVATE inline void OutS_embedded_newlines(OutputContext *out, const char *s)
{
     char c;
     while ((c=*s++)) {
	  if (c=='\n') OutNewLine(out);
	  else OutCh(out, c);
     }
}

PRIVATE inline void OutOffset(OutputContext *out, int len)
{
     out->curr.offset += len;
}

PRIVATE void StartBlockIndent(OutputContext *out, List *block_items)
{
     Node *item = NULL;

     while (block_items != NULL &&
	    ((item = FirstItem(block_items)) == NULL ||
	     IsUnknownCoord(item->coord)))
	  block_items = Rest(block_items);

     ++out->block;
     out->block_indents[out->block] =
	 block_items == NULL
	 ? out->block_indents[out->block - 1] + 2
	 : item->coord.offset;
}

PRIVATE void EndBlockIndent(OutputContext *out)
{
     assert(out->block > 0);
     --out->block;
}

PRIVATE inline int BlockIndent(OutputContext *out)
{
     return out->block_indents[out->block];
}

PRIVATE Bool IsSourceExpression(Node *node)
{
     while (node && node->typ == ImplicitCast)
	  node = node->u.implicitcast.expr;

     return (node != NULL);
}

PRIVATE int Precedence(Node *node, Bool *left_assoc)
{
     switch (node->typ) {
	 case Unary:
	      return OpPrecedence(Unary, node->u.unary.op, left_assoc);
	 case Binop:
	      return OpPrecedence(Binop, node->u.binop.op, left_assoc);
	 case Cast:
	      *left_assoc = FALSE;
	      return 14;
	 case Comma:
	      *left_assoc = TRUE;
	      return 1;
	 case Ternary:
	      *left_assoc = FALSE;
	      return 3;
	 case Array:
	      *left_assoc = TRUE;
	      return 15;
	 case Call:
	      *left_assoc = TRUE;
	      return 15;
	 default:
	      *left_assoc = TRUE;
	      return 20;	/* highest precedence */
     }
     /* unreachable */
}

PRIVATE void OutputBinop(OutputContext *out, Node *node, int my_precedence)
{
     assert(node->typ == Binop);

#if 0
     /* force parenthesizing of subexpressions of certain ops -- this prevents 
      * backend compiler (like gcc) from warning about common precedence 
      * errors */
     switch (node->u.binop.op) {
	 case OROR:
	 case '|':
	 case '^':
	      my_precedence = 20;
	      break;
	 default:
	      break;
     }
#endif

     /* Hacks to deal with the designated initializer and array initializer syntax */
     if (node->u.binop.op == DESIGNATED_INITIALIZER) {
	  SetOutputCoord(out, &node->coord);
	  gbputs(".", out->gb);
	  OutOffset(out, 1);
     } else if (node->u.binop.op == ARRAY_LABELED_INITIALIZER) {
	  SetOutputCoord(out, &node->coord);
	  gbputs("[", out->gb);
	  OutOffset(out, 1);
     }

     OutputInnerExpr(out, node->u.binop.left, my_precedence, Left);

     SetOutputCoord(out, &node->coord);
     OutOffset(out, PrintOp(out->gb, node->u.binop.op));

     OutputInnerExpr(out, node->u.binop.right, my_precedence, Right);
}

PRIVATE void OutputUnary(OutputContext *out, Node *node, int my_precedence)
{
     assert(node->typ == Unary);

     switch (node->u.unary.op) {
	 case SIZEOF:
	 case ALIGNOF:
	      SetOutputCoord(out, &node->coord);
	      if (node->u.unary.op == SIZEOF) 
		   OutS(out, "sizeof(");
	      else
		   OutS(out, "__alignof__(");
	      if (IsType(node->u.unary.expr))
		   OutputType(out, node->u.unary.expr);
	      else
		   OutputExpr(out, node->u.unary.expr);
	      OutCh(out, ')');
	      break;

	 case PREDEC:
	 case PREINC:
	      SetOutputCoord(out, &node->coord);
	      OutOffset(out, PrintOp(out->gb, node->u.unary.op));
	      OutputInnerExpr(out, node->u.unary.expr, my_precedence, Right);
	      break;

	 case POSTDEC:
	 case POSTINC:
	      OutputInnerExpr(out, node->u.unary.expr, my_precedence, Left);
	      SetOutputCoord(out, &node->coord);
	      OutOffset(out, PrintOp(out->gb, node->u.unary.op));
	      break;

	 default:
	      SetOutputCoord(out, &node->coord);
	      OutOffset(out, PrintOp(out->gb, node->u.unary.op));
	      OutputInnerExpr(out, node->u.unary.expr, my_precedence, Right);
	      break;
     }
}

PRIVATE void OutputCommaList(OutputContext *out, List *list)
{
     ListMarker marker;
     Node *expr;

     if (list == NULL)
	  return;

     IterateList(&marker, list);
     if (!NextOnList(&marker, (GenericREF) & expr))
	  return;

     /* hack to allow offsetof() macro: represented as a Call
      * with a type argument */
     if (IsType(expr))
	  OutputType(out, expr);
     else
	  OutputExpr(out, expr);
     while (NextOnList(&marker, (GenericREF) & expr)) {
	  OutCh(out, ',');
	  if (IsType(expr))
	       OutputType(out, expr);
	  else
	       OutputExpr(out, expr);
     }
}

PRIVATE void OutputDimensions(OutputContext *out, List *dim)
{
     ListMarker marker;
     Node *expr;

     IterateList(&marker, dim);
     while (NextOnList(&marker, (GenericREF) & expr)) {
	  OutCh(out, '[');
	  OutputExpr(out, expr);
	  OutCh(out, ']');
     }
}

PRIVATE void OutputArray(OutputContext *out, Node *node, int my_precedence)
{
     OutputInnerExpr(out, node->u.array.name, my_precedence, Left);
     SetOutputCoord(out, &node->coord);
     OutputDimensions(out, node->u.array.dims);
}

PRIVATE void OutputCall(OutputContext *out, Node *node, int my_precedence)
{
     OutputInnerExpr(out, node->u.call.name, my_precedence, Left);
     SetOutputCoord(out, &node->coord);
     OutCh(out, '(');
     OutputCommaList(out, node->u.call.args);
     OutCh(out, ')');
}

PRIVATE void output_constant(OutputContext *out, Node *c, Bool with_name)
{
     if (with_name)
	  switch (c->u.Const.type->typ) {
	      case Prim:
		   output_prim_type(out, c->u.Const.type, NULL, TRUE);
		   OutS(out, " ");
		   OutOffset(out, 1);
		   break;
		   /* Used for strings */
	      case Adcl:
		   assert(c->u.Const.type->u.adcl.type->typ == Prim);
		   OutS(out, "array of ");
		   OutOffset(out, 9);
		   output_prim_type(out, c->u.Const.type->u.adcl.type,  0, TRUE);
		   OutS(out, " ");
		   OutOffset(out, 1);
		   break;
	      default:
		   OutS(out, "??? ");
	  }
     switch (c->u.Const.type->typ) {
	 case Prim:
	      switch (c->u.Const.type->u.prim.basic) {
		  case Sint:
		       OutOffset(out, gbprintf(out->gb, "%d", c->u.Const.value.i));
		       return;
		       /*Manish 2/3 hack to print pointer constants */
		  case Uint:
		       OutOffset(out, gbprintf(out->gb, "%uU", c->u.Const.value.u));
		       return;
		  case Slong:
		       OutOffset(out, gbprintf(out->gb, "%ldL", c->u.Const.value.l));
		       return;
		  case Ulong:
		       OutOffset(out, gbprintf(out->gb, "%luUL", c->u.Const.value.ul));
		       return;
		  case Float:
		       OutOffset(out, print_float(out->gb, c->u.Const.value.f));
		       return;
		  case Double:
		       OutOffset(out, print_double(out->gb, c->u.Const.value.d));
		       return;
		  case Char:
		  case Schar:
		  case Uchar:
		       OutOffset(out, PrintChar(out->gb, c->u.Const.value.i));
		       return;
		  default:
		       Fail(__FILE__, __LINE__, "");
		       return;
	      }

	      /* Manish 2/3  Print Constant Pointers */
	 case Ptr:
	      OutOffset(out, gbprintf(out->gb, "%u", c->u.Const.value.u));
	      return;
	      /* Used for strings */
	 case Adcl:
	      OutOffset(out, PrintString(out->gb, c->u.Const.value.s));
	      return;

	 default:
	     FAIL("Unrecognized constant type");
     }
}

PRIVATE void output_prim_type (OutputContext *out, Node *type, char *lastc, Bool print_coords)
{
    const char *tn=TypeName(type->u.prim.basic);
    int tn_len=strlen(tn);
    assert(type);
    assert(type->typ == Prim);
    OutputTQ(out, tq_remove_sue_elaborated(type->u.prim.tq), print_coords? &type->coord : 0); /* Don't print sue_elaborated */
    if (print_coords) SetOutputCoord(out, &type->coord);
    OutS(out, tn);
    if (lastc)
	*lastc = tn[tn_len-1];
    return;
}

GLOBAL void output_prim_type_to_GBUF(GBUF *gb, Node *type, char *lastc) {
     OutputContext out;
     out.prev_char=' ';
     out.curr=DontPrintCoord;
     out.block = 0;
     out.block_indents[0]=0;
     out.gb = gb;
     return output_prim_type(&out, type, lastc, FALSE);
}

GLOBAL void output_constant_to_GBUF(GBUF * gb, Node *node, Bool with_name) {
     /* Make a dummy outputcontext.  This function is only called through the print_ast methods (and possibly for error messages)
      * so the exact coordinate is not very important. */
     OutputContext out;
     out.prev_char=' ';
     out.curr=UnknownCoord;
     out.block = 0;
     out.block_indents[0]=0;
     out.gb = gb;
     output_constant(&out, node, with_name);
}


PRIVATE void OutputInnerExpr(OutputContext *out, Node *node,
			     int enclosing_precedence, Context context)
{
     int my_precedence;
     Bool left_assoc;
     Bool parenthesize;

     if (node == NULL)
	  return;

     my_precedence = Precedence(node, &left_assoc);

     /* determine whether node needs enclosing parentheses */
     if (node->typ==Block) {
	  parenthesize = FALSE;
     } else if (node->parenthesized) {
	  parenthesize = TRUE;	/* always parenthesize if source did */
     } else if (my_precedence < enclosing_precedence) {
	  parenthesize = TRUE;
     } else if (my_precedence > enclosing_precedence) {
	  parenthesize = FALSE;
     } else 
	  /* my_precedence == enclosing_precedence */
	  if ((left_assoc && context == Right) || (!left_assoc && context == Left)) {
	       parenthesize = TRUE;
	  } else
	       parenthesize = FALSE;

     if (parenthesize) {
	  SetOutputCoord(out, &node->coord);
	  OutCh(out, '(');
     }

     switch (node->typ) {

	 case Block:
	      {
		   int np = node->parenthesized;
		   SetOutputCoord(out, &node->coord);
		   /* If the node has the parenthesized bit set, then when we print the statemet it will get parens.  But we will also get a semicolon*/
		   /* So save the parenthesized bit, clear it, and restore it.*/
		   node->parenthesized = 0;
		   OutCh(out, '(');
		   OutputStatement(out, node);
		   OutCh(out, ')');
		   node->parenthesized = np;
	      }
	      break;
	 case ImplicitCast:
	      /* ignore implicitcasts inserted for typechecking */
	      OutputInnerExpr(out, node->u.implicitcast.expr, enclosing_precedence, context);
	      break;
	 case Id:
	      SetOutputCoord(out, &node->coord);
	      OutS(out, node->u.id.text);
	      break;
	 case Const:
	      SetOutputCoord(out, &node->coord);
	      if (node->u.Const.text)
		   OutS(out, node->u.Const.text);
	      else
		   output_constant(out, node, FALSE);
	      break;
	 case Binop:
	      OutputBinop(out, node, my_precedence);
	      break;
	 case Unary:
	      OutputUnary(out, node, my_precedence);
	      break;
	 case Ternary:
	      OutputInnerExpr(out, node->u.ternary.cond, my_precedence, Left);
	      SetOutputCoord(out, &node->coord);
	      OutCh(out, '?');
	      OutputExpr(out, node->u.ternary.true);
	      SetOutputCoord(out, &node->u.ternary.colon_coord);
	      OutCh(out, ':');
	      OutputInnerExpr(out, node->u.ternary.false, my_precedence, Right);
	      break;
	 case Cast:
	      SetOutputCoord(out, &node->coord);
	      OutCh(out, '(');
	      OutputType(out, node->u.cast.type);
	      OutCh(out, ')');
	      OutputInnerExpr(out, node->u.cast.expr, my_precedence, Right);
	      break;
	 case Comma:
	      /*SetOutputCoord(out, &node->coord); // Don't set the output coordinate for comma expressions, since the first subexpression will place itself correctly.*/
	      OutputCommaList(out, node->u.comma.exprs);
	      break;
	 case Constructor:
	      SetOutputCoord(out, &node->coord);
	      OutCh(out, '(');
	      OutputType(out, node->u.constructor.type);
	      OutCh(out, ')');
	      OutputInnerExpr(out, node->u.constructor.initializerlist, 0, Right);
	      break;
	 case Array:
	      OutputArray(out, node, my_precedence);
	      break;
	 case Call:
	      OutputCall(out, node, my_precedence);
	      break;
	 case Initializer:
	      SetOutputCoordStmt(out, &node->coord);
	      OutCh(out, '{');
	      StartBlockIndent(out, node->u.initializer.exprs);
	      OutputCommaList(out, node->u.initializer.exprs);
	      EndBlockIndent(out);
	      SetOutputCoordStmt(out, &UnknownCoord);
	      OutCh(out, '}');
	      break;
	 case Text:
	      OutputTextNode(out, node);
	      break;
	 case AsmArg:
	      OutputExpr(out, node->u.AsmArg.constraint);
	      SetOutputCoordStmt(out, &node->coord);
	      OutCh(out, '(');
	      OutputExpr(out, node->u.AsmArg.expr);
	      OutCh(out, ')');
	      break;
         case BuiltinVaArg:
	      SetOutputCoord(out, &node->coord);
	      OutS(out, "__builtin_va_arg(");
	      OutputExpr(out, node->u.builtinvaarg.expr);
	      OutCh(out, ',');
	      OutputType(out, node->u.builtinvaarg.type);
	      OutCh(out, ')');
	      break;
	 default:
	      fprintf(stderr, "Internal error: unexpected node (%s:%d)\n", __FILE__, __LINE__);
	      fPrintNode(stderr, node, 2);
	      UNREACHABLE;
     }

     if (parenthesize)
	  OutCh(out, ')');
}

PRIVATE void OutputExpr(OutputContext *out, Node *node)
{
     OutputInnerExpr(out, node, 0, Left);
}

GLOBAL int add_string_to_OutputContext (const char *string, void *output_context_v) {
     OutputContext *out = output_context_v;
     OutS(out, string);
     return 1;
}

GLOBAL void TQtoGBUF (GBUF *gb, TypeQual tq) {
     OutputContext out;
     out.prev_char=' ';
     out.curr=DontPrintCoord;
     out.block = 0;
     out.block_indents[0]=0;
     out.gb = gb;
     OutputTQ (&out, tq, 0);
}

PRIVATE void OutputTQ (OutputContext *out, TypeQual tq, Coord *coord)
/* Print the type qualifier at coord.  If the type qualifier is empty, then nothing is printed. */
{
     if (tq_has_anything(tq) && coord) {
	  SetOutputCoord(out, coord);
     }
#define ASF(_string) OutS(out, _string)
     if (tq_has_extension(tq)) {
	 ASF("__extension__ ");
     }

     /* storage class */
     if (tq_has_typedef(tq))
	  ASF("typedef ");
     else if (tq_has_extern(tq))
	  ASF("extern ");
     else if (tq_has_static(tq))
	  ASF("static ");
     else if (tq_has_auto(tq))
	  ASF("auto ");
     else if (tq_has_register(tq))
	  ASF("register ");
     else {
	  ; /* no explicit storage class */
     }

     /* location */
     if (tq_has_top_decl(tq)) 
	  ASF("top_decl ");
     else if (tq_has_block_decl(tq))
	  ASF("block_decl ");
     else if (tq_has_formal_decl(tq))
	  ASF("formal_decl ");
     else if (tq_has_su_decl(tq))
	  ASF("su_decl ");
     else if (tq_has_enum_decl(tq))
	  ASF("enum_decl ");
     else {
	  ; /* no explicit location */
     }

     if (tq_has_redundant_external_decl(tq))
	 ASF("redundant_extern ");

     if (tq_has_shared(tq))
	 ASF("shared ");
     if (tq_has_private(tq))
	 ASF("private ");
     
     if (tq_has_inline(tq)) {
          #ifdef inline
/* This hacking is the standard CPP idiom for stringifying the result of a macro expansion.
 * In this case, the macro inline might be defined in ../config.h as
 *   #define inline __inline
 * and we want to get the string "__inline"
 * Suppose we did
 *   STR(inline)
 * that would expand to
 *   #inline
 * which would expand to
 *   "inline"
 * which is not what we want.  If we do
 *  XSTR(inline)
 * that expands to
 *  STR(inline)
 * and then the args are expanded to give
 *  STR(__inline)
 * and then the STR macro is expanded to give
 *  #__inline
 * finally yielding
 *  "__inline"
 * The whole point of this hack is to produce a string for "inline" that will work with the
 * C compiler we are using,as determined by ../configure and stored in ../config.h
 */
#define STR(S) #S
#define XSTR(S) STR(S)
   	    ASF(XSTR(inline) " "); /* Use the system-specific inline from config.h  */
          #else
            ASF("inline ");
          #endif
     }
     if (tq_has_const(tq))
	  ASF("const ");
     if (tq_has_volatile(tq))
	  ASF("volatile ");
     if (tq_has_restrict(tq))
	  ASF("__restrict ");
     if (tq_has_complex(tq))
	  ASF("__complex__ ");
     if (tq_has_nooutput(tq)) {
	  ASF("__nooutput__ ");
     }
     if (tq_has_cilkonly(tq)) {
	  ASF("__cilkonly__ ");
     }
     if (tq_has_cilksafe(tq)) {
	  ASF("__cilksafe__ ");
     }
     if (tq_has_sue_elaborated(tq))
	  ASF("sue_elaborated ");
     if (tq_has_procedure(tq))
	  ASF("cilk ");
     if (tq_has_inlet(tq))
	  ASF("inlet ");

     if (tq.attributes) {
	  OutputAttribs(out, tq.attributes);
     }

}

/*
 * OutputPartialType() should be called twice: first with context==Left
 * to output type components appearing before the declared identifier,
 * then with context==Right to output type components appearing after.
 * sc is the storage class, which is emitted before all components in
 * the Left context.
 */
PRIVATE void OutputPartialType(OutputContext *out, Context context, Node *node,
			       TypeQual sc)
{
     if (node == NULL)
	  return;

     switch (node->typ) {
	      /* 
	       *  base types: primitive, typedef, SUE 
	       */
	 case Prim:
	      if (context == Left) {
		   OutputTQ(out, sc, &node->coord);
		   output_prim_type(out, node, &out->prev_char, TRUE);
	      }
	      /* no action in Right context */
	      break;

	 case Tdef:
	      if (context == Left) {
		   SetOutputCoord(out, &node->coord);
		   OutputTQ(out, sc, 0);
		   OutputTQ(out, node->u.tdef.tq, 0);
		   OutS(out, node->u.tdef.name);
	      }
	      /* no action in Right context */
	      break;

	 case Sdcl:
	 case Udcl:
	 case Edcl:
	      if (context == Left) {
		   SetOutputCoord(out, &node->coord);
		   OutputTQ(out, sc, 0);
		   OutputTQ(out, (TypeQual){tq_remove_sue_elaborated(node->u.sdcl.tq).tq, 0}, 0); /* Don't print the attributes here.  or sue_elaborated at all */
		   OutputSUE(out, node->u.sdcl.type, tq_has_sue_elaborated(node->u.sdcl.tq), node->u.sdcl.tq.attributes);
	      }
	      /* no action in Right context */
	      break;

	      /* 
	       *  type operators: pointer, array, function.
	       */
#define IS_ARRAY_OR_FUNC(n)  (n->typ == Adcl || n->typ == Fdcl)

	 case Ptr:
	      if (context == Left) {
		   OutputPartialType(out, context, node->u.ptr.type, sc);

		   if (IS_ARRAY_OR_FUNC(node->u.ptr.type))
			OutCh(out, '(');

		   SetOutputCoord(out, &node->coord);
		   OutCh(out, '*');
		   /* don't output "cilk" or "inlet" as keyword */
		   NodeUpdateTq(node, tq_remove_procedure);
		   NodeUpdateTq(node, tq_remove_inlet);
		   OutputTQ(out, node->u.ptr.tq, 0);
	      } else {
		   if (IS_ARRAY_OR_FUNC(node->u.ptr.type))
			OutCh(out, ')');
		   OutputPartialType(out, context, node->u.ptr.type, sc);
	      }
	      break;

	 case Adcl:
	      if (context == Left)
		   OutputPartialType(out, context, node->u.adcl.type, sc);
	      else {
		   SetOutputCoord(out, &node->coord);
		   OutCh(out, '[');
		   if (IsSourceExpression(node->u.adcl.dimp->dim))
			OutputExpr(out, node->u.adcl.dimp->dim);
		   OutCh(out, ']');

		   OutputPartialType(out, context, node->u.adcl.type, sc);
	      }
	      break;

	 case Fdcl:
	      if (context == Left) {
		   /* don't output "cilk" or "inlet" as keyword */
		   NodeUpdateTq(node, tq_remove_procedure);
		   NodeUpdateTq(node, tq_remove_inlet);
		   OutputTQ(out, node->u.fdcl.tq, &node->coord);
		   OutputPartialType(out, context, node->u.fdcl.returns, sc);
	      } else {
		   SetOutputCoord(out, &node->coord);
		   OutCh(out, '(');
		   OutputDeclList(out, FormalDecl, node->u.fdcl.args);
		   OutCh(out, ')');

		   OutputPartialType(out, context, node->u.fdcl.returns, sc);
	      }
	      break;

	 default:
	      fprintf(stderr, "Internal error: unexpected node (%s:%d)\n", __FILE__, __LINE__);
	      fPrintNode(stderr, node, 2);
	      UNREACHABLE;
     }
}

PRIVATE void OutputSUE(OutputContext *out, SUEtype *sue, Bool elaboratedp, List *attributes)
{

     assert(sue != NULL);

     switch (sue->typ) {
	 case Sdcl:
	      OutS(out, "struct");
	      break;
	 case Udcl:
	      OutS(out, "union");
	      break;
	 case Edcl:
	      OutS(out, "enum");
	      break;
	 default:
	      UNREACHABLE;
     }

     if (attributes) OutputAttribs(out, attributes);


     if (sue->name) {
	  OutCh(out, ' ');
	  OutS(out, sue->name);
     }
     if (elaboratedp) {
	  SetOutputCoordStmt(out, &sue->coord);
	  OutCh(out, '{');
	  StartBlockIndent(out, sue->fields);
	  OutputDeclList(out,
			 (sue->typ == Edcl) ? EnumConstDecl : SUFieldDecl,
			 sue->fields);
	  EndBlockIndent(out);
	  SetOutputCoordStmt(out, &sue->right_coord);
	  OutCh(out, '}');
	  if (sue->attributes) {
	       OutputAttribs(out, sue->attributes);
	  }
     }
}

PRIVATE void OutputType(OutputContext *out, Node *node)
{
     OutputPartialType(out, Left, node, EMPTY_TQ);
     OutputPartialType(out, Right, node, EMPTY_TQ);
}

PRIVATE void OutputTextNode(OutputContext *out, Node *node)
{
     assert(node->typ == Text);

     if (node->u.text.start_new_line)
	  ForceNewLine(out, &node->coord);
     else
	  SetOutputCoord(out, &node->coord);

     if (node->u.text.text)
	  OutS_embedded_newlines(out, node->u.text.text);
}

PRIVATE int isidentifierchar (unsigned char c) {
     return isalnum((int)c) || (c=='_');
}

PRIVATE int OutputDecl(OutputContext *out, DeclKind k, Node *node)
     /* Output a decl, and return 0 if nothing actually printed, else return 1. */
{
     assert(node != NULL);
     assert(node->typ == Decl);

     /* do not output __nooutput__ declarations */
     if (tq_has_nooutput(node->u.decl.tq))
	  return 0;

     SetOutputCoord(out, &node->coord);

     if (tq_has_extension(node->u.decl.tq)) {
	  OutS(out, "__extension__");
     }

     if (k != EnumConstDecl) {
	  TypeQual sc = NodeStorageClass(node);
	  List *attribs = NodeDeclTq(node).attributes;
	  OutputTQ(out, sc, &node->coord);
	  if (attribs) OutputAttribs(out, attribs);
	  OutputPartialType(out, Left, node->u.decl.type, EMPTY_TQ);

	  if (node->u.decl.attribs)
	       OutputAttribs(out, node->u.decl.attribs);

	  if (node->u.decl.name) {
	       SetOutputCoord(out, &node->coord);
	       if (isidentifierchar(out->prev_char))
		    OutCh(out, ' ');
	       OutS(out, node->u.decl.name);
	  }

	  OutputPartialType(out, Right, node->u.decl.type, EMPTY_TQ);
     } else {
	  SetOutputCoord(out, &node->coord);
	  OutS(out, node->u.decl.name);
	  if (node->u.decl.attribs)
	       OutputAttribs(out, node->u.decl.attribs);
     }
     switch (k) {
	 case SUFieldDecl:
	      if (node->u.decl.bitsize != NULL) {
		   OutCh(out, ':');
		   OutputExpr(out, node->u.decl.bitsize);
	      }
	      if (node->u.decl.asmdecl)
		   OutputAsmDecl(out, node->u.decl.asmdecl);
	      break;

	 case TopDecl:
	 case BlockDecl:
	 case EnumConstDecl:
	      if (node->u.decl.asmdecl)
		   OutputAsmDecl(out, node->u.decl.asmdecl);
	      if (IsSourceExpression(node->u.decl.init)) {
		   OutCh(out, '=');
		   OutputExpr(out, node->u.decl.init);
	      }
	      break;

	 case FormalDecl:
	      if (node->u.decl.asmdecl)
		   OutputAsmDecl(out, node->u.decl.asmdecl);
	      break;
     }
     return 1;
}

PRIVATE void OutputAttribs(OutputContext *out, List *attribs)
{
     if (out->prev_char!=' ') OutCh(out, ' ');
     OutS(out, "__attribute__((");
     while (attribs != NULL) {
	  Node *attrib = FirstItem(attribs);

	  /* no output of nocheck attribute  (nondeterminator support) */
	  if (strcmp(attrib->u.attrib.name, "nocheck") == 0) {
	      attribs = Rest(attribs);
	      continue;
	  }

	  if (attrib != NULL) {
	       ChildList *l = attrib->u.attrib.arglist;
	       Node *expr;
	       OutS(out, attrib->u.attrib.name);
	       if (l) {
		    ListMarker marker;
		    IterateList(&marker, l);
		    OutS(out, " (");
		    if (NextOnList(&marker, (GenericREF) & expr)) {
			 OutputExpr(out, expr);
			 while (NextOnList(&marker, (GenericREF) & expr)) {
			      OutCh(out, ',');
			      OutputExpr(out, expr);
			 }
		    }
		    OutS(out, ")");
	       }
	  }
	  attribs = Rest(attribs);
	  if (attribs)
	       OutCh(out, ',');
     }
     OutS(out, "))");
}

PRIVATE void OutputAsmDecl(OutputContext *out, Node *a)
{
     OutS(out, " __asm__(");
     OutputExpr(out, a->u.Asm.template);
     OutS(out, ")");
}

PRIVATE void OutputStatement(OutputContext *out, Node *node)
{
     if (node == NULL) {
	  /* empty statement */
	  OutCh(out, ';');
	  return;
     }
     switch (node->typ) {
	 case Id:
	 case Const:
	 case Binop:
	 case Unary:
	 case Cast:
	 case Ternary:
	 case Comma:
	 case Call:
	 case Array:
	 case ImplicitCast:
	      if (FormatReadably && IsUnknownCoord(node->coord))
		   SetOutputCoordStmt(out, &UnknownCoord);

	      OutputExpr(out, node);
	      OutCh(out, ';');
	      break;
	 case Label:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, node->u.label.name);
	      OutS(out, ":");
	      break;
	 case Switch:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "switch (");
	      OutputExpr(out, node->u.Switch.expr);
	      OutS(out, ") ");
	      OutputStatement(out, node->u.Switch.stmt);
	      break;
	 case Case:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "case ");
	      OutputExpr(out, node->u.Case.expr);
	      OutS(out, ": ");
	      break;
	 case Default:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "default: ");
	      break;
	 case If:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "if (");
	      OutputExpr(out, node->u.If.expr);
	      OutS(out, ") ");
	      OutputStatement(out, node->u.If.stmt);
	      break;
	 case IfElse:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "if (");
	      OutputExpr(out, node->u.IfElse.expr);
	      OutS(out, ") ");
	      OutputStatement(out, node->u.IfElse.true);
	      SetOutputCoordStmt(out, &node->u.IfElse.else_coord);
	      OutS(out, "else ");
	      OutputStatement(out, node->u.IfElse.false);
	      break;
	 case While:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "while (");
	      OutputExpr(out, node->u.While.expr);
	      OutS(out, ") ");
	      OutputStatement(out, node->u.While.stmt);
	      break;
	 case Do:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "do");
	      OutputStatement(out, node->u.Do.stmt);
	      SetOutputCoordStmt(out, &node->u.Do.while_coord);
	      OutS(out, "while (");
	      OutputExpr(out, node->u.Do.expr);
	      OutS(out, ");");
	      break;
	 case For:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "for (");
	      OutputExpr(out, node->u.For.init);
	      OutS(out, "; ");
	      OutputExpr(out, node->u.For.cond);
	      OutS(out, "; ");
	      OutputExpr(out, node->u.For.next);
	      OutS(out, ") ");
	      OutputStatement(out, node->u.For.stmt);
	      break;
	 case Goto:
	      assert(node->u.Goto.label->typ == Label);
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "goto ");
	      OutS(out, node->u.Goto.label->u.label.name);
	      OutCh(out, ';');
	      break;
	 case Continue:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "continue;");
	      break;
	 case Break:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "break;");
	      break;
	 case Return:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "return");
	      if (node->u.Return.expr) {
		   OutCh(out, ' ');
		   OutputExpr(out, node->u.Return.expr);
	      }
	      OutCh(out, ';');
	      break;
	 case Block:
	      SetOutputCoordStmt(out, &node->coord);
	      if (node->parenthesized) OutCh(out, '(');
	      OutCh(out, '{');

	      StartBlockIndent(out, node->u.Block.stmts);
	      OutputDeclList(out, BlockDecl, node->u.Block.decl);
	      OutputStatementList(out, node->u.Block.stmts);
	      EndBlockIndent(out);

	      SetOutputCoordStmt(out, &node->u.Block.right_coord);
	      OutCh(out, '}');
	      if (node->parenthesized) OutS(out, ");");
	      break;
	 case Text:
	      OutputTextNode(out, node);
	      break;
	      /* GCC asm extensions --- commented by fengmd */
	 case Asm:
	      SetOutputCoordStmt(out, &node->coord);
	      OutS(out, "__asm__");
	      if (tq_has_anything(node->u.Asm.tq)) {
		   OutCh(out, ' ');
		   OutputTQ(out, node->u.Asm.tq, 0);
	      }
	      OutS(out, " (");
	      OutputExpr(out, node->u.Asm.template);
	      OutS(out, " : ");
	      if (node->u.Asm.output) {
		   OutputCommaList(out, node->u.Asm.output);
	      }
	      OutS(out, " : ");
	      if (node->u.Asm.input) {
		   OutputCommaList(out, node->u.Asm.input);
	      }
	      if (node->u.Asm.clobbered) {
		   OutS(out, " : ");
		   OutputCommaList(out, node->u.Asm.clobbered);
	      }
	      OutS(out, ");");
	      break;
	      /***  This should really only be temporary.... */
	 case XBlock:
	      SetOutputCoordStmt(out, &node->coord);
	      if (node->parenthesized) OutCh(out, '(');
	      OutS(out, "xbegin");

	      StartBlockIndent(out, node->u.Block.stmts);
	      OutputDeclList(out, BlockDecl, node->u.Block.decl);
	      OutputStatementList(out, node->u.Block.stmts);
	      EndBlockIndent(out);

	      SetOutputCoordStmt(out, &node->u.Block.right_coord);
	      OutS(out, "xend");
	      if (node->parenthesized) OutS(out, ");");
	      break;
	 default:
	      fprintf(stderr, "Internal error: unexpected node (%s:%d)\n", __FILE__, __LINE__);
	      fPrintNode(stderr, node, 2);
	      UNREACHABLE;
     }

     if (PrintLiveVars && node->analysis.livevars) {
	  OutS(out, " /* ");
	  OutOffset(out, PrintAnalysis(out->gb, node));
	  OutS(out, " */");
     }
}

PRIVATE void OutputDeclList(OutputContext *out, DeclKind k, List *lst)
{
     ListMarker marker;
     Node *node;
     Bool firstp = TRUE;
     Bool commalistp;

     if (lst == NULL)
	  return;

     commalistp = (k == FormalDecl || k == EnumConstDecl);

     IterateList(&marker, lst);
     while (NextOnList(&marker, (GenericREF) & node)) {
	  /* ignore decls deleted by transformation */
	  if (node == NULL)
	       continue;

	  if (k != FormalDecl)
	       SetOutputCoordStmt(out, &UnknownCoord);

	  /* output Text nodes without inserting delimiters (like ",") */
	  if (node->typ == Text) {

	       /* if text node is an #include, it must be at the top level
	        * for us to output it. */
	       if (!strncmp(node->u.text.text, "#include", 8) && k != TopDecl)
		    continue;

	       OutputTextNode(out, node);
	       continue;
	  }
	  if (firstp)
	       firstp = FALSE;
	  else if (commalistp)
	       OutCh(out, ',');

	  switch (node->typ) {
	  case Proc:
	      /* an inlet in SerialVersion could be defined inside a function,
		 if so, k == BlockDecl --- commented by fengmd
		 assert(k == TopDecl);
	      */
	      assert(node->u.proc.decl->u.decl.type);
	      assert(node->u.proc.decl->u.decl.type->typ == Fdcl);

	      {
		  /* output code for CILK_WHERE_AM_I */
		  TypeQual tq = node->u.proc.decl->u.decl.tq;;
		  if (Transform) {

		      gbputs("\n#undef CILK_WHERE_AM_I", out->gb);
		      gbputs("\n#define CILK_WHERE_AM_I ", out->gb);
		      
		      if (tq_has_fast_procedure(tq))
			  gbputs("IN_FAST_PROCEDURE\n", out->gb);
		      else if (tq_has_slow_procedure(tq))
			  gbputs("IN_SLOW_PROCEDURE\n", out->gb);
		      else if (tq_has_fast_inlet(tq))
			  gbputs("IN_FAST_INLET\n", out->gb);
		      else if (tq_has_slow_inlet(tq))
			  gbputs("IN_SLOW_INLET\n", out->gb);
		      else if (tq_has_standalone_inlet(tq))
			  gbputs("IN_STANDALONE_INLET\n", out->gb);
		      else if (tq_has_c_code(tq))
			  gbputs("IN_C_CODE\n", out->gb);
		      else {
			  fprintf(stderr, "unknown type of procedure\n");
			  {
			       GBUF *gb=make_file_gbuf(stderr);
			       PrintTQ(gb, tq, 0, 0);
			       PrintNode(gb, node, 2);
			       free_gbuf(gb);
			  }
			  UNREACHABLE;
		      }
		      out->curr.line += 3;
		      out->curr.offset = 0;
		  } else {
		       assert(!tq_has_cilk_where_am_i_decl_quals(tq));
		  }
	      }
      
	      {
		  int r = OutputDecl(out, k, node->u.proc.decl);
		  assert(r);
	      }
 	      if (node->u.proc.body)
		  OutputStatement(out, node->u.proc.body);
	      else {
		  /* must output an empty pair of braces to distinguish this
		   * function definition (with an empty body) from a
		   * function prototype */
		  OutS(out, "{}");
	      }
	      break;

	  case Decl:
	      {
		  int r = OutputDecl(out, k, node);
		  if (r && !commalistp)
		      OutCh(out, ';');
	      }
	      break;

	  case Prim:
	  case Tdef:
	  case Adcl:
	  case Fdcl:
	  case Ptr:
	  case Sdcl:
	  case Udcl:
	  case Edcl:
	      OutputType(out, node);
	      if (k != FormalDecl)
		  OutCh(out, ';');
	      break;

	  default:
	      fprintf(stderr, "Internal error: unexpected node (%s:%d)\n", __FILE__, __LINE__);
	      fPrintNode(stderr, node, 2);
	      UNREACHABLE;
	  }
     }
}

PRIVATE void OutputStatementList(OutputContext *out, List *lst)
{
     ListMarker marker;
     Node *node;

     if (lst == NULL)
	  return;

     IterateList(&marker, lst);
     while (NextOnList(&marker, (GenericREF) & node))
	  OutputStatement(out, node);
}
