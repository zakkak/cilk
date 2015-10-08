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


#include "basics.h"
#include "testEquals.h"
#include "stringParse.h"
#include "wildcard.h"


GLOBAL void RunAllTests() {
  
  RunWildcardParsingTests();
  RunAllEqualsTests();

  printf("\nPassed all wildcard tests. \n");
}

GLOBAL void RunWildcardParsingTests() {
  Bool correct = TRUE;
  printf("\nRunning Wildcard Parsing Tests \n");

  correct = TestWildcardLetterFunctions();

  correct = correct && TestGetWildcardTypeCases();

  correct = correct && TestGetWildcardNameCases();

  correct = correct && TestNameAllWildcards();
  
  correct = correct && TestParseStringNoWildcards();

  correct = correct && TestWCTypeCheck();
  
  correct = correct && TestParseStringWithWildcards();

  correct = correct && TestParseBadWildcards();

  if (correct) {
    printf("\nPassed Wildcard Parsing Tests \n");
  }
  else {
    FAIL("\nFailed Wildcard parsing tests. \n");
  }

  TestParseWildcardString();
  printf("Run diff on ParseWildOut.txt and ParseWildCorrect.txt to check TestParseWildcardString(). \n");
  
}


GLOBAL void RunAllEqualsTests() {
  int numErrors = 0;
  printf("\nRunning Equals Tests \n");
  if (!RunEqualNodeTestCases()) numErrors++;
  if (!RunEqualASTTreeTestCases()) numErrors++;
  if (!RunContainsASTTreeTestCases()) numErrors++;
  if (!RunReplaceASTTestCases()) numErrors++;
  if (!RunContainsSubtreeTestCases()) numErrors++;
  if (!RunReplaceSyntaxSubtreeTestCases()) numErrors++;
  if (!RunCopyASTTestCases()) numErrors++;
  if (!RunGetAllMatchingSubtreeTestCase()) numErrors++;
  if (!RunReplaceAllMatchingSubtreeTestCases()) numErrors++;
  if (!RunPatternEqualTestCases()) numErrors++;
  if (!RunPatternMatchReplaceTestCases()) numErrors++;
  printf("\n");
  if (numErrors == 0) {
    printf("\nPassed all equals tests. \n");
  }
  else {
    FAIL("\nFailed Equals Tests. \n");
  }
}

void TestParseString(char* input) {

  Node *n1;
  
  printf("String is: %s \n", input);
  n1 = ParseStringIntoASTNode(input);
  /* n2 = ParseStringIntoASTNode(input2);*/
  
  printf("The node: \n");
  fPrintNode(stdout, n1, 0);
  printf("\n\n");
}


/* Returns TRUE if the strings parse into
    trees with equal root nodes.  (FALSE if
    either string parses incorrectly.
*/
Bool TestEqualNodes(char *str1, char *str2) {
  Node *n1, *n2;

  n1 = ParseStringIntoASTNode(str1);
  n2 = ParseStringIntoASTNode(str2);

  if ((n1 != NULL) & (n2 != NULL)) {
    return EqualNodes(n1, n2);
  }
  else {
    return FALSE;
  }
}

/* Returns TRUE if the strings parse into
    equal trees.  (FALSE if
    either string parses incorrectly.
*/
Bool TestEqualSyntaxTrees(char *str1, char *str2) {
  List *l1, *l2;
  
  l1 = ParseStringIntoAST(str1);
  l2 = ParseStringIntoAST(str2);

  if ((l1 != NULL) && (l2 != NULL)) {
    ListMarker lwalk1, lwalk2;
    Node *n1, *n2;
    IterateList(&lwalk1, l1);
    IterateList(&lwalk2, l2);

    while ((!EndOfList(&lwalk1)) && (!EndOfList(&lwalk2))) {
      NextOnList(&lwalk1, (GenericREF)&n1);
      NextOnList(&lwalk2, (GenericREF)&n2);
      if (!EqualAST(n1, n2)) {
	return FALSE;
      }
    }
    return (EndOfList(&lwalk1) && (EndOfList(&lwalk2)));
  }
  else {
    return FALSE;
  }
  
/*    n1 = ParseStringIntoASTNode(str1); */
/*    n2 = ParseStringIntoASTNode(str2); */

/*    if ((n1 != NULL) & (n2 != NULL)) { */
/*      //    printf("Both not null. \n"); */
/*      return EqualAST(n1, n2); */
/*    } */
/*    else { */
/*      //   printf("False because here. \n"); */
/*      return FALSE; */
/*    } */
}

/* Returns TRUE if tree represented by str1 contains
   the tree in str2 */
Bool TestContainsSyntaxTrees(char *str1, char *str2) {
  Node *n1, *n2;

  n1 = ParseStringIntoASTNode(str1);
  n2 = ParseStringIntoASTNode(str2);

  if ((n1 != NULL) && (n2 != NULL)) {
      /*    printf("Both not null. \n");*/
    return ContainsAST(n1, n2);
  }
  else {
      /*   printf("False because here. \n");*/
    return FALSE;
  }
}
  
Node* TestReplaceSyntaxSubtree(char *str1, char *str2, char *str3) {
  Node *n1, *n2, *n3;

  n1 = ParseStringIntoASTNode(str1);
  n2 = ParseStringIntoASTNode(str2);
  n3 = ParseStringIntoASTNode(str3);

  if ((n1 != NULL) && (n2 != NULL) && (n3 != NULL)) {
      /*    printf("Both not null. \n");*/
    if( ReplaceSyntaxSubtree(n1, n2, n3)) {
      return n1;
    }
    else {
      return NULL;
    }
  }
  else {
      /*   printf("False because here. \n");*/
    return NULL;
  }
}

