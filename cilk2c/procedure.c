
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/procedure.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

#include "conversions.h"	/* for UsualUnaryConversionsType */

/* From ANSI-C.y */
GLOBAL Bool OmitLookups();

/************************************************************************
* FunctionConflict
*
* This routine is called if a function declaration, i.e. an Fdcl, is parsed 
* and there is already a declaration.
*
* In general, either of ORIG or NEW could be a declaration,
* a prototype, or the start of a function definition and it is not possible to
* tell which is which.  In particular, the expression 
*
*       int f()
*
* may be an old-style function declaration that permits an arbitrary number
* of arguments of types compatible with "the usual unary conversions" or it
* may be a function definition for a function with no arguments.
* 
* We assume that
* In a "standard" program, the original expression is an ANSI-C prototype
* and the create expression is the function declaration
*
* In the general case, this confirms that the two specifications are
* the same.  If the create declaration forces a revision of the old one
*
*     e.g.    int foo();
*             int foo(void) { ... }
*
* then it is required that the OLD one be mutated accordingly since there
* are already references to the old one
*
\***********************************************************************/

/* Must mutate original if changes required for consistency */
GLOBAL void FunctionConflict(Node *orig, Node *create)
{
     Node *ofdcl, *nfdcl;

     assert(orig);
     assert(create);
     assert(orig->typ == Decl);
     assert(create->typ == Decl);

     ofdcl = NodeDataType(orig);
     nfdcl = NodeDataType(create);

     if (ofdcl->typ != Fdcl || nfdcl->typ != Fdcl)
	  goto Mismatch;


     /* 
      * Check if one declaration is T_PROCEDURE and the other is not.
      * BUG: I think the whole way we treat 'cilk' and 'inlet' keywords
      * is broken.  We treat 'cilk' like 'const', but 
      *
      *  const int foo()
      *
      * is not the same as
      *
      *  cilk int foo()
      *
      * The former returns a 'const int', but the latter does not return a
      * 'cilk int' !  - athena
      *
      * BTW, the following line is a hack :-)
      *   ((ofdcl->u.fdcl.tq ^ nfdcl->u.fdcl.tq) & (T_PROCEDURE | T_INLET))
      * Bradley rewrote it has follows:
      */
     if ((tq_has_procedure(ofdcl->u.fdcl.tq) != tq_has_procedure(nfdcl->u.fdcl.tq))
	 || (tq_has_inlet(ofdcl->u.fdcl.tq) != tq_has_inlet(nfdcl->u.fdcl.tq)))
	  goto Mismatch;

     /* The Result Type must be equal */
     if (!TypeEqual(ofdcl->u.fdcl.returns, nfdcl->u.fdcl.returns))
	  goto Mismatch;

     /* Inspect the parameter lists */
     {
	  List *optr = ofdcl->u.fdcl.args, *nptr = nfdcl->u.fdcl.args;

	  /* Are both definitions in prototype form? */
	  if (optr && nptr) {

	       /* Then every parameter must be compatible */
	       for (; optr && nptr; optr = Rest(optr), nptr = Rest(nptr)) {
		    Node *oitem = FirstItem(optr), *otype = NodeDataType(oitem),
		    *nitem = FirstItem(nptr), *ntype = NodeDataType(nitem);

		    if (!TypeEqualFormals(otype, ntype)) {
			 SetItem(optr, nitem);
			 goto Mismatch;
		    }

	       }

	       /* And the parameter lists must be of the same length */
	       if (optr || nptr)
		    goto Mismatch;
	  }
	  /* Check for <Type> f(void)  vs  <Type> f() */
	  else if (IsVoidArglist(optr));

	  /* Check for <Type> f()  vs  <Type> f(void) */
	  else if (IsVoidArglist(nptr))
	       ofdcl->u.fdcl.args = MakeNewList(PrimVoid);

	  /* Else the provided types must be the "usual unary conversions" */
	  else {

	       /* Either this loop will run */
	       for (; optr; optr = Rest(optr)) {
		    Node *oitem = FirstItem(optr), *otype = NodeDataType(oitem);
		    if (!TypeEqual(otype, UsualUnaryConversionType(otype)) ||
			IsEllipsis(otype))
			 goto Mismatch;
	       }

	       /* Or this one will */
	       for (; nptr; nptr = Rest(nptr)) {
		    Node *nitem = FirstItem(nptr), *ntype = NodeDataType(nitem);
		    if (!TypeEqual(ntype, UsualUnaryConversionType(ntype)) ||
			IsEllipsis(ntype))
			 goto Mismatch;
	       }
	  }
     }

     return;

   Mismatch:
     SyntaxErrorCoord(create->coord,
		      "identifier `%s' redeclared", VAR_NAME(orig));
     fprintf(stderr, "\tPrevious declaration: ");
     PRINT_COORD(stderr, orig->coord);
     fputc('\n', stderr);
     return;
}

