/************************************************************************
 * Copyright (c) 1994-2003 Massachusetts Institute of Technology
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

#define EQDEBUG FALSE  /* Whether to print debugging  messages */
#define DUALWALKDEBUG FALSE
#include <stdarg.h>

#include "ast.h"
#include "astEquals.h"
#include "stringParse.h" 

/* Returns TRUE if S1 and S2 are equal strings (or both NULL) */
Bool EqualStrings(const char *s1, const char *s2) {
  if (s1 == NULL) {
    return (s2 == NULL);
  }
  else {
    if (s2 == NULL) {
      return FALSE;
    }
    else {
      return (strcmp(s1, s2) == 0);
    }
  }
}


/* Returns TRUE if u1 and u2 have the same typ
   fields, or if they both NULL */
Bool EqualTypeNodes(Node *u1, Node *u2) {
  if (u1 == NULL) {
    return (u2 == NULL); 
  }
  else {
    if (u2 == NULL) {
      return FALSE;
    }
    else {
      return (u1->typ == u2->typ);
    }
  }
}


/************************************************************************/
/* Expression Equals procedure for each type of node 
 *  Not every field is used in the equality comparison. 
 *   For example, the Coord field is not compared. Only the fields
 *    which make the node syntactically different should be 
 *    compared. 
 */   

/* Prototypes for some helper functions. */
Bool EqualPrim(primNode *u1, primNode *u2);