Node* TestContainsSubtree(char *str1, char *str2) {
  Node *n1, *n2;

  n1 = ParseStringIntoASTNode(str1);
  n2 = ParseStringIntoASTNode(str2);

  if ((n1 != NULL) & (n2 != NULL)) {
      /*    printf("Both not null. \n");*/
    return ContainsSubtree(n1, n2);
  }
  else {
      /*   printf("False because here. \n");*/
    return NULL;
  }
}

/************************************************************************/

/** Automated Test Cases **/


/* Returns TRUE if all strings passed in parse without error into
   syntax trees. */
Bool RunParseStringTest(char *testStrings[], int numStrings) {
  int i = 0;
  Bool parseError = FALSE;
  int numErrors = 0;

  for (i = 0; i < numStrings; i++) { 
    parseError = (!ParseStringIntoASTNode(testStrings[i]));
    if (parseError) {
      printf("Error parsing string %d:  %s \n", i, testStrings[i]);
      numErrors++;
    }
  }

  return (numErrors == 0);
}


Bool RunEqualNodeTestCases() {
  int i, j;
  int numErrors = 0;
  int numValues = 11;
  char* stringArray1[] = {"10;", "11;", "12.23;", "x;", "x + 5;", "x[32];", "3<<5;", "(int) 32.2;", "g(x+2);", "-3;", "3 ? 4: 0x234;"};
  char* stringArray2[] = {"10;", "11;", "12.23 ;", "x;", "x + 5;", "x[32];", "3<<5;", "(int) 32.2 ;", "g(x+2);", "-3;", "3 ? 4: 0x234;"};
  
  for (i = 0; i < numValues; i++) {
    for (j = 0; j < numValues; j++) {
      if (i == j) {
	if (!TestEqualNodes(stringArray1[i], stringArray2[j])) {
	  numErrors++;
	  printf("Error in EqualNode Test Cases, comparing (%s, %s). \n", stringArray1[i], stringArray2[j]);
	}
      }
      else {
	if (TestEqualNodes(stringArray1[i], stringArray2[j])) {
	  numErrors++;
	  printf("Error in EqualNode Test Cases at (%s, %s) comparison. \n", stringArray1[i], stringArray2[j]);
	}
      }
    }
  }

  if (numErrors == 0) {
    printf("Passed all EqualNode Test Cases \n");
    return TRUE;
  }
  else {
    printf("%d errors in EqualNode Test Cases \n", numErrors);
    return FALSE;
  }
}



/* numValues is the number of test cases provided.
   testCases is the array contains the strings to test. testCase[2i] and testCase[2i+1] are
   equal.  All other pairs should be unequal.  */
