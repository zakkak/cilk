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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/operators.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");


GLOBAL OpEntry Operator[MAX_OPERATORS];

/************************************************************************\
* Evaluation functions
*     used for statically evaluating constant expressions
\************************************************************************/

/************************************************************************\
* InitOperatorTable
\************************************************************************/

PRIVATE void SET_OP(int i, const char *text, const char *name, int unary, int binary)
{
     assert(i >= 0 && i < MAX_OPERATORS);
     assert(Operator[i].text == NULL);

     Operator[i].text = text;
     Operator[i].name = name;
     Operator[i].left_assoc = TRUE;
     Operator[i].unary_prec = unary;
     Operator[i].binary_prec = binary;
}

PRIVATE void RIGHT_ASSOC(int op)
{

     assert(op >= 0 && op < MAX_OPERATORS);
     assert(Operator[op].text != NULL);

     Operator[op].left_assoc = FALSE;
}

GLOBAL void InitOperatorTable()
{
     SET_OP(':',   ":",   "COLON", 0, 16);
     SET_OP(DESIGNATED_INITIALIZER,  "=", "DESIGNATED_INITIALIZER", 0, 16);       /* this is the .x=y syntax*/
     SET_OP(ARRAY_LABELED_INITIALIZER, "]=", "ARRAY_LABELED_INITIALIZER", 0, 16); /* this is the [x]=y syntax*/
     SET_OP(ARROW, "->", "ARROW", 0, 15);
     SET_OP('.', ".", "DOT", 0, 15);

     SET_OP('!', "!", "not", 14, 0);
     SET_OP('~', "~", "bitnot", 14, 0);
     SET_OP(ICR, "++", "ICR", 14, 0);
     SET_OP(POSTINC, "++", "postinc", 14, 0);
     SET_OP(PREINC, "++", "preinc", 14, 0);
     SET_OP(DECR, "--", "DECR", 14, 0);
     SET_OP(POSTDEC, "--", "postdec", 14, 0);
     SET_OP(PREDEC, "--", "predec", 14, 0);

     SET_OP(SIZEOF, "sizeof", "sizeof", 14, 0);
     SET_OP(ALIGNOF, "__alignof__", "alignof", 14, 0);
     SET_OP(IMAG, "__imag__", "__imag__", 14, 0);
     SET_OP(REAL, "__real__", "__real__", 14, 0);

     SET_OP(ADDRESS, "&", "addrof", 14, 0);
     SET_OP(INDIR, "*", "indir", 14, 0);
     SET_OP(UPLUS, "+", "UPLUS", 14, 0);
     SET_OP(UMINUS, "-", "neg", 14, 0);

     SET_OP('*', "*", "mul", 0, 13);
     SET_OP('/', "/", "div", 0, 13);
     SET_OP('%', "%", "mod", 0, 13);

     SET_OP('+', "+", "add", 0, 12);
     SET_OP('-', "-", "sub", 0, 12);

     SET_OP(LS, "<<", "lsh", 0, 11);
     SET_OP(RS, ">>", "rsh", 0, 11);

     SET_OP('<', "<", "lt", 0, 10);
     SET_OP('>', ">", "gt", 0, 10);
     SET_OP(LE, "<=", "le", 0, 10);
     SET_OP(GE, ">=", "ge", 0, 10);

     SET_OP(EQ, "==", "eq", 0, 9);
     SET_OP(NE, "!=", "ne", 0, 9);

     SET_OP('&', "&", "band", 0, 8);

     SET_OP('^', "^", "bxor", 0, 7);

     SET_OP('|', "|", "bor", 0, 6);

     SET_OP(ANDAND, "&&", "and", 0, 5);
     SET_OP(OROR, "||", "or", 0, 4);

     /* ternary operator has precedence three, but is handled separately */

     SET_OP('=', "=", "asgn", 0, 2);
     RIGHT_ASSOC('=');
     SET_OP(MULTassign, "*=", "*=", 0, 2);
     RIGHT_ASSOC(MULTassign);
     SET_OP(DIVassign, "/=", "/=", 0, 2);
     RIGHT_ASSOC(DIVassign);
     SET_OP(MODassign, "%=", "%=", 0, 2);
     RIGHT_ASSOC(MODassign);
     SET_OP(PLUSassign, "+=", "+=", 0, 2);
     RIGHT_ASSOC(PLUSassign);
     SET_OP(MINUSassign, "-=", "-=", 0, 2);
     RIGHT_ASSOC(MINUSassign);
     SET_OP(LSassign, "<<=", "<<=", 0, 2);
     RIGHT_ASSOC(LSassign);
     SET_OP(RSassign, ">>=", ">>=", 0, 2);
     RIGHT_ASSOC(RSassign);
     SET_OP(ANDassign, "&=", "&=", 0, 2);
     RIGHT_ASSOC(ANDassign);
     SET_OP(ERassign, "^=", "^=", 0, 2);
     RIGHT_ASSOC(ERassign);
     SET_OP(ORassign, "|=", "|=", 0, 2);
     RIGHT_ASSOC(ORassign);

     /* comma operator has precedence one, but is handled separately */
}

