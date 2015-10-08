/* Simplify C code, putting it into canonical form.
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/simplify.c $ $LastChangedBy: bradley $ $Rev: 2672 $ $Date: 2005-12-20 13:30:02 -0500 (Tue, 20 Dec 2005) $");

/* The simplifications we do are as follows
 *   Get rid of casts on lvalues.  Thus
 *          (TYPE1)x=y
 *      becomes
 *          (TYPE1)(x=(typeof(x))(TYPE1)y)
 *      and
 *          (TYPE1)x+=y
 *      becomes
 *             x=(typeof(x))(TYPE1)((TYPE1)x + y)
 *      but without the side effects of doing x twice.
 *      (See the info page for gcc under "Generalized Lvalues")
 *
 *   Get rid of commas in lvalues.  Thus
 *          (a,b,c,...,x,y) =z
 *      could become
 *          (a,b,c,...,x,y=z)
 *      or
 *          ({typeof(z) zz=z; (a,b,c,...,x,y=z);})
 *      Both have the problem that they serialize the evaluation of z with the others, whereas in the original code, z could be evaluated in either order with a.
 *      So I'll go for the second version.
 *
 *   Get rid of ->.  a->b is syntactic sugar for (*a).b.
 */

#include "astEquals.h"
#include "stringParse.h"

PRIVATE Node *SimplifyNode(Node *list);
PRIVATE List *SimplifyList(List *list);

#define SimplifyNop(kind,utype) PRIVATE Node* Simplify##kind(Node *n, utype##Node *UNUSED(u)) { return n; }


SimplifyNop(Id,id)
SimplifyNop(Label,label)
SimplifyNop(Goto,Goto)
SimplifyNop(Default,Default)
SimplifyNop(Continue,Continue)
SimplifyNop(Break,Break)
SimplifyNop(Prim,prim)
SimplifyNop(Sync,sync)
SimplifyNop(Abort,abort)
SimplifyNop(Synched,synched)
SimplifyNop(Attrib,attrib)
SimplifyNop(Text,text)

PRIVATE Node* SimplifyConst (Node *node, ConstNode *u) {
    u->type    = SimplifyNode(u->type);
    return node;
}
\
PRIVATE Node* SimplifyUnary (Node *node, unaryNode *u) {
    Node *expr = u->expr             = SimplifyNode(u->expr);
    if (IsIncDecOp(u->op) || u->op==ADDRESS) {
	if (expr->typ==Comma) {
	    List *last = Last(expr->u.comma.exprs);
	    u->expr = FirstItem(last);
	    SetItem(last, node);
	    return SimplifyNode(expr);
	}
    }
    return node;
}

PRIVATE Node* RemoveSideEffectsNode(Node *n, List **newdecls) {
    *newdecls = 0;
    return n; /* This is not right, but it is a stub that will do for now.*/
}

PRIVATE Node* SetBinopType (Node *binop, Node *type) {
    assert(binop->typ==Binop);
    binop->u.binop.type = type;
    return binop;
}