Bool RunEqualASTTreeTestCases() {
  int i, j;
  int numErrors = 0;
  Bool testResult;

  char* testCases[] = {"10;", "10;",
                       "(121);", "((121));",
		       "12;", "(12);",
		       "12L;", "12L;",
		       "12LL;", "12LL;",
		       "12Ul;", "12uL;",
		       "12Ull;", "12ull;",
		       "12.0f;", "12.0F;",
		       "11.323 ;", "11.32300 ;", 
		       "12.23 ;", "12.23 ;",
		       "x;", "x;",
		       "x + 5;", "x+5;",
		       "x[32];", "x[32];",
		       "x[(12)];", "x[12];",
		       "x || y;", "x ||y;",
		       "x & 3;", "x & 3;",
		       "g->x;", "(g->x);",
		       "g->x.qwer;", "(g->x).qwer;",
		       "g->x->qwer;", "(g->x)->qwer;",
		       "x =21;", "x = (21);",
		       "x++;", "x++;",
		       "++x;", "++x;",
		       "x--;", "x--;",
		       "--x;", "--x;",
		       "x +=4;", "x+=4;",
		       "3<<5;", "3<<5;",
		       "(int) 32.2;", "(int) 32.2 ;",
		       "(double) 32.2;"," (double) 32.2;",
		       "-3;", "-3;",
		       "-5;", "-5;",
		       "3 ? 4: 0x234;", "3? 4: 0x234;",
		       "g(x-2);", "g(x-2);",
		       "g(x+2);", "g(x+2);",
		       "g(x+3, 21);", "g(x+3, 21);",
		       "g;", "   g;",
		       "ga3;", "ga3 ;",
		       "f(x++, q(1, 324, 43), x *=4, (int) e);", "f(x++, q(1, (324), 43), x *= 4 , ((int) e));",
		       "\"qwer\";", "\"qwer\";",
		       "sizeof(int *);", "sizeof(int *);",
		       "sizeof(int);", "sizeof(int  );",
		       "list.q;", "list.q;",
		       "synched;", "synched;",
		       "sizeof(volatile const int);", "sizeof (const volatile int);",
		       "sizeof(struct {int x; double y;});", "sizeof(struct {int x;  double y;});",
		       "sizeof(struct {double x; int y;});", "sizeof(struct {double x;  int y;});",
		       "sizeof(struct {double y;  int x;});", "sizeof(struct {double y;  int x;});",
		       "(12 + (x + (21 +321)));", "(12 + (x + (21 + 321)));",
		       "(12 + (x + (12 - 32)));", "(12 + (x + (12 - 32)));",
		       "-(-(-(-3)));", "- (-(-((-3))));",
		       "sizeof(qwer[32] );", " sizeof(qwer [32]);",
		       "if (x < 10) {x = 2;} else {x = 32;}", "if (x<10) {x = 2;} else {x = 32;}",
		       "if (x < 10) x = 2; else x = 32;", "if (x<10) x = 2; else x = 32;",
		       "if (x < 10) x = 2;", "if (x < 10) x = 2;",
		       "for (i = 0; i < 10; i++) {x *= i;}", "for (i = 0; i < 10; i++) {x*= i;}",
		       "while (i > 0) {i--;}", "while(i > 0) {i--;}",
		       "do {i++;} while (i < 0);", "do {i++;} while (i < 0);",
		       "switch(x) { case 0: break; case 1: x++; break; default: x = 12;}", "switch(x) { case 0: break; case 1: x++; break; default: x = 12;}",
		       "switch(x) { case 3: break; case 4: x++; break; default: x = 12;}", "switch(x) { case 3: break; case 4: x++; break; default: x = 12;}",
		       "int x = 0;", "int x = 0;",
		       "double y;", "double y;",
		       "const int x = 0;", "const int x = 0;",
		       "const long y = 123;", "const long y = 123;",
		       "const double pi = 3.14159;", "const double pi = 3.14159;",
		       "const char c = '2';", "const char c = '2';",
		       "const char c = '3';", "const char c = '3';",
		       "const char* s = \"234\";", "const char* s = \"234\";",
		       "int x;", "int x;",
		       "int y;", "int y;",
		       "const char s[] = \"234\";", "const char s[] = \"234\";",
		       "double x = 10;", "double x = 10;",
		       "double x = 12;", "double x = 12;",
		       "start: x = 10; if (x > 12) goto stop; stop: x = 23;", "start: x = 10; if (x > 12) goto stop; stop: x = 23;",
		       "start: x = 10; if (x > 12) goto start; stop: x = 23;", "start: x = 10; if (x > 12) goto start; stop: x = 23;",
		       "int s[10];", "int s[10];",
		       "int s[12];", "int s[12];",
		       "double s[10];", "double s[10];",
		       "typedef int q;", "typedef int q;",
		       "typedef double q;", "typedef double q;",
		       "void f(long x) {return (x + 10);}", "void f(long x) {return (x + 10);}", 		       "void f(int x) {return (x + 10);}", "void f(int x) {return (x + 10);}", 
		       "void f(int x) {return (x + 12);}", "void f(int x) {return (x + 12);}",
		       "void f(int x) {x++; return (x + 10);}", "void f(int x) {x++; return (x + 10);}",
		       "void f(int x, int y) {return (x*y + 10);}", "void f(int x, int y) {return (x*y + 10);}",
		       "(1, 2, 3, 4);", "(1, 2, 3, 4);",
		       "(1, 2, 3);", "(1, 2, 3);",
		       "(1, 2, x);", "(1, 2, x);",
		       "(1, 2, 5);", "(1, 2, 5);",
		       "for (i = 0; i < 10; i++) {if (i== 2) continue;}", "for (i = 0; i < 10; i++) {if (i== 2) continue;}",
		       "struct {double x; int y;} q;", "struct {double x; int y;} q;",
		       "struct {double x; int y;} r;", "struct {double x; int y;} r;",
		       "union {double x; int y;} q;", "union {double x; int y;} q;",
		       "union {double x; int y;} r;", "union {double x; int y;} r;",
		       "enum {x, y} q;", "enum{x, y} q;",
		       "enum {x, y, z} q;", "enum{x, y, z} q;",
		       "enum {x, y} r;", "enum {x, y} r;",
		       "spawn fib(n-1);", "spawn fib(n-1);",
		       "spawn fib(n-2);", "spawn fib(n-2);",
		       "inlet void catch(int eval, int i1) { 3 ;}", "inlet void catch(int eval, int i1) { 3 ;}",
		       "inlet void catch(int eval, int i1) { 35 ;}", "inlet void catch(int eval, int i1) { 35 ;}",
		       "result = spawn fib(n); sync;", "result = spawn fib(n); sync;",
		       "int ep(int *u1 __attribute__((__unused__)), int *u2 __attribute__((__unused__)));", "int ep(int *u1 __attribute__((__unused__)), int *u2 __attribute__((__unused__)));", 
  };

  int numTestCases = (sizeof(testCases)/(2*sizeof(char *)));
		     
  printf("Number of test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 2*numTestCases)) {
    for (i = 0; i < 2*numTestCases; i++) {
      for (j = 0; j < 2*numTestCases; j++) {
	  /*	printf("(%d, %d): \n", i, j);*/
	testResult = TestEqualSyntaxTrees(testCases[i], testCases[j]);

	if (((i/2) == (j/2) && (!testResult))
	    || (((i/2) != (j/2)) && (testResult))) {
	  numErrors++;
	  printf("Error in EqualASTTree TestCase (%d, %d): (%s, %s) comparison. \n", i, j, testCases[i], testCases[j]);
	}
      }
    }

    if (numErrors == 0) {
      printf("Passed all EqualASTTree Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in EqualASTTree Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to EqualASTTree do not parse. \n");
    return FALSE;
  }
}




/* numValues is the number of test cases provided.
   testCases is the array contains the strings to test plus answers.
   contains(testCase[3i], testCase[3i+1]) should return result in  testCase[3i+2] (either "TRUE" or "FALSE").  
   equal. */
Bool RunContainsASTTreeTestCases() {
  int i;
  int numErrors = 0;
  Bool testResult;
  
  char* testCases[] = {"10 + 42;", "10;", "TRUE;",
                       "100 - 32;", "12;", "FALSE;",
		       "-(12 + 32.3) ;", "12;", "TRUE;",
		       "4 - 3;", "3;", "TRUE;",
		       "4 + -(3+5);", "3;", "TRUE;",
		       "((23-x) + (4+3)*3);", "x;", "TRUE;",
		       "4 + -6;", "3;", "FALSE;",
		       "f(12.32, 32.21, -231);", "32.21;", "TRUE;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int x; double y;});", "TRUE;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int u; double y;});", "FALSE;",
		       "sizeof(qwer[32]);", "qwer;", "TRUE;",
		       "sizeof(qwer[32] );", "er;", "FALSE;",
		       "23 + f(12.34, x, y);", "f(12.34, x, y);", "TRUE;",
		       "printf(\"qwertyString\");", "\"qwertyString\";", "TRUE;",
		       
  };

  int numTestCases = (sizeof(testCases)/(3*sizeof(char *)));
  printf("Number of Contains test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 3*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

	testResult = TestContainsSyntaxTrees(testCases[3*i], testCases[3*i+1]);


	if ((testResult && (strcmp(testCases[3*i+2], "FALSE") == 0))
	    || (!testResult && (strcmp(testCases[3*i+2], "TRUE") == 0))) {
	  numErrors++;
	  printf("Error: ContainsAST TestCase %i: (%s, %s) comparison not %s. \n", i,
		 testCases[3*i], testCases[3*i+1], testCases[3*i+2]);
	}
    }
  
    if (numErrors == 0) {
      printf("Passed all ContainsAST Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in ContainsAST Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to ContainsAST do not parse. \n");
    return FALSE;
  }
}



Bool RunReplaceASTTestCases() {


  Node *n1, *n2;

  Node *matchPtr = NULL;
  int numErrors = 0;

  printf("Running ReplaceAST Test cases \n");


  n1 = ParseStringIntoASTNode("x + 3*f(q);");
  n2 = ParseStringIntoASTNode("x;");

/*    printf("Node 1: \n"); */
/*    PrintNode(stdout, n1, 0); */

/*    printf("\nNode 2: \n"); */
/*    PrintNode(stdout, n2, 0); */

  ReplaceAST(n1, n2);


/*    printf("\n"); */
  matchPtr = ContainsSubtree(n1, n2);
  
 /*   printf("Matchptr: \n"); */
/*    PrintNode(stdout, matchPtr, 0); */
/*    printf("\n"); */
  
/*    printf("\n\nAfter replacement: \n"); */
/*    printf("Node 1: \n"); */
/*    PrintNode(stdout, n1, 0); */

/*    printf("\nNode 2: \n"); */
/*    PrintNode(stdout, n2, 0); */

  if (numErrors == 0) {
    printf("Passed all  ReplaceAST Test cases \n");
  }
  else {
    printf("%d errors in ReplaceAST \n", numErrors);
  }

  return (numErrors == 0);

}



/* numValues is the number of test cases provided.
   testCases is the array contains the strings to test plus answers.
   contains(testCase[3i], testCase[3i+1]) should return result in  testCase[3i+2] (either "TRUE" or "FALSE").  
   equal. */
Bool RunContainsSubtreeTestCases() {
  int i;
  int numErrors = 0;
  Node* testResult = NULL;
  
  char* testCases[] = {"10 + 42;", "10;", "TRUE;",
                       "100 - 32;", "12;", "FALSE;",
		       "-(12 + 32.3);", "12;", "TRUE;",
		       "4 - 3;", "3;", "TRUE;",
		       "4 + -(3+5);", "3;", "TRUE;",
		       "((23-x) + (4+3)*3);", "x;", "TRUE;",
		       "4 + -6;", "3;", "FALSE;",
		       "f(12.32, 32.21, -231);", "32.21;", "TRUE;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int x; double y;});", "TRUE;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int u; double y;});", "FALSE;",
		       "sizeof(qwer[32]);", "qwer;", "TRUE;",
		       "sizeof(qwer[32] );", "er;", "FALSE;",
		       "23 + f(12.34, x, y);", "f(12.34, x, y);", "TRUE;",
		       "printf(\"qwertyString\");", "\"qwertyString\";", "TRUE;",
		       
  };

  int numTestCases = (sizeof(testCases)/(3*sizeof(char *)));
  printf("Number of Contains test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 3*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

	testResult = TestContainsSubtree(testCases[3*i], testCases[3*i+1]);


	if ((testResult && (strcmp(testCases[3*i+2], "FALSE") == 0))
	    || (!testResult && (strcmp(testCases[3*i+2], "TRUE") == 0))) {
	  numErrors++;
	  printf("Error: ContainsSubtree TestCase %i: (%s, %s) comparison not %s. \n", i,
		 testCases[3*i], testCases[3*i+1], testCases[3*i+2]);
	}
    }
  
    if (numErrors == 0) {
      printf("Passed all ContainsSubtree Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in ContainsSubtree Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to ContainsSubtree do not parse. \n");
    return FALSE;
  }
}



/* numValues is the number of test cases provided.
   testCases is the array contains the strings to test plus answers.
   ReplaceSyntaxSubtree(testCase[4i], testCase[4i+1], testCase[4*i+2]) should return result in  
   testCase[4i+3] (another tree, or NULL if no replacement).  
   equal. */
Bool RunReplaceSyntaxSubtreeTestCases() {
  int i;
  int numErrors = 0;
  Node* testResult = NULL;
  Node* correctAnswer = NULL;
  Node* emptyResult = ParseStringIntoASTNode("NO_REPLACEMENT;");
  
  char* testCases[] = {"10 + 42;", "10;", "23;", "23 + 42;",
                       "100 - 32;", "12;", "x;", "NO_REPLACEMENT;",
		       "-(12 + 32.3);", "12;", "qwert;", "-(qwert + 32.3);",
		       "4 - 3;", "3;", "(int)(23.43);", "4 - (int)(23.43);",
 		       "4 + -(3+5);", "3;", "x.wer;", "4 + -(x.wer + 5);", 
 		       "((23-x) + (4+3)*3);", "x;", "sizeof(int);",  "((23 - sizeof(int)) + (4+3)*3);", 
 		       "4 + -6;", "3;", "23 + sin(pi);", "NO_REPLACEMENT;", 
 		       "f(12.32, 32.21, -231);", "32.21;", "f(x);", "f(12.32, f(x), -231);", 
 		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int x; double y;});", "3;", "231 - 3*3;", 
 		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int u; double y;});", "x || 4;", "NO_REPLACEMENT;", 
 		       "sizeof(qwer[32]);", "qwer;", "diffArray;", "sizeof(diffArray[32]);", 
 		       "sizeof(qwer[32] );", "er;", "erd3;", "NO_REPLACEMENT;", 
 		       "23 + f(12.34, x, y);", "f(12.34, x, y);", "r(4);", "23 + r(4);", 
		       "printf(\"qwertyString\");", "\"qwertyString\";", "\"alpha\";", "printf(\"alpha\");",		       
  };

  int numTestCases = (sizeof(testCases)/(4*sizeof(char *)));
  printf("Number of Replacement test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 4*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

	testResult = TestReplaceSyntaxSubtree(testCases[4*i], testCases[4*i+1], testCases[4*i+2]);

	correctAnswer = ParseStringIntoASTNode(testCases[4*i+3]);

	if ((testResult && (!EqualAST(testResult, correctAnswer)))
	    || (!testResult && (!EqualAST(correctAnswer, emptyResult)))) { 
	  numErrors++;
	  printf("Error: ReplaceSyntaxSubtree TestCase %i: (%s, %s, %s) replace not %s. \n", i,
		 testCases[4*i], testCases[4*i+1], testCases[4*i+2], testCases[4*i+3]);
	  printf("The answer node we got back: \n");
	  fPrintNode(stdout, testResult, 0);
	}
    }
  
    if (numErrors == 0) {
      printf("Passed all ReplaceSyntaxSubtree Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in ReplaceSyntaxSubtree Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to ReplaceSyntaxSubtree do not parse. \n");
    return FALSE;
  }
}


