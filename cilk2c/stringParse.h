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


#ifndef _STRING_PARSE_H
#define _STRING_PARSE_H

/* This is the character that we
   put in front of any string
   so that Bison knows not to
   expect a program.  See
   ANSI-C.y  */
#define STRING_START_SYMBOL '?'

#include "ast.h"
#include "astModify.h"
#include "wildcard.h"
/* Types of input that the parser will be
   allowed to accept.  The normal mode of
   operation is EntireProgram  */

typedef enum {
  EntireProgram,        /* Program input from a file (the normal case) */
  GenericStringInput,   /* Any input from a string (either of the last 2 cases ) */
  StatementListString, /* An statement list */
  DeclListString      /* A declaration list */
} BisonInputType;


/* These are the global variables where the parser in
 *  ANSI-C.y stores the output AST, when input is read 
 *  in from a string. 
 * (Normal operation, when the input is read in from a
 *   file, stores the AST in the global variable 
 *   Program.
 */

GLOBAL extern List *StmtListOutput;
GLOBAL extern List *DeclListOutput;

/* This function is called by the
   Bison parser to signify that
   an error has occurred. */
GLOBAL void StringParseError();

/* Function returns the syntax tree obtained by
   parsing INPUT_STRING.  If there is an
   error in parsing, then NULL is returned.

   This function assumes the Bison parser does
   the following:
      1. Calls StringParseError() if the string
          parses incorrectly
      2. Sets the bison input type (retrieved using
	  GetBisonInputType() to be either
	  StatementListString or DeclListString
	  if the string parses correctly into
	  one of those two types of Lists.
   Strings that contain wildcards
   can be parsed.  A more detailed description of the
   kinds of strings that can be parsed is given below.
*/
GLOBAL List* ParseStringIntoAST(char *inputString);

/* This version of the function parses the input string
    into a syntax tree, but only returns the first node
    in the list.  For a valid input, this has the
    same effect as FirstItem(ParseStringIntoAST(inputString));
*/
GLOBAL Node* ParseStringIntoASTNode(char *inputString);

/* Generates the AST corresponding to the string that
   was passed in, with wildcards filled in with the optional
   arguments provided.  The optional wildcards should be provided
   in the same order they appear in the string. */
GLOBAL List* ParseWildcardString(char* formatString, ...);

/* Same function, but only returns the first node of the list. */
GLOBAL Node* ParseWildcardStringNode(char* formatString, ...);

/* Returns TRUE if the node N can fill the place of
   the specified wildcard. */
GLOBAL Bool IsCorrectWCType(WildcardType wType, Node *n);



/*  Test Case Procedures and other misc. functions*/

/* Tests IsCorrectWCType */
GLOBAL Bool TestWCTypeCheck();

/* Tests the parsing of statementlist and declarationlist strings
 that don't have wildcards.  Returns TRUE if no errors encountered. */
GLOBAL Bool TestParseStringNoWildcards();

/* Tests the parsing of statementlist and declarationlist strings
   with  wildcards.  Returns TRUE if no errors encountered. */
GLOBAL Bool TestParseStringWithWildcards();



/* Non-automated test for checking ParseWildcardString */
/*  Run diff on ParseWildOut.txt and ParseWildCorrect.txt
    Output is correct if there are no differences */
GLOBAL void TestParseWildcardString();

/* Tests to make sure bad strings fail to parse. */
GLOBAL Bool TestParseBadWildcards();



#endif /* _STRING_PARSE_H */