Node* SimplifyBinop (Node *node, binopNode *u) {
    Node *left;
    u->left = SimplifyNode(u->left);
    left = u->left;
    u->right = SimplifyNode(u->right);
    if (IsAssignmentOp(u->op)) {
	if (left->typ==Comma) {
	    List *last = Last(left->u.comma.exprs);
	    u->left = FirstItem(last);
	    SetItem(last, node);
	    return SimplifyNode(left); /* Now simplify the whole thing again, just in case something pops up due to the rewrite.  There should be no infinite recursion.*/
	}
	if (left->typ==Cast) {
	    if (u->op=='=') {
		/* Convert (T)Y=Z into  (T)(x=(typeof(Y))((T)Z))*/
		Node *T=left->u.cast.type;
		Node *Y=left->u.cast.expr;
		Node *Z=u->right;
		Node *typeofY = SetCoords(NodeDataType(Y), node->coord, Subtree);
		Node *result = MakeCastCoord(T, /* Reuse T (we can reuse each subtree at most once.)*/
					     MakeBinopCoord(u->op,
							    Y, /* Reuse Y*/
							    MakeCastCoord(NodeCopy(typeofY, Subtree),
									  MakeCastCoord(NodeCopy(T, Subtree), /* Used T above, so copy here*/
											Z, /* Reuse Z*/
											node->coord),
									  node->coord),
							    node->coord),
					     left->coord);
		return SimplifyNode(result);
	    } else {
		/* it is something like +=, which has a different rule.
		 * Convert   (T)Y += Z   into   (T)(Y = (typeof(Y))(T)((T)Y + Z))
		 * Note this requires removing side effects from Y.
		 */
		Node *T=left->u.cast.type;
		Node *Y=left->u.cast.expr;
		Node *Z=u->right;
		Node *typeofY= SetCoords(NodeCopy(NodeDataType(Y), Subtree), node->coord, Subtree);
		List *newdecls;
		Node *result;
		Y=RemoveSideEffectsNode(Y, &newdecls);
		result = MakeCastCoord(T, /* Reuse T here (we can reuse each subtree at most once.)*/
				       SetBinopType(MakeBinopCoord('=',
								   Y,
								   MakeCastCoord(typeofY,
										 MakeCastCoord(NodeCopy(T, Subtree),
											       SetBinopType(MakeBinopCoord(OpOfAssignmentOp(u->op),
															   MakeCastCoord(NodeCopy(T, Subtree),
																	 NodeCopy(Y, Subtree),
																	 node->coord),
															   Z,
															   node->coord),
													    NodeCopy(T, Subtree)),
											       node->coord),
										 node->coord),
								   node->coord),
						    NodeCopy(typeofY, Subtree)),
				       left->coord);
		if (newdecls) {
		    result = MakeBlockCoord(NodeCopy(T, Subtree),
					    newdecls,
					    List1(result),
					    node->coord, node->coord);
		}
		return SimplifyNode(result);
	    }
	}
    } else {
	switch(u->op) {
	case ARROW:
	    /* change a->b to (*a).b */
	    /* However, as a special case, we change (&a)->b to a.b */
	    /* Also if a is an array, then we change it to a[0].b */
	    {
		Node *ltype = NodeDataType(u->left);
		if (u->left->typ==Unary && u->left->u.unary.op==ADDRESS) {
		    u->left = u->left->u.unary.expr;
		    u->op  = '.';
		} else if (ltype->typ==Ptr) {
		    Node *ino=MakeUnaryCoord(INDIR, u->left, u->left->coord);
		    Node *ltype2 = NodeDataType(u->left);
		    assert(ltype2);
		    u->op='.';
		    u->left=ino;
		    ino->u.unary.type = ltype2->u.ptr.type;
		} else if (ltype->typ==Adcl) {
		    u->op='.';
		    u->left = MakeArray(u->left, List1(MakeConstUint(0)));
		    u->left->u.array.type = ltype->u.adcl.type;
		} else {
		    fprintf(stderr, "Failing because I have an arrow from a nonpointer type: \n");
		    fPrintNode(stderr, ltype, 0);
		    fprintf(stderr, "\nin this expression:\n");
		    fPrintNode(stderr, node, 0);
		}
	    }
	    break;
	default:
	    break;
	}
    }	
    return node;
}