/* numValues is the number of test cases provided.
   testCases is the array contains the strings to test. testCase[2i] and testCase[2i+1] are
   equal.  All other pairs should be unequal.  */
Bool RunCopyASTTestCases() {
  int i;
  int numErrors = 0;

  Node *copyTree, *correctResult;
  
  char* testCases[] = {"10;", "10;",
                       "(121);", "((121));",
		       "12;", "(12);",
		       "11.323 ;", "11.32300 ;", 
		       "12.23 ;", "12.23 ;",
		       "x;", "x;",
		       "x + 5;", "x+5;",
		       "x[32];", "x[32];",
		       "x[(12)];", "x[12];",
		       "x || y;", "x ||y;",
		       "x & 3;", "x & 3;",
		       "g->x;", "(g->x);",
		       "g->x.qwer;", "(g->x).qwer;",
		       "g->x->qwer;", "(g->x)->qwer;",
		       "x =21;", "x = (21);",
		       "x++;", "x++;",
		       "++x;", "++x;",
		       "x--;", "x--;",
		       "--x;", "--x;",
		       "x +=4;", "x+=4;",
		       "3<<5;", "3<<5;",
		       "(int) 32.2 ;", "(int) 32.2 ;",
		       "(double) 32.2;"," (double) 32.2 ;",
		       "-3;", "-3;",
		       "-5;", "-5;",
		       "3 ? 4: 0x234;", "3? 4: 0x234;",
		       "g(x-2);", "g(x-2);",
		       "g(x+2);", "g(x+2);",
		       "g(x+3, 21);", "g(x+3, 21);",
		       "g;", "   g ;",
		       "ga3;", "ga3 ;",
		       "f(x++, q(1, 324, 43), x *=4, (int) e);", "f(x++, q(1, (324), 43), x *= 4 , ((int) e));",
		       "\"qwer\";", "\"qwer\";",
		       "sizeof(int *);", "sizeof(int *);",
		       "sizeof(int);", "sizeof(int  );",
		       "list.q;", "list.q;",
		       "synched;", "synched;",
		       "sizeof(volatile const int);", "sizeof (const volatile int);",
		       "sizeof(struct {int x; double y;});", "sizeof(struct {int x;  double y;});",
		       "sizeof(struct {double x; int y;});", "sizeof(struct {double x;  int y;});",
		       "sizeof(struct {double y;  int x;});", "sizeof(struct {double y;  int x;});",
		       "(12 + (x + (21 +321)));", "(12 + (x + (21 + 321)));",
		       "(12 + (x + (12 - 32)));", "(12 + (x + (12 - 32)));",
		       "-(-(-(-3)));", "- (-(-((-3))));",
		       "sizeof(qwer[32] );", " sizeof(qwer [32]);",
  };

  int numTestCases = (sizeof(testCases)/(2*sizeof(char *)));
		     
  printf("Number of test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 2*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

      copyTree = CopyAST(ParseStringIntoASTNode(testCases[2*i]));
      correctResult = ParseStringIntoASTNode(testCases[2*i + 1]);
      
      if (!EqualAST(copyTree, correctResult)) {
	numErrors++;
	printf("Error in CopyAST TestCase %d: copying %s \n", i, testCases[2*i]);
	printf("The copied result: \n");
	fPrintNode(stdout, copyTree, 0);
      	printf("\n");
	
	printf("The correct result: \n");
	fPrintNode(stdout, correctResult, 0);
	printf("\n");
      }
    }    
  
    if (numErrors == 0) {
      printf("Passed all CopyAST Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in CopyAST Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to CopyAST do not parse. \n");
    return FALSE;
  }
}