#if 0
/* DEAD CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
GLOBAL Bool IsCompatibleFdcls(Node *node1, Node *node2)
{
     assert(node1->typ == Fdcl);
     assert(node2->typ == Fdcl);

#if 0
     printf("IsCompatibleFdcls\n");
     PrintNode(stdout, node1, 0);
     printf("\n");
     PrintNode(stdout, node2, 0);
     printf("\n");
#endif

     /* The Result Type must be equal */
     if (!TypeEqual(node1->u.fdcl.returns, node2->u.fdcl.returns))
	  return FALSE;

     /* Inspect the parameter lists */
     {
	  List *optr = node1->u.fdcl.args, *nptr = node2->u.fdcl.args;

	  /* Are both definitions in prototype form? */
	  if (optr && nptr) {

	       /* Then every parameter must be compatible */
	       for (; optr && nptr; optr = Rest(optr), nptr = Rest(nptr)) {
		    Node *oitem = FirstItem(optr), *otype = NodeDataType(oitem),
		    *nitem = FirstItem(nptr), *ntype = NodeDataType(nitem);

		    if (!TypeEqual(otype, ntype)) {
#if 0
			 PrintNode(stdout, otype, 0);
			 printf("\n");
			 PrintNode(stdout, ntype, 0);
			 printf("\n");
#endif
			 return FALSE;
		    }
	       }

	       /* And the parameter lists must be of the same length */
	       if (optr || nptr)
		    return FALSE;
	       else
		    return TRUE;
	  }
	  /* Check for <Type> f(void)  vs  <Type> f() */
	  else if (IsVoidArglist(optr))
	       return TRUE;

	  /* Check for <Type> f()  vs  <Type> f(void) */
	  else if (IsVoidArglist(nptr))
	       return TRUE;

	  /* Else the provided types must be the "usual unary conversions" */
	  else {

	       /* Either this loop will run */
	       for (; optr; optr = Rest(optr)) {
		    Node *oitem = FirstItem(optr), *otype = NodeDataType(oitem);

		    if (!TypeEqual(otype, UsualUnaryConversionType(otype)) ||
			IsEllipsis(otype))
			 return FALSE;
	       }

	       /* Or this one will */
	       for (; nptr; nptr = Rest(nptr)) {
		    Node *nitem = FirstItem(nptr), *ntype = NodeDataType(nitem);

		    if (!TypeEqual(ntype, UsualUnaryConversionType(ntype)) ||
			IsEllipsis(ntype))
			 return FALSE;
	       }

	       return TRUE;
	  }
     }
}
/* DEAD CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
#endif

/************************************************************************
* Routines for building Proc nodes
\***********************************************************************/

#define MAX_PROC_DEPTH 100
PRIVATE Node *CurrentProc[MAX_PROC_DEPTH] =
{NULL, NULL};
PRIVATE int proc_depth;		/* how many procedures we're inside */

/* a list of implicit inlets need be added into the procedure */
PRIVATE List *InsertedImplicitInlet;

/* a flag to indicate whether currently we are parsing a procedure or an inlet.
 * this is used to avoid InsertedImplicitInlet being overwritten in DefineProc
 * as now there is no inlet ast node */
PRIVATE Bool in_inlet = FALSE;

/* formal_conflict handles conflicts between a formal and
 * something of the same name in the same scope.  Since formals
 * are added at the beginning of the body (and the scope), the
 * only conflict can be with earlier formals for the same procedure */

PRIVATE void formal_conflict(Node *orig, Node *create)
{
     SyntaxErrorCoord(create->coord,
		      "formal `%s' used multiple times", VAR_NAME(orig));
}


