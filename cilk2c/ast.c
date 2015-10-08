/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 *  
 *************************************************************************/

/*
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
 * Copyright (c) 2003 Jim Sukha
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/ast.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

Coord UnknownCoord = { 
  /* line:   */ 0,
  /* offset: */ 0,
  /* file:   */	0
  };
Coord DontPrintCoord = { 
  /* line:   */ -2,
  /* offset: */ -2,
  /* file:   */	-2,
  };

/* use HeapNew() (defined in ast.h) to allocate whole objects */
GLOBAL void *HeapAllocate(unsigned int number, unsigned int size)
{
     return calloc(number, size);
}

GLOBAL void HeapFree(void *ptr)
{
     free(ptr);
}

GLOBAL Node *NewNode(NodeType typ)
{
     Node *create = HeapNew(Node);

    create->typ = typ;
    create->coord = UnknownCoord;
    create->pragmas_before = NULL;
    create->pragmas_after = NULL;
    create->parenthesized = FALSE;
    create->analysis.livevars = NULL;

    /* nondeterminator stuff */
    create->nondeterminator = FALSE;
    create->memorycheckedp = MemoryCheckedp;
    create->memorywarningp = MemoryWarningp;
    create->wTyp = NonWildcard;

    return(create);
}

GLOBAL ARRAYtype *NewARRAYtype(void)
{
     ARRAYtype *create = HeapNew(ARRAYtype);
     return create;
}