Bool RunGetAllMatchingSubtreeTestCase() {

  int i;
  int numErrors = 0;
  Node* testResult;
  Node* correctResult;
  List* matchList;
  ListMarker testMarker;
  int numMatches;
  
  char* testCases[] = {"10 + 42;", "10;", "1;",
                       "100 - 32;", "12;", "0;",
		       "-(12 + 32.3) ;", "12 ;", "1;",
		       "4 - 3;", "3;", "1;",
		       "4 + -(3+5);", "3;", "1;",
		       "((23-x) + (4+3)*3);", "x;", "1;",
		       "4 + -6;", "3;", "0;",
		       "f(12.32, 32.21, -231);", "32.21 ;", "1;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int x; double y;});", "1;",
		       "231 - 3*sizeof(struct {int x; double y;});", "sizeof(struct {int u; double y;});","0;",
		       "sizeof(qwer[32]);", "qwer;", "1;",
		       "sizeof(qwer[32] );", "er;", "0;",
		       "23 + f(12.34, x, y);", "f(12.34, x, y);", "1;",
		       "printf(\"qwertyString\");", "\"qwertyString\";", "1;",
		       "f(f(f(f(x))));", "f;", "4;",
		       "10 + (10 + 3 - 10*sizeof(int)) + q;", "10;", "3;",		       
  };

  int numTestCases = (sizeof(testCases)/(3*sizeof(char *)));
  printf("Number of GetAllMatchingSubtrees test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 3*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

      numMatches = atoi(testCases[3*i+2]);
  
      correctResult = ParseStringIntoASTNode(testCases[3*i+1]);
      matchList = GetAllMatchingSubtrees(ParseStringIntoASTNode(testCases[3*i]),
					 correctResult);

      /*      printf("List length on case %d: %d \n", i, ListLength(matchList));*/


      if ((numMatches != ListLength(matchList))) {
	numErrors++;
	printf("Error: Found %d of %d matches in TestCase %d: (%s, %s)\n",
	        ListLength(matchList), numMatches, i, testCases[3*i], testCases[3*i+1]);
	       
      }
      else {
	  IterateList(&testMarker, matchList);
	  while (!EndOfList(&testMarker)) {
	    NextOnList(&testMarker, (GenericREF)&testResult);
	    if (!EqualAST(testResult, correctResult)) {
	      numErrors++;
	      printf("Found match but returned wrong value in TestCase %d\n", i);
	    }
	  }
      }
    }
  
    if (numErrors == 0) {
      printf("Passed all GetAllMatchingSubtree Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in GetAllMatchingSubtree Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to GetAllMatchingSubtree do not parse. \n");
    return FALSE;
  }
}



