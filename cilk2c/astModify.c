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

#include "astModify.h"


GLOBAL Node* CopyAST(Node *n) {
  return NodeCopy(n, SubtreeExact);
}


/* Replaces the node N1 points to with a copy of 
 * the tree N2 points to.  Requires that N1 and
 *  N2 are both not NULL */
GLOBAL void ReplaceAST(Node *n1, Node *n2) {
  if (n1 && n2) {
    *n1 = *CopyAST(n2);
  }
}



/* Helper functions used for the ASTWALKTWO macro. */

PRIVATE void FillWildcardsFromTableFail(WildcardTable *UNUSED(wcTable)) {
  return;
}

PRIVATE Bool FillWildcardsFromTableContinue(Node *UNUSED(patternTree),
					    Node *UNUSED(placeHolder),
					    WildcardTable *UNUSED(wcTable)) {
  return TRUE;
}


PRIVATE void FillWildcardsFromTableHelper(Node *patternTree,
					  Node *UNUSED(placeHolder),
					 WildcardTable *wcTable) {
  Node *lookupValue;
  const char* wcName;

  if (patternTree) {
   
    if (IsWildcard(patternTree)) {
      wcName = GetWildcardName(patternTree);
      /*      printf("Looking for wildcard with name %s \n", wcName);*/
      lookupValue = LookupWildcard(wcTable, wcName);
      if (!lookupValue) {
	printf("Error: wildcard not found. \n");
	/*	PrintWildcardTable(stdout, wcTable);*/
	/*	printf("Printed table. \n");*/
      }
      else {
	
	if (IsDecl(patternTree)) {
	  patternTree->u.decl.name = lookupValue->u.id.text;
	  patternTree->wTyp = NonWildcard;
	  FillWildcardsFromTableHelper(patternTree, placeHolder, wcTable);
	}
	else {
	    /*	  printf("Getting here to replace pattern tree:. \n");*/
	    /*	  PrintNode(stdout, patternTree, 4);*/
	    /*	  printf("\n");*/
     	  
	  ReplaceAST(patternTree,lookupValue);
	  patternTree->wTyp = NonWildcard;
	}
      }
    }
    else {
      ASTWALKTWO(patternTree, placeHolder, wcTable,
		 FillWildcardsFromTableHelper,
		 FillWildcardsFromTableContinue,
		 FillWildcardsFromTableFail);
    }
    
  }
}



/* Takes in PATTERN_TREE (an AST with wildcards) and a WildcardTable.
 * The tree is traversed, and the wildcards are filled in using
 *  values stored in the table.
 *
 *
 */
GLOBAL void FillWildcardsFromTable(Node *patternTree, WildcardTable *wcTable) {
  FillWildcardsFromTableHelper(patternTree, NULL, wcTable);
}















