/* Profile loads and stores, as part of the C-to-C Translator.
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/profile.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

/* ProfileProgram should convert a type-checked C (or Cilk) tree into a tree with profiling added.
 *
 * The profiling rconversion eturns the new tree, and it may modify the existing tree.
 *
 * We current support load-store profiling.
 * We convert every load from
 *    *address
 *  to
 *    (__c2c_profile_ld(address), *address)
 * And we convert every store from
 *    *address = value
 *  to
 *    (__c2c_profile_st(address), *address)
 *
 * This may require creating a temporary variable, so the right way to do it is
 *   ({ typeof(address) __address_0001 = address; __c2c_profile_ld(__address_0001); *__address_0001; })
 * and 
 *   ({ typeof(address) __address_0001 = address; __c2c_profile_ld(__address_0001); *__address_0001=value; })
 *
 * Loads and stores can appear as other constructs, such as arrays.  This rvalue  
 *     a[i]
 * should be converted to
 *   ({ typeof(a[i]) __address_0002 = &(a[i]); __c2c_profile_ld(__address_0002); *__address_0002; })
 * whereas
 *     a[i] = v;
 * should become
 *   ({ typeof(a[i]) __address_0003 = &(a[i]); __c2c_profile_st(__address_0003); *address_0003 = v; })
 *
 * Similarly for a->b and a.b.
 *
 */

PRIVATE List *ProfileList(List *list, Bool readvalue_elements, Bool readvalue_lastelement);
PRIVATE Node *ProfileNode(Node *node, Bool readvalue);

PRIVATE const char *ProfileReadName;
PRIVATE const char *ProfileWriteName;

/* You should almost always parenthesize comma expressions because they might become embedded in the arguments to a call, and you would lose the needed parens due to a bug in output.c  See Bug 68.*/
PRIVATE Node *BuildComma(List *exprs) {
    Node *result=MakeComma(exprs);
    result->parenthesized=TRUE;
    return result;
}

PRIVATE Node *BuildProfileRead(Node *addressnode, Node *node_to_take_sizeof, Coord coord) {
    return MakeCall(MakeId(ProfileReadName),
		    List4(MakeCast(MakePtr(TQ_CONST, MakePrim(TQ_CONST, Void)),
				   addressnode),
			  MakeUnary(SIZEOF, node_to_take_sizeof),
			  MakeImplicitCast(MakePtr(EMPTY_TQ, MakePrim(EMPTY_TQ, Char)),
					   MakeString(FileNames[coord.file])),
			  MakeConstUint(coord.line)));
}

PRIVATE Node *BuildProfileWrite(Node *addressnode, Node *node_to_take_sizeof, Coord coord) {
    return MakeCall(MakeId(ProfileWriteName),
		    List4(MakeCast(MakePtr(TQ_CONST, MakePrim(TQ_CONST, Void)),
				   addressnode),
			  MakeUnary(SIZEOF, node_to_take_sizeof),
			  MakeImplicitCast(MakePtr(EMPTY_TQ, MakePrim(EMPTY_TQ, Char)),
					   MakeString(FileNames[coord.file])),
			  MakeConstUint(coord.line)));
}

PRIVATE Node* SetUnaryType (Node *unary, Node *type) {
    assert(unary->typ==Unary);
    unary->u.unary.type = type;
    return unary;
}


//PRIVATE void MyNodeAddTq(Node *node, TypeQual tq) {
//    NodeUpdateTq2(node, tq_union, tq_remove_sue_elaborated(tq)); /* Never stick SUE_ELABORATED on anything.*/
//}


PRIVATE const char *create_unique_identifier (Node *var, const char *root) {
     char buf[33];
     const char *name;
     static unsigned counter = 0;
     Generic *existing;

     do {
	  sprintf(buf, "%.16s%d", root, ++counter);
	  name = UniqueString(buf);
     }
     while (LookupSymbol(Identifiers, name, &existing) || 
	    LookupSymbol(Externals, name, &existing));

     InsertSymbol(Identifiers, name, var, NULL);	/* NULL conflict procedure should
				 		         * never be called */
     assert(var->typ==Decl);
     var->u.decl.name = name;
     return name;
}

