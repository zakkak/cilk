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

#ifndef _AST_EQUALS_H
#define _AST_EQUALS_H


#include "astModify.h"

/*** Note: the behavior of these functions
 *   on trees has been tested on expressions
 *   and on most types of statements. However,
 *   comparisons on declarations may not work.
 *
 *   There are still questions remaining in the
 *   Equals function about which fields for each
 *   node to compare and which ones to ignore.
 *
 *   For ex. an idNode has a decl field which is
 *   currently being ignored.
 *   Other nodes have fields which are filled in by
 *   the semantic-checker (ex. a binop has a Type field).
 *   These fields are also currently being ignored.
 *   
 *
 *   The test cases for these functions in testEquals.c
 *     obtained the AST from the functions in stringParse.c
 *     These AST's are often incomplete. Therefore,
 *     it is more likely that these functions will 
 *     consider two things equal that one might consider
 *     unequal, rather than the other way around.
 *
 *
 *   In short - if you use these functions, be prepared to modify
 *     them when things don't work... (especially the Equal___ functions
 *     for each node)
 *
 *
 *
 * Things which are known to be odd?
 *  
 *   1. structs - putting fields in different order
 *        struct {int x; double y;} and struct {double y; int x;}
 *        are different.
 *
 * 
 *
***/

/*****************************************************************************/

/* Returns TRUE if S1 and S2 are equal strings (or both NULL) */
Bool EqualStrings(const char *s1, const char *s2);


/* Returns TRUE if u1 and u2 have the same typ
   fields, or if they both NULL */
Bool EqualTypeNodes(Node *u1, Node *u2);


/* Returns TRUE if n1 and n2 are equal nodes.  n1 and n2 
   can still have different children however.  (i.e. any field
   which is labeled as ChildNode or ChildList can still be
   different).   */
Bool EqualNodes(Node *n1, Node *n2);


/* Returns TRUE if n1 and n2 represent the same syntax tree 
 *  Not every field is used in the equality comparison.
 *   For example, the Coord field for nodes is ignored.
 *    Only fields which make the node syntactically different  
 *    compared.
 */
Bool EqualAST(Node *n1, Node *n2);

/* Same function as EqualAST, but takes in a list of nodes */
GLOBAL Bool EqualASTList(List *l1, List *l2);



/* Returns TRUE if tree n1 contains n2.
   If either tree is NULL, returns FALSE */
Bool ContainsAST(Node *n1, Node *n2);

/* Returns a pointer to the subtree of n1 which
   matches n2, or NULL if no such subtree is found. */
Node* ContainsSubtree(Node *n1, Node *n2);

/* Same as previous two, except that all matches
   are returned.  The list is stored in a post order, with
   the deepest matches in the tree coming first. */
List* GetAllMatchingSubtrees(Node *n1, Node *n2);

/* Searches N1 for a subtree matching N2, and replaces it a copy of the
   REPLACEMENT tree.  Returns TRUE if a replacement was made, FALSE otherwise. */
Bool ReplaceSyntaxSubtree(Node *n1, Node *n2, Node *replacement);

/* Searches N1 for all subtrees matching N2, and replaces each with a copy of
   REPLACEMENT tree.  Returns the number of replacements made.  */
int ReplaceAllMatchingSubtrees(Node *original, Node *n2, Node *replacement);


/*int ReplaceAllMatchingSubtreesInProgram(List *original, Node *match, Node *replacement);*/


Bool MatchReplacePatternTree(Node *original, Node *pattern, Node *replacement);

GLOBAL Bool PatternEqualsAST(Node *original, Node *pattern);


/* Attempts to match ORIGINAL to the specified PATTERN.  The matching uses
   any wildcard values that were stored in wcTable.  If the function
   returns TRUE, the appropriate wildcard values to satisfy the pattern
   match are stored in the table.
   The function requires wcTable has been initialized.
*/
GLOBAL Bool PatternMatch(Node *original, Node *pattern, WildcardTable *wcTable);

/* Attempts to match ORIGINAL_TREE to the pattern string.  If there is a match,
   originalTree is modified to reflect the change. Otherwise, returns NULL */
Node* MatchReplace(Node *originalTree, char *pattern, char *replacement);


#endif   /* ifndef _AST_EQUALS_H */












