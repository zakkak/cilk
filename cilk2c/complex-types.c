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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/complex-types.c $ $LastChangedBy: bradley $ $Rev: 2780 $ $Date: 2006-03-30 08:45:19 -0500 (Thu, 30 Mar 2006) $");

     /* From ANSI-C.y */
GLOBAL Bool OmitLookups();



/* SetBaseType:
   Follow chain of pointers, arrays, and functions to bottom,
   then set the base type, which should be NULL coming in.

   Example:
         In: base=int, complex=(Ptr (Adcl NULL))
                               [pointer to an array of ???]
	 Out:  (Ptr (Adcl int))
               [pointer to an array of int]
	 In: base=void, complex=(Ptr (Fdcl (int) NULL))
	                        [pointer to a function from int to ???]
	 Out:  (Ptr (Fdcl (int) void))
	       [pointer to a function from int to void]
*/

GLOBAL Node *SetBaseType(Node *complex, Node *base)
{
    Node *chain = complex;

    assert(complex != NULL);

    for (;;) {
	switch (chain->typ) {
	  case Ptr:
	    if (chain->u.ptr.type == NULL) {
		chain->u.ptr.type = base;
		return(complex);
	    } else {
		chain = chain->u.ptr.type;
	    }
	    continue;
	  case Adcl:
	    if (chain->u.adcl.type == NULL) {
		chain->u.adcl.type = base;
		return(complex);
	    } else {
		chain = chain->u.adcl.type;
	    }
	    continue;
	  case Fdcl:
	    if (chain->u.fdcl.returns == NULL) {
		chain->u.fdcl.returns = base;
		return(complex);
	    } else {
		chain = chain->u.fdcl.returns;
	    }
	    continue;
	  default:
	    Warning(1, "Internal Error!: invalid `complex' arg\n");
	    DPN(complex);
	    fprintf(stderr, "\n");
	    exit(1);
	    return(complex);  /* unreachable */
	}
    }
    /* unreachable */
}


GLOBAL Node *GetShallowBaseType(Node *complex)
{
  assert(complex != NULL);
  switch (complex->typ) {
  case Ptr:
    return complex->u.ptr.type;
  case Adcl:
    return complex->u.adcl.type;
  case Fdcl:
    return complex->u.fdcl.returns;
  default:
    return complex;
  }
}


GLOBAL Node *GetDeepBaseType(Node *complex)
{
    Node *chain = complex;

    assert(complex != NULL);

    for (;;) {
	assert(chain != NULL);
	switch (chain->typ) {
	  case Ptr:
	    chain = chain->u.ptr.type;
	    continue;
	  case Adcl:
	    chain = chain->u.adcl.type;
	    continue;
	  case Fdcl:
	    chain = chain->u.fdcl.returns;
	    continue;
	  default:
	    return(chain);
	}
    }
    /* unreachable */
}


/* Tell me if this type is all filled in (e.g.,, it is known that it is a "pointer to a pointer to an int", rather  than that it is a "pointer to a pointer to something" */
GLOBAL Bool DeepBaseTypeIsComplete(Node *complex)
{
    Node *chain = complex;

    while (chain) {
	switch (chain->typ) {
	  case Ptr:
	    chain = chain->u.ptr.type;
	    continue;
	  case Adcl:
	    chain = chain->u.adcl.type;
	    continue;
	  case Fdcl:
	    chain = chain->u.fdcl.returns;
	    continue;
	  default:
	    return TRUE;
	}
    }
    return FALSE;
}


GLOBAL Node *ExtendArray(Node *array, Node *dim, Coord coord)
{
    assert(array != NULL);
    if (array->typ == Array) {
	AppendItem(array->u.array.dims, dim);
	return array;
    } else {
	return MakeArrayCoord(array, MakeNewList(dim), coord);
    }
}

#if 0
GLOBAL Node *AddArrayDimension(Node *array, Node *dim)
{
    assert(array != NULL  &&  array->typ == Adcl);
    assert(array->u.adcl.dims != NULL);
    AppendItem(array->u.adcl.dims, dim);
    return(array);
}
#endif