Bool RunReplaceAllMatchingSubtreeTestCases() {

  int i;
  int numErrors = 0;

  Node* correctResult;
  Node* replacement;
  Node* original;
  int correctMatches;
  int numMatches;
  
  char* testCases[] = {"10 + 42;", "10;", "23;", "23+42;", "1;",
                       "100 - 32;", "12;", "x;", "100-32;", "0;",
		       "f(f(f(x)));", "f;", "g;", "g(g(g(x)));", "3;",
		       "f + f + f;", "f;", "f+f;", "(f+f) + (f+f) + (f+f);", "3;",
		       "4 - 3;", "3;", "1;", "4 - 1;", "1;",
  		       "4 + -(3+5);", "3;", "1;","4 + -(1+5);", "1;",
  		       "((23-x) + (4+3)*3);", "x;", "z;", "((23-z) + (4+3)*3);", "1;", 
  		       "4 + -6;", "3;", "0;", "4 + -6;", "0;",
  		       "f(12.32, 32.21, -231);", "32.21;", "3.14159;", "f(12.32, 3.14159, -231);", "1;",
  		       "f(f(f(f(x))));", "f;", "g;", "g(g(g(g(x))));", "4;",
  		       "10 + (10 + 3 - 10*sizeof(int)) + q;", "10;", "x;", "x+(x+3-x*sizeof(int))+q;", "3;",    
  };

  int numTestCases = (sizeof(testCases)/(5*sizeof(char *)));
  printf("Number of ReplaceAll test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 5*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

      correctMatches = atoi(testCases[5*i+4]);
      correctResult = ParseStringIntoASTNode(testCases[5*i+3]);
      replacement = ParseStringIntoASTNode(testCases[5*i+2]);
      original = ParseStringIntoASTNode(testCases[5*i]);
  
      numMatches = ReplaceAllMatchingSubtrees(original,
					      ParseStringIntoASTNode(testCases[5*i+1]),
					      replacement);

      /*      printf("List length on case %d: %d \n", i, numMatches);*/


      if ((numMatches != correctMatches)) {
	numErrors++;
	printf("Error: Found %d of %d matches in TestCase %d: (%s, %s, %s)\n",
	       numMatches, correctMatches, i, testCases[5*i], testCases[5*i+1], testCases[5*i+2]);
	       
      }
      else {
	if (!EqualAST(correctResult, original)) {
	  numErrors++;
	  printf("Error: incorrect result for TestCase %d: (%s, %s, %s)\n",
		 i, testCases[5*i], testCases[5*i+1], testCases[5*i+2]);
	}
      }
    }
  
    if (numErrors == 0) {
      printf("Passed all ReplaceAllMatchingSubtree Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in ReplaceAllMatchingSubtree Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to ReplaceAllMatchingSubtree do not parse. \n");
    return FALSE;
  }
}






