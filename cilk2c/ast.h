/* -*- c-indentation-style: "bsd" -*- */
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

#ifndef _AST_H_
#define _AST_H_

#include "basics.h"
#include "dataflow.h"
#include "wildcard.h"
#include "typequal.h"

FILE_IDENTITY(ast_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/ast.h $ $LastChangedBy: bradley $ $Rev: 2739 $ $Date: 2006-02-09 00:16:43 -0500 (Thu, 09 Feb 2006) $");

/* definition of Node data structure */

typedef enum {
     /* expression nodes */
     Const, Id, Binop, Unary, Cast, Comma, Constructor, Ternary, Array, Call, Initializer,
     ImplicitCast,

     /* statement nodes */
     Label, Switch, Case, Default, If, IfElse, While, Do, For, Goto, Continue,
     Break, Return, Block,

     /* type nodes */
     Prim, Tdef, Ptr, Adcl, Fdcl, Sdcl, Udcl, Edcl,

     /* GCC asm extension */
     Asm, AsmArg,

     /* GCC stdarg.h support */
     BuiltinVaArg,

     /* Cilk extensions */
     Spawn, Sync,
     InletCall, Abort, Synched, 
     XBlock, /* Not necessarily a Cilk-specific extension. */
     /* end Cilk extensions */

     /* declaration node */
     Decl,

     /* GCC __attribute__ extension */
     Attrib,

     /* procedure def node */
     Proc,

     /* random text and preprocessor command node */
     Text

} NodeType;

/*************************************************************************/
/*                                                                       */
/*                      Type representations                             */
/*                                                                       */
/*************************************************************************/

/* Type qualifiers, storage classes, and additional attributes now are found in typequal.h and typequal.c */


/* BasicType covers all of the different fundamental types.
 * New basic types should also be added to InitTypes(). */
typedef enum {
     /* Unspecified=0, */
     Uchar = 1, Schar, Char,
     Ushort, Sshort,
     Uint, Sint,
     Int_ParseOnly /* used only in parsing -- FinishPrimType converts to Sint */ ,
     Ulong, Slong,
     Ulonglong, Slonglong,
     Float, Double, Longdouble,
     Void, Ellipsis,
     MaxBasicType		/* must be last */
} BasicType;


/* 

While a BasicType is constructed during parsing, its high-order bits
(above the bits needed for the BasicType itself) are flags
representing type modifiers like signedness and length.  (Extension
languages may have additional flags for their own type modifiers.)
After the BasicType is finalized with FinishPrimType, these 
high-order bits are cleared and the BasicType is set to one of the
enumerated values above.  (Thus the final BasicType value is a small
integer, which will be faster in a switch statement than a bitfield
representation like Sshort == Short | Int.)


To add a new category of type modifier:

   1. Insert it in TypeSpecifier (below), being careful not to conflict
      with bits used by other modifiers.
   2. Change to BASIC2EXPLODED and EXPLODED2BASIC to convert it (below).
   3. Add merging and finishing logic to MergePrimTypes and FinishPrimType
      (type.c).
   4. Add type names to TypeSpecifierName (type.c).

*/

typedef enum { 
  /* Length -- 0 in these bits means no sign specified */
  Short    = 0x0100, 
  Long     = 0x0200, 
  Longlong = 0x0300,
  LengthMask = Short | Long | Longlong,


  /* Signedness -- 0 in these bits means no sign specified */
  Signed   = 0x0400,
  Unsigned = 0x0800,
  SignMask = Signed | Unsigned,


  /* INSERT EXTENSIONS HERE */ 

  /* Base type -- Char, Int, Float, Double, etc.  Uses subset of
     values defined for BasicType. */
  BasicTypeMask = 0x00FF

  } TypeSpecifier;


typedef struct {
  TypeSpecifier base;
  TypeSpecifier sign;
  TypeSpecifier length;
  /* INSERT EXTENSIONS HERE */ 
} ExplodedType;


#define BASIC2EXPLODED(bt, et)  \
   ((et).sign = (bt) & SignMask, (et).length = (bt) & LengthMask, \
  /* INSERT EXTENSIONS HERE */     \
    (et).base = (bt) & BasicTypeMask)

#define EXPLODED2BASIC(et, bt)  \
   ((bt) = (et).sign | (et).length | \
  /* INSERT EXTENSIONS HERE */     \
    (et).base)




typedef struct {
     NodeType typ;

     Bool complete;		/* true if struct has been elaborated with field list */
     Coord coord;		/* location of definition (left brace) */
     Coord right_coord;		/* location of right brace */

     Bool visited;		/* this struct has been visited in size calculation */
     Bool transformed;		/* this struct has been visited in transform.c */
     int size;
     int align;

     const char *name;
     List *fields;
     List *attributes;
} SUEtype;			/* struct/union/enum type */

/*************************************************************************/
/*                                                                       */
/*                         Node structure                                */
/*                                                                       */
/*************************************************************************/

/* ChildNode and ChildList are defined for documentation only: these
 * represent the tree-like links of the AST.  All other links point
 * across in the tree (e.g., decl for an Id node). Any procedure walking
 * the AST must follow only ChildNode links, or it runs the risk of falling
 * into a cycle. -- rcm */
typedef Node ChildNode;
typedef List ChildList;

/*
 * type that holds array dimensions.  We use this type to
 * force different declarations of the same array to share
 * the same dimension.  For example, in
 *
 *  extern int foo [];
 *  int foo [] = { 1, 2, 3 };
 *
 * we want to keep two separate array declarations, but both
 * should have size 3, as implied by the second declaration.
 */
typedef struct {
     ChildNode *dim;
     int size;
} ARRAYtype;


/*************************************************************************/
/*                                                                       */
/*                          Expression nodes                             */
/*                                                                       */
/*************************************************************************/

typedef struct {
	const char* text;  /* text will be NULL if constant was not
			      derived from source, but computed by 
			      type-checker. */
	ChildNode *type;  /* type is required to be one of:
			     Sint, Uint, Slong, Ulong,
			     Float, Double, or Adcl(Char) */
	union {
	  TARGET_INT    i;
	  TARGET_UINT   u;
	  TARGET_LONG   l;
	  TARGET_ULONG ul;
	  float         f;
	  double        d;
	  const char *  s;
	} value;
} ConstNode;

typedef struct {
	const char* text;
	Node *decl;  
	/* type is decl->u.decl.type */
	Node *value;
} idNode;

typedef struct {
     OpType op;
     ChildNode *left;
     ChildNode *right;
     Node *type;
     Node *value;
} binopNode;

typedef struct {
     OpType op;
     ChildNode *expr;
     Node *type;
     Node *value;
} unaryNode;

typedef struct {
     ChildNode *type;
     ChildNode *expr;
     Node *value;
} castNode;

typedef struct {
     ChildNode *type;
     ChildNode *initializerlist;
} constructorNode;

/* Formerly Comma was overloaded to represent both comma expressions and 
 * for brace-initializers.  The semantics of brace-initializers are quite
 * different, however, so it seemed clearer to eliminate the overloading and
 * introduce a new AST node, Initializer, instead. -- rcm */
typedef struct {
     ChildList *exprs;
} commaNode;

typedef struct {
     ChildNode *cond;
     ChildNode *true;
     ChildNode *false;
     Coord colon_coord;		/* source line and offset of colon */
     Node *type;
     Node *value;
} ternaryNode;

typedef struct {
     Node *type;
     ChildNode *name;
     ChildList *dims;
} arrayNode;

typedef struct {
     ChildNode *name;
     ChildList *args;
} callNode;

typedef struct {
     ChildList *exprs;
} initializerNode;

/* ImplicitCasts are used by the semantic checker to implicitly give
 * an expression a type or value "from above";  for instance, char variables
 * are implicitly converted to integers in expressions, not by
 * changing the type on the identifier node, but by inserting
 * a implicitcast above the identifier node which specifies its new
 * type-in-context. -- rcm */
typedef struct {
     ChildNode *expr;
     Node *type;
     Node *value;
} implicitcastNode;


/*************************************************************************/
/*                                                                       */
/*                          Statement nodes                              */
/*                                                                       */
/*************************************************************************/

typedef struct {
     const char *name;
     List *references;
     FlowValue label_values;
     Bool  declared;
} labelNode;



typedef struct {
     ChildNode *expr;
     ChildNode *stmt;
     List *cases;
     Bool has_default;		/* true if cases includes a Default node */
     struct SwitchCheck *check;	/* points to hash table of case
				 * expression values for
				 * duplicate-checking */
     FlowValue switch_values, break_values;
} SwitchNode;

typedef struct {
     ChildNode *expr;
     Node *container;
} CaseNode;

typedef struct {
     Node *container;
} DefaultNode;

/* If statement with no Else */
typedef struct {
     ChildNode *expr;
     ChildNode *stmt;
} IfNode;

typedef struct {
     ChildNode *expr;
     ChildNode *true;
     ChildNode *false;
     Coord else_coord;		/* coordinates of ELSE keyword */
} IfElseNode;

typedef struct {
     ChildNode *expr;
     ChildNode *stmt;
     FlowValue loop_values, break_values;
} WhileNode;

typedef struct {
     ChildNode *stmt;
     ChildNode *expr;
     Coord while_coord;		/* coordinates of WHILE keyword */
     FlowValue loop_values, continue_values, break_values;
} DoNode;

typedef struct {
	ChildNode *init;
	ChildNode *cond;
	ChildNode *next;
	ChildNode *stmt;
        ChildList *rwlist;  /* use for Nondeterminator to group read/write */
	FlowValue loop_values, continue_values, break_values;
} ForNode;

typedef struct {
     Node *label;
} GotoNode;

typedef struct {
     Node *container;
} ContinueNode;

typedef struct {
     Node *container;
} BreakNode;

typedef struct {
     ChildNode *expr;
     Node *proc;		/* points to Proc node containing this return stmt */
     Bool needs_sync;		/* children might be oustanding at this return */
     List *livevars, *dirtyvars;	/* live and dirty variables BEFORE
					 * return statement.  Used to implement
					 * implicit syncs.
					 */
} ReturnNode;

typedef struct {
	ChildList *decl;
	ChildList *stmts;
	Coord right_coord;  /* coordinates of right brace */
	Node *type;  /* the type of a {...} block is void;
			the type of a ({..}) block is the type of its last
			  statement (initially NULL, then filled in by
			  SemCheckNode) */
} BlockNode;

/*************************************************************************/
/*                                                                       */
/*                          Type nodes                                   */
/*                                                                       */
/*************************************************************************/

typedef struct {
     TypeQual tq;
     BasicType basic;
} primNode;

typedef struct {
     const char *name;
     TypeQual tq;
     Node *type;
} tdefNode;

typedef struct {
     TypeQual tq;
     ChildNode *type;
} ptrNode;

typedef struct {
     TypeQual tq;
     ChildNode *type;
     ARRAYtype *dimp;
} adclNode;

typedef struct {
     TypeQual tq;
     ChildList *args;
     ChildNode *returns;
     Node *proc_node;		/* reference to defining proc - used for
				 * dataflow analysis -KHR
				 */
} fdclNode;

typedef struct {
     TypeQual tq;
     SUEtype *type;		/* this is a child link iff SUE_ELABORATED(tq) */
} sdclNode;

typedef struct {
     TypeQual tq;
     SUEtype *type;		/* this is a child link iff SUE_ELABORATED(tq) */
} udclNode;

typedef struct {
     TypeQual tq;
     SUEtype *type;		/* this is a child link iff SUE_ELABORATED(tq) */
} edclNode;

/*************************************************************************/
/*                                                                       */
/*                          Other nodes                                  */
/*                                                                       */
/*************************************************************************/

typedef struct {
     const char *name;
     TypeQual tq;		/* storage class and decl qualifiers */
     ChildNode *type;
     ChildNode *init;		/* in other versions of c-parser, init is
				 * overloaded to be the offset for structs -- 
				 * but NOT in c-to-c */
     ChildNode *bitsize;
     int references;		/* number of references to declared name */
     List *attribs;		/* GCC __attribute__ declarations */
     ChildNode *asmdecl;	/* GCC asm declarations */

     Bool register_p;           /* Is it a register variable?   Don't expect each pass to keep this up-to-date.  If you need it, recompute it at the beginning of your pass. */
/* Cilk extensions */
     int kind;			/* kind of variable:
				 * 0 - not stored in frame (externs and such)
				 * 1 - register variable: copy kept in register,
				 *     stored to frame when necessary.
				 * 2 - frame variable: only kept in frame.
				 * 3 - spawn register variable: variable of kind 1
				 *     for the fast version and kind 2 for the
				 *     slow version.  Variables in the procedure's
				 *     scope modified by an inlet are also type 3.
				 * 4 - inlet formal argument.  kept in a register
				 *     for fast and slow versions of inlet, and
				 *     kept in an inlet argument area for the
				 *     independent version of the inlet.
				 */
     const char *scope;		/* scope name of variable's declaration */
} declNode;

typedef struct {
  const char *name;         /* name of attribute, like "packed" or "const" */
  ChildList *arglist;       /* expression arguments of attribute */
} attribNode;

typedef struct {
     ChildNode *decl;
     ChildNode *body;
     FlowValue return_values;
     Bool needs_sync;
     Bool needs_return;
} procNode;

typedef struct {
     const char *text;		/* may be NULL (treated same as "") */
     Bool start_new_line;	/* true ==> nothing but whitespace should appear
				 * before <text> on line */
} textNode;

/*************************************************************************/
/*                                                                       */
/*                            GCC asm Extensions                         */
/*                                                                       */
/*************************************************************************/

typedef struct {
     TypeQual tq;
     ChildNode *template;
     ChildList *output;
     ChildList *input;
     ChildList *clobbered;
} asmNode;

typedef struct {
     ChildNode *constraint;
     ChildNode *expr;
} asmargNode;

/*************************************************************************/
/*                                                                       */
/*                            GCC stdarg.h support                       */
/*                                                                       */
/*************************************************************************/
typedef struct {
    ChildNode *expr;
    ChildNode *type;
} builtinvaargNode;

/*************************************************************************/
/*                                                                       */
/*                            Extensions                                 */
/*                                                                       */
/*************************************************************************/

typedef struct {
     ChildNode *receiver;	/* may be NULL if thread/proc returns void */
     OpType assign_op;		/* assignment operator used to affect receiver */
     ChildNode *name;		/* always Id */
     ChildList *args;
     Bool in_inlet;		/* called from within an inlet_call */
} spawnNode;

typedef struct dummy {
    int dummy;
} DUMMY;

typedef struct {
   DUMMY dummy; /* ansi C says structs cannot be empty. */
} syncNode;

typedef struct {
    /* No longer can you have both an inlet and an implicit inlet. -Bradley*/
    /*ChildNode *receiver;*/	/* may be NULL if inlet returns void */
     ChildNode *name;		/* always Id */
     ChildList *args;
     Bool implicit_inlet;	/* whether the inlet being called is implicit */
} inletcallNode;

typedef struct {
     DUMMY dummy;
} abortNode;

typedef struct {
    DUMMY dummy;
} synchedNode;

/** The node type for transaction blocks.
 *   The structure of the node is identical to
 *   a normal BlockNode
 */
typedef struct {
	ChildList *decl;
	ChildList *stmts;
	Coord right_coord;  /* coordinates of right brace */
	Node *type;  /* the type of a {...} block is void;
			the type of a ({..}) block is the type of its last
			  statement (initially NULL, then filled in by
			  SemCheckNode) */
} XBlockNode;

/*************************************************************************/
/*                                                                       */
/*                            Node definition                            */
/*                                                                       */
/*************************************************************************/


struct nodeStruct {
	NodeType typ;
	Coord coord; /* location of first terminal in production.  Watch out for explicit parens, because the coord is the position of the paren.
		      * There was some code in output.c that would write the paren, and then set the output coordinate to that location when doing the next terminal.
		      * Generally, there is no reason to set the output coordinate until it is time to output the terminal. */

        List *pragmas_before, *pragmas_after;  /* Each node in the tree may have some pragmas before it or after it.
						* It is a big hassle to always be handling the pragmas,
						* so usually we are lazy and collect the pragmas after handling lots of tokens.
						* The pragmas are any text that is lexically positioned rather than syntactially positioned.
						* For example, if we ever modify the code to pass comments through, then we would stick them into the pragmas_before and pragmas_after.
						* Why have both pragmas_before and pragmas_after?  So we can be lazy and deal with the pragmas at top-level. */

	/* pass is incremented for each top-level call to PrintNode, so 
	   that PrintNode can print out decls anywhere they are used
	   without infinite recursion on recursive data structures. */
	short pass;

	/* parenthesized is set on expressions which were parenthesized
	   in the original source:  e.g., (x+y)*(w+z) would have 
	   parenthesized==TRUE on both PLUS nodes, and parenthesized==FALSE
	   on both MULT nodes. */
	short parenthesized;
	int print_uid;

	/* data-flow analysis information */
	Analysis analysis;

        /* for instrumenting read/write global memory for Nondeterminator */
	short nondeterminator;
        /* a boolean to indicate whether instrumentation need be done */
	short memorycheckedp;
        /* a boolean to indicate whether Nondeterminator warning is on/off */
	short memorywarningp;
  
        WildcardType wTyp;

	union {
		ConstNode Const;
		idNode id;
		binopNode binop;
		unaryNode unary;
		castNode cast;
		commaNode comma;
	        constructorNode constructor;
		ternaryNode ternary;
		arrayNode array;
		callNode call;
		initializerNode initializer;
		implicitcastNode implicitcast;
		builtinvaargNode builtinvaarg;
		labelNode label;
		SwitchNode Switch;
		CaseNode Case;
		DefaultNode Default;
		IfNode If;
		IfElseNode IfElse;
		WhileNode While;
		DoNode Do;
		ForNode For;
		GotoNode Goto;
		ContinueNode Continue;
		BreakNode Break;
		ReturnNode Return;
		BlockNode Block;
		primNode prim;
		tdefNode tdef;
		ptrNode ptr;
		adclNode adcl;
		fdclNode fdcl;
		sdclNode sdcl;
		udclNode udcl;
		edclNode edcl;
/*GCC asm extensions */
		asmNode Asm;
	        asmargNode AsmArg;
/* Cilk extensions */
	  spawnNode spawn;
	  syncNode sync;
	  inletcallNode inletcall;
	  abortNode cilkabort;
	  synchedNode synched;
	  XBlockNode xBlock;
/* end Cilk extensions */
	  declNode decl;
	  attribNode attrib;
	  procNode proc;
	  textNode text;
     } u;
};




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

          ASTSWITCH, ASTWALK, LISTWALK


  These macros abstracts over a switch statement applied to 
  node->typ, to simplify operations on the syntax tree.


ASTSWITCH
  Parameters:
      node    abstract syntax tree node

      CODE    #defined macro taking three parameters:

                   name       enumerated constant corresponding to node->typ
		   node       same as node passed to ASTSWITCH
		   union      pointer to field in node->u appropriate for
		                 this type of node


  ASTSWITCH expands into a big switch statement.  The code after each
  case of the switch statement is CODE(name, node, union), followed by
  a break.  (Thus CODE does not need to include a break statement.)
  The default case of the switch statement, which is executed when the
  node type is invalid, is an assertion failure.

  Typically, CODE will concatenate its name parameter with a prefix to create
  the name of a method function, then call that function with some parameters.
  Examples may be found in SemCheckNode (sem-check.c) and PrintNode 
  (print-ast.c).



ASTWALK
  Parameters:
      node    abstract syntax tree node

      CODE    #defined macro taking one parameter, which is a child
              pointer of <node>.  The parameter may be used in an 
	      assignment to change the child pointer.


  ASTWALK expands CODE once for each non-nil child of <node>.  
  (Child pointers which are NULL will not be seen by CODE.)
  It also walks over child lists of <node>, calling CODE on each
  member.

  Typically CODE will make a recursive call on each child node.
  For examples, see NodeCopy and DiscardCoords (ast.c).



LISTWALK
  Parameters:
      l       List pointer

      CODE    #defined macro taking one parameter, which is a
              member of list l.  The parameter may be used in an
              assignment to change the member of the list.


  LISTWALK executes CODE  for each non-nil member of <l>.
  For examples, see the definition of ASTWALK below.



* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ASTSWITCH(n, CODE)  \
switch (n->typ) { \
 case Const:         CODE(Const, n, &n->u.Const); break; \
 case Id:            CODE(Id, n, &n->u.id); break; \
 case Binop:         CODE(Binop, n, &n->u.binop); break; \
 case Unary:         CODE(Unary, n, &n->u.unary); break; \
 case Cast:          CODE(Cast, n, &n->u.cast); break; \
 case Comma:         CODE(Comma, n, &n->u.comma); break; \
 case Constructor:   CODE(Constructor, n, &n->u.constructor); break; \
 case Ternary:       CODE(Ternary, n, &n->u.ternary); break; \
 case Array:         CODE(Array, n, &n->u.array); break; \
 case Call:          CODE(Call, n, &n->u.call); break; \
 case Initializer:   CODE(Initializer, n, &n->u.initializer); break; \
 case ImplicitCast:  CODE(ImplicitCast, n, &n->u.implicitcast); break; \
 case Label:         CODE(Label, n, &n->u.label); break; \
 case Switch:        CODE(Switch, n, &n->u.Switch); break; \
 case Case:          CODE(Case, n, &n->u.Case); break; \
 case Default:       CODE(Default, n, &n->u.Default); break; \
 case If:            CODE(If, n, &n->u.If); break; \
 case IfElse:        CODE(IfElse, n, &n->u.IfElse); break; \
 case While:         CODE(While, n, &n->u.While); break; \
 case Do:            CODE(Do, n, &n->u.Do); break; \
 case For:           CODE(For, n, &n->u.For); break; \
 case Goto:          CODE(Goto, n, &n->u.Goto); break; \
 case Continue:      CODE(Continue, n, &n->u.Continue); break; \
 case Break:         CODE(Break, n, &n->u.Break); break; \
 case Return:        CODE(Return, n, &n->u.Return); break; \
 case Block:         CODE(Block, n, &n->u.Block); break; \
 case Prim:          CODE(Prim, n, &n->u.prim); break; \
 case Tdef:          CODE(Tdef, n, &n->u.tdef); break; \
 case Ptr:           CODE(Ptr, n, &n->u.ptr); break; \
 case Adcl:          CODE(Adcl, n, &n->u.adcl); break; \
 case Fdcl:          CODE(Fdcl, n, &n->u.fdcl); break; \
 case Sdcl:          CODE(Sdcl, n, &n->u.sdcl); break; \
 case Udcl:          CODE(Udcl, n, &n->u.udcl); break; \
 case Edcl:          CODE(Edcl, n, &n->u.edcl); break; \
/* GCC asm extensions */ \
 case Asm:	     CODE(Asm, n, &n->u.Asm); break; \
 case AsmArg:        CODE(AsmArg, n, &n->u.AsmArg); break; \
/* GCC builtin stdarg.h support */ \
 case BuiltinVaArg:  CODE(BuiltinVaArg, n, &n->u.builtinvaarg); break; \
/* Cilk extensions */ \
 case Spawn:         CODE(Spawn, n, &n->u.spawn); break; \
 case Sync:          CODE(Sync, n, &n->u.sync); break; \
 case InletCall:     CODE(InletCall, n, &n->u.inletcall); break; \
 case Abort:         CODE(Abort, n, &n->u.cilkabort); break; \
 case Synched:       CODE(Synched, n, &n->u.synched); break; \
 case XBlock:        CODE(XBlock, n, &n->u.xBlock); break; \
/* End Cilk extensions */ \
 case Decl:          CODE(Decl, n, &n->u.decl); break; \
 case Attrib:     CODE(Attrib, n, &n->u.attrib); break; \
 case Proc:          CODE(Proc, n, &n->u.proc); break; \
 case Text:          CODE(Text, n, &n->u.text); break; \
 default:            FAIL("unexpected node type"); /* no return required since this call doesn'treturn*/ \
 }


#define ASTWALK(n, CODE)  \
switch ((n)->typ) { \
 case Const:         if ((n)->u.Const.type) {CODE((n)->u.Const.type);} break; \
 case Id:            break; \
 case Binop:         if ((n)->u.binop.left) {CODE((n)->u.binop.left);} if ((n)->u.binop.right) {CODE((n)->u.binop.right);} break; \
 case Unary:         if ((n)->u.unary.expr) {CODE((n)->u.unary.expr);} break; \
 case Cast:          if ((n)->u.cast.type) {CODE((n)->u.cast.type);} if ((n)->u.cast.expr) {CODE((n)->u.cast.expr);} break; \
 case Constructor:   if ((n)->u.constructor.type) {CODE((n)->u.constructor.type);} if ((n)->u.constructor.initializerlist) {CODE((n)->u.constructor.initializerlist);} break; \
 case Comma:         if ((n)->u.comma.exprs) {LISTWALK((n)->u.comma.exprs, CODE);} break; \
 case Ternary:       if ((n)->u.ternary.cond) {CODE((n)->u.ternary.cond);} if ((n)->u.ternary.true) {CODE((n)->u.ternary.true);} if ((n)->u.ternary.false) {CODE((n)->u.ternary.false);} break; \
 case Array:         if ((n)->u.array.name) {CODE((n)->u.array.name);} if ((n)->u.array.dims) {LISTWALK((n)->u.array.dims, CODE);} break; \
 case Call:          if ((n)->u.call.name) {CODE((n)->u.call.name);} if ((n)->u.call.args) {LISTWALK((n)->u.call.args, CODE);} break; \
 case Initializer:   if ((n)->u.initializer.exprs) {LISTWALK((n)->u.initializer.exprs, CODE);} break; \
 case ImplicitCast:  if ((n)->u.implicitcast.expr) {CODE((n)->u.implicitcast.expr);} break; \
 case Label:         break; \
 case Switch:        if ((n)->u.Switch.expr) {CODE((n)->u.Switch.expr);} if ((n)->u.Switch.stmt) {CODE((n)->u.Switch.stmt);} break; \
 case Case:          if ((n)->u.Case.expr) {CODE((n)->u.Case.expr);} break; \
 case Default:       break; \
 case If:            if ((n)->u.If.expr) {CODE((n)->u.If.expr);} if ((n)->u.If.stmt) {CODE((n)->u.If.stmt);} break; \
 case IfElse:        if ((n)->u.IfElse.expr) {CODE((n)->u.IfElse.expr);} if ((n)->u.IfElse.true) {CODE((n)->u.IfElse.true);} if ((n)->u.IfElse.false) {CODE((n)->u.IfElse.false);} break; \
 case While:         if ((n)->u.While.expr) {CODE((n)->u.While.expr);} if ((n)->u.While.stmt) {CODE((n)->u.While.stmt);} break; \
 case Do:            if ((n)->u.Do.stmt) {CODE((n)->u.Do.stmt);} if ((n)->u.Do.expr) {CODE((n)->u.Do.expr);} break; \
 case For:           if ((n)->u.For.init) {CODE((n)->u.For.init);} if ((n)->u.For.cond) {CODE((n)->u.For.cond);} if ((n)->u.For.next) {CODE((n)->u.For.next);} if ((n)->u.For.stmt) {CODE((n)->u.For.stmt);} break; \
 case Goto:          break; \
 case Continue:      break; \
 case Break:         break; \
 case Return:        if ((n)->u.Return.expr) {CODE((n)->u.Return.expr);} break; \
 case Block:         if ((n)->u.Block.decl) {LISTWALK((n)->u.Block.decl, CODE);} if ((n)->u.Block.stmts) {LISTWALK((n)->u.Block.stmts, CODE);} break; \
 case Prim:          break; \
 case Tdef:          break; \
 case Ptr:           if ((n)->u.ptr.type) {CODE((n)->u.ptr.type);} break; \
 case Adcl:          if ((n)->u.adcl.type) {CODE((n)->u.adcl.type);} if ((n)->u.adcl.dimp->dim) {CODE((n)->u.adcl.dimp->dim);} break; \
 case Fdcl:          if ((n)->u.fdcl.args) {LISTWALK((n)->u.fdcl.args, CODE);} if ((n)->u.fdcl.returns) {CODE((n)->u.fdcl.returns);} break; \
 case Sdcl:          if (tq_has_sue_elaborated((n)->u.sdcl.tq)) { if ((n)->u.sdcl.type->fields) {LISTWALK((n)->u.sdcl.type->fields, CODE);} if ((n)->u.sdcl.type->attributes) {LISTWALK((n)->u.sdcl.type->attributes, CODE);}} break; \
 case Udcl:          if (tq_has_sue_elaborated((n)->u.udcl.tq)) { if ((n)->u.udcl.type->fields) {LISTWALK((n)->u.udcl.type->fields, CODE);} if ((n)->u.udcl.type->attributes) {LISTWALK((n)->u.udcl.type->attributes, CODE);}} break; \
 case Edcl:          if (tq_has_sue_elaborated((n)->u.edcl.tq)) { if ((n)->u.edcl.type->fields) {LISTWALK((n)->u.edcl.type->fields, CODE);} if ((n)->u.edcl.type->attributes) {LISTWALK((n)->u.edcl.type->attributes, CODE);}} break; \
 case Asm:	     if ((n)->u.Asm.template) {CODE((n)->u.Asm.template);} if ((n)->u.Asm.output) {LISTWALK((n)->u.Asm.output, CODE);} if ((n)->u.Asm.input) {LISTWALK((n)->u.Asm.input, CODE);} if ((n)->u.Asm.clobbered) {LISTWALK((n)->u.Asm.clobbered, CODE);} break; \
 case AsmArg:        if ((n)->u.AsmArg.constraint) {CODE((n)->u.AsmArg.constraint);} if ((n)->u.AsmArg.expr) {CODE((n)->u.AsmArg.expr);} break; \
/* GCC builtin stdarg.h support */ \
 case BuiltinVaArg:  if ((n)->u.builtinvaarg.expr) {CODE((n)->u.builtinvaarg.expr);} if ((n)->u.builtinvaarg.type) {CODE((n)->u.builtinvaarg.type);} break; \
 case Spawn:         if ((n)->u.spawn.receiver) {CODE((n)->u.spawn.receiver);} if ((n)->u.spawn.name) {CODE((n)->u.spawn.name);} if ((n)->u.spawn.args) {LISTWALK((n)->u.spawn.args, CODE);} break; \
 case Sync:          break; \
 case InletCall:     if ((n)->u.inletcall.name) {CODE((n)->u.inletcall.name);} if ((n)->u.inletcall.args) {LISTWALK((n)->u.inletcall.args, CODE);} break; \
 case Abort:         break; \
 case Synched:       break; \
 case XBlock:         if ((n)->u.xBlock.decl) {LISTWALK((n)->u.xBlock.decl, CODE);} if ((n)->u.xBlock.stmts) {LISTWALK((n)->u.xBlock.stmts, CODE);} break; \
 case Decl:          if ((n)->u.decl.type) {CODE((n)->u.decl.type);} if ((n)->u.decl.init) {CODE((n)->u.decl.init);} if ((n)->u.decl.bitsize) {CODE((n)->u.decl.bitsize);} break; \
 case Attrib:        if ((n)->u.attrib.arglist) {LISTWALK((n)->u.attrib.arglist, CODE);} break; \
 case Proc:          if ((n)->u.proc.decl) {CODE((n)->u.proc.decl);} if ((n)->u.proc.body) {CODE((n)->u.proc.body);} break; \
 case Text:          break; \
 default:            FAIL("Unrecognized node type"); break; \
 }


#define LISTWALK(l, CODE)  \
{ ListMarker _listwalk_marker; Node *_listwalk_ref; \
  IterateList(&_listwalk_marker, l); \
  while (NextOnList(&_listwalk_marker, (GenericREF)&_listwalk_ref)) { \
     if (_listwalk_ref) {CODE(_listwalk_ref);}                     \
     SetCurrentOnList(&_listwalk_marker, (Generic *)_listwalk_ref); \
  }\
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
                            AST constructors
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

GLOBAL Node *NewNode(NodeType typ);



GLOBAL Node *MakeConstSint(int value);
GLOBAL Node *MakeConstSintTextCoord(const char *text, int value, Coord coord);
GLOBAL Node *MakeConstPtr(unsigned int value);
GLOBAL Node *MakeConstPtrTextCoord(const char *text, unsigned int value, Coord coord);
GLOBAL Node *MakeConstUint(unsigned int value);
GLOBAL Node *MakeConstUintTextCoord(const char *text, unsigned int value, Coord coord);
GLOBAL Node *MakeConstSlong(long value);
GLOBAL Node *MakeConstSlongTextCoord(const char *text, long value, Coord coord);
GLOBAL Node *MakeConstUlong(unsigned long value);
GLOBAL Node *MakeConstUlongTextCoord(const char *text, unsigned long value, Coord coord);
GLOBAL Node *MakeConstFloat(float value);
GLOBAL Node *MakeConstFloatTextCoord(const char *text, float value, Coord coord);
GLOBAL Node *MakeConstDouble(double value);
GLOBAL Node *MakeConstDoubleTextCoord(const char *text, double value, Coord coord);
GLOBAL Node *MakeString(const char *value);
GLOBAL Node *MakeStringTextCoord(const char *text, const char *value, Coord coord);
GLOBAL Node *MakeId(const char *text);
GLOBAL Node *MakeIdCoord(const char *text, Coord coord);
GLOBAL Node *MakeIdFromDecl(Node *decl);
GLOBAL Node *MakeUnary(OpType op, Node *expr);
GLOBAL Node *MakeUnaryCoord(OpType op, Node *expr, Coord coord);
GLOBAL Node *MakeBinop(OpType op, Node *left, Node *right);
GLOBAL Node *MakeBinopCoord(OpType op, Node *left, Node *right, Coord coord);
GLOBAL Node *MakeCast(Node *type, Node *expr);
GLOBAL Node *MakeCastCoord(Node *type, Node *expr, Coord coord);
GLOBAL Node *MakeComma(List *exprs);
GLOBAL Node *MakeCommaCoord(List *exprs, Coord coord);
GLOBAL Node *MakeTernary(Node *cond, Node *true, Node *false);
GLOBAL Node *MakeTernaryCoord(Node *cond, Node *true, Node *false, Coord qmark_coord, Coord colon_coord);
GLOBAL Node *MakeArray(Node *name, List *dims);
GLOBAL Node *MakeArrayCoord(Node *name, List *dims, Coord coord);
GLOBAL Node *MakeCall(Node *name, List *args);
GLOBAL Node *MakeCallCoord(Node *name, List *args, Coord coord);
GLOBAL Node *MakeInitializer(List *exprs);
GLOBAL Node *MakeInitializerCoord(List *exprs, Coord coord);
GLOBAL Node *MakeImplicitCast(Node *type, Node *expr);
GLOBAL Node *MakeImplicitCastCoord(Node *type, Node *expr, Coord coord);
GLOBAL Node *MakeBuiltinVaArg(Node *type, Node *expr);
GLOBAL Node *MakeBuiltinVaArgCoord(Node *type, Node *expr, Coord coord);
GLOBAL Node *MakeLabel(const char *name);
GLOBAL Node *MakeLabelCoord(const char *name, Coord coord);
GLOBAL Node *MakeSwitch(Node *expr, Node *stmt, List *cases);
GLOBAL Node *MakeSwitchCoord(Node *expr, Node *stmt, List *cases, Coord coord);
GLOBAL Node *MakeCase(Node *expr, Node *container);
GLOBAL Node *MakeCaseCoord(Node *expr, Node *container, Coord coord);
GLOBAL Node *MakeDefault(Node *container);
GLOBAL Node *MakeDefaultCoord(Node *container, Coord coord);
GLOBAL Node *MakeIf(Node *expr, Node *stmt);
GLOBAL Node *MakeIfCoord(Node *expr, Node *stmt, Coord coord);
GLOBAL Node *MakeIfElse(Node *expr, Node *true, Node *false);
GLOBAL Node *MakeIfElseCoord(Node *expr, Node *true, Node *false, Coord if_coord, Coord else_coord);
GLOBAL Node *MakeWhile(Node *expr, Node *stmt);
GLOBAL Node *MakeWhileCoord(Node *expr, Node *stmt, Coord coord);
GLOBAL Node *MakeDo(Node *stmt, Node *expr);
GLOBAL Node *MakeDoCoord(Node *stmt, Node *expr, Coord do_coord, Coord while_coord);
GLOBAL Node *MakeFor(Node *init, Node *cond, Node *next, Node *stmt);
GLOBAL Node *MakeForCoord(Node *init, Node *cond, Node *next, Node *stmt, Coord coord);
GLOBAL Node *MakeGoto(Node *label);
GLOBAL Node *MakeGotoCoord(Node *label, Coord coord);
GLOBAL Node *MakeContinue(Node *container);
GLOBAL Node *MakeContinueCoord(Node *container, Coord coord);
GLOBAL Node *MakeBreak(Node *container);
GLOBAL Node *MakeBreakCoord(Node *container, Coord coord);
GLOBAL Node *MakeReturn(Node *expr);
GLOBAL Node *MakeReturnCoord(Node *expr, Coord coord);
GLOBAL Node *MakeBlock(Node *type, List *decl, List *stmts);
GLOBAL Node *MakeBlockCoord(Node *type, List *decl, List *stmts, Coord left_coord, Coord right_coord);
GLOBAL Node *MakePrim(TypeQual tq, BasicType basic);
GLOBAL Node *MakePrimCoord(TypeQual tq, BasicType basic, Coord coord);
GLOBAL Node *MakeTdef(TypeQual tq, const char *name);
GLOBAL Node *MakeTdefCoord(TypeQual tq, const char *name, Coord coord);
GLOBAL Node *MakePtr(TypeQual tq, Node *type);
GLOBAL Node *MakePtrCoord(TypeQual tq, Node *type, Coord coord);
GLOBAL Node *MakeAdcl(TypeQual tq, Node *type, Node *dim);
GLOBAL Node *MakeAdclCoord(TypeQual tq, Node *type, Node *dim, Coord coord);
GLOBAL Node *MakeFdcl(TypeQual tq, List *args, Node *returns);
GLOBAL Node *MakeFdclCoord(TypeQual tq, List *args, Node *returns, Coord coord);
GLOBAL Node *MakeSdcl(TypeQual tq, SUEtype *type);
GLOBAL Node *MakeSdclCoord(TypeQual tq, SUEtype *type, Coord coord);
GLOBAL Node *MakeUdcl(TypeQual tq, SUEtype *type);
GLOBAL Node *MakeUdclCoord(TypeQual tq, SUEtype *type, Coord coord);
GLOBAL Node *MakeEdcl(TypeQual tq, SUEtype *type);
GLOBAL Node *MakeEdclCoord(TypeQual tq, SUEtype *type, Coord coord);
GLOBAL Node *MakeDecl(const char *name, TypeQual tq, Node *type, Node *init, Node *bitsize);
GLOBAL Node *MakeDeclCoord(const char *name, TypeQual tq, Node *type, Node *init, Node *bitsize, Coord coord);
GLOBAL Node *MakeAttrib(const char *name, ChildList *arglist);
GLOBAL Node *MakeAttribCoord(const char *name, ChildList *argl, Coord coord);
GLOBAL Node *MakeProc(Node *decl, Node *body);
GLOBAL Node *MakeProcCoord(Node *decl, Node *body, Coord coord);
GLOBAL Node *MakeText(const char *text, Bool start_new_line);
GLOBAL Node *MakeTextCoord(const char *text, Bool start_new_line, Coord coord);

/* Insert your new constructors here */
GLOBAL Node *MakeConstructor(Node *type, Node *initializerlist);
GLOBAL Node *MakeConstructorCoord(Node *type, Node *initializerlist, Coord coord);

GLOBAL Node *MakeAsm(TypeQual tq, Node *template, List *output, List *input, List *clobbered);
GLOBAL Node *MakeAsmCoord(TypeQual tq, Node *template, List *output, List *input, List *clobbered, Coord coord);

GLOBAL Node *MakeAsmArg(Node *constraint, Node *expr);
GLOBAL Node *MakeAsmArgCoord(Node *constraint, Node *expr, Coord coord);

GLOBAL Node *MakeSpawn(Node *receiver, OpType assign_op, Node *name, List *args, Bool in_inlet);
GLOBAL Node *MakeSpawnCoord(Node *receiver, OpType assign_op, Node *name, List *args, Bool in_inlet, Coord coord);
GLOBAL Node *MakeSync(void);
GLOBAL Node *MakeSyncCoord(Coord coord);

GLOBAL Node *MakeInletCall(Node *name, List *args);
GLOBAL Node *MakeInletCallCoord(Node *name, List *args, Coord coord);
GLOBAL Node *MakeAbort(void);
GLOBAL Node *MakeAbortCoord(Coord coord);
GLOBAL Node *MakeSynched(void);
GLOBAL Node *MakeSynchedCoord(Coord coord);

GLOBAL Node *MakeXBlock(Node *type, List *decl, List *stmts);
GLOBAL Node *MakeXBlockCoord(Node *type, List *decl, List *stmts, Coord left_coord, Coord right_coord);
/* End of new constructors */


/* Wildcard Constructors. */

GLOBAL Node *MakeWildcardId(const char *text);
GLOBAL Node *MakeWildcardIdCoord(const char *text, Coord coord);

GLOBAL Node *MakeWildcardExp(const char *text);
GLOBAL Node *MakeWildcardExpCoord(const char *text, Coord coord);

GLOBAL Node *MakeWildcardType(const char *text);
GLOBAL Node *MakeWildcardTypeCoord(const char *text, Coord coord);


GLOBAL Node *ConvertIdToTdef(Node *id, TypeQual tq, Node *type);
GLOBAL Node *ConvertIdToDecl(Node *id, TypeQual tq, Node *type, Node *init, Node *bitsize);
GLOBAL Node *ConvertIdToAttrib(Node *id, ChildList *arg);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
                           AST operations
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


typedef enum {
     NodeOnly,
     Subtree,
     NodeOnlyExact,
     SubtreeExact
} TreeOpDepth;

typedef enum {
     Preorder,
     Postorder
} WalkOrder;

typedef void (*WalkProc) (Node *, void *);

GLOBAL Node *NodeCopy(Node *from, TreeOpDepth d);
GLOBAL Node *SetCoords(Node *tree, Coord c, TreeOpDepth d); /* Returns the modified node. */
GLOBAL void WalkTree(Node *tree, WalkProc proc, void *ptr, WalkOrder order);
GLOBAL void PrintCoord (GBUF *out, Coord *c);

/* copy a list of nodes, using the TreeOpDepth to determine how to copy each node. */
GLOBAL List *ListCopyNodes(List*, TreeOpDepth d);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
                           AST predicates
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

GLOBAL Bool IsExpr(Node *node);
GLOBAL Bool IsStmt(Node *node);
GLOBAL Bool IsType(Node *node);
GLOBAL Bool IsDecl(Node *node);


GLOBAL Bool IsWildcard(Node *node);
GLOBAL Bool IsWildcardD(Node *node);

typedef int Kinds;
#define KIND_EXPR  1
#define KIND_STMT  2
#define KIND_TYPE  4
#define KIND_DECL  8

GLOBAL Kinds KindsOfNode(Node *node);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
               Other AST declarations and prototypes
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* operators.c */
#include "operators.h"
#ifndef _Y_TAB_H_
#define _Y_TAB_H_
#include "ANSI-C.h"		/* for definitions of operator symbols */
#endif

/* container.c */
GLOBAL void PushContainer(NodeType typ);
GLOBAL Node *PopContainer(Node *n);
GLOBAL Node *AddContainee(Node *c);

/* from various files */
#include "type.h"

GLOBAL List *GrabPragmas(List *stmts_or_decls);  /* from c4.l */
GLOBAL List *GrabOldPragmas(void);  /* from c4.l */
GLOBAL List *GrabNewPragmas(void);  /* from c4.l */




GLOBAL const char* GetNodeName(Node* n);
GLOBAL const char* GetWildcardName(Node *wildcardNode);


/* transform.c had this included as a special case.  I want the type checker to see the same definition,
 * so I moved it here.  Maybe there is a better place. -Bradley */
GLOBAL SUEtype *make_SUE(NodeType typ, const char *name, List *fields);


#endif				/* ifndef _AST_H_ */