/* Return TRUE iff node, when treated as an lvalue, does not refer to a register variable.*/
/* That is it refers to a memory location.*/
PRIVATE Bool lvalue_is_memory (Node *node)
{
    switch (node->typ) {
    case Id:
	/*printf("%s %s memory (%s array) (%sregister)\n", node->u.id.text, node->u.id.decl->u.decl.register_p ? "is not" : "is", IsArrayType(NodeDataType(node)) ? "is" : "is not", node->u.id.decl->u.decl.register_p ? "" : "not ");	DPN(node);*/
	if (IsArrayType(NodeDataType(node))) {
	    /* If an lvalue is an array type, it is really a pointer to that array.*/
	    /* That is if we see*/
	    /*   void f(int A[10], int B[10]) { A=B; }*/
	    /* then the assignment is really on the pointers, and the variable doesn't need a memory reference*/
	    /* */
	    return FALSE;
	} else {
	    assert(node->u.id.decl->typ==Decl);
	    return !node->u.id.decl->u.decl.register_p;
	}
    case Array:
	return TRUE;
    case Binop:
	switch (node->u.binop.op) {
	case '.':
	    return TRUE;
	case ARROW:
	    FAIL("There should be no -> constructs after simplification");
	default:
	    fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); DPN(node); abort();
	}
	/* ?? don't forget conditionals (gcc extension), predec, preinc, postdec, postinc*/
    case Comma:
	return lvalue_is_memory(LastItem(node->u.comma.exprs));
    case Unary:
	switch (node->u.unary.op) {
	case INDIR:  return TRUE;
	default:
	    fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); DPN(node); abort();
	}
    case Cast:
	/* It doesn't really matter what the cast says.*/
	/* According to gcc documentation  int *x; (char*)x += y would be the same as*/
	/*                                 x += (int)((char*)x + y)*/
	/* which requires that the cast subexpression be memory for this to be memory.*/
	return lvalue_is_memory(node->u.cast.expr);
    case Call:
	/* The value returned by a call is never profiled memory*/
	return FALSE;
    case Constructor:
	/* The value returned by a constructor is never profiled memory.*/
	return FALSE;
    default:
	fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); DPN(node); abort();
    }
}

PRIVATE Node *lvalue_ProfileNode (Node *node)
{
    switch (node->typ) {
    case Id:
	/* It is OK for it not refer to memory in this case.*/
	return node;
    case Array:
	node->u.array.name=ProfileNode(node->u.array.name, TRUE);
	node->u.array.dims=ProfileList(node->u.array.dims, TRUE, TRUE);
	return node;
    case Binop:
	switch (node->u.binop.op) {
	case '.':
	    node->u.binop.left=lvalue_ProfileNode(node->u.binop.left);
	    return node;
	case ARROW:
	    FAIL("a->b should have been removed by the simplifier");
	default:
	    fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); DPN(node); abort();
	}
    case Comma:
	fprintf(stderr, "%s:%d: Cannot have comma lvalues (simplify.c should have removed them\n", __FILE__, __LINE__);
	abort(); /* Because of simplification, we never have comma lvalues*/
    case Unary:
	switch (node->u.unary.op) {
	case INDIR:
	    node->u.unary.expr = ProfileNode(node->u.unary.expr, TRUE);
	    return node;
	default:
	    fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); DPN(node); abort();
	}
    case Constructor:
	/* If an lvalue is a constructor, it is all constants, so we can just return it*/
	return node;
    case Cast:
	/* The problem here is will be that we cannot take the address of a cast expression, which we will want to do in the context*/
	/* So we cannot just do this:*/
	/*  NOPE:  node->u.cast.expr = lvalue_ProfileNode(node->u.cast.expr);*/
	/* I want to convert this:*/
	/*   ((char *) dirent) += reclen;*/
	/* into this*/
	/*   */

	abort();
	return node;
    default:
	fprintf(stderr, "\n%s:%d, aborting\n", __FILE__, __LINE__); PrintASTcoords=1; DPN(node); abort();
    }	
}