GLOBAL Node *DemoteProcArgs(Node *fdcl)
{
     Node *arg;
     ListMarker marker;

     assert(fdcl != NULL);

     IterateList(&marker, fdcl->u.fdcl.args);
     while (NextOnList(&marker, (GenericREF) & arg)) {
	  if (arg->typ == Decl) {
	       Node *decltype = NodeDataType(arg);

	       /* convert Adcl to pointer */
	       if (decltype->typ == Adcl) {
		    /* 
		     * ``If the specification of an array type
		     * includes any type qualifiers, the element type
		     * is so-qualified, not the array type. If the
		     * specification of a function type includes any
		     * type qualifiers, the behavior is
		     * undefined.'' 
		     * 
		     * (WG14/N843 6.7.3.8)
		     */

		    Node *t = decltype->u.adcl.type;

		    /* the type qualifiers of the specification */
		    TypeQual tq = NodeTq(NodeDataTypeSuperior(arg));
		    
		    if (tq_has_anything(tq)) {
			 t = NodeCopy(t, NodeOnly);
			 /* add qualifiers to the element type */
			 NodeUpdateTq2(t, tq_union, tq);
		    }
		    /* the pointer type is never qualified, hence EMPTY_TQ */
		    arg->u.decl.type = 
			 MakePtrCoord(EMPTY_TQ, t, decltype->coord);
	       }
	  }
     }
     
     return fdcl;
}
	       

GLOBAL Node *DefineProc(Bool old_style, Node *decl)
{
     Node *fdcl, *arg;
     ListMarker marker;
     Bool first;

     assert(decl != NULL && decl->typ == Decl);

     fdcl = decl->u.decl.type;
     assert(fdcl != NULL);
     if (fdcl->typ != Fdcl) {
	  SyntaxErrorCoord(decl->coord, "expecting a function definition");
	  /* return Proc with no body */
	  return MakeProcCoord(decl, NULL, decl->coord);	/* IL */
     }

     fdcl = DemoteProcArgs(fdcl);

     /* since the parser uses the '{' to determine that
      * this is a definition, we know that yylex() just returned
      * the '{' and so has entered the scope for the upcoming body. */
     /* assert(*yytext == '{');  verify that last token was '{' */
     /* *yytext is not '{' when defining an implicit inlet - KHR */
     /* assert(Level == 0); not valid anymore - KHR */
     assert(proc_depth < MAX_PROC_DEPTH);

     decl = FinishDecl(decl);
     if (Level == 0)
	  NodeSetDeclLocation(decl, TQ_TOP_DECL);
     else
	  NodeSetDeclLocation(decl, TQ_BLOCK_DECL);

     /* 
      * We know that this is a function definition, rather than merely a
      * declaration.  Convert an empty arglist to (void) and then recheck
      * the definition against the external scope (where the previous
      * declarations will be).
      */
     if (fdcl->u.fdcl.args == NULL) {
	  fdcl->u.fdcl.args = MakeNewList(MakePrimCoord(EMPTY_TQ, Void, fdcl->coord));

	  /* Recheck against the prototype */
	  if (!OmitLookups()) {
	    InsertSymbol(Level == 0 ? Externals : Identifiers,
			 decl->u.decl.name,
			 decl,
			 (ConflictProc) FunctionConflict);
	  }
     }
     if (old_style) {
	  Node *var = decl;

	  /* Convert any formals of type ID to the default DECL */
	  IterateList(&marker, fdcl->u.fdcl.args);
	  while (NextOnList(&marker, (GenericREF) & arg))
	       if (arg->typ == Id)
		    ConvertIdToDecl(arg, TQ_FORMAL_DECL,
				    MakeDefaultPrimType(EMPTY_TQ, arg->coord),
				    NULL, NULL);


	  /* insert the declaration (for the first time) */
	  if (!OmitLookups()) {
	    var = InsertSymbol(Externals, decl->u.decl.name, var,
			       (ConflictProc) FunctionConflict);
	    InsertSymbol(Identifiers, decl->u.decl.name, var,
			 (ConflictProc) FunctionConflict);
	  }
     }

     /* enter scope of function body */
     EnterScope();

     /* add formals to the scope of the upcoming block */
     IterateList(&marker, fdcl->u.fdcl.args);
     first = TRUE;
     while (NextOnList(&marker, (GenericREF) & arg)) {
       if (arg->typ == Decl) {
	 if (!OmitLookups()) {
	   InsertSymbol(Identifiers,
			arg->u.decl.name,
			arg,
			(ConflictProc) formal_conflict);
	 }
       } else {
	 if (IsEllipsis(arg))
	   /* okay */ ;
	 else if (IsVoidType(arg)) {
	   if (!first)
	     SyntaxErrorCoord(arg->coord,
			      "void argument must be first");
	 } else {
	   SyntaxErrorCoord(arg->coord, "argument without a name");
	 }
       }
       first = FALSE;
     }
     /* initialize InsertedImplicitInlet for this procedure --- fengmd */
     if (!in_inlet)
	  InsertedImplicitInlet = NULL;

     /* return Proc with no body */
     CurrentProc[proc_depth] = MakeProcCoord(decl, NULL, decl->coord);
     proc_depth++;
     return CurrentProc[proc_depth - 1];
}