Node* SimplifyBinopNewVersion (Node *node, binopNode *u) {
    Node *left;
    u->left = SimplifyNode(u->left);
    left = u->left;
    u->right = SimplifyNode(u->right);
    if (IsAssignmentOp(u->op)) {
	if (left->typ==Comma) {
	    List *last = Last(left->u.comma.exprs);
	    u->left = FirstItem(last);
	    SetItem(last, node);
	    return SimplifyNode(left); /* Now simplify the whole thing again, just in case something pops up due to the rewrite.  There should be no infinite recursion.*/
	}
	if (left->typ==Cast) {
	    if (u->op=='=') {
		/* Convert (T)Y=Z into  (T)(x=(typeof(Y))((T)Z))*/
		Node *T=left->u.cast.type;
		Node *Y=left->u.cast.expr;
		Node *Z=u->right;
		Node *typeofY = SetCoords(NodeDataType(Y), node->coord, Subtree);

		Node *result = ParseWildcardStringNode("(%t1) %e1 = ((%t2) ((%t1) %e2));",
						       T, Y, typeofY, Z);
		
/*  	        Node *result = MakeCastCoord(T, // Reuse T (we can reuse each subtree at most once.) */
/*  					     MakeBinopCoord(u->op, */
/*  							    Y, // Reuse Y */
/*  							    MakeCastCoord(NodeCopy(typeofY, Subtree), */
/*  									  MakeCastCoord(NodeCopy(T, Subtree), // Used T above, so copy here */
/*  											Z, // Reuse Z */
/*  											node->coord), */
/*  									  node->coord), */
/*  							    node->coord), */
/*  					     left->coord); */
		  
		return SimplifyNode(result);
	    } else {
		/* it is something like +=, which has a different rule.
		 * Convert   (T)Y += Z   into   (T)(Y = (typeof(Y))(T)((T)Y + Z))
		 * Note this requires removing side effects from Y.
		 */
		Node *T=left->u.cast.type;
		Node *Y=left->u.cast.expr;
		Node *Z=u->right;
		Node *typeofY= SetCoords(NodeCopy(NodeDataType(Y), Subtree), node->coord, Subtree);
		List *newdecls;
		Node *result;
		Y=RemoveSideEffectsNode(Y, &newdecls);
		result = MakeCastCoord(T, /* Reuse T here (we can reuse each subtree at most once.)*/
				       SetBinopType(MakeBinopCoord('=',
								   Y,
								   MakeCastCoord(typeofY,
										 MakeCastCoord(NodeCopy(T, Subtree),
											       SetBinopType(MakeBinopCoord(OpOfAssignmentOp(u->op),
															   MakeCastCoord(NodeCopy(T, Subtree),
																	 NodeCopy(Y, Subtree),
																	 node->coord),
															   Z,
															   node->coord),
													    NodeCopy(T, Subtree)),
											       node->coord),
										 node->coord),
								   node->coord),
						    NodeCopy(typeofY, Subtree)),
				       left->coord);
		if (newdecls) {
		    result = MakeBlockCoord(NodeCopy(T, Subtree),
					    newdecls,
					    List1(result),
					    node->coord, node->coord);
		}
		return SimplifyNode(result);
	    }
	}
    } else {
	switch(u->op) {
	case ARROW:
	    /* change a->b to (*a).b */
	    /* However, as a special case, we change (&a)->b to a.b */
          printf("Attempting this simplification. \n");
	  {
	    Node *answer = MatchReplace(node, "(&%e1)->%d1;", "%e1.%d1;");
	    if (answer != NULL)
	      return answer;

	    printf("Making it to the inner one... \n");
	    
	    answer = MatchReplace(node, "%e1->%d1;", "(*%e1).%d1;");
	    if (answer != NULL)
	      return answer;
	  }
	  break;
	  
	    
/*  	    if (u->left->typ==Unary && u->left->u.unary.op==ADDRESS) { */
/*  		u->left = u->left->u.unary.expr; */
/*  		u->op  = '.'; */
/*  	    } else { */
/*  		Node *ino=MakeUnaryCoord(INDIR, u->left, u->left->coord); */
/*  		Node *ltype = NodeDataType(u->left); */
/*  		assert(ltype); */
/*  		assert(ltype->typ==Ptr); */
/*  		u->op='.'; */
/*  		u->left=ino; */
/*  		ino->u.unary.type = ltype->u.ptr.type; */
/*  	    } */
/*  	    break; */
	default:
	    break;
	}
    }	
    return node;
}



PRIVATE Node* SimplifyCast (Node *node, castNode *u) {
    u->type             = SimplifyNode(u->type);
    u->expr             = SimplifyNode(u->expr);
    return node;
}

PRIVATE Node* SimplifyComma (Node *node, commaNode *u) {
    u->exprs   = SimplifyList(u->exprs);
    return node;

}

PRIVATE Node* SimplifyConstructor (Node *node, constructorNode *u) {
    u->type             = SimplifyNode(u->type);
    u->initializerlist  = SimplifyNode(u->initializerlist);
    return node;

}
PRIVATE Node* SimplifyTernary (Node *node, ternaryNode *u) {
    u->cond  = SimplifyNode(u->cond);
    u->true  = SimplifyNode(u->true);
    u->false = SimplifyNode(u->false);
    return node;
}

PRIVATE Node* SimplifyArray (Node *node, arrayNode *u) {
    u->name = SimplifyNode(u->name);
    u->dims = SimplifyList(u->dims);
    return node;
}

PRIVATE Node* SimplifyCall (Node *node, callNode *u) {
    u->name = SimplifyNode(u->name);
    u->args = SimplifyList(u->args);
    return node;
}

PRIVATE Node* SimplifyInitializer (Node *node, initializerNode *u) {
    u->exprs   = SimplifyList(u->exprs);
    return node;
}

PRIVATE Node* SimplifyImplicitCast (Node *node, implicitcastNode *u) {
    u->expr    = SimplifyNode(u->expr);
    return node;
}

PRIVATE Node* SimplifySwitch (Node *node,  SwitchNode *u) {
    u->expr    = SimplifyNode(u->expr);
    u->stmt    = SimplifyNode(u->stmt);
    return node;
}

PRIVATE Node* SimplifyCase (Node *node,  CaseNode *u) {
    u->expr    = SimplifyNode(u->expr);
    return node;
}


PRIVATE Node* SimplifyIf (Node *node, IfNode *u) {
    u->expr    = SimplifyNode(u->expr);
    u->stmt    = SimplifyNode(u->stmt);
    return node;
}