Bool EqualConst(ConstNode *u1, ConstNode *u2) {

  /* The type field of Const nodes is a Node which is either of
     type Prim or Adcl(Char)
     Legal BasicTypes for primNodes are Sin, Uint,
     Slong, Ulong, Float, or Double */

  primNode *p1, *p2;
  
  if (EqualTypeNodes(u1->type, u2->type)) {
    switch(u1->type->typ) {
    case Prim:
      p1 = &(u1->type->u.prim);  /* Get BasicType of the Type nodes */
      p2 = &(u2->type->u.prim);

      if(EqualPrim(p1, p2)) {
	switch(p1->basic) {
	case Sint:
	  return (u1->value.i == u2->value.i);
	case Uint:
	  return (u1->value.u == u2->value.u);
	case Slong:
	  return (u1->value.l == u2->value.l);
	case Ulong:
	  return (u1->value.ul == u2->value.ul);
	case Float:
	  return (u1->value.f == u2->value.f);
	case Double:
	  return (u1->value.d == u2->value.d);
	default:
	  return FALSE;
	}
      }
      else {
	return FALSE;
      }
    case Adcl:
      return EqualStrings(u1->value.s, u2->value.s);
    default:
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}


Bool EqualId(idNode *u1, idNode *u2) {
  return (EqualStrings(u1->text, u2->text));
}



Bool EqualBinop(binopNode *u1, binopNode *u2) {
  return (u1->op == u2->op);
}

Bool EqualUnary(unaryNode *u1, unaryNode *u2) {
  return (u1->op == u2->op);
}

Bool EqualCast(castNode *UNUSED(u1),
	       castNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualConstructor(constructorNode *UNUSED(u1),
		      constructorNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualComma(commaNode *UNUSED(u1),
		commaNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualTernary(ternaryNode *UNUSED(u1),
		  ternaryNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualArray(arrayNode *UNUSED(u1),
		arrayNode *UNUSED(u2)) {
  return TRUE;
  /*  return (EqualTypeNodes(u1->type, u2->type)); */
}

Bool EqualCall(callNode *UNUSED(u1),
	       callNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualInitializer(initializerNode *UNUSED(u1),
		      initializerNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualImplicitCast(implicitcastNode *u1, implicitcastNode *u2) {
  return (NodeDataType(u1->type) == NodeDataType(u2->type));
}




/* Statement Nodes */

Bool EqualLabel(labelNode *u1, labelNode *u2) {
  return (EqualStrings(u1->name, u2->name));
}


Bool EqualSwitch(SwitchNode *UNUSED(u1),
		 SwitchNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualCase(CaseNode *UNUSED(u1),
	       CaseNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualDefault(DefaultNode *UNUSED(u1),
		  DefaultNode *UNUSED(u2)) {
  return TRUE;
}


Bool EqualIf(IfNode *UNUSED(u1),
	     IfNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualIfElse(IfElseNode *UNUSED(u1),
		 IfElseNode *UNUSED(u2)) {
  return TRUE;
}
Bool EqualWhile(WhileNode *UNUSED(u1),
		WhileNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualDo(DoNode *UNUSED(u1),
	     DoNode *UNUSED(u2)) {
  return TRUE;
}


Bool EqualFor(ForNode *UNUSED(u1),
	      ForNode *UNUSED(u2)) {
  return TRUE;
}


Bool EqualGoto(GotoNode *u1, GotoNode *u2) {
  if (u1->label == NULL) {
    return (u2->label == NULL);
  }
  else {
    if (u2->label == NULL) {
      return FALSE;
    }
    else {
      return (EqualLabel(&(u1->label->u.label), &(u2->label->u.label))); 
    }
  }
}




Bool EqualContinue(ContinueNode *UNUSED(u1),
		   ContinueNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualBreak(BreakNode *UNUSED(u1),
		BreakNode *UNUSED(u2)) {
  return TRUE;
}


Bool EqualReturn(ReturnNode *UNUSED(u1),
		 ReturnNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualBlock(BlockNode *UNUSED(u1),
		BlockNode *UNUSED(u2)) {
/*    printf("Block 1: \n"); */
/*    PrintNode(stdout, u1->type, 0); */
/*    printf("\nBlock 2: \n"); */
/*    PrintNode(stdout, u2->type, 0); */
/*    printf("\n"); */

    /*  printf("Equal?  %d \n", EqualTypeNodes(u1->type, u2->type)); */
    /*  return EqualTypeNodes(u1->type, u2->type); */
  return TRUE;
}





Bool EqualPrim(primNode *u1, primNode *u2) {
  return (tq_equal(u1->tq, u2->tq) && (u1->basic == u2->basic));
}



Bool EqualTdef(tdefNode *u1, tdefNode *u2) {
  return ( EqualStrings(u1->name, u2->name)
	   && tq_equal(u1->tq, u2->tq));
}

Bool EqualPtr(ptrNode *u1, ptrNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}


Bool EqualAdcl(adclNode *u1, adclNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}

Bool EqualFdcl(fdclNode *u1, fdclNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}

Bool EqualSdcl(sdclNode *u1, sdclNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}

Bool EqualUdcl(udclNode *u1, udclNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}

Bool EqualEdcl(edclNode *u1, edclNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}


Bool EqualDecl(declNode *u1, declNode *u2) {
  return (tq_equal(u1->tq, u2->tq)
	  && (EqualStrings(u1->name, u2->name)));
}

Bool EqualAttrib(attribNode *u1, attribNode *u2) {
  return (EqualStrings(u1->name, u2->name));
}


Bool EqualProc(procNode *UNUSED(u1),
	       procNode *UNUSED(u2)) {
 /*   return ((u1->needs_sync == u2->needs_sync) */
/*  	  && (u1->needs_return == u2->needs_return)); */
  return TRUE;
}

Bool EqualText(textNode *u1, textNode *u2) {
  return (EqualStrings(u1->text, u2->text));
}

Bool EqualAsm(asmNode *u1, asmNode *u2) {
  return tq_equal(u1->tq, u2->tq);
}

Bool EqualAsmArg(asmargNode *UNUSED(u1),
		 asmargNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualBuiltinVaArg(builtinvaargNode *UNUSED(u1),
		       builtinvaargNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualSpawn(spawnNode *u1, spawnNode *u2) {
  return ((u1->assign_op == u2->assign_op)
	  && (u1->in_inlet == u2->in_inlet));
}

Bool EqualSync(syncNode *UNUSED(u1),
	       syncNode *UNUSED(u2)) {
  return TRUE;
}


Bool EqualInletCall(inletcallNode *u1, inletcallNode *u2) {
  return (u1->implicit_inlet == u2->implicit_inlet);
}

Bool EqualAbort(abortNode *UNUSED(u1),
		abortNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualSynched(synchedNode *UNUSED(u1),
		  synchedNode *UNUSED(u2)) {
  return TRUE;
}

Bool EqualXBlock(XBlockNode *UNUSED(u1),
		 XBlockNode *UNUSED(u2)) {
  return TRUE;
}



/* Returns TRUE if n1 and n2 are equal nodes.  n1 and n2 
   can still have different children however.  (i.e. any field
   which is labeled as ChildNode or ChildList can still be
   different).   */
Bool EqualNodes(Node *n1, Node *n2) {
  if (EqualTypeNodes(n1, n2)) {
    if (n1 == NULL) {
      return TRUE;
    }
    else {				    
#define CODE(name, n1, n2) return Equal##name(n1, n2)
         DUAL_ASTSWITCH(n1, n2, CODE)
#undef CODE 
      /* The default case is not handled by the switch. */
       return FALSE;
    }
  }
  else {
    return FALSE;
  }
}
							    



/* This is the CONTINUE function provided to DUAL_AST_WALK when doing 
    an equals comparison.
    We stop testing if ANSWER ever becomes FALSE */
Bool EqualASTContinue(Node *UNUSED(n1),
		      Node *UNUSED(n2),
		      Bool *answer) {
  return (*answer);
}

/* This is the FAILOPTION function provided to DUAL_AST_WALK when doing 
   an equals comparison. */
void EqualASTFailcode(Bool *answer) {
  *answer = FALSE;
}


void EqualASTHelper(Node *n1, Node *n2, Bool *answer) {
  if (EqualNodes(n1, n2)) {
    if (n1 != NULL) {
      DUAL_AST_WALK(n1, n2, answer, EqualASTHelper, EqualASTContinue, EqualASTFailcode);
    }
  }
  else {
    *answer = FALSE;
  }
}


GLOBAL Bool EqualAST(Node *n1, Node *n2) {
  Bool answer = TRUE;
  EqualASTHelper(n1, n2, &answer);
  return answer;
}

GLOBAL Bool EqualASTList(List *l1, List *l2) {
  if ((l1 == NULL) && (l2 == NULL)) {
    return TRUE;
  }
  else {
    if ((l1 && l2)) {
      Node *n1, *n2;
      ListMarker lwalk1, lwalk2;

      IterateList(&lwalk1, l1);
      IterateList(&lwalk2, l2);

      while ((!EndOfList(&lwalk1)) && (!EndOfList(&lwalk2))) {
	NextOnList(&lwalk1, (GenericREF)&n1);
	NextOnList(&lwalk2, (GenericREF)&n2);

	if (!EqualAST(n1, n2)) {
	  return FALSE;
	}
      }

      return (EndOfList(&lwalk1) && EndOfList(&lwalk2));
      
    }
    else {
      return FALSE;
    }
  }
}



/****************************************************************************/
/*  Functions for Contain  */


/* This is the CONTINUE function provided to ASTWALKTWO when checking to see
  if N1 contains the tree in N2.
    We continue testing as long as ANSWER = FALSE */
Bool ContainsASTContinue(Node *UNUSED(n1),
			 Node *UNUSED(n2),
			 Bool *answer) {
  return !(*answer);
}

/* This is the FAILOPTION function provided to DUAL_AST_WALK when doing 
   an equals comparison. */
void ContainsASTFailcode(Bool *answer) {
  *answer = FALSE;
}


/* This is the CODE function provided to ASTWALKTWO when checking to see
   if tree in N1 contains tree in N2.  This method requires that
   N2 is not NULL. */
void ContainsASTHelper(Node *n1, Node *n2, Bool *answer) {
  /*  printf("Searching Node 1: \n"); */
/*    PrintNode(stdout, n1, 0); */
/*    printf("\n"); */
  if (EqualAST(n1, n2)) {
    *answer = TRUE;
  }
  else {
    if (n1 != NULL) {
      ASTWALKTWO(n1, n2, answer, ContainsASTHelper, ContainsASTContinue, ContainsASTFailcode);
    }
  }
}


Bool ContainsAST(Node *n1, Node *n2) {
  Bool answer = FALSE;
  if (n2 != NULL) {
    ContainsASTHelper(n1, n2, &answer);
  }
  return answer;
}


/* Keep searching for matches */
Bool ContainsSubtreeContinue(Node *UNUSED(n1),
			     Node *UNUSED(n2),
			     Node **UNUSED(answer)) {
  return TRUE;
}

/* This is the FAILOPTION function provided to DUAL_AST_WALK when doing 
   an equals comparison. */
void ContainsSubtreeFailcode(Node **UNUSED(answer)) {
  /* Does nothing */
}


/* This is the CODE function provided to ASTWALKTWO when checking to see
   if tree in N1 contains tree in N2.  This method requires that
   N2 is not NULL. */
void ContainsSubtreeHelper(Node *n1, Node *n2, Node **answer) {
  /*  printf("Searching Node 1: \n"); */
/*    PrintNode(stdout, n1, 0); */
/*    printf("\n"); */


  if (EqualAST(n1, n2)) {
    *answer = n1;
  }
  else {
    if (n1 != NULL) {
      ASTWALKTWO(n1, n2, answer, ContainsSubtreeHelper, ContainsSubtreeContinue, ContainsSubtreeFailcode);
    }
  }
}


Node* ContainsSubtree(Node *n1, Node *n2) {
  Node *answer = NULL;
  if (n2 != NULL) {
    ContainsSubtreeHelper(n1, n2, &answer);
  }
  return answer;
}





PRIVATE void GetAllMatchingSubtreesHelper(Node *n1, Node *n2, List **answer);

List* GetAllMatchingSubtrees(Node *n1, Node *n2) {
  List *answer = NULL;
  if (n2 != NULL) {
    GetAllMatchingSubtreesHelper(n1, n2, &answer);
  }
  return answer;
}

/* Keep searching for matches */
Bool GetAllMatchingSubtreesContinue(Node *UNUSED(n1),
				    Node *UNUSED(n2),
				    List **UNUSED(answer)) {
  return TRUE;
}

/* This is the FAILOPTION function provided to DUAL_AST_WALK when doing 
   an equals comparison. */
void GetAllMatchingSubtreesFailcode(List **UNUSED(answer)) {
  /* Does nothing */
  return;
}


/* This is the CODE function provided to ASTWALKTWO when checking to see
   if tree in N1 contains tree in N2.  This method requires that
   N2 is not NULL. */
PRIVATE void GetAllMatchingSubtreesHelper(Node *n1, Node *n2, List **answer) {

  if (EqualAST(n1, n2)) {
    if (ListLength(*answer) == 0) {
      *answer = MakeNewList(n1);
    }
    else {
      *answer = ConsItem(n1, *answer);
    }
  }
  else {
    if (n1 != NULL) {
       ASTWALKTWO(n1, n2, answer, GetAllMatchingSubtreesHelper, GetAllMatchingSubtreesContinue, GetAllMatchingSubtreesFailcode);
    }
  }
}







/* Searches N1 for a subtree matching N2, and replaces it a copy of the
   REPLACEMENT tree.  Returns TRUE if a replacement was made, FALSE otherwise. */
Bool ReplaceSyntaxSubtree(Node *n1, Node *n2, Node *replacement) {
  Node *matchPtr;
  if (n1 && n2) {
    matchPtr = ContainsSubtree(n1, n2);
    if (matchPtr != NULL) {
      ReplaceAST(matchPtr, replacement);
      return TRUE;
    }
  }
  return FALSE;
}



/* Searches N1 for all subtrees matching N2, and replaces each with a copy of
   REPLACEMENT tree.  Returns the number of replacements made.  */
int ReplaceAllMatchingSubtrees(Node *original, Node *n2, Node *replacement) {
  List *matchList;
  ListMarker listWalk;
  Node *currentMatch;
  
  if (original && n2) {
    matchList = GetAllMatchingSubtrees(original, n2);
    if (matchList != NULL) {
      IterateList(&listWalk, matchList);

      while (!EndOfList(&listWalk)) {
	NextOnList(&listWalk, (GenericREF)&currentMatch);
	ReplaceAST(currentMatch, CopyAST(replacement));
	/*	PrintNode(stdout, original, 0); */
	/*	printf("\n"); */
      }

      return ListLength(matchList);
    }
  }
  return 0;
}

/* Untested function. */
/*
int ReplaceAllMatchingSubtreesInProgram(List *original, Node *match, Node *replacement) {

  ListMarker _listwalk;
  Node *currentNode;
  int numMatches = 0;

  IterateList(&_listwalk, original);
  while (NextOnList(&_listwalk, (GenericREF)&currentNode)) {
    if (currentNode) {
      numMatches  = numMatches +  ReplaceAllMatchingSubtrees(currentNode, match, replacement);
    }
    SetCurrentOnList(&_listwalk, (Generic *)currentNode);
  }

  return numMatches;
}
*/



/************* Pattern Equal functions  **************/

typedef struct {
  Bool answer;
  WildcardTable *table;
} patternAnswer;


/* This is the CONTINUE function provided to DUAL_AST_WALK when doing 
    an equals comparison.
    We stop testing if ANSWER ever becomes FALSE */
Bool PatternEqualASTContinue(Node *UNUSED(n1),
			     Node *UNUSED(n2),
			     patternAnswer *p) {
  return (p->answer);
}

/* This is the FAILOPTION function provided to DUAL_AST_WALK when doing 
   an equals comparison. */
void PatternEqualASTFailcode(patternAnswer *p) {
  p->answer = FALSE;
}


void PatternEqualASTHelper(Node *original, Node *pattern, patternAnswer *p) {
  Node *lookupVal;
  const char* name;
  const char* originalName;

  /*  printf("Start fo helper. \n"); */
  if ((pattern != NULL) && (IsWildcard(pattern))) {
      /*    printf("seeing a wildcard. \n"); */
    name = GetWildcardName(pattern);
    /*    printf("got tnae as %s \n", name);*/
    lookupVal = LookupWildcard(p->table, name);

    if (WildcardTypeFromName(name) == WildcardD) {
      originalName = GetNodeName(original);
      if (originalName == NULL) {
	printf("Error: trying to match a %%d to a node without a name. \n");
	p->answer = FALSE;
      }
      else {
	  /*	printf("The original node has name %s \n", originalName);*/
	if (lookupVal == NULL) {
	    /*	  printf("inserting wildcard with name %s \n", originalName);*/
	  InsertWildcard(p->table, name, MakeId(originalName));
	}
	else {
	  p->answer = EqualStrings(originalName, GetNodeName(lookupVal));
	}	
      }

      /* If names match, keep walking the tree.*/
      if (p->answer) {
	DUAL_AST_WALK(original, pattern, p, PatternEqualASTHelper,
		      PatternEqualASTContinue,
		      PatternEqualASTFailcode);
      }
    }
    else {
	/* The wildcard is %t or %e. So it is a leaf.*/
	/*  If it isn't in the table, it automatically matches.*/
      if (lookupVal == NULL) {
	InsertWildcard(p->table, name, original);
      }
      else {
	p->answer = EqualAST(original, lookupVal);
      }
    }
  }
  else {
      /*    printf("Not seeing wildcard. \n");*/
    if (EqualNodes(original, pattern)) {
      if (original != NULL) {
	DUAL_AST_WALK(original, pattern, p,  PatternEqualASTHelper, PatternEqualASTContinue, PatternEqualASTFailcode)
      }
    }
    else {
      p->answer = FALSE;
    }
  }
}




GLOBAL Bool PatternEqualsAST(Node *original, Node *pattern) {
  patternAnswer p;
  p.answer = TRUE;
  p.table = NewWildcardTable(0);

  /*printf("Getting here... before call. \n");*/
  PatternEqualASTHelper(original, pattern, &p);


  printf("about to print wildcard table. \n");
  PrintWildcardTable(stdout, p.table);

  return (p.answer);  
}


/* Attempts to match ORIGINAL to the specified PATTERN.  The matching uses
   any wildcard values that were stored in wcTable.  If the function
   returns TRUE, the appropriate wildcard values to satisfy the pattern
   match are stored in the table.
   The function requires wcTable has been initialized.
*/
GLOBAL Bool PatternMatch(Node *original, Node *pattern, WildcardTable *wcTable) {
  patternAnswer p;
  p.answer = TRUE;
  if (wcTable) {
    p.table = wcTable;
    PatternEqualASTHelper(original, pattern, &p);
  }

  return p.answer;
}




Bool MatchReplacePatternTree(Node *original, Node *pattern, Node *replacement) {
  patternAnswer p;
  p.answer = TRUE;
  p.table = NewWildcardTable(0);
  PatternEqualASTHelper(original, pattern, &p);

  if (p.answer) {
    FillWildcardsFromTable(replacement, p.table);
    ResetWildcardTable(p.table);
    return TRUE;
  }
  else {
    ResetWildcardTable(p.table);
    return FALSE;
  }
}


/* Attempts to match ORIGINAL_TREE to the pattern string.  If there is a match,
   function returns the tree corresponding to the REPLACEMENT. Otherwise, returns NULL */
Node* MatchReplace(Node *originalTree, char *patternString, char *replacementString) {
  Node *pattern, *replacement;

  /*  printf("Got into MatchReplace with strings %s, %s\n", patternString, replacementString);*/
  /*  printf("The original tree is : \n");*/
  /* PrintNode(stdout, originalTree, 0);*/
  pattern = ParseStringIntoASTNode(patternString);
  /*  printf("Finishing here... \n");*/
  replacement = ParseStringIntoASTNode(replacementString);
  /*  printf("Got into MatchReplace21 \n");*/
  
  if (pattern && replacement) {
    Bool answer = MatchReplacePatternTree(originalTree, pattern, replacement);
    if (answer) {
	/*      printf("Replacing with \n");*/
	/*    PrintNode(stdout, replacement, 0);*/
      return replacement;
    }
    return NULL;
  }
  else {
    printf("Error: input strings to MatchReplace do not parse. \n");
    return NULL;
  }
}