GLOBAL Node *SetProcBody(Node *proc, Node *block)
{
     assert(proc != NULL && proc->typ == Proc);
     assert(block == NULL || block->typ == Block);
     assert(proc->u.proc.decl != NULL && proc->u.proc.decl->typ == Decl);

     /* exit function body scope */
     ExitScope();

     /* add implicit inlets to the end of the block declarations --- fengmd */
     if (!in_inlet)
	  block->u.Block.decl = JoinLists(block->u.Block.decl,
					  InsertedImplicitInlet);

     proc->u.proc.body = block;

     if (block == NULL) {
	  WarningCoord(4, proc->u.proc.decl->coord,
		       "procedure `%s' has no code",
		       proc->u.proc.decl->u.decl.name);
     } else {
	  /* check for unreferenced/unresolved labels,
	   * all labels are now out of scope */
	  ResetSymbolTable(Labels);
     }

     proc_depth--;
     CurrentProc[proc_depth] = NULL;

     return (proc);
}

GLOBAL Node *AddReturn(Node *returnnode)
{
     assert(proc_depth > 0 && CurrentProc[proc_depth - 1] != NULL);
     assert(returnnode->typ == Return);
     returnnode->u.Return.proc = CurrentProc[proc_depth - 1];
     return returnnode;
}

/************************************************************************
* Support for old-style function definitions
\***********************************************************************/

GLOBAL Bool OldStyleFunctionDefinition = FALSE;

/* AddParameterTypes
 * 
 * This takes a old-style function declaration `decl', which has a
 * list of identifiers (Id nodes) as its argument list, and and a list of
 * the parameter declarations `types', and converts the list
 * of identifiers to a list of declarations (Decl nodes) with the types
 * determined by the declaration list.  It is called upon the reduction of a
 * procedure defined using the old-sytle function declaration.
 * 
 * In: decl = (Decl name (Fdcl args=(List Id's) returntype) NULL NULL)
 * types = (List Decl's)
 * Out : (Decl name (Fdcl args=(List Decl's) returntype) NULL NULL)
 */
GLOBAL Node *AddParameterTypes(Node *decl, List *types)
{
     Node *fdcl, *type, *id;
     List *ids;
     ListMarker tl, il;
     const char *name;
     Bool found;

     assert(decl != NULL && decl->typ == Decl);
     fdcl = decl->u.decl.type;
     assert(fdcl != NULL && fdcl->typ == Fdcl);
     ids = fdcl->u.fdcl.args;

     IterateList(&tl, types);
     while (NextOnList(&tl, (GenericREF) & type)) {
	  assert(type->typ == Decl);
	  name = type->u.decl.name;
	  found = FALSE;
	  IterateList(&il, ids);
	  while (NextOnList(&il, (GenericREF) & id)) {
	       if (id->typ == Id && id->u.id.text == name) {
		    /*
		     * if a name appears twice in the identifer list,
		     * it will be caught by DefineProc when its adds the
		     * formals to the scope of the body 
		     */
		    memcpy((char *) id, (char *) type,
			   sizeof(Node));	/* replace Id with Decl */
		    found = TRUE;
		    break;
	       } else if (id->typ == Decl) {
		    if (id->u.decl.name == name) {
			 SyntaxErrorCoord(type->coord,
			      "multiple declarations for parameter `%s'",
					  name);
			 found = TRUE;	/* name does exist */
			 break;
		    }
	       }
	  }
	  if (!found)
	       SyntaxErrorCoord(type->coord,
		     "declaration for nonexistent parameter `%s'", name);
     }

     /* check for missing declarations */
     IterateList(&il, ids);
     while (NextOnList(&il, (GenericREF) & id)) {
	  if (id->typ == Id) {
	       WarningCoord(2, id->coord,
			    "parameter `%s' defaults to signed int",
			    id->u.id.text);
	       ConvertIdToDecl(id, TQ_FORMAL_DECL,
			       MakeDefaultPrimType(EMPTY_TQ, id->coord),
			       NULL, NULL);
	  }
     }

     return decl;
}

