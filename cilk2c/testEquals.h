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

#ifndef _TEST_EQUALS_H
#define _TEST_EQUALS_H

#include "ast.h"
#include "astEquals.h"

/* Defined in main.c*/
extern Node* ParseStringIntoSyntaxTree(char *inputString);

void TestParseString(char* input);

/* Returns TRUE if the strings parse into
    trees with equal root nodes.  (FALSE if
    either string parses incorrectly. */
Bool TestEqualNodes(char *str1, char *str2);

/* Returns TRUE if the strings parse into
    equal trees.  (FALSE if
    either string parses incorrectly. */
Bool TestEqualSyntaxTrees(char *str1, char *str2);

/* Returns TRUE if tree represented by str1 contains
   the tree in str2 */
Bool TestContainsSyntaxTrees(char *str1, char *str2);

Node* TestReplaceSyntaxSubtree(char *str1, char *str2, char *str3);

Node* TestContainsSubtree(char *str1, char *str2);



/* void TestWildcardTable();*/

/* Returns TRUE if all strings passed in parse without error into
   syntax trees. */
Bool RunParseStringTest(char *testStrings[], int numStrings);

Bool RunEqualNodeTestCases();
Bool RunEqualASTTreeTestCases();
Bool RunContainsASTTreeTestCases();
Bool RunReplaceASTTestCases();
Bool RunContainsSubtreeTestCases();
Bool RunReplaceSyntaxSubtreeTestCases();
Bool RunCopyASTTestCases();
Bool RunGetAllMatchingSubtreeTestCase();
Bool RunReplaceAllMatchingSubtreeTestCases();

Bool RunPatternEqualTestCases();
Bool RunPatternMatchReplaceTestCases();



GLOBAL void RunAllEqualsTests();
GLOBAL void RunWildcardParsingTests();

GLOBAL void RunAllTests();

/* Does sizeof on each of the nodes
   in the union u in Node struct */
GLOBAL void PrintNodeSizes();

GLOBAL void UserPromptParseTest();

#endif /*ifndef _TEST_EQUALS_H */