PRIVATE Node* SimplifyIfElse (Node *node, IfElseNode *u) {
    u->expr    = SimplifyNode(u->expr);
    u->true    = SimplifyNode(u->true);
    u->false   = SimplifyNode(u->false);
    return node;
}

PRIVATE Node* SimplifyWhile (Node *node, WhileNode *u) {
    u->expr    = SimplifyNode(u->expr);
    u->stmt    = SimplifyNode(u->stmt);
    return node;
}

PRIVATE Node* SimplifyDo (Node *node, DoNode *u) {
    u->stmt    = SimplifyNode(u->stmt);
    u->expr    = SimplifyNode(u->expr);
    return node;
}

PRIVATE Node* SimplifyFor (Node *node, ForNode *u) {
    u->init    = SimplifyNode(u->init);
    u->cond    = SimplifyNode(u->cond);
    u->next    = SimplifyNode(u->next);
    u->stmt    = SimplifyNode(u->stmt);
    return node;
}

PRIVATE Node* SimplifyReturn (Node *node, ReturnNode *u) {
    u->expr    = SimplifyNode(u->expr);
    return node;
}

PRIVATE Node* SimplifyBlock (Node *node, BlockNode *u) {
    u->decl = SimplifyList(u->decl);
    u->stmts = SimplifyList(u->stmts);
    return node;
}


PRIVATE Node* SimplifyXBlock (Node *node, XBlockNode *u) {
    u->decl = SimplifyList(u->decl);
    u->stmts = SimplifyList(u->stmts);
    return node;
}


PRIVATE Node* SimplifyTdef (Node *node, tdefNode *u) {
    u->type    = SimplifyNode(u->type);
    return node;
}

PRIVATE Node* SimplifyPtr (Node *node, ptrNode *u) {
    u->type    = SimplifyNode(u->type);
    return node;
}

PRIVATE Node* SimplifyAdcl (Node *node, adclNode *u) {
    u->type    = SimplifyNode(u->type);
    return node;
}

PRIVATE Node* SimplifyFdcl (Node *node, fdclNode *u) {
    u->args    = SimplifyList(u->args);
    u->returns = SimplifyNode(u->returns);
    return node;
}

PRIVATE void SimplifySUEtype(SUEtype *type) {
    type->fields=SimplifyList(type->fields);
    type->attributes=SimplifyList(type->attributes);
}

PRIVATE Node *SimplifySdcl(Node *node, sdclNode *u) {
    SimplifySUEtype(u->type);
    return node;
}

PRIVATE Node *SimplifyUdcl(Node *node, udclNode *u) {
    SimplifySUEtype(u->type);
    return node;
}

PRIVATE Node *SimplifyEdcl(Node *node, edclNode *u) {
    SimplifySUEtype(u->type);
    return node;
}


PRIVATE Node *SimplifyAsm(Node *node, asmNode *u) {
    u->output    = SimplifyList(u->output);
    u->input     = SimplifyList(u->input);
    u->clobbered = SimplifyList(u->clobbered);
    return node;
}

PRIVATE Node *SimplifyAsmArg(Node *node, UNUSED(asmargNode *u)) {
    /* You cannot do anything to the asmargs, so don't even think of it.*/
    return node;
}    

PRIVATE Node *SimplifyBuiltinVaArg(Node *node, builtinvaargNode *u) {
    u->expr    = SimplifyNode(u->expr);
    u->type    = SimplifyNode(u->type);
    return node;
}

PRIVATE Node *SimplifySpawn(Node *node, spawnNode *u) {
    u->receiver = SimplifyNode(u->receiver);
    u->args = SimplifyList(u->args);
    return node;
}

PRIVATE Node *SimplifyInletCall(Node *node, inletcallNode *u) {
    u->args     = SimplifyList(u->args);
    return node;
}
PRIVATE Node *SimplifyDecl(Node *node, declNode *u) {
    u->init = SimplifyNode(u->init);
    return node;
}

PRIVATE Node *SimplifyProc(Node *node, procNode *u) {
    u->body = SimplifyNode(u->body);
    return node;
}

GLOBAL List *SimplifyProgram(List *program) {
    return SimplifyList(program);
}

PRIVATE Node *SimplifyNode(Node *node) {
    if (node == NULL)
	  return NULL;
#define CODE(name, node, union) return Simplify##name(node, union)
     ASTSWITCH(node, CODE)
#undef CODE
	 UNREACHABLE;
     return NULL;
}
PRIVATE List *SimplifyList(List *list) {
    List *aptr;
    for (aptr = list; aptr; aptr = Rest(aptr)) {
	Node *item = FirstItem(aptr);
	
	SetItem(aptr, SimplifyNode(item));
    }

    return list;
}