/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakeConstSint(int value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimSint;
     node->u.Const.value.i = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstSintTextCoord(const char *text, int value, Coord coord)
{
     Node *create = MakeConstSint(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstPtr(unsigned int value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PtrVoid;
     node->u.Const.value.u = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstPtrTextCoord(const char *text, unsigned int value, Coord coord)
{
     Node *create = MakeConstPtr(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstUint(unsigned int value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimUint;
     node->u.Const.value.u = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstUintTextCoord(const char *text, unsigned int value, Coord coord)
{
     Node *create = MakeConstUint(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstSlong(long value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimSlong;
     node->u.Const.value.l = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstSlongTextCoord(const char *text, long value, Coord coord)
{
     Node *create = MakeConstSlong(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstUlong(unsigned long value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimUlong;
     node->u.Const.value.ul = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstUlongTextCoord(const char *text, unsigned long value, Coord coord)
{
     Node *create = MakeConstUlong(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstFloat(float value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimFloat;
     node->u.Const.value.f = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstFloatTextCoord(const char *text, float value, Coord coord)
{
     Node *create = MakeConstFloat(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstDouble(double value)
{
     Node *node = NewNode(Const);
     node->u.Const.type = PrimDouble;
     node->u.Const.value.d = value;
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeConstDoubleTextCoord(const char *text, double value, Coord coord)
{
     Node *create = MakeConstDouble(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeString(const char *value)
{
     Node *node = NewNode(Const),
	  *adcl = MakeAdcl(EMPTY_TQ, PrimChar, MakeConstSint((signed int) (strlen(value) + 1)));

     node->u.Const.type = adcl;
     node->u.Const.value.s = value;	/* quotes stripped, escape sequences converted */
     node->u.Const.text = NULL;
     return node;
}

GLOBAL Node *MakeStringTextCoord(const char *text, const char *value, Coord coord)
{
     Node *create = MakeString(value);
     create->u.Const.text = text;
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeId(const char *text)
{
     Node *create = NewNode(Id);

     create->u.id.text = text;
     create->u.id.decl = NULL;
     return (create);
}

GLOBAL Node *MakeIdFromDecl(Node *decl) {
    Node *id;
    assert(decl->typ==Decl);
    id = MakeId(decl->u.decl.name);
    id->u.id.decl = decl;
    return id;    
}

GLOBAL Node *MakeIdCoord(const char *text, Coord coord)
{
     Node *create = MakeId(text);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeWildcardId(const char *text) {
  Node *create = MakeId(text);
  create->wTyp = WildcardD;
  return (create);
}

GLOBAL Node *MakeWildcardIdCoord(const char *text, Coord coord) {
  Node *create = MakeWildcardId(text);
  create->coord = coord;
  return (create);
}

GLOBAL Node *MakeWildcardExp(const char *text)
{
  Node *create = MakeId(text);
  create->wTyp = WildcardE;
  return (create);
}

GLOBAL Node *MakeWildcardExpCoord(const char *text, Coord coord)
{
  Node *create = MakeWildcardExp(text);
  create->coord = coord;
  return (create);
}

GLOBAL Node *MakeWildcardType(const char *text)
{
     Node *create = MakeTdef(EMPTY_TQ, text);
     create->wTyp = WildcardT;
     create->u.tdef.type = MakePrim(EMPTY_TQ, Int_ParseOnly);
     return (create);
}

GLOBAL Node *MakeWildcardTypeCoord(const char *text, Coord coord)
{
     Node *create = MakeWildcardType(text);
     create->coord = coord;
     return (create);
}


GLOBAL Node *MakeUnary(OpType op, Node *expr)
{
     Node *create = NewNode(Unary);

     if (op == '*')
	  op = INDIR;
     else if (op == '&')
	  op = ADDRESS;
     else if (op == '-')
	  op = UMINUS;
     else if (op == '+')
	  op = UPLUS;

     create->u.unary.op = op;
     create->u.unary.expr = expr;

     create->u.unary.type = NULL;
     create->u.unary.value = NULL;

     return create;
}

GLOBAL Node *MakeUnaryCoord(OpType op, Node *expr, Coord coord)
{
     Node *create = MakeUnary(op, expr);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeBinop(OpType op, Node *left, Node *right)
{
     Node *create = NewNode(Binop);

     create->u.binop.op = op;
     create->u.binop.left = left;
     create->u.binop.right = right;
     create->u.binop.type = NULL;
     create->u.binop.value = NULL;
     return (create);
}

GLOBAL Node *MakeBinopCoord(OpType op, Node *left, Node *right, Coord coord)
{
     Node *create = MakeBinop(op, left, right);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeCast(Node *type, Node *expr)
{
     Node *create = NewNode(Cast);

     create->u.cast.type = type;
     create->u.cast.expr = expr;
     create->u.cast.value = NULL;
     return (create);
}

GLOBAL Node *MakeCastCoord(Node *type, Node *expr, Coord coord)
{
     Node *create = MakeCast(type, expr);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeComma(List *exprs)
{
     Node *create = NewNode(Comma);
     create->u.comma.exprs = exprs;
     return (create);
}

GLOBAL Node *MakeCommaCoord(List *exprs, Coord coord)
{
     Node *create = MakeComma(exprs);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeConstructor(Node *type, Node *initializerlist)
{
     Node *create = NewNode(Constructor);

     create->u.constructor.type = type;
     create->u.constructor.initializerlist = initializerlist;
     return (create);
}

GLOBAL Node *MakeConstructorCoord(Node *type, Node *initializerlist, Coord coord)
{
     Node *create = MakeConstructor(type, initializerlist);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeTernary(Node *cond, Node *true, Node *false)
{
     Node *create = NewNode(Ternary);
     create->u.ternary.cond = cond;
     create->u.ternary.true = true;
     create->u.ternary.false = false;
     create->u.ternary.colon_coord = UnknownCoord;
     create->u.ternary.type = NULL;
     create->u.ternary.value = NULL;
     return (create);
}

GLOBAL Node *MakeTernaryCoord(Node *cond, Node *true, Node *false, Coord qmark_coord, Coord colon_coord)
{
     Node *create = MakeTernary(cond, true, false);
     create->coord = qmark_coord;
     create->u.ternary.colon_coord = colon_coord;
     return (create);
}

GLOBAL Node *MakeArray(Node *name, List *dims)
{
     Node *create = NewNode(Array);
     create->u.array.type = NULL;
     create->u.array.name = name;
     create->u.array.dims = dims;
     return (create);
}

GLOBAL Node *MakeArrayCoord(Node *name, List *dims, Coord coord)
{
     Node *create = MakeArray(name, dims);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeCall(Node *name, List *args)
{
     Node *create = NewNode(Call);
     create->u.call.name = name;
     create->u.call.args = args;
     create->coord = name->coord;
     return (create);
}

GLOBAL Node *MakeCallCoord(Node *name, List *args, Coord coord)
{
     Node *create = MakeCall(name, args);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeInitializer(List *exprs)
{
     Node *create = NewNode(Initializer);
     create->u.comma.exprs = exprs;
     return (create);
}

GLOBAL Node *MakeInitializerCoord(List *exprs, Coord coord)
{
     Node *create = MakeInitializer(exprs);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeImplicitCast(Node *type, Node *expr)
{
     Node *create = NewNode(ImplicitCast);

     create->u.implicitcast.type = type;
     create->u.implicitcast.expr = expr;
     create->u.implicitcast.value = NULL;
     return (create);
}

GLOBAL Node *MakeImplicitCastCoord(Node *type, Node *expr, Coord coord)
{
     Node *create = MakeImplicitCast(type, expr);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeBuiltinVaArg(Node *expr, Node *type) {
    Node *create = NewNode(BuiltinVaArg);
    create->u.builtinvaarg.expr = expr;
    create->u.builtinvaarg.type = type;
    return (create);
    
}
GLOBAL Node *MakeBuiltinVaArgCoord(Node *expr, Node *type, Coord coord)
{
    Node *create = MakeBuiltinVaArg(expr, type);
    create->coord = coord;
    return (create);
}

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakeLabel(const char *name)
{
     Node *create = NewNode(Label);
     create->u.label.name = name;
     create->u.label.references = NULL;
     create->u.label.declared = TRUE; /* By default, labels are declared. */
     return (create);
}

GLOBAL Node *MakeLabelCoord(const char *name, Coord coord)
{
     Node *create = MakeLabel(name);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeSwitch(Node *expr, Node *stmt, List *cases)
{
     Node *create = NewNode(Switch);
     create->u.Switch.expr = expr;
     create->u.Switch.stmt = stmt;
     create->u.Switch.cases = cases;
     create->u.Switch.has_default = FALSE;
     while (cases) {
	  Node *n = FirstItem(cases);
	  assert(n);
	  if (n->typ == Default)
	       create->u.Switch.has_default = TRUE;
	  cases = Rest(cases);
     }
     return (create);
}

GLOBAL Node *MakeSwitchCoord(Node *expr, Node *stmt, List *cases, Coord coord)
{
     Node *create = MakeSwitch(expr, stmt, cases);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeCase(Node *expr, Node *container)
{
     Node *create = NewNode(Case);
     create->u.Case.expr = expr;
     create->u.Case.container = container;
     return (create);
}

GLOBAL Node *MakeCaseCoord(Node *expr, Node *container, Coord coord)
{
     Node *create = MakeCase(expr, container);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeDefault(Node *container)
{
     Node *create = NewNode(Default);
     create->u.Default.container = container;
     return (create);
}

GLOBAL Node *MakeDefaultCoord(Node *container, Coord coord)
{
     Node *create = MakeDefault(container);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeIf(Node *expr, Node *stmt)
{
     Node *create = NewNode(If);
     create->u.If.expr = expr;
     create->u.If.stmt = stmt;
     return (create);
}

GLOBAL Node *MakeIfCoord(Node *expr, Node *stmt, Coord coord)
{
     Node *create = MakeIf(expr, stmt);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeIfElse(Node *expr, Node *true, Node *false)
{
     Node *create = NewNode(IfElse);
     create->u.IfElse.expr = expr;
     create->u.IfElse.true = true;
     create->u.IfElse.false = false;
     create->u.IfElse.else_coord = UnknownCoord;
     return (create);
}

GLOBAL Node *MakeIfElseCoord(Node *expr, Node *true, Node *false, Coord if_coord, Coord else_coord)
{
     Node *create = MakeIfElse(expr, true, false);
     create->coord = if_coord;
     create->u.IfElse.else_coord = else_coord;
     return (create);
}

GLOBAL Node *MakeWhile(Node *expr, Node *stmt)
{
     Node *create = NewNode(While);
     create->u.While.expr = expr;
     create->u.While.stmt = stmt;
     return (create);
}

GLOBAL Node *MakeWhileCoord(Node *expr, Node *stmt, Coord coord)
{
     Node *create = MakeWhile(expr, stmt);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeDo(Node *stmt, Node *expr)
{
     Node *create = NewNode(Do);
     create->u.Do.stmt = stmt;
     create->u.Do.expr = expr;
     create->u.Do.while_coord = UnknownCoord;
     return (create);
}

GLOBAL Node *MakeDoCoord(Node *stmt, Node *expr, Coord do_coord, Coord while_coord)
{
     Node *create = MakeDo(stmt, expr);
     create->coord = do_coord;
     create->u.Do.while_coord = while_coord;
     return (create);
}

GLOBAL Node *MakeFor(Node *init, Node *cond, Node *next, Node *stmt)
{
     Node *create = NewNode(For);
     create->u.For.init = init;
     create->u.For.cond = cond;
     create->u.For.next = next;
     create->u.For.stmt = stmt;
     return (create);
}

GLOBAL Node *MakeForCoord(Node *init, Node *cond, Node *next, Node *stmt, Coord coord)
{
     Node *create = MakeFor(init, cond, next, stmt);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeGoto(Node *label)
{
     Node *create = NewNode(Goto);
     create->u.Goto.label = label;
     return (create);
}

GLOBAL Node *MakeGotoCoord(Node *label, Coord coord)
{
     Node *create = MakeGoto(label);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeContinue(Node *container)
{
     Node *create = NewNode(Continue);
     create->u.Continue.container = container;
     return (create);
}

GLOBAL Node *MakeContinueCoord(Node *container, Coord coord)
{
     Node *create = MakeContinue(container);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeBreak(Node *container)
{
     Node *create = NewNode(Break);

     create->u.Break.container = container;
     return (create);
}

GLOBAL Node *MakeBreakCoord(Node *container, Coord coord)
{
     Node *create = MakeBreak(container);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeReturn(Node *expr)
{
     Node *create = NewNode(Return);

     create->u.Return.expr = expr;
     create->u.Return.proc = NULL;
     create->u.Return.needs_sync = FALSE;
     return (create);
}

GLOBAL Node *MakeReturnCoord(Node *expr, Coord coord)
{
     Node *create = MakeReturn(expr);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeBlock(Node *type, List *decl, List *stmts)
{
     Node *create = NewNode(Block);
     create->u.Block.type = type;
     create->u.Block.decl = decl;
     create->u.Block.stmts = stmts;
     create->u.Block.right_coord = UnknownCoord;
     return (create);
}

GLOBAL Node *MakeBlockCoord(Node *type, List *decl, List *stmts, Coord left_coord, Coord right_coord)
{
     Node *create = MakeBlock(type, decl, stmts);
     create->coord = left_coord;
     create->u.Block.right_coord = right_coord;
     return (create);
}

/*************************************************************************/
/*                                                                       */
/*                            Type nodes                                 */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakePrim(TypeQual tq, BasicType basic)
{
     Node *create = NewNode(Prim);

     create->u.prim.tq = tq;
     create->u.prim.basic = basic;

     create->print_uid=basic;        /* This is an ugly hack to make print_ast work. The problem is that ASTWALK doesn't visit every node in the AST.  It can avoid visiting the prim types.  */
     
     return (create);
}

GLOBAL Node *MakePrimCoord(TypeQual tq, BasicType basic, Coord coord)
{
     Node *create = MakePrim(tq, basic);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeTdef(TypeQual tq, const char *name)
{
     Node *create = NewNode(Tdef);
     create->u.tdef.name = name;
     create->u.tdef.tq = tq;
     create->u.tdef.type = NULL;
     return (create);
}

GLOBAL Node *MakeTdefCoord(TypeQual tq, const char *name, Coord coord)
{
     Node *create = MakeTdef(tq, name);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakePtr(TypeQual tq, Node *type)
{
     Node *create = NewNode(Ptr);
     create->u.ptr.tq = tq;
     create->u.ptr.type = type;
     return (create);
}

GLOBAL Node *MakePtrCoord(TypeQual tq, Node *type, Coord coord)
{
     Node *create = MakePtr(tq, type);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeAdcl(TypeQual tq, Node *type, Node *dim)
{
     Node *create = NewNode(Adcl);
     ARRAYtype *atype = NewARRAYtype();
     create->u.adcl.tq = tq;
     create->u.adcl.type = type;
     atype->dim = dim;
     atype->size = 0;
     create->u.adcl.dimp = atype;
     return (create);
}

GLOBAL Node *MakeAdclCoord(TypeQual tq, Node *type, Node *dim, Coord coord)
{
     Node *create = MakeAdcl(tq, type, dim);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeFdcl(TypeQual tq, List *args, Node *returns)
{
     Node *create = NewNode(Fdcl);
     create->u.fdcl.tq = tq;
     create->u.fdcl.args = args;
     create->u.fdcl.returns = returns;
     return (create);
}

GLOBAL Node *MakeFdclCoord(TypeQual tq, List *args, Node *returns, Coord coord)
{
     Node *create = MakeFdcl(tq, args, returns);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeSdcl(TypeQual tq, SUEtype *type)
{
     Node *create = NewNode(Sdcl);
     create->u.sdcl.tq = tq;
     create->u.sdcl.type = type;
     return (create);
}

GLOBAL Node *MakeSdclCoord(TypeQual tq, SUEtype *type, Coord coord)
{
     Node *create = MakeSdcl(tq, type);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeUdcl(TypeQual tq, SUEtype *type)
{
     Node *create = NewNode(Udcl);
     create->u.udcl.tq = tq;
     create->u.udcl.type = type;
     return (create);
}

GLOBAL Node *MakeUdclCoord(TypeQual tq, SUEtype *type, Coord coord)
{
     Node *create = MakeUdcl(tq, type);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeEdcl(TypeQual tq, SUEtype *type)
{
     Node *create = NewNode(Edcl);
     create->u.edcl.tq = tq;
     create->u.edcl.type = type;
     return (create);
}

GLOBAL Node *MakeEdclCoord(TypeQual tq, SUEtype *type, Coord coord)
{
     Node *create = MakeEdcl(tq, type);
     create->coord = coord;
     return (create);
}

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakeDecl(const char *name, TypeQual tq, Node *type, Node *init, Node *bitsize)
{
     Node *create = NewNode(Decl);
     create->u.decl.name = name;
     create->u.decl.tq = tq;
     create->u.decl.type = type;
     create->u.decl.init = init;
     create->u.decl.bitsize = bitsize;
     create->u.decl.references = 0;
     create->u.decl.attribs = NULL;
     create->u.decl.register_p = FALSE;
/* Cilk additions */
     create->u.decl.kind = 0;
     create->u.decl.scope = NULL;
     return (create);
}

GLOBAL Node *MakeDeclCoord(const char *name, TypeQual tq, Node *type, Node *init, Node *bitsize, Coord coord)
{
     Node *create = MakeDecl(name, tq, type, init, bitsize);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeAttrib(const char *name, ChildList *arglist)
{
     Node *create = NewNode(Attrib);
     create->u.attrib.name = name;
     create->u.attrib.arglist = arglist;
     return create;
}

GLOBAL Node *MakeAttribCoord(const char *name, ChildList *argl, Coord coord)
{
     Node *create = MakeAttrib(name, argl);
     create->coord = coord;
     return create;
}

GLOBAL Node *MakeProc(Node *decl, Node *body)
{
     Node *create = NewNode(Proc);
     create->u.proc.decl = decl;
     create->u.proc.body = body;
     create->u.proc.needs_sync = FALSE;
     create->u.proc.needs_return = FALSE;
     return (create);
}

GLOBAL Node *MakeProcCoord(Node *decl, Node *body, Coord coord)
{
     Node *create = MakeProc(decl, body);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeText(const char *text, Bool start_new_line)
{
     Node *create = NewNode(Text);
     create->u.text.text = text;
     create->u.text.start_new_line = start_new_line;
     return create;
}

GLOBAL Node *MakeTextCoord(const char *text, Bool start_new_line, Coord coord)
{
     Node *create = MakeText(text, start_new_line);
     create->coord = coord;
     return create;
}

/*************************************************************************/
/*                                                                       */
/*                            GCC asm Extensions                         */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakeAsm(TypeQual tq, Node *template, List *output, List *input, List *clobbered)
{
     Node *create = NewNode(Asm);
     create->u.Asm.tq = tq;
     create->u.Asm.template = template;
     create->u.Asm.output = output;
     create->u.Asm.input = input;
     create->u.Asm.clobbered = clobbered;
     return (create);
}

GLOBAL Node *MakeAsmCoord(TypeQual tq, Node *template, List *output, List *input, List *clobbered, Coord coord)
{
     Node *create = MakeAsm(tq, template, output, input, clobbered);
     create->coord = coord;
     return (create);
}

GLOBAL Node *MakeAsmArg(Node *constraint, Node *expr)
{
     Node *create = NewNode(AsmArg);
     create->u.AsmArg.constraint = constraint;
     create->u.AsmArg.expr = expr;
     return (create);
}
GLOBAL Node *MakeAsmArgCoord(Node *constraint, Node *expr, Coord coord)
{
     Node *create = MakeAsmArg(constraint, expr);
     create->coord = coord;
     return (create);
}

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *MakeSpawn(Node *receiver, OpType assign_op, Node *name, List *args, Bool in_inlet)
{
     Node *create = NewNode(Spawn);
     create->u.spawn.receiver = receiver;
     create->u.spawn.assign_op = assign_op;
     create->u.spawn.name = name;
     create->u.spawn.args = args;
     create->u.spawn.in_inlet = in_inlet;

     return create;
}

GLOBAL Node *MakeSpawnCoord(Node *receiver, OpType assign_op, Node *name, List *args, Bool in_inlet, Coord coord)
{
     Node *create = MakeSpawn(receiver, assign_op, name, args, in_inlet);
     create->coord = coord;
     return create;
}

GLOBAL Node *MakeSync()
{
     Node *create = NewNode(Sync);
     return create;
}

GLOBAL Node *MakeSyncCoord(Coord coord)
{
     Node *create = MakeSync();
     create->coord = coord;
     return create;
}

GLOBAL Node *MakeInletCall(Node *name, List *args)
{
     Node *create = NewNode(InletCall);
     create->u.inletcall.name = name;
     create->u.inletcall.args = args;
     return create;
}

GLOBAL Node *MakeInletCallCoord(Node *name, List *args, Coord coord)
{
     Node *create = MakeInletCall(name, args);
     create->coord = coord;
     return create;
}


GLOBAL Node *MakeAbort()
{
     Node *create = NewNode(Abort);
     return create;
}

GLOBAL Node *MakeAbortCoord(Coord coord)
{
     Node *create = MakeAbort();
     create->coord = coord;
     return create;
}

GLOBAL Node *MakeSynched()
{
     Node *create = NewNode(Synched);
     return create;
}

GLOBAL Node *MakeSynchedCoord(Coord coord)
{
     Node *create = MakeSynched();
     create->coord = coord;
     return create;
}

GLOBAL Node *MakeXBlock(Node *type, List *decl, List *stmts)
{
     Node *create = NewNode(XBlock);
     create->u.xBlock.type = type;
     create->u.xBlock.decl = decl;
     create->u.xBlock.stmts = stmts;
     create->u.xBlock.right_coord = UnknownCoord;
     return (create);
}

GLOBAL Node *MakeXBlockCoord(Node *type, List *decl, List *stmts, Coord left_coord, Coord right_coord)
{
     Node *create = MakeXBlock(type, decl, stmts);
     create->coord = left_coord;
     create->u.xBlock.right_coord = right_coord;
     return (create);
}

/*****************************************************************
 
                Converting nodes between types

*****************************************************************/

GLOBAL Node *ConvertIdToTdef(Node *id, TypeQual tq, Node *type)
{
     assert(id->typ == Id);
     id->typ = Tdef;
     id->u.tdef.name = id->u.id.text;
     id->u.tdef.tq = tq;
     id->u.tdef.type = type;
     return (id);
}

GLOBAL Node *ConvertIdToDecl(Node *id, TypeQual tq, Node *type, Node *init, Node *bitsize)
{
     assert(id->typ == Id);
     id->typ = Decl;
     id->u.decl.name = id->u.id.text;
     id->u.decl.tq = tq;
     id->u.decl.type = type;
     id->u.decl.init = init;
     id->u.decl.bitsize = bitsize;
     id->u.decl.references = 0;
     return (id);
}

GLOBAL Node *ConvertIdToAttrib(Node *id, ChildList *arglist)
{
     assert(id->typ == Id);
     id->typ = Attrib;
     id->u.attrib.name = id->u.id.text;
     id->u.attrib.arglist = arglist;
     return (id);
}

#if 0
/* dead code -- rcm */
GLOBAL Node *AdclFdclToPtr(Node *node)
{
     if (node->typ == Decl)
	  if (node->u.decl.type->typ == Adcl)
	       node->u.decl.type->typ = Ptr;
	  else if (node->u.decl.type->typ == Fdcl)
	       node->u.decl.type = MakePtrCoord(EMPTY_TQ, node->u.decl.type, node->coord);

     return node;
}
#endif

/*****************************************************************
 
                      Node-kind predicates

*****************************************************************/

GLOBAL Bool IsExpr(Node *node)
{ return KindsOfNode(node) & KIND_EXPR; }

GLOBAL Bool IsStmt(Node *node)
{ return KindsOfNode(node) & KIND_STMT; }

GLOBAL Bool IsType(Node *node)
{ return KindsOfNode(node) & KIND_TYPE; }

GLOBAL Bool IsDecl(Node *node)
{ return KindsOfNode(node) & KIND_DECL; }


GLOBAL Bool IsWildcard(Node *node)
{ return (node->wTyp != NonWildcard); }

GLOBAL Bool IsWildcardD(Node *node)
{ return (node->wTyp == WildcardD); }




/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfConst (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfId (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfBinop (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfUnary (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfCast (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfComma (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfConstructor (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfTernary (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfArray (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfCall (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfInitializer (void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfImplicitCast (void)
{ return KIND_EXPR | KIND_STMT; }

/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfLabel (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfSwitch (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfCase (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfDefault (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfIf (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfIfElse (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfWhile (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfDo (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfFor (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfGoto (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfContinue (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfBreak (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfReturn (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfBlock (void)
{ return ANSIOnly ? KIND_STMT : KIND_STMT | KIND_EXPR; }


/*************************************************************************/
/*                                                                       */
/*                             Type nodes                                */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfPrim (void)
{ return KIND_TYPE; }

PRIVATE Kinds KindsOfTdef (void)
{ return KIND_TYPE; }

PRIVATE Kinds KindsOfPtr (void)
{ return KIND_TYPE; }

PRIVATE Kinds KindsOfAdcl (void)
{ return KIND_TYPE; }

PRIVATE Kinds KindsOfFdcl (void)
{ return KIND_TYPE; }

PRIVATE Kinds KindsOfSdcl (void)
{ return KIND_TYPE | KIND_DECL; }

PRIVATE Kinds KindsOfUdcl (void)
{ return KIND_TYPE | KIND_DECL; }

PRIVATE Kinds KindsOfEdcl (void)
{ return KIND_TYPE | KIND_DECL; }

/*************************************************************************/
/*                                                                       */
/*                      Other nodes (decls et al.)                       */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfDecl (void)
{ return KIND_DECL; }

PRIVATE Kinds KindsOfAttrib (void)
{ return 0; }

PRIVATE Kinds KindsOfProc (void)
{ return KIND_DECL; }

PRIVATE Kinds KindsOfText (void)
{ return KIND_DECL | KIND_STMT; }


/*************************************************************************/
/*                                                                       */
/*                            GCC asm Extensions                         */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfAsm (void) 
{ return KIND_STMT; }
PRIVATE Kinds KindsOfAsmArg (void) 
{ return KIND_EXPR; }

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
PRIVATE Kinds KindsOfBuiltinVaArg(void)
{ return KIND_EXPR; }

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

PRIVATE Kinds KindsOfSync (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfSpawn (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfInletCall (void) 
{ return KIND_STMT; }

/*
PRIVATE Kinds KindsOfResume (void)
{ return KIND_STMT; }
*/

PRIVATE Kinds KindsOfAbort (void)
{ return KIND_STMT; }

PRIVATE Kinds KindsOfSynched(void)
{ return KIND_EXPR | KIND_STMT; }

PRIVATE Kinds KindsOfXBlock (void)
{ return KIND_STMT; }


/*************************************************************************/
/*                                                                       */
/*                           KindsOfNode                                 */
/*                                                                       */
/*************************************************************************/

GLOBAL Kinds KindsOfNode(Node *node)
{
#define CODE(name, node, union) return KindsOf##name()
     ASTSWITCH(node, CODE);
#undef CODE
     UNREACHABLE;
}

/*************************************************************************/
/*                                                                       */
/*                           AST Operations                              */
/*                                                                       */
/*************************************************************************/

GLOBAL Node *NodeCopy(Node *from, TreeOpDepth d)
{
  Node *new = NewNode(from->typ);
  *new = *from;

  switch (from->typ) {
    /* for nodes with sub-lists, make new copy of sub-list */
  case Comma:         new->u.comma.exprs = ListCopy(new->u.comma.exprs);  break;
  case Array:         new->u.array.dims = ListCopy(new->u.array.dims);  break;
  case Call:          new->u.call.args = ListCopy(new->u.call.args);  break;
  case Initializer:   new->u.initializer.exprs = ListCopy(new->u.initializer.exprs);  break;
  case Block:         new->u.Block.decl = ListCopy(new->u.Block.decl);  new->u.Block.stmts = ListCopy(new->u.Block.stmts);  break;
  case Fdcl:          new->u.fdcl.args = ListCopy(new->u.fdcl.args);  break;

  case Sdcl:
  case Udcl:
  case Edcl:
    /* if from elaborated a struct/union's fields, new should not,
       since it's sharing */
    if (d == NodeOnly || d == Subtree)
	NodeUpdateTq(new, tq_remove_sue_elaborated);
    break;
  case Spawn:          new->u.spawn.args = ListCopy(new->u.spawn.args);  break;
  case Asm:            new->u.Asm.output = ListCopy(new->u.Asm.output);  new->u.Asm.input = ListCopy(new->u.Asm.input);  new->u.Asm.clobbered = ListCopy(new->u.Asm.clobbered);  break;
  default:
    break;
  }

  if (d == Subtree || d == SubtreeExact) {
    /* recursively copy children */
#define CHILD(n)   n = NodeCopy(n, d)
    ASTWALK(new, CHILD)
#undef CHILD
  }
  return new;
}

GLOBAL List *ListCopyNodes(List* l, TreeOpDepth d) {
    if (l==NULL) return l;
    else return ConsItem(NodeCopy(FirstItem(l), d),
			 ListCopyNodes(Rest(l), d));
}


PRIVATE void SetCoordsNode(Node *node, Coord *c)
{
     node->coord = *c;

     /* handle special-case coordinates as well */
     switch (node->typ) {
	 case Ternary:
	      node->u.ternary.colon_coord = *c;
	      break;
	 case IfElse:
	      node->u.IfElse.else_coord = *c;
	      break;
	 case Do:
	      node->u.Do.while_coord = *c;
	      break;
	 case Block:
	      node->u.Block.right_coord = *c;
	      break;
	 case Sdcl:
	      if (tq_has_sue_elaborated(node->u.sdcl.tq)) {
		   node->u.sdcl.type->coord = *c;
		   node->u.sdcl.type->right_coord = *c;
	      }
	      break;
	 case Udcl:
	      if (tq_has_sue_elaborated(node->u.udcl.tq)) {
		   node->u.udcl.type->coord = *c;
		   node->u.udcl.type->right_coord = *c;
	      }
	      break;
	 case Edcl:
	      if (tq_has_sue_elaborated(node->u.edcl.tq)) {
		   node->u.edcl.type->coord = *c;
		   node->u.edcl.type->right_coord = *c;
	      }
	      break;
	 default:
	      break;
     }
}


/* SetCoords sets all coordinates on a node or subtree to c.
 * And it returns the node. */
GLOBAL Node *SetCoords(Node *node, Coord c, TreeOpDepth d)
{
  if (d == NodeOnly)
      SetCoordsNode(node, &c);
  else
      WalkTree(node, (WalkProc)SetCoordsNode, &c, Preorder);

  if (d == Subtree) {
      #define CHILD(n)   SetCoords(n, c, d)
      ASTWALK(node, CHILD);
      #undef CHILD
  }
  return node;
}


GLOBAL void WalkTree(Node *node, WalkProc proc, void *ptr, WalkOrder order)
{
  if (order == Preorder)
    proc(node, ptr);

  #define CHILD(n)   WalkTree(n, proc, ptr, order)
  ASTWALK(node, CHILD);
  #undef CHILD
  
  if (order == Postorder)
    proc(node, ptr);
}




GLOBAL const char* GetNodeName(Node* n) {

  if (n == NULL) {
    return NULL;
  } 
  switch(n->typ) {
  case Id:
    return n->u.id.text;
  case Decl:
    return n->u.decl.name;
  case Tdef:
    return n->u.tdef.name;
  default:
    return NULL;
  }
}



GLOBAL const char* GetWildcardName(Node* wildcardNode) {
  if (IsWildcard(wildcardNode)) {
    return GetNodeName(wildcardNode);    
  }
  else {
    return NULL;
  }
}