/* Return NULL if it is not a bitfield*/
/* Otherwise return a node which calculates the containing struct's lvalue*/
/* and set the bitfield_selector to the name of the bitfield being modified.*/
PRIVATE Node *lvalue_is_bitfield (Node *node, Node **bitfield_selector) {
    if (node->typ==Binop) {
	if (node->u.binop.op=='.') {
	    Node *left  = node->u.binop.left;
	    Node *right = node->u.binop.right;
	    Node *lefttype = NodeDataType(left);
	    List *fields;
	    assert(right->typ==Id);
	    if (lefttype->typ!=Sdcl) return 0; /* It could be a union, for example.*/
	    assert(lefttype->typ==Sdcl);
	    assert(lefttype->u.sdcl.type);
	    fields=lefttype->u.sdcl.type->fields;
	    while (fields) {
		Node *field = FirstItem(fields);
		fields = Rest(fields);
		assert(field->typ==Decl);
		if (strcmp(field->u.decl.name, right->u.id.text)==0) {
		    if (field->u.decl.bitsize) {
			*bitfield_selector = right;
			return left;
		    } else {
			return 0;
		    }
		}
	    }
	    assert(0);
	} else if (node->u.binop.op==ARROW) {
	    /* a->b is really syntactic sugar for (*a).b.  It should have been gotten rid of during the simplify step */
	    FAIL("a->b should have been gotten rid of by the simplify stage");
#if 0
	    Node *left  = node->u.binop.left;
	    Node *right = node->u.binop.right;
	    Node *lefttype = NodeDataType(left);
	    Node *subtype;
	    List *fields;
	    assert(right->typ==Id);
	    assert(lefttype->typ==Ptr);
	    subtype = lefttype->u.ptr.type;
	    if (subtype->typ!=Sdcl) return 0; /* It could be a union, for example.*/
	    assert(subtype->u.sdcl.type);
	    fields=subtype->u.sdcl.type->fields;
	    while (fields) {
		Node *field = FirstItem(fields);
		fields = Rest(fields);
		assert(field->typ==Decl);
		if (strcmp(field->u.decl.name, right->u.id.text)==0) {
		    if (field->u.decl.bitsize) {
			Node *result=MakeUnary(INDIR, left);
			*bitfield_selector = right;
			result->u.unary.type = SetCoords(NodeCopy(subtype, Subtree), UnknownCoord, Subtree); /* Need to set the type, or the profile won't be able to figure out what variable to create.*/
			return result;
		    } else {
			return 0;
		    }
		}
	    }
	    assert(0);
#endif
	}
    }
    return 0;
}

PRIVATE Node *ProfileConst(Node *node, UNUSED(ConstNode *u), UNUSED(Bool readvalue)) {
    return node;
}

/* Cases to worry about:*/
/*  If the Id is an array reference, then there is no reference here.*/
/*  If the Id is a variable that could be a register, then nothing happens here.*/
PRIVATE Node *ProfileId(Node *node, idNode *u, Bool readvalue) {
    if (!u->decl) DPN(node);
    assert(u->decl);
    assert(u->decl->typ==Decl);
    if (!readvalue) return node;
    if (u->decl->u.decl.register_p) return node;
    if (DeclIsEnumConst(u->decl)) return node;       /* References to enum constants are not profiled. */
    if (IsFunctionType(NodeDataType(node))) return node; /* References to functions are not profiled. */
    if (IsArrayType(NodeDataType(node))) return node; /* References to arrays are not profiled. */
    {
	Node *block = BuildComma(List2(BuildProfileRead(MakeUnary(ADDRESS, NodeCopy(node, Subtree)),
							NodeCopy(node, Subtree),
							node->coord),
				       node));
	return block;
    }
}