GLOBAL Bool RunPatternEqualTestCases() {

  int i;
  int numErrors = 0;

  Node* original;
  Node* pattern;
  int correctMatch;
  int wasMatch;
  
  char* testCases[] = {"10 + 42;", "10 + %e1;",  "1;",
                       "100 - 32;", "%e1 - %e2;", "1;",
		       "f(g(h(x)));", "f(%e1(%e2(%e3)));", "1;",
		       "f + f + h;", "%e1 + %e1 + %e2;", "1;",
		       "f + g + h;", "%e1 + %e1 + %e2;", "0;",
		       "f(x) + f(x) + h;", "%e1 + %e1 + %e2;", "1;",
		       "f + h + h;", "%e1 + %e1 + %e2;", "0;",
		       "f(90, 0);", "f(%e1, %e2);", "1;",
		       "231 - 3*sizeof(struct {int x; double y;});", "%e1 - %e2*%e3;", "1;",
		       "f[x+y] - 4;", "%e1[%e2] - %e3;", "1;",
		       "f.x + f.y;", "%e1.x + %e1.y;", "1;",
		       "g.x + g.y;", "%e1.x + %e1.y;", "1;",
		       "{int x = 10;}", "{%t x = %e;}", "1;",
		       "{int x = 10;}", "{%t %d = %e;}", "1;",
		       "{int x = 10; x + 3;}", "{%t %d1 = %e1; %d1 + %e2;}", "1;",
		       "{int x = 10; y + 3;}", "{%t %d1 = %e1; %d1 + %e2;}", "0;",
		       "{int x = 10; x + 10;}", "{%t %d1 = %e1; %d1 + %e1;}", "1;",
  };

  int numTestCases = (sizeof(testCases)/(3*sizeof(char *)));
  printf("Number of Pattern Equal cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 3*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

      correctMatch = atoi(testCases[3*i+2]);
      pattern = ParseStringIntoASTNode(testCases[3*i+1]);
      original = ParseStringIntoASTNode(testCases[3*i]);

      wasMatch = PatternEqualsAST(original, pattern);
      

      printf("Testing %s, %s: \n", testCases[3*i], testCases[3*i+1]);
      
      if ((wasMatch != correctMatch)) {
	numErrors++;
	printf("Failed PatternEqualsAST: original %s and pattern %s. Should be %d \n",
	       testCases[3*i], testCases[3*i+1], correctMatch);
	       
      }
    }
  
    if (numErrors == 0) {
      printf("Passed all PatternEqualsAST Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in PatternEqualsAST Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to PatternEqualsAST do not parse. \n");
    return FALSE;
  }
}


Bool RunPatternMatchReplaceTestCases() {

  int i;
  int numErrors = 0;

  Node* correctResult;
  Node* replacement;
  Node* original;
  int correctMatch;
  int wasMatch;
  
  char* testCases[] = {"10 + 42;", "10 + %e1;", "%e1 * 23;", "42*23;", "1;",
                       "100 - 32;", "%e1 - %e2;", "x(%e1, %e2);", "x(100, 32);", "1;",
		       "f(g(h(x)));", "f(%e1(%e2(%e3)));", "%e3(%e2(%e1(f)));", "x(h(g(f)));", "1;",
		       "f + f + h;", "%e1 + %e1 + %e2;", "%e1 * %e2;", "f*h;", "1;",
		       "f + g + h;", "%e1 + %e1 + %e2;", "%e1 * %e2;", "%e1 * %e2;", "0;",
		       "f(x) + f(x) + h;", "%e1 + %e1 + %e2;", "%e1 * %e2;", "f(x)*h;", "1;",
		       "f + h + h;", "%e1 + %e1 + %e2;", "%e1 * %e2;", "%e1 * %e2;", "0;",
		       "f(90, 0);", "f(%e1, %e2);", "%e1 || %e2;", "90 || 0;", "1;",
		       "231 - 3*sizeof(struct {int x; double y;});", "%e1 - %e2*%e3;", "%e1 * %e2;", "231 * 3;", "1;",
		       "f[x+y] - 4;", "%e1[%e2] - %e3;", "%e1[%e3] + %e2;", "f[4] + (x+y);", "1;",
		       "f.x + f.y;", "%e1.x + %e1.y;", "%e1(x) + %e1(y);", "f(x) + f(y);", "1;",
		       "g.x + g.y;", "%e1.x + %e1.y;", "%e1;", "g;", "1;",
		        "{int x = 10;}", "{%t1 x = %e1;}","%t1 q = (%e1 + %e1);", "int q = 10 + 10;", "1;",
		       "{int x = 10;}", "{%t1 %d1 = %e1;}", "double %d1 = 3; %t1 z = 2*%e1;", "double x = 3; int z = 2*10;",  "1;",
		       "for (i = 0; i < 10; i++) {x += i;}", "for (%e1; %e2; %e3) {%e4;}", "%e1; while (%e2) { %e3; %e4; }", "i = 0; while (i<10) {i++; x+=i;}", "1;",
		       "x->y.z;", "%d1->%d2.%d3;", "%d3->%d1.%d2;", "z->x.y;", "1;",
		        "x->y.z;", "%e1->%d2.%d3;", "%e1.%d3.%d2;", "x.z.y;", "1;",
		       "a->b;", "%e1->%d1;", "(*%e1).%d1;", "(*a).b;", "1;",
		       "(&a)->b;", "(&%e1)->%d1;", "%e1.%d1;", "a.b;", "1;",
		       "(int) x;", "(%t1) x;", "%t1 x;", "int x;", "1;", 
  };

  int numTestCases = (sizeof(testCases)/(5*sizeof(char *)));
  printf("Number of MatchReplacePattern test cases: %d \n", numTestCases);

  if (RunParseStringTest(testCases, 5*numTestCases)) {
    for (i = 0; i < numTestCases; i++) {

      correctMatch = atoi(testCases[5*i+4]);
      correctResult = ParseStringIntoASTNode(testCases[5*i+3]);
      replacement = ParseStringIntoASTNode(testCases[5*i+2]);
      original = ParseStringIntoASTNode(testCases[5*i]);

      printf("Running testcase %s, %s \n", testCases[5*i], testCases[5*i+1]);
      wasMatch = MatchReplacePatternTree(original,
					   ParseStringIntoASTNode(testCases[5*i+1]),
					   replacement);

      /*      printf("List length on case %d: %d \n", i, wasMatch);*/


      if ((wasMatch != correctMatch)) {
	numErrors++;
	printf("Error: Found %d of %d matches in TestCase %d: (%s, %s, %s)\n",
	       wasMatch, correctMatch, i, testCases[5*i], testCases[5*i+1], testCases[5*i+2]);
	       
      }
      else {
	if (!EqualAST(correctResult, replacement)) {
	  numErrors++;
	  printf("Error: incorrect result for TestCase %d: (%s, %s, %s)\n",
		 i, testCases[5*i], testCases[5*i+1], testCases[5*i+2]);

	  printf("Desired result: %s \n", testCases[5*i+3]);

	  printf("replacement is actually : \n");
	  OutputStmtList(stdout, MakeNewList(replacement));
	}
      }
    }
  
    if (numErrors == 0) {
      printf("Passed all MatchPatternReplace Test Cases \n");
      return TRUE;
    }
    else {
      printf("%d errors in MatchPatternReplace Test Cases \n", numErrors);
      return FALSE;
    }
  }
  else {
    printf("Error: Test inputs to MatchPatternReplace do not parse. \n");
    return FALSE;
  }
}