/* ModifyDeclType:
   Modifies a current declaration with a pointer/array/function.
   Example:
      in: decl=(Decl x int NULL)  mod=(Ptr NULL)
      out: (Decl x (Ptr int) NULL)
*/
GLOBAL Node *ModifyDeclType(Node *decl, Node *mod)
{
    assert(decl != NULL && decl->typ == Decl);
    assert(mod != NULL &&
	   (mod->typ == Ptr || mod->typ == Adcl || mod->typ == Fdcl));
    if (decl->u.decl.type == NULL)
      decl->u.decl.type = mod;
    else
      decl->u.decl.type = SetBaseType(decl->u.decl.type, mod);
    return(decl);
}


PRIVATE void extern_conflict(Node *orig, Node *create)
{
  assert(orig   && orig->typ == Decl);
  assert(create && create->typ == Decl);

  /* "external declarations for the same identifier must agree in
     type and linkage" --  K&R2, A10.2 */
  if (!TypeEqual(orig->u.decl.type, create->u.decl.type))
    {
      SyntaxErrorCoord(create->coord,
		       "extern `%s' redeclared", VAR_NAME(orig));
      fprintf(stderr, "\tPrevious declaration: ");
      PRINT_COORD(stderr, orig->coord);
      fputc('\n', stderr);
      /* These were deleted between cilk 5.2.1 and cilk 5.3.2.  I don't know why. */
      /* orig->u.decl.type = create->u.decl.type; */
      /* orig->coord = create->coord; */
    }
}

PRIVATE void var_conflict(Node *orig, Node *create)
{
    /* the two are equal for redundant function/extern declarations */
    if (orig != create)
      SyntaxErrorCoord(create->coord,
		       "variable `%s' redeclared", VAR_NAME(orig));
}


GLOBAL Node *SetDeclType(Node *decl, Node *type, ScopeState redeclare)
{
    TypeQual sc;
    Node *var;

#if 0
    printf("\n%s:%d SetDeclType(decl, type, %d)\n", __FILE__, __LINE__, redeclare);
    DPN(decl);
    DPN(type);
#endif

    assert(decl != NULL && decl->typ == Decl);
    assert(type != NULL && IsType(type));


    if (decl->u.decl.type == NULL) {
	decl->u.decl.type = type;
    } else {  /* must be pointer/array/function of 'type' */
	decl->u.decl.type = SetBaseType(decl->u.decl.type, type);
    }

    if (type->typ == Ptr  ||  type->typ == Adcl  || type->typ == Fdcl) {
	/* these three will call SetDeclType again when
	   their base type is set, unless the type is already filled in */
	if (!DeepBaseTypeIsComplete(type))
	    return(decl);
    }

/*******************************************************

  Hereafter we finish up the decl, cleaning it up, moving
  storage classes to Decl node, and inserting it in the
  symbol table.

  Also move attributes up to the decl.

********************************************************/

    decl = FinishDecl(decl);
    //printf("%s:%d ", __FILE__, __LINE__); DPN(decl);
    sc = NodeStorageClass(decl);

    if (OldStyleFunctionDefinition) {
	/* this declaration is part of an old-style function definition,
	   so treat it as a formal parameter */
	if (redeclare != SU) redeclare = Formal;
    }

    if (tq_has_typedef(sc)) {
      var = decl;
    } else if (decl->u.decl.type->typ == Fdcl && (redeclare != Formal)) {

      /* the formal parameter line was added by Manish 2/2/94  this fixes bugs
	   like :  
                  int a(int f(int,int))
                  {}
                  
                  int b(int f(double,float)) 
                  {} 


		  */

	/* if the arglist contains Id's, then we are in the middle of
	   an old-style function definition, so don't insert the symbol.
	   It will be inserted by DefineProc */
      List *args = decl->u.decl.type->u.fdcl.args;

      if (args) {
	Node *first_arg = FirstItem(args);
	if (first_arg->typ == Id) return(decl);
      }

      /* normal function declaration, check for consistency with Externals */
      var = (OmitLookups() ? decl : InsertSymbol(Level == 0 ? Externals : Identifiers,
						    decl->u.decl.name,
						    decl,
						    (ConflictProc) FunctionConflict));
      
    } else if (tq_has_extern(sc) ||
	       (Level == 0  &&  redeclare == Redecl)) {
      /* top-level variable, check for consistency with Externals */
      var = (OmitLookups() ? decl : InsertSymbol(Externals,
						    decl->u.decl.name,
						    decl,
						    (ConflictProc) extern_conflict));
      
    } else var = decl;

    /* 
     * Check if decl is a redundant external declaration.  (See
     * description of T_REDUNDANT_EXTERNAL_DECL in ast.h.)
     */
    if (var != decl) {
      /* Name was already in Externals symbol table, so possibly redundant.
	 Look for previous declaration in scope */
      Generic *trash;
      if (LookupSymbol(Identifiers, decl->u.decl.name, &trash))
	/* decl could be redundant, but we don't know if it's a 
	   definition or declaration yet.  Mark it, and let SetDeclInit
	   decide. */
	NodeUpdateTq(decl, tq_add_redundant_external_decl);
    }

    switch (redeclare) {
      case NoRedecl:
	if (IsAType(decl->u.decl.name)) {
	    SyntaxErrorCoord(decl->coord,
			"invalid edeclaration of typedef `%s' with default type",
		        decl->u.decl.name);
	}
	/* falls through to Redecl */
      case Redecl:
	NodeSetDeclLocation(decl, Level == 0 ? TQ_TOP_DECL : TQ_BLOCK_DECL);

	/* add to current scope */
	if (!OmitLookups()) {
	  InsertSymbol(Identifiers, decl->u.decl.name, var,
		       (ConflictProc) var_conflict);
	}
	break;
      case SU:
	NodeSetDeclLocation(decl, TQ_SU_DECL);

	/* each struct/union has it own namespace for fields */
	break;
      case Formal:
	NodeSetDeclLocation(decl, TQ_FORMAL_DECL);

	if (tq_has_anything(sc) && !tq_has_register(sc)) {
	    SyntaxErrorCoord(decl->coord,
			     "illegal storage class for parameter `%s'",
			     decl->u.decl.name);
	    if (tq_has_typedef(sc)) break;
	    /* reset storage class for body */
	    NodeSetStorageClass(decl, EMPTY_TQ);
	}

	/* convert Adcl to pointer */
	{
	  Node *decltype = decl->u.decl.type;
	  
	  if (decltype->typ == Adcl)
	    decl->u.decl.type = MakePtrCoord(decltype->u.adcl.tq,
					     decltype->u.adcl.type,
					     decltype->coord);
	}	

	/* formals are not in scope yet; either
	   1) this is only a function declaration, in which case the
	      identifiers are only for documentation,  or
	   2) this is part of a function definition, in which case the
	      formal are not in scope until the upcoming function body.
	      In this case, the formals are added by DefineProc just
	      before the body is parsed. */
	break;
    }
#if 0
    printf("\n%s:%d SetDeclType returns\n", __FILE__, __LINE__);
    DPN(decl);
#endif
    return(decl);
}