PRIVATE Node *ProfileAttrib(Node *node, UNUSED(attribNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileSpawn(Node *node, spawnNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileText(Node *node, UNUSED(textNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileBinop(Node *node, binopNode *u, Bool readvalue) {
    Bool sideeffects = SideeffectsNode(node);
    if (IsAssignmentOp(u->op)) {
	Node *bitfield_container;
	Node *bitfield_selector;
	u->right = ProfileNode(u->right, TRUE);
	bitfield_container=lvalue_is_bitfield(u->left, &bitfield_selector);
	if (bitfield_container) {
	    Node *profiled_container = lvalue_ProfileNode(bitfield_container);
	    Node *decl = MakeDecl(NULL,
				  EMPTY_TQ,
				  MakePtr(EMPTY_TQ,
					  SetCoords(NodeCopy(NodeDataType(bitfield_container), Subtree),
						    UnknownCoord, Subtree)),
				  MakeUnary(ADDRESS, profiled_container),
				  NULL);
	    const char *addr_var = create_unique_identifier(decl, "__addressEE");
	    Node *newid = MakeId(addr_var);
	    newid->u.id.decl = decl;
	    return MakeBlockCoord(NodeCopy(u->type, Subtree),
				  MakeNewList(decl),
				  List2(BuildProfileWrite(newid, MakeUnary(INDIR, newid), node->coord),
					MakeBinop(u->op,
						  MakeBinop(ARROW,
							    newid,
							    bitfield_selector),
						  u->right)),
				  u->left->coord, u->right->coord);
	} else if (lvalue_is_memory(u->left)) {
	    Node *decl = MakeDecl(NULL, EMPTY_TQ, MakePtr(EMPTY_TQ, SetCoords(NodeCopy(u->type, Subtree), UnknownCoord, Subtree)), MakeUnary(ADDRESS, lvalue_ProfileNode(u->left)), NULL);
	    Node *newid = MakeId(create_unique_identifier(decl, "__addressDD"));
	    newid->u.id.decl = decl;
	    {
		Node *doit = MakeBlock(u->type,
				       MakeNewList(decl),
				       List2(BuildProfileWrite(newid, MakeUnary(INDIR, newid), node->coord),
					     MakeBinop(u->op,
						       MakeUnary(INDIR, newid),
						       u->right)));
		/*printf("The doit is\n"); DPN(doit); */
		/*printf("The left is\n"); DPN(u->left);*/
		return doit;
	    }
	} else {
	    return node;
	}
    } else if (u->op==ARROW) {
	FAIL("a->b should have been gotten rid of by the simplify stage");
	
	/* when we see readvalue==FALSE   e.g., "(a->b).c" then a's value is being read but a->b is not being read*/
	/* when we see readvalue==TRUE    e.g., "(a->b)+c" then a's value is being read, and so is a->b's value*/
	u->left=ProfileNode(u->left, TRUE);
	if (readvalue) {
	    Node *bitfield_selector;
	    if (lvalue_is_bitfield(node, &bitfield_selector)) {
		/* If it is a bitfield, we need to record the whole value of *a as being read*/
		if (sideeffects) {
		    /* Tested*/
		    Node *decl  = MakeDecl(NULL, EMPTY_TQ, SetCoords(NodeCopy(NodeDataType(u->left), Subtree), UnknownCoord, Subtree), u->left, NULL);
		    Node *newid = MakeId(create_unique_identifier(decl, "__addressCC"));
		    NodeUpdateTq(decl->u.decl.type, tq_add_const);
		    return MakeBlock(NodeCopy(u->type, Subtree),
				     List1(decl),
				     List2(BuildProfileRead(newid,
							    MakeUnary(INDIR, NodeCopy(newid, Subtree)),
							    node->coord),
					   MakeBinop(ARROW,
						     NodeCopy(newid, Subtree),
						     u->right)));
		} else {
		    return BuildComma(List2(BuildProfileRead(NodeCopy(u->left, Subtree),
							     MakeUnary(INDIR, NodeCopy(u->left, Subtree)),
							     node->coord),
					    node));
		}
	    } else {
		/* It is not a bitfield, so we record only the parts of a->b that are read*/
		Node *leftdatatype = NodeDataType(u->left);
		Node *leftbasetype;
		assert(leftdatatype->typ==Ptr);
		leftbasetype = leftdatatype->u.ptr.type;
		if (sideeffects) {
		    Node *idbasetype = SetCoords(NodeCopy(NodeDataType(node), Subtree), UnknownCoord, Subtree);
		    Node *decl = MakeDecl(NULL,
					  EMPTY_TQ,
					  MakePtr(TQ_CONST, idbasetype),
					  MakeUnary(ADDRESS, node),
					  NULL);
		    Node *newid = MakeId(create_unique_identifier(decl, "__addressBB"));
		    NodeUpdateTq2(idbasetype, tq_union,
				  tq_remove_sue_elaborated(NodeTypeQuals(leftbasetype))); /* Never stick SUE_ELABORATED on anything. */
		    return MakeBlock(NodeCopy(u->type, Subtree),
				     List1(decl),
				     List2(BuildProfileRead(newid,
							    MakeUnary(INDIR, NodeCopy(newid, Subtree)),
							    node->coord),
					   MakeUnary(INDIR, NodeCopy(newid, Subtree))));
		} else {
		    return BuildComma(List2(BuildProfileRead(MakeUnary(ADDRESS, NodeCopy(node, Subtree)),
							     NodeCopy(node, Subtree),
							     node->coord),
					    node));
		}
	    }
	} else {
	    return node;
	}
    } else if (u->op=='.') {
	if (!lvalue_is_memory(u->left)) return node; /* If it is not memory, then don't mess with it.  An example would be a constructor or a structure returned from a function call.*/
	u->left = ProfileNode(u->left, FALSE);  /* We don't actually read the contents of the left (i.e.., it is an lvalue) */
	if (readvalue) {
	    /* In this case a.b is being read. */
	    Node *bitfield_selector;
	    if (lvalue_is_bitfield(node, &bitfield_selector)) {
		/* If it is a bitfield, we record the whole contents of a as being read.*/
		/**/
		/* We may have started with a->b, which the simplifier turned into (*a).b*/
		/* Now we basically want to take the address of the left side, and pass it to the profiling funcdtion*/
		/**/
		/* It would be better if we were to not take the whole containing structure, but that would require*/
		/* some detailed address calculations.*/
		if (!sideeffects) {
		    /* If there are no side effects, then duplicate the left side with impunity.*/
		    if (u->left->typ==Unary && u->left->u.unary.op==INDIR) {
			/* If the left side is *a, then strip of the * and do it.*/
			return BuildComma(List2(BuildProfileRead(NodeCopy(u->left->u.unary.expr, Subtree),
								 NodeCopy(u->left, Subtree),
								 node->coord),
						node));
		    } else {
			/* The left side is not a *, so we have to take its address*/
			Node *ltype = NodeDataType(u->left);
			return BuildComma(List2(BuildProfileRead(SetUnaryType(MakeUnary(ADDRESS, NodeCopy(u->left, Subtree)),
									      MakePtr(EMPTY_TQ, ltype)),
								 NodeCopy(u->left,Subtree),
								 node->coord),
						node));
		    }
		} else {
		    Node *decl = MakeDecl(NULL,
					  EMPTY_TQ,
					  SetCoords(MakePtr(EMPTY_TQ, NodeDataType(u->left)), u->left->coord, Subtree),
					  MakeUnary(ADDRESS, NodeCopy(u->left, Subtree)),
					  NULL);
		    Node *newid = MakeId(create_unique_identifier(decl, "__addressII"));
		    newid->u.id.decl = decl;
		    return MakeBlock(NodeCopy(u->type, Subtree),
				     List1(decl),
				     List2(BuildProfileRead(newid, NodeDataType(u->left), node->coord),
					   MakeBinop('.',
						     MakeUnary(INDIR, NodeCopy(newid, Subtree)),
						     u->right)));
		}
	    } else {
		/* It is not a bitfield, so we can record the field itself, rather than the whole structure.*/
		if (!sideeffects) {
		    return BuildComma(List2(BuildProfileRead(SetUnaryType(MakeUnary(ADDRESS, NodeCopy(node, Subtree)),
									  MakePtr(EMPTY_TQ, NodeDataType(node))),
							     NodeCopy(node, Subtree),
							     node->coord),
					    node));
		} else {
		    Node *decl = MakeDecl(NULL,
					  EMPTY_TQ,
					  SetCoords(MakePtr(EMPTY_TQ, NodeCopy(NodeDataType(node), Subtree)), u->left->coord, Subtree),
					  MakeUnary(ADDRESS, NodeCopy(node, Subtree)),
					  NULL);
		    Node *newid = MakeId(create_unique_identifier(decl, "__addressHH"));
		    newid->u.id.decl = decl;
		    return MakeBlock(NodeCopy(u->type, Subtree),
				     List1(decl),
				     List2(BuildProfileRead(newid,
							    SetCoords(NodeCopy(NodeDataType(node), Subtree), u->left->coord, Subtree),
							    node->coord),
					   MakeUnary(INDIR, newid)));
		}
	    }
	} else {
	    /* We are not reading a.b, so just return the node */
	    return node;
	}
    } else if (u->op==':' || u->op==DESIGNATED_INITIALIZER) {
	u->right=ProfileNode(u->right, TRUE);
	return node;
    } else {
	assert(readvalue);
	u->left=ProfileNode(u->left, TRUE);
	u->right=ProfileNode(u->right, TRUE);
	return node;
    }
}

PRIVATE Node *ProfileUnary(Node *node, unaryNode *u, Bool readvalue) {
    switch (u->op) {
    case INDIR:
	u->expr=ProfileNode(u->expr, TRUE);
	if (readvalue) {
	    Node *decl = MakeDecl(NULL, EMPTY_TQ, MakePtr(TQ_CONST, SetCoords(NodeCopy(u->type, Subtree), UnknownCoord, Subtree)), NULL, NULL);
	    const char *addr_var = create_unique_identifier(decl, "__addressAA");
	    Node *newid = MakeId(addr_var);
	    newid->u.id.decl = decl;
	    {
		Node *doit = MakeBlock(NodeCopy(u->type, Subtree),
				       MakeNewList(decl),
				       List2(BuildProfileRead(newid, MakeUnary(INDIR, NodeCopy(newid, Subtree)), node->coord),
					     MakeUnary(INDIR, newid)));
		decl->u.decl.init=u->expr;
		return doit;
	    }
	} else {
	    return node;
	}
    case UMINUS:
    case UPLUS:
    case '!':
    case '~':
	assert(readvalue);
	u->expr=ProfileNode(u->expr, TRUE);
	return node;
    case SIZEOF:
    case ALIGNOF:
	return node;
    case ADDRESS:
	/* assert(lvalue_is_memory(u->expr));  // If I am taking the address, I better think it is memory, but arrays are special, so handle it in lvalue_ProfileNode*/
	assert(readvalue);
	u->expr=ProfileNode(u->expr, FALSE);
	return node;
    case PREINC:
    case PREDEC:
    case POSTINC:
    case POSTDEC:
	if (!lvalue_is_memory(u->expr)) return node;
	else {
	    Node *decl = MakeDecl(NULL, EMPTY_TQ, MakePtr(TQ_CONST, SetCoords(NodeCopy(u->type, Subtree), UnknownCoord, Subtree)), NULL, NULL);
	    const char *addr_var = create_unique_identifier(decl, "__addressFF");
	    Node *newid = MakeId(addr_var);
	    newid->u.id.decl = decl;
	    {
		Node *doit = MakeBlock(NodeCopy(u->type, Subtree),
				       MakeNewList(decl),
				       /* We assume that a write is a superset of a read.  Thus profiling a write means that the read doesn't need to be profiled.*/
				       List2(BuildProfileWrite(newid, MakeUnary(INDIR, newid), node->coord),
					     MakeUnary(u->op,
						       MakeUnary(INDIR, newid))));
		decl->u.decl.init = MakeUnary(ADDRESS,
					      lvalue_ProfileNode(u->expr));  /* This code would be cleaner if it copied the types to all the right places...*/
		return doit;
	    }
	}
	printf("%s:%d Don't know this yet, aborting", __FILE__, __LINE__); abort();
    default:
	fprintf(stderr, "%s:%d, aborting, op is %d\n", __FILE__, __LINE__, u->op); abort();
    }
    return node;
}

PRIVATE Node *ProfileCast(Node *node, castNode *u, Bool readvalue) {
    assert(readvalue); /* Because we already simplified, casts are always performed for the value.*/
    u->expr = ProfileNode(u->expr, readvalue);
    return node;
}

PRIVATE Node *ProfileComma(Node *node, commaNode *u, Bool readvalue) {
    u->exprs=ProfileList(u->exprs, TRUE, readvalue);
    return node;
}

PRIVATE Node *ProfileConstructor(Node *node, constructorNode *u, Bool readvalue) {
    u->initializerlist=ProfileNode(u->initializerlist, readvalue);
    return node;
}

PRIVATE Node *ProfileTernary(Node *node, ternaryNode *u, UNUSED(Bool readvalue)) {
    u->cond = ProfileNode(u->cond, TRUE);
    u->true = ProfileNode(u->true, TRUE);
    u->false = ProfileNode(u->false, TRUE);
    return node;
}

PRIVATE Node *ProfileArray(Node *node, arrayNode *u, Bool readvalue) {
    Node *ptrtype = MakePtr(EMPTY_TQ, SetCoords(TypeQualifyNode(NodeCopy(u->type, Subtree), NodeTypeQuals(NodeDataType(u->name)), &UnknownCoord),UnknownCoord, Subtree));
    Node *decl = MakeDecl(NULL, EMPTY_TQ, ptrtype, MakeUnary(ADDRESS, node), NULL);
    Node *newid = MakeId(create_unique_identifier(decl, "__addressGG"));
    /*    printf("%s:%d tq of u->type is %d\n", __FILE__, __LINE__, NodeTypeQuals(u->type)); DPN(u->type);*/
    /*    printf("tq of u->name is %d\n", NodeTypeQuals(NodeDataType(u->name))); DPN(NodeDataType(u->name)); // The TQ on this value should include const*/
    /*    assert(u->name->typ==Binop); // That is to make sure the following printfs work, but I only had those printfs there so I could try to understand what is going on.fs*/
    /*    printf("tq of u->name->u.binop.left is %d\n", NodeTypeQuals(NodeDataType(u->name->u.binop.left))); DPN(u->name->u.binop.left);*/
    /*    printf("\nThe type of left is\n"); DPN(NodeDataType(u->name->u.binop.left));*/
    newid->u.id.decl = decl;
    if (readvalue) {
	Node *doit = MakeBlock(u->type,
			       List1(decl),
			       List2(BuildProfileRead(newid,
						      MakeUnary(INDIR, newid),
						      node->coord),
				     MakeUnary(INDIR, NodeCopy(newid, Subtree))));
	u->name=ProfileNode(u->name, TRUE);
	u->dims=ProfileList(u->dims, TRUE, TRUE);
	return doit;
    } else {
	u->name=ProfileNode(u->name, TRUE);
	u->dims=ProfileList(u->dims, TRUE, TRUE);
	return node;
    }
}

/* This was my attempt to get the const type qualifiers propaged properly, but it didn't work.*/
/* Something is wrong with the const type qualifiers to make this work right.*/

#if 0
// PRIVATE Node *ProfileArray(Node *node, arrayNode *u) {
//    Node *arraytype = NodeDataType(u->name);
//     if (arraytype->typ!=Adcl) DPN(arraytype);
//    switch (arraytype->typ) {
//     assert(arraytype->typ==Adcl);
//    {
// 	Node *decl = MakeDecl(NULL, EMPTY_TQ, MakePtr(arraytype->u.adcl.tq, NodeCopy(arraytype->u.adcl.type, Subtree)), NULL, NULL);
//	const char *addr_var = create_unique_identifier(decl, "__address");
// 	Node *newid = MakeId(addr_var);
////	printf("arraynode is\n"); DPN(node);
// //	printf("array name\n"); DPN(u->name);
////	printf("array type is\n"); DPN(NodeDataType(u->name));
// //	printf("arraytype is (tq=%d)\n", arraytype->u.adcl.tq); DPN(arraytype);
//	newid->u.id.decl = decl;
// 	{
//	    Node *doit = MakeBlock(NodeCopy(u->type, Subtree),
// 				   MakeNewList(decl),
//				   List2(MakeCall(MakeId(ProfileReadName),
// 						  List2(newid,
//							MakeUnary(SIZEOF, MakeUnary(INDIR, newid)))),
// 					 MakeUnary(INDIR, newid)));
//	    SetCoords(doit, node->coord, Subtree);
// 	    decl->u.decl.init = MakeUnary(ADDRESS,
//					  node);
// 	    u->name=ProfileNode(u->name);
//	    u->dims=ProfileList(u->dims);
// 	    return doit;
//	}
//     }
//}
#endif 
PRIVATE Node *ProfileCall(Node *node, callNode *u, UNUSED(Bool readvalue)) {
     u->name = ProfileNode(u->name, TRUE);
     u->args = ProfileList(u->args, TRUE, TRUE);
     return node;
}

PRIVATE Node *ProfileInitializer(Node *node, initializerNode *u, Bool readvalue) {
    u->exprs=ProfileList(u->exprs, readvalue, readvalue);
    return node;
}

PRIVATE Node *ProfileImplicitCast(Node *node, implicitcastNode *u, Bool readvalue) {
    u->expr = ProfileNode(u->expr, readvalue);
    return node;
}

PRIVATE Node *ProfileLabel(Node *node, UNUSED(labelNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileSwitch(Node *node, SwitchNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE);
    u->stmt = ProfileNode(u->stmt, TRUE);
    return node;
}

PRIVATE Node *ProfileCase(Node *node, CaseNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE); /* expr is supposed to be a constant */
    return node;
}

PRIVATE Node *ProfileDefault(Node *node, UNUSED(DefaultNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileIf(Node *node, IfNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE);
    u->stmt = ProfileNode(u->stmt, TRUE);
    return node;
}

PRIVATE Node *ProfileIfElse(Node *node, IfElseNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE);
    u->true = ProfileNode(u->true, TRUE);
    u->false = ProfileNode(u->false, TRUE);
    return node;
}

PRIVATE Node *ProfileWhile(Node *node, WhileNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE);
    u->stmt = ProfileNode(u->stmt, TRUE);
    return node;
}

PRIVATE Node *ProfileDo(Node *node, DoNode *u, UNUSED(Bool readvalue)) {
    u->stmt = ProfileNode(u->stmt, TRUE);
    u->expr = ProfileNode(u->expr, TRUE);
    return node;
}

PRIVATE Node *ProfileFor(Node *node, ForNode *u, UNUSED(Bool readvalue)) {
    u->init=ProfileNode(u->init, TRUE);
    u->cond=ProfileNode(u->cond, TRUE);
    u->next=ProfileNode(u->next, TRUE);
    u->stmt=ProfileNode(u->stmt, TRUE);
    return node;
}

PRIVATE Node *ProfileGoto(Node *node, UNUSED(GotoNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileContinue(Node *node, UNUSED(ContinueNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileBreak(Node *node, UNUSED(BreakNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileReturn(Node *node, ReturnNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, TRUE);
    return node;
}

PRIVATE Node *ProfileBlock(Node *node, BlockNode *u, Bool readvalue) {
    u->decl=ProfileList(u->decl, FALSE, FALSE);
    u->stmts=ProfileList(u->stmts, TRUE, readvalue);
    return node;
}

/* Most of the type-related nodes are never transformed. */
PRIVATE Node *ProfilePrim(Node *node, UNUSED(primNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileTdef(Node *node, UNUSED(tdefNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfilePtr(Node *node, UNUSED(ptrNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileAdcl(Node *node, UNUSED(adclNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileFdcl(Node *node, UNUSED(fdclNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileSdcl(Node *node, UNUSED(sdclNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileUdcl(Node *node, UNUSED(udclNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileEdcl(Node *node, UNUSED(edclNode *u), UNUSED(Bool readvalue)) {
    return node;
}

PRIVATE Node *ProfileAsm(Node *node, asmNode *u, UNUSED(Bool readvalue)) {
    return node;
    fprintf(stderr, "%s:%d, aborting <%s,%d,%d>\n", __FILE__, __LINE__, FileNames[node->coord.file], node->coord.line, node->coord.offset); DPN(node); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileAsmArg(Node *node, asmargNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileBuiltinVaArg(Node *node, builtinvaargNode *u, UNUSED(Bool readvalue)) {
    u->expr = ProfileNode(u->expr, readvalue);
    return node;
}


PRIVATE Node *ProfileSync(Node *node, syncNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileInletCall(Node *node, inletcallNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileAbort(Node *node, abortNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}

PRIVATE Node *ProfileSynched(Node *node, synchedNode *u, UNUSED(Bool readvalue)) {
    fprintf(stderr, "%s:%d, aborting\n", __FILE__, __LINE__); abort(); u=u;
    return node;
}


PRIVATE Node *ProfileXBlock(Node *node, XBlockNode *u, Bool readvalue) {
    u->decl=ProfileList(u->decl, FALSE, FALSE);
    u->stmts=ProfileList(u->stmts, TRUE, readvalue);
    return node;
}

PRIVATE Node *ProfileDecl(Node *node, declNode *u, UNUSED(Bool readvalue)) {
    if (tq_has_top_decl(NodeDeclLocation(node))
	|| DeclIsStatic(node)) 
	return node;
    /*u->type=ProfileNode(u->type);*/
    u->init=ProfileNode(u->init, TRUE);
    /*u->asmdecl=ProfileNode(u->asmdecl);*/
    return node;
}

PRIVATE Node *ProfileProc(Node *node, procNode *u, Bool readvalue) {
    List *registervars = RegisterVariables(node, NULL);
    /*fprintf(stderr, "Got registervars\n");  PrintList(stderr, registervars, 0);*/
    assert(!readvalue);
    {
	List *aptr;
	for (aptr = registervars; aptr; aptr = Rest(aptr)) {
	    Node *item = FirstItem(aptr);
	    switch(item->typ) {
	    case Decl:
		item->u.decl.register_p = TRUE;
		break;
	    default:
		fprintf(stderr, "What is this decl (%s:%d)\n", __FILE__, __LINE__);
		fPrintNode(stderr, item, 0);
		abort();
	    }
	}
    }

    u->decl = ProfileNode(u->decl, FALSE);
    u->body = ProfileNode(u->body, TRUE);

    return node;
}

GLOBAL List *ProfileProgram (List *program) {
    ProfileReadName=UniqueString("__c2c_ProfileRead");
    ProfileWriteName=UniqueString("__c2c_ProfileWrite");
    /*DPL(program);*/
    return JoinLists(List2(MakeDecl(ProfileReadName,
				    EMPTY_TQ,
				    MakeFdcl(EMPTY_TQ,
					     List4(MakePtr(TQ_CONST, MakePrim(TQ_CONST, Void)),
						   MakePrim(EMPTY_TQ, Uint),
						   MakePtr(EMPTY_TQ, MakePrim(TQ_CONST, Char)),
						   MakePrim(EMPTY_TQ, Uint)),
					     MakePrim(EMPTY_TQ, Void)),
				    NULL, NULL),
			   MakeDecl(ProfileWriteName,
				    EMPTY_TQ,
				    MakeFdcl(EMPTY_TQ,
					     List4(MakePtr(TQ_CONST, MakePrim(TQ_CONST, Void)),
						   MakePrim(EMPTY_TQ, Uint),
						   MakePtr(EMPTY_TQ, MakePrim(TQ_CONST, Char)),
						   MakePrim(EMPTY_TQ, Uint)),
					     MakePrim(EMPTY_TQ, Void)),
				    NULL, NULL)),
		     ProfileList(program, FALSE, FALSE));
}

PRIVATE List *ProfileList(List *list, Bool readvalue_elements, Bool readvalue_lastelement) {
    List *aptr;

    for (aptr = list; aptr; aptr = Rest(aptr)) {
	Node *item = FirstItem(aptr);
	
	SetItem(aptr, ProfileNode(item,
				  Rest(aptr) ? readvalue_elements : readvalue_lastelement));
    }

    return list;
}

/* When you profile a node, there are two things that can happen:*/
/*  It may be that the node itself will be read from, or it may be that we are only interested in the subexpressions*/
/*  E.g.,   when profiling "a.b" it is not all of a that is being read, so profiling should do nothing to a.*/
/*  But when profiling "(a[i]).b", although we don't read (a[i]), we do read a and i.*/
/*  The second argument boolean argument is TRUE if we want to actually use the value of node, otherwise it is FALSE.*/
PRIVATE Node *ProfileNode(Node *node, Bool readvalue) {
     if (node == NULL)
	  return NULL;
#define CODE(name, node, union) return Profile##name(node, union, readvalue)
     ASTSWITCH(node, CODE)
#undef CODE
	 UNREACHABLE;
     return NULL;
}