GLOBAL const char *OperatorName(OpType op)
{
     assert(op >= 0 && op < MAX_OPERATORS);
     return Operator[op].name;
}

GLOBAL const char *OperatorText(OpType op)
{
     assert(op >= 0 && op < MAX_OPERATORS);
     return Operator[op].text;
}

GLOBAL int PrintOp(GBUF *gb, OpType op)
{
     OpEntry *operator = &Operator[op];

     assert(op > 0 && op < MAX_OPERATORS);

     if (operator->text == NULL) {
	  fprintf(stderr, "unknown operator %d\n", op);
	  FAIL("");
     }
     gbputs(operator->text, gb);
     return strlen(operator->text);
   
}

GLOBAL int OpPrecedence(NodeType typ, OpType op, Bool *left_assoc)
{
     OpEntry *operator = &Operator[op];

     assert(op > 0 && op < MAX_OPERATORS);

     if (operator->text == NULL) {
	  fprintf(stderr, "unknown operator %d\n", op);
	  FAIL("");
     }
     if (typ == Binop) {
	  *left_assoc = operator->left_assoc;
	  return (operator->binary_prec);
     } else {
	  *left_assoc = FALSE;	/* all unary ops are right associative */
	  return (operator->unary_prec);
     }
     /* unreachable */
}

GLOBAL Bool IsAssignmentOp(OpType op)
{
     switch (op) {
	 case '=':
	 case MULTassign:
	 case DIVassign:
	 case MODassign:
	 case PLUSassign:
	 case MINUSassign:
	 case LSassign:
	 case RSassign:
	 case ANDassign:
	 case ERassign:
	 case ORassign:
	      return TRUE;
	 default:
	      return FALSE;
     }
}

/* What is the underlying operation?*/
GLOBAL OpType OpOfAssignmentOp(OpType op)
{
     switch (op) {
     case MULTassign:  return '*';
     case DIVassign:   return '/';
     case MODassign:   return '%';
     case PLUSassign:  return '+';
     case MINUSassign: return '-';
     case LSassign:    return LS;
     case RSassign:    return RS;
     case ANDassign:   return '&';
     case ERassign:    return '^';
     case ORassign:    return '|';
     default:
	 abort();
     }
}

GLOBAL Bool IsIncDecOp(OpType op)
{
     switch (op) {
	 case PREINC:
	 case PREDEC:
	 case POSTINC:
	 case POSTDEC:
	      return TRUE;

	 default:
	      return FALSE;
     }
}

GLOBAL Bool IsComparisonOp(OpType op)
{
     switch (op) {
	 case '!':
	 case EQ:
	 case NE:
	 case '<':
	 case LE:
	 case '>':
	 case GE:
	      return TRUE;
	 default:
	      return FALSE;
     }
}

GLOBAL Bool IsArithmeticOp(OpType op)
{
     switch (op) {
	 case '+':
	 case '-':
	 case '*':
	 case '/':
	 case '%':
	 case '|':
	 case '&':
	 case '^':
	 case ANDAND:
	 case OROR:
	 case LS:
	 case RS:
	      return TRUE;
	 default:
	      return FALSE;
     }
}