GLOBAL Node *SetDeclInit(Node *decl, Node *init)
{
    assert(decl != NULL  &&  decl->typ == Decl);
    assert(decl->u.decl.init == NULL);
    decl->u.decl.init = init;

    if (init) {
      if (tq_has_redundant_external_decl(NodeTq(decl)))
	/* fix up misprediction made in SetDeclType.  
	   decl has an initializer, so it isn't redundant. */
	  NodeUpdateTq(decl, tq_remove_redundant_external_decl);
    }

    return(decl);
}

GLOBAL Node *SetDeclBitSize(Node *decl, Node *bitsize)
{
    assert(decl != NULL  &&  decl->typ == Decl);
    assert(decl->u.decl.bitsize == NULL);
    decl->u.decl.bitsize = bitsize;
    return(decl);
}

GLOBAL Node *SetDeclAttribs(Node *decl, TypeQual attribs)
{
     assert(decl != NULL && decl->typ == Decl);
     decl->u.decl.tq = tq_union(decl->u.decl.tq, attribs);
     return (decl);
}

GLOBAL Node *SetDeclAsm(Node *decl, Node *a)
{
    assert(decl != NULL  &&  decl->typ == Decl);
    assert(decl->u.decl.asmdecl == NULL);
    decl->u.decl.asmdecl = a;
    return(decl);
}