/* AddDefaultParameterTypes
 * 
 * This takes a badly formed prototype, which parses like an old-style function
 * declarator without the type list, and sets all of the parameters to be
 * signed int.  The grammar has already complained about this declarator
 */
GLOBAL Node *AddDefaultParameterTypes(Node *decl)
{
     Node *fdcl, *id;
     ListMarker il;
     List *ids;

     assert(decl != NULL && decl->typ == Decl);
     fdcl = decl->u.decl.type;
     assert(fdcl != NULL && fdcl->typ == Fdcl);
     ids = fdcl->u.fdcl.args;

     /* Set the type */
     IterateList(&il, ids);
     while (NextOnList(&il, (GenericREF) & id))
	 ConvertIdToDecl(id, TQ_FORMAL_DECL,
			 MakeDefaultPrimType(EMPTY_TQ, id->coord), NULL, NULL);

     return decl;
}

/************************************************************************
* Routines for handling labels and goto statements
************************************************************************/

PRIVATE void label_conflict(Node *orig, Node *create)
{
     assert(orig);
     assert(create);
     assert(orig->typ == Label);
     assert(create->typ == Label);

     if (create->u.label.declared) {
	  if (orig->u.label.declared)
	       SyntaxErrorCoord(create->coord,
				"multiple definitions of label `%s'",
				orig->u.label.name);
	  else
	       /* Loop over all the references to the old one and point them here */
	  {
	       ListMarker marker;
	       Node *item;

	       IterateList(&marker, orig->u.label.references);
	       while (NextOnList(&marker, (GenericREF) & item)) {
		    assert(item->typ == Goto);
		    item->u.Goto.label = create;
	       }

	       /* Prevent EndOfScope checker from whining */
	       orig->u.label.declared = TRUE;
	       create->u.label.references = orig->u.label.references;
	  }
     }
}

GLOBAL Node *BuildLabel(Node *id)
{
     Node *var, *label;

     assert(id->typ == Id);
     if (!OmitLookups()) {
       assert(Level > 0);
     }

     label = id;
     label->typ = Label;
     label->u.label.name = id->u.id.text;  /* This is truly gross, but I guess it is portable.  It would be cleaner to create a new node. -Bradley*/
     label->u.label.references = NULL;
     label->u.label.declared = TRUE;

     
     var = OmitLookups() ? label: InsertSymbol(Labels,
						   label->u.label.name,
						   label,
						   (ConflictProc) label_conflict);
     return label;
}

GLOBAL Node *ResolveGoto(Node *id, Coord coord)
{
     Node *label, *goto_node;
     const char *name = id->u.id.text;

     assert(id->typ == Id);

     if (!LookupSymbol(Labels, name, (GenericREF) & label)) {
	  label = MakeLabelCoord(name, UnknownCoord);
	  label->u.label.declared = FALSE;
	  label->coord = id->coord;
	  if (!OmitLookups()) {
	    label = InsertSymbol(Labels, name, label,
				 (ConflictProc) label_conflict);
	  }
     }
     assert(label);
     assert(label->typ == Label);

     goto_node = id;
     goto_node->typ = Goto;
     goto_node->coord = coord;
     goto_node->u.Goto.label = label;

     label->u.label.references = ConsItem(goto_node, label->u.label.references);
     return (goto_node);
}

/* This is called for all labels at the end of a function definition.
 * The call chain is: SetProcBody -> ResetSymbolTable -> EndOfLabelScope */

GLOBAL void EndOfLabelScope(Node *label)
{
     assert(label);
     assert(label->typ == Label);

     if (!label->u.label.declared)
	  SyntaxErrorCoord(label->coord,
			   "undefined label `%s'",
			   label->u.label.name);
     else if (label->u.label.references == NULL)
	  WarningCoord(2, label->coord,
		       "unreferenced label `%s'",
		       label->u.label.name);
}


/* If we are ansionly, check to see if the the last statement in the list is a label. */
GLOBAL void  MaybeCheckLabelAtEndOfCompoundStatement(List *l) {
    if (l) {
	List *last = Last(l);
	Node *item = FirstItem(last);
	if (item) { 
	    switch (item->typ) {
	    case Label:
	    case Default:
	    case Case:
		if (ANSIOnly) {
		    SyntaxErrorCoord(item->coord,
				     "ISO C forbids label at end of compound statement");
		} else {
		    /* silently get rid of the label at the end of the compound statement by inserting a null statement */
		    JoinLists(last, List1(0));
		}
		return;
	    default:
		return;
	    }
	} else {
	    return;
	}
    }
}