/* Does sizeof on each of the nodes
   in the union u in Node struct */
void PrintNodeSizes() {
  printf("Size of Const: %lu \n", (unsigned long)(sizeof(ConstNode)));
  printf("Size of Id: %lu \n", (unsigned long)(sizeof(idNode)));
  printf("Size of Binop: %lu \n", (unsigned long)(sizeof(binopNode)));
  printf("Size of Unary: %lu \n", (unsigned long)(sizeof(unaryNode)));
  printf("Size of Cast: %lu \n", (unsigned long)(sizeof(castNode)));
  printf("Size of Comma: %lu \n", (unsigned long)(sizeof(commaNode)));
  printf("Size of constr. %lu \n", (unsigned long)(sizeof(constructorNode)));
  printf("Size of ternary %lu \n", (unsigned long)(sizeof(ternaryNode)));
  printf("Size of array %lu \n", (unsigned long)(sizeof(arrayNode)));
  printf("Size of call %lu \n", (unsigned long)(sizeof(callNode)));
  printf("Size of initializer %lu \n", (unsigned long)(sizeof(initializerNode)));
  printf("Size of implicitCast %lu \n", (unsigned long)(sizeof(implicitcastNode)));
  printf("Size of label %lu \n", (unsigned long)(sizeof(labelNode)));
  printf("Size of switch %lu \n", (unsigned long)(sizeof(SwitchNode)));
  printf("Size of case %lu \n", (unsigned long)(sizeof(CaseNode)));
  printf("Size of Default %lu \n", (unsigned long)(sizeof(DefaultNode)));
  printf("Size of ifNode %lu \n", (unsigned long)(sizeof(IfNode)));
  printf("Size of ifelseNode %lu \n", (unsigned long)(sizeof(IfElseNode)));
  printf("Size of while %lu \n", (unsigned long)(sizeof(WhileNode)));
  printf("Size of do %lu \n", (unsigned long)(sizeof(DoNode)));
  printf("Size of for %lu \n", (unsigned long)(sizeof(ForNode)));
  printf("Size of goto %lu \n", (unsigned long)(sizeof(GotoNode)));
  printf("Size of Continue %lu \n", (unsigned long)(sizeof(ContinueNode)));
  printf("Size of break %lu \n", (unsigned long)(sizeof(BreakNode)));
  printf("Size of prim %lu \n", (unsigned long)(sizeof(primNode)));
  printf("Size of tdefNode %lu \n", (unsigned long)(sizeof(tdefNode)));
  printf("Size of ptrNode %lu \n", (unsigned long)(sizeof(ptrNode)));
  printf("Size of adclNode %lu \n", (unsigned long)(sizeof(adclNode)));
  printf("Size of fdcl %lu \n", (unsigned long)(sizeof(fdclNode)));
  printf("Size of sdcl %lu \n", (unsigned long)(sizeof(sdclNode)));
  printf("Size of udcl %lu \n", (unsigned long)(sizeof(udclNode)));
  printf("Size of edcl %lu \n", (unsigned long)(sizeof(edclNode)));
  printf("Size of asm %lu \n", (unsigned long)(sizeof(asmNode)));
  printf("Size of asmArg %lu \n", (unsigned long)(sizeof(asmargNode)));
  printf("Size of spawn %lu \n", (unsigned long)(sizeof(spawnNode)));
  printf("Size of sync %lu \n", (unsigned long)(sizeof(syncNode)));
  printf("Size of inlet %lu \n", (unsigned long)(sizeof(inletcallNode)));
  printf("Size of abortNode %lu \n", (unsigned long)(sizeof(abortNode)));
  printf("Size of synched %lu \n", (unsigned long)(sizeof(synchedNode)));
  printf("Size of declNode %lu \n", (unsigned long)(sizeof(declNode)));
  printf("Size of attribNode %lu \n", (unsigned long)(sizeof(attribNode)));
  printf("Size of procNode %lu \n", (unsigned long)(sizeof(procNode)));
  printf("Size of textNode %lu \n", (unsigned long)(sizeof(textNode)));
}


/* Prompts the user to enter a string, and
   displays the resulting AST generated by
   ParseStringIntoAST() */
GLOBAL void UserPromptParseTest() {
  char someTree[300];

  List *n; 
 
  printf("Enter statement or decl list to be parsed: \n");
  fgets(someTree, sizeof someTree, stdin);
  
 /*   printf("The string is now: %s \n", someTree); */
  n = ParseStringIntoAST(someTree);

  while (n == NULL) {
    printf("Error. Please enter a valid string.\n");
    fgets(someTree, sizeof someTree, stdin);
  
    printf("The string is now: %s \n", someTree);
    n = ParseStringIntoAST(someTree);
  }

  if (n != NULL) {
    printf("The list: \n");
    fPrintList(stdout, n, 0);
    printf("\n");
  }

}