/*
   FinishDecl moves declaration qualifiers from the type to the Decl node,
   and does various other operations to turn a Decl into its final
   form.

   WARNING:  FinishDecl may be run more than once on a decl, so it
   should not blindly make unnecessary changes.

   Move attributes up as well as storage class.
*/
GLOBAL Node *FinishDecl(Node *decl)
{
  Node *deepbasetype;
  TypeQual tq, sc;

  assert(decl->typ == Decl);

  decl->u.decl.type = FinishType(decl->u.decl.type);
  deepbasetype = GetDeepBaseType(decl->u.decl.type);

  /* move decl qualifiers to decl */
  tq = NodeTq(deepbasetype);
  //printf("%s:%d deepbasetype is (are attributes there?)", __FILE__, __LINE__); DPN(deepbasetype);
  NodeUpdateTq(deepbasetype, tq_remove_all_decl_quals);
  //printf("%s:%d deepbasetype is (are attributes there?)", __FILE__, __LINE__); DPN(deepbasetype);
  NodeUpdateTq2(decl, tq_union, (TypeQual){tq_decl_quals(tq).tq,tq.attributes});
  //decl->u.decl.attribs = JoinLists(decl->u.decl.attribs, tq.attributes);
  //printf("%s:%d decl is (are attributes there?)", __FILE__, __LINE__); DPN(decl);
  minf_coord(&decl->coord, &deepbasetype->coord); /* when we move type qualifers around, we also need to note that the position of the declaration may have moved to an earlier spot. */

  /* check for incomplete struct/union/enum -- only if not typedef and
     not definition */
  sc = NodeStorageClass(decl);
  if (!tq_has_typedef(sc) && !(tq_has_extern(sc) && !decl->u.decl.init))
    VerifySUEcomplete(decl->u.decl.type);
  
  return decl;
}



/* 
   FinishType performs consistency checks that can't be conveniently 
   expressed in the grammar, some time after the type has been
   constructed.  It is called for both declarations and type names
   (such as in a cast or sizeof expression). 

   WARNING:  FinishType may be run more than once on a type, so it
   should not blindly make unnecessary changes.
*/
GLOBAL Node *FinishType(Node *type)
{
  Node *deepbasetype = GetDeepBaseType(type);
  TypeQual basetq = NodeTq(deepbasetype);

  if (tq_has_inline(basetq)) {
    if (ANSIOnly)
      SyntaxError("inline keyword not allowed with -ansi switch");
    else if (!IsFunctionType(type))
      WarningCoord(1, type->coord, 
		   "inline qualifier applies only to functions");
    else NodeUpdateTq(type, tq_add_inline);
    NodeUpdateTq(deepbasetype, tq_remove_inline);
    minf_coord(&type->coord, &deepbasetype->coord); /* when we move type qualifers around, we also need to note that the position of the declaration may have moved to an earlier spot. */
  }

  /* Insert your extensions here */
  if (tq_has_procedure(basetq)) {
    if (!IsFunctionType(type))
      {
	if (!IsPtrToFunction(type))
	  SyntaxErrorCoord(deepbasetype->coord, 
			   "cilk keyword applies only to functions");
	else
	  NodeUpdateTq(type->u.ptr.type, tq_add_procedure);
      }
    else NodeUpdateTq(type, tq_add_procedure);
    NodeUpdateTq(deepbasetype, tq_remove_procedure);
  }
  if (tq_has_inlet(basetq)) {
    if (!IsFunctionType(type))
      {
	if (!IsPtrToFunction(type))
	  SyntaxErrorCoord(deepbasetype->coord, 
			   "inlet keyword applies only to functions");
	else
	  NodeUpdateTq(type->u.ptr.type, tq_add_inlet);
      }
    else NodeUpdateTq(type, tq_add_inlet);
    NodeUpdateTq(deepbasetype, tq_remove_inlet);
  }

  return type;
}



/* 
  Append decl adds decl to list, giving it the same type and declaration
   qualifiers as the decls already on list.
*/
GLOBAL List *AppendDecl(List *list, Node *decl, ScopeState redeclare)
{
    Node *firstdecl, *type;

    if (list == NULL)
      return NULL;

    assert(decl != NULL  && decl->typ == Decl);

    firstdecl = FirstItem(list);
    assert(firstdecl != NULL  &&  firstdecl->typ == Decl);
    assert(firstdecl->u.decl.type != NULL);

    type = NodeCopy(GetDeepBaseType(firstdecl->u.decl.type), NodeOnly);

    decl->u.decl.tq = tq_union(decl->u.decl.tq, firstdecl->u.decl.tq);
    AppendItem(list, SetDeclType(decl, type, redeclare));

    return(list);
}