/***************************************************************
 * ParseStringIntoAST and ParseWildcardString:
 * 
 *  Both these functions are designed to accept a string of C
 *   code and return an abstract syntax tree associated with
 *   the statement.
 *  Any string of C code which represents is a list of statements
 *   or a list of declarations is accepted as input
 *   (exceptions are listed below)
 *
 *  Properties of the Current implementation:
 *
 *    - All strings are parsed without using a symbol table.  
 *       Unknown identifiers are parsed into idNode's automatically.
 *       Some effects of this are:
 *         1. Declarations of a type defined using "typedef" are
 *             not accepted. Because no lookups/insertions are done
 *             with a symbol table, the Bison parser won't accept 
 *             "typedef int q; q x;" because it doesn't know "q" is
 *             now a type.
 *         2. All declarations are given the type-qualifier T_TOP_DECL.
 *             Duplicate declarations are possible.
 *             "int x = 10; double x = 123.3;" parses.
 *         3. When identifiers are parsed, the decl field for the
 *             idNode is NULL.
 *    - Some types of inputs are accepted but break the program:
 *         1. Statements which are usually inside a container when
 *             being parsed with Bison.  For example, a "return"
 *             statement which is not contained within a function
 *             will not parse correctly.
 *         2. Other potential problems might be "case" or "default"
 *             outside a "switch", or "continue" or "goto".
 *
 *
 *  Both functions also accept wildcard tokens in their string input.
 *  Wildcard tokens are denoted by "%", followed by the wildcard letter
 *    and any number of digits.  For example, "%e" or "%d1234"
 *
 *    
 *  Valid wildcard letters are: %d, %e, %t
 *
 *  When ParseWildcardString is called, the caller must pass in one
 *   argument for every wildcard token present in the string.
 *
 *    %d : The user passes in the string representing the identifier
 *          name.  In the abstract syntax tree, if %d takes the place
 *          of an identifier, we call MakeId() with the specified id
 *          name.  The generated Id node has a NULL decl field.
 *
 *            Ex. ParseWildcardString("%d + 3;", "x") generates the syntax
 *                 tree for the statement "x + 3;"
 *                ParseWildcardString("int %d;", "x") generates the
 *                  syntax tree for the declaration "int x;"
 *
 *    %e : The user passes in a Node* object representing an expression,
 *           A copy of this Node* object replaces the wildcard in the AST.
 *
 *            Ex. ParseWildcardString("23 + %e;", MakeConstSint(32))
 *                generates the tree "23 + 32;"
 *
 *         Note that "%e" can represent expressions, but in many cases
 *           "%e;" effectively can hold the place for a statement.
 *         So the user can pass in an "IfNode", for ex.
 *
 *    %t:  The user passes in a Node* object that represents a type.  
 *            
 *            Ex. ParseWildcardString("%t x = 10;", MakePrim(Sint))
 *                generates the AST for  "int x = 10;"
 *
 *    Combinations of the all the wildcards are possible.
 *      Ex. ParseWildcardString("%t %d = %e;",
 *                              MakePrim(Sint),
 * 				x,
 *				MakeConstSint(32))
 *
 *  Addition from last version:
 *     Using digits after the wildcard letter allows the user
 *     to name the wildcard, so the same wildcard can be repeated twice.
 *     For example,  when parsing "%e1 + %e1 - %e2", the user only
 *     needs to pass in two arguments.  The order of the arguments is
 *     still determined by reading from left to right, but skipping
 *     any wildcard names which are repeated.
 *        Ex. ParseWildcardString("%e1 - %e2 + %e1.%d1->%d1;", e1, e2, d1);
 *
 *     Having an unnamed wildcard (omitting the digits) still legal
 *     In this case, it is assumed that each wildcard is unique.  So parsing
 *      "%e1 + %e2;" and "%e + %e;" is the same.  
 *
 *     Limitations:
 *         Named wildcards and unnamed wildcards should not be
 *         mixed in the same string.  (The current implementation
 *         deals with unnamed wildcards by appending a "unique" number
 *         to each name.  It is possible that the user might
 *         inadvertently use the same name.)
 *
 *         The number of unnamed wildcards that can be used
 *          in one string is currently limited to be less than
 *          10^NUM_EXTRA_DIGITS (see InsertUniqueWildcards in wildcard.c)
 *      
 *
 * Details of the current implementation:
 *  1. The order that the arguments are supplied
 *       to the function should be the same order as they appear
 *       in the string. This is usually not the same as the order
 *       we hit them in the tree walk.
 *      For example, "%t %d = 10;" has a root Decl node whose
 *        name must be changed to the %d argument, and then the
 *        type child must be replaced with the node for %t.
 *
 *    
 *  2. There is no checking to see if enough arguments were passed
 *      in (how do we do this?)
 *  3. There is no check to see if the right kind of node is passed
 *       in for %e or %t. We could fill it with something invalid
 *       using this function.
 *  4. Coords are handled in somewhat of an undetermined manner.
 *      I think when reading in from a string, the line number is
 *       set to 0, and the column number is set to the index in the
 *       string.
 *
 * Possible changes:
 *   1. Possibly have a wildcard for declarations.  For example, right now
 *       "{%e; %e; %e;}" parses as a block with no declarations,
 *        but 3 statements. We can't just substitute a declaration
 *        node for %e. 
 *         
 * Modifications to the AST, and other code:
 *   1. Instead of creating a wildcard node, an extra type field, wTyp
 *       was added to each Node.  By default, this field is NonWildcard
 *       for most nodes, but for certain nodes it can be WildcardE,
 *       WildcardD, or WildcardT
 *     The %e wildcard is actually a made from an IdNode, with the same
 *       name as the wildcard.
 *
 *     The %t wildcard is a Tdef node which has no type qualifiers,
 *        same name as the wildcard, and whose type pointer is to
 *        a primitive int (the Int_ParseOnly to be more precise).
 *        This is as though someone wrote the code
 *          "typedef int %t;"
 *          This type actually serves no purpose other than to
 *           keep some things from breaking (some functions assert
 *           that the type field is not NULL).
 * 
 *     The %d wildcard is also (at least initially) an IdNode, like
 *        the %e wildcard.  However, in the bison parser,
 *        when doing a declaration, we sometimes call
 *        ConvertIdToDecl.
 *
 *   The main advantage of adding an extra field instead of creating
 *    a new type of node is that most of the existing code in the Bison
 *    parser will automatically work.  If we were to add a new type of 
 *    node, then if we change how the program deals with an IdNode, then
 *    the we have to make the same changes for the appropriate wildcard
 *    node.
 *   The main disadvantage is that this may let things that we don't
 *     want through the parser?  
 *     
 *
 *   The Flex file is modified so that the user can switch to read input
 *     from a string, by calling SwitchLexInputToString(), and
 *     then calling DeleteLexInputBuffer() after it has been parsed
 *     using yyparse().
 *     Currently there is no mechanism for switching input back to a file.
 *       so files have to be read in first.  (maybe calling SetFile
 *       in main would work though)
 *
 *   The Bison grammer usually accepts an entire program as input.
 *     However, it has been modified so it can also take in
 *     statement.list and declaration.list as starting points as well.
 *     To prevent introducing additional conflicts, an extra
 *     start symbol is added to the beginning of each string
 *     so Bison can distinguish from its normal mode of operation.
 *
 *    %e in the grammar file is accepted as a "primary.expression" are.
 *    %t is accepted as a valid "declaration.specifier"
 *    %d is accepted in some (but not all yet) places where
 *       IDENTIFIER was.
 */
