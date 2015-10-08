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


#include <stdarg.h>
#include <stdio.h>

#include "stringParse.h"

GLOBAL List *StmtListOutput;
GLOBAL List *DeclListOutput;

/** Functions defined in c4.l ***/
GLOBAL void SwitchLexInputToString(const char *str);
GLOBAL void DeleteLexInputBuffer();

     
/** Functions defined in ANSI-C.y ***/

GLOBAL void SetBisonInputType(BisonInputType t);
GLOBAL BisonInputType GetBisonInputType();


PRIVATE int numStringParseErrors = 0;

/* This function is called by the
   Bison parser to signify that
   an error has occurred. */
GLOBAL void StringParseError() {
  numStringParseErrors++;
}


/* Adds STRING_START_SYMBOL plus a blank space
   to INPUT_STRING, returns the resulting string */
PRIVATE char* ConcatStartSymbol(char *inputString) {
  int length;
  char* outputString;

  if (inputString == NULL) {
    outputString = (char *)malloc(2);
    strcpy(outputString, " ");
    outputString[0] = STRING_START_SYMBOL;
  }
  else {
    length = strlen(inputString) + 1;
    outputString = (char *)malloc(2 + length);
    strcpy(outputString, "  ");
    outputString[0] = STRING_START_SYMBOL;
    strcat(outputString, inputString);
  }

  return outputString;
}


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
*/
GLOBAL List* ParseStringIntoAST(char *inputString) {

  Bool parse_failed;

  numStringParseErrors = 0;

  SwitchLexInputToString(ConcatStartSymbol(inputString));
  SetBisonInputType(GenericStringInput);
  parse_failed = yyparse();

  DeleteLexInputBuffer();

  if ((parse_failed) || (numStringParseErrors > 0)) {
    return NULL;
  }
  else {
    switch (GetBisonInputType()) {
    case StatementListString:
      return StmtListOutput;
    case DeclListString:
      return DeclListOutput;
    default:
      return NULL;
    }
  }
}


GLOBAL Node* ParseStringIntoASTNode(char *inputString) {
  List *answer = ParseStringIntoAST(inputString);
  if (answer == NULL) {
    return NULL;
  }
  else {
    return FirstItem(answer);
  }
}


/* This function is called by both
   ParseWildcardString and ParseWildcardStringNode
*/
PRIVATE List* ParseWildcardStringHelper(char *formatString, va_list ap) {

  int numArgs;
  List *argList;
  argList = NULL;
  
  /*  printf("Rnnung parse statement\n");*/
  if (formatString == NULL) {
    return NULL;
  }
  else {
    WildcardTable *myWildcardTable = NewWildcardTable(0);
    
    List *wildcardNameList;
  
    Node *currentNode;
    char *currentIdString;
    const char *wcName;
    List* Prog;
    Node* original;
    Node* lookupValue;
    ListMarker listwalk, wTypeListWalk;
    char *currentName;

    /*printf("Parsing %s \n", formatString);*/

    formatString = NameAllWildcards(formatString);

    /* Determine types and number of wildcard
       arguments we expect by scanning through
       the string. */
    wildcardNameList = GetWildcardNames(formatString);
    numArgs = ListLength(wildcardNameList);
    
    
    /* Read in arguments into a list. */
    IterateList(&wTypeListWalk, wildcardNameList);
  
    argList = NULL;

    /*    printf("Num args is:  %d \n", numArgs);*/
    
    while (!EndOfList(&wTypeListWalk)) {
      NextOnList(&wTypeListWalk, (GenericREF)&currentName);
     

      wcName = currentName;
      /*      printf("Going to lookup wildcard %s in format %s. \n", wcName, formatString);*/
      if ((wcName != NULL)) {
	lookupValue = LookupWildcard(myWildcardTable, wcName);
	if (lookupValue) {
	    /*	  printf("Ignoring  %s: entry already in the table.\n", wcName);*/
	}
	else {
	  WildcardType wcTyp = WildcardTypeFromName(currentName);
	  if (wcTyp == WildcardD) {
	      /*   printf("Found wildcard d. \n"); */
	    currentIdString = va_arg(ap, char *);
	    currentNode = MakeId(currentIdString);
	  }
	  else {
	      /*  printf("Not finding wdilcard D?? \n");*/

	    currentNode = va_arg(ap, Node *);
	  }

	  /* Before inserting it, we might consider type-checking
	     the node to see if it matches the wildcard.  For now
	     we assume it is ok.  */

	  if (IsCorrectWCType(wcTyp, currentNode)) {
	      /*  printf("Getting to insert... \n");*/
	    InsertWildcard(myWildcardTable, wcName, currentNode);
	  }
	  else {
	    printf("Error: attempting to fill wildcard %s with wrong node type. \n", formatString);
	    return NULL;
	  }  
	}
      }      
    }

    /* Run through DeclList, fill each statement. */
    Prog = ParseStringIntoAST(formatString);
    if (Prog == NULL) {
      printf("Error: wildcard string %s does not parse. \n", formatString);
      return NULL;
    }
    else {
      IterateList(&listwalk, Prog);

      while (!EndOfList(&listwalk)) {
	NextOnList(&listwalk, (GenericREF)&original);
	FillWildcardsFromTable(original, myWildcardTable);
	SetCurrentOnList(&listwalk, (Generic *)original);
      }
    }

    ResetWildcardTable(myWildcardTable);
    /*    printf("Finished..... \n\n");*/
    return Prog;
  }
}


/* The newer version which uses a wildcard table
   instead of a list to process the arguments. */
GLOBAL List* ParseWildcardString(char* formatString, ...) {
  List *answer;
  va_list ap;
  va_start(ap, formatString);
  answer = ParseWildcardStringHelper(formatString, ap);
  va_end(ap);

  return answer;
}

GLOBAL Node* ParseWildcardStringNode(char* formatString, ...) {
  List *answer;
  va_list ap;
  va_start(ap, formatString);
  answer = ParseWildcardStringHelper(formatString, ap);
  va_end(ap);

  assert(answer);
  assert(Rest(answer)==0);
  return FirstItem(answer);  
}


/* Returns TRUE if the node N can fill the place of
   the specified wildcard. */
GLOBAL Bool IsCorrectWCType(WildcardType wType, Node *n) {
  if (n) {
    switch (wType) {
    case WildcardD:
      return (n->typ == Id);
      break;
    case WildcardE:
      return (IsExpr(n) || IsStmt(n));
      break;
    case WildcardT:
      return (IsType(n));
      break;
    default:
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

GLOBAL Bool TestWCTypeCheck() {
  Node *e1 = ParseWildcardStringNode("x + 32;");
  Node *e2 = ParseWildcardStringNode("if (x < 0) {x = 3;} else {x = 34;}");
  Node *t1 = MakePrim(EMPTY_TQ, Sint);
  Node *t2 = MakePrim(EMPTY_TQ, Double);

  Node *d1 = MakeId("x");
  Node *d2 = MakeId("qwerty");

  Bool correct = TRUE;

  printf("IsType(t1)? %d \n", IsType(t1));
  printf("IsType(t2)? %d \n", IsType(t2));

  printf("Running TestWCTypeCheck() \n");

  correct = correct && !IsCorrectWCType(WildcardD, NULL);
  
  correct = correct && IsCorrectWCType(WildcardD, d1);
  correct = correct && IsCorrectWCType(WildcardD, d2);
  correct = correct && !IsCorrectWCType(WildcardD, e1);
  correct = correct && !IsCorrectWCType(WildcardD, e2);
  correct = correct && !IsCorrectWCType(WildcardD, t1);
  correct = correct && !IsCorrectWCType(WildcardD, t2);
  
  if (!correct)
    printf("Failed TestWCTypeCheck on WildcardD \n");



  correct = correct && IsCorrectWCType(WildcardE, e1);
  correct = correct && IsCorrectWCType(WildcardE, e2);
  /* At this point since we already called MakeID,
     anything that fits into %d can also be %e */
  correct = correct && IsCorrectWCType(WildcardE, d1);
  correct = correct && IsCorrectWCType(WildcardE, d2);
  correct = correct && !IsCorrectWCType(WildcardE, t1);
  correct = correct && !IsCorrectWCType(WildcardE, t2);
  
  
  if (!correct)
    printf("Failed TestWCTypeCheck on WildcardE \n");

  
  correct = correct && IsCorrectWCType(WildcardT, t1);
  correct = correct && IsCorrectWCType(WildcardT, t2);
  correct = correct && !IsCorrectWCType(WildcardT, d1);
  correct = correct && !IsCorrectWCType(WildcardT, d2);
  correct = correct && !IsCorrectWCType(WildcardT, e1);
  correct = correct && !IsCorrectWCType(WildcardT, e2);

  if (!correct)
    printf("Failed TestWCTypeCheck on WildcardT \n");


  correct = correct && !IsCorrectWCType(NonWildcard, d1);
  correct = correct && !IsCorrectWCType(NonWildcard, d2);
  correct = correct && !IsCorrectWCType(NonWildcard, e1);
  correct = correct && !IsCorrectWCType(NonWildcard, e2);
  correct = correct && !IsCorrectWCType(NonWildcard, t1);
  correct = correct && !IsCorrectWCType(NonWildcard, t2);

  
  if (!correct)
    printf("Failed TestWCTypeCheck on NonWildcards \n");

    

  if (correct) {
    printf("Passed TestWCTypeCheck() \n");
  }
  else {
    printf("Error: failed TestWCTypeCheck() \n");
  }
  return correct;
}



/**************** Test Case code *****************/




/* These strings should be correctly parsed into
   statement lists. This set of test cases
   uses only numbers and primitive types,
   and should not involve the symbol table. */
char* correctStmtTestCases[] =
{"10;",
 "(1232);",
 "123.213;",
 "0x123l;",
 "3 + 5;",
 "-32;",				
 "3<<5;",
 "(int) 32.2; ",
 "(double) 32.2; ",
 "3 ? 4: 0x234;", 
 "while(1) {32; 341;}",
 "do {234; 0x123;} while (0);",
 "if (2 < 3) {32; } else {3241;}",
 "(12 + (23 + (21 +321)));",
 "-(-(-(-3)));",
 "sizeof(int *);",
 "sizeof(volatile const int);",
 "32; 1239; 34;",
 "12323 + 32; while (1) {32; 341;}",
 "do {123; } while (0);",
 "if (3 < 4) 32; else 123;",
 "do {123; } while (0); if (3 <4) 32; else 123;",
 "-(32);",
 "+32;",
 "sizeof  3;",
 "alignof(3);",
 "43 >> 3;",
 "43 << 3;",
 "43 / 3 + 43 % 3;",
 "(3 <= 4) && (3 >= 4);",
 "(32 == 32) + (32 != 32);",
 "32^16 + 32|16;",
 "(21, 341);",
 "(1, 2, 3, 4, 5);",
 "~0xF0F0F;",
 "{};",
 "01234;",  /* octal constant....*/
 "0x123 + 0X123;",
 /* "return 32;"  This case fails...*/
};

/* These strings should also correctly parse
   into statement lists.  These test cases
   use variables. */
char* correctStmtVarTestCases[] =
{"x;",
 "x + 5;", 
 "x[32];",
 "x[(12)];", 
 "x || y;", 
 "x & 3;", 
 "g->x;", 
 "g->x.qwer;",
 "g->x->qwer;", 
 "x =21;" 
 "x++;", 
 "++x;", 
 "x--;",
 "--x;",
 "x > 234;",
 "x +=4;",
 "5*sizeof(struct {int x; double y;});",
 "sizeof(struct {int x; double y;});",
 "sizeof(struct {double x; int y;});",
 "sizeof(struct {double y;  int x;});", 
 "(12 + (x + (12 - 32)));",    
 "sizeof(qwer[32] );",
 "for (i = 0; i < 10; i++) { x = x + i; }",
 "while (x < 0) {x--;}",
 "{32;} ",
 "{32;}",
 "switch(i) { case 0: i = 3; break; case 1: i = 2; break; default: i = 12; } ",
 "(int) x;",
 "*(&(x));",
 "f();",
 "x %= (!(3 > 4)*2 + 1);",
 "x /= 2; x-=3;",
 "q <<= 2; q >>=2;",
 "w^=3; w &= 3; w |= 3;",
  "{ int x;}",
 "{int x; x /= 4;}",
 "{int x; double y;}",
 "for (i = 0; i< 10; i++) { i*= 2; break;}",
 "while (x < 32) {x++; if (x%3 == 0) continue;}",
 "start: x = 32; for (i = 0; i < 10; i++) {if (i == -2) goto stop;} stop: i = 21;",
 "__alignof__(x);",
 "__alignof__(double);",
 "(int) ((double) x);",
};


/* These strings should correctly parse
   into declaration lists. */
char* correctDeclTestCases[] =
{
  "int x;",
  "double y;",
  "unsigned int x;",
  "const int q;",
  "signed x;",
  "unsigned q;",
  "static float qw;",
  "char c;",
  "short w;",
  "long qwerty = 0xFFFFFFF;",
  "register int x;",
  "auto int x;",
  "struct {int x; double y;} q;",
  "struct empty { };",
  "struct empty e1;",
  "struct empty2 {} e2;",
  "void foo() { struct empty; struct empty2 { struct empty *x; }; struct empty {  struct empty2 *y; }; struct incomplete *pi; }",
  /*  "struct empty a[5];",*/
  "union uempty {};",
  "union uempty ue1;",
  "union uempty2 {} ue2;",
  "void ufoo() { union uempty;  union uempty2 {  union uempty *x;  };  union uempty { union uempty2 *y; }; }",
  "typedef int q;",
  /*  "typedef int qqwe;  qqwe x;",*/
  /*  "qqwe cr;",*/
  /*  "union uempty ua[5]",*/
  "int testFunction(int numArgs, ...);",
};



/* These strings should correctly parse into
  statement lists, with %e wildcards */
char* correctStmtWildcardETestCases[] =
{"%e;",
 "x + %e;", 
 "%e + f(%e);",
 "x[%e];", 
 "x || %e;", 
 "%e & 3;", 
 "x > %e;",
 "y = %e;",
 "x += %e;",
 "if (%e) { %e;} else { %e;}",
 "if (%e) { %e;} else { %e;} ",
 "{ 34; %e; 234; }",
 "while (%e) {%e; }",
 "for (i = %e; i < %e; %e) {%e;}",
 "do {%e;} while (%e );",
 "switch(%e) {case 1: %e; break; case %e: break; default: break;}",
 "%e + (%e + (%e + (%e + (%e))));",
};


/* These strings should parse correctly into
   declaration lists with %t tokens */
char* correctDeclWildcardTestCases[] =
{
  "%t x;",
  "double y; %t q;",
  "%t f(int x, double y);",
  "int f(%t x, double q);",
  "%t x = 12;",
  "%t x = %e;"
  "%t f(int x, double y) {return (x + y);}",
  "int f(%t x, double q) {return (x + q);}",
  "int f(%t x, double y) {x = x + 1; return f(x, y);}",
  "int f(%t x, double q) {return (x + q + %e);}",
  "int f(%t x, double y) {x = x + 1; %e; return f(x, %e);}",
};



/* Parses all the strings in INPUT_ARRAY, and returns
   the number of errors encountered. */
PRIVATE int TestParseHelper(char* inputArray[], int numTestCases) {
  List *answer = NULL;
  int numErrors = 0;
  int i;
  for (i = 0; i < numTestCases; i++) {
      /*    printf("  Testcase %d: %s \n", i, inputArray[i]); */
    answer = ParseStringIntoAST(inputArray[i]);
    if (answer == NULL) {
      numErrors++;
      printf("    Error in testcase %d: failed to parse  %s \n", i, inputArray[i]);
    }
  }
  return numErrors; 
}

/* Tests the parsing of statementlist and declarationlist strings
 that don't have wildcards.  Returns TRUE if no errors encountered. */
GLOBAL Bool TestParseStringNoWildcards() {

  int numErrors = 0;
  int numTestCases = 0;

  printf("Running TestParseStringNoWildcards(): \n");

  printf("  executing Statement test. \n");
  numTestCases = sizeof(correctStmtTestCases) / sizeof(char *);
  numErrors += TestParseHelper(correctStmtTestCases, numTestCases);

  printf("  executing Statement with variable test. \n");
  numTestCases = sizeof(correctStmtVarTestCases) / sizeof(char *);
  numErrors += TestParseHelper(correctStmtVarTestCases, numTestCases);
  
  printf("  executing Declaration test. \n");
  numTestCases = sizeof(correctDeclTestCases) / sizeof(char *);
  numErrors += TestParseHelper(correctDeclTestCases, numTestCases);


  if (numErrors == 0) {
    printf("Passed TestParseStringNoWildcards() \n");
    return TRUE;
  }
  else {
    printf("%d errors in TestParseStringNoWildcards() \n", numErrors);
    return FALSE;
  } 
}
/* Tests the parsing of statementlist and declarationlist strings
   with  wildcards.  Returns TRUE if no errors encountered. */
Bool TestParseStringWithWildcards() {

  int numErrors = 0;
  int numTestCases = 0;

  printf("Running TestParseStringWithWildcards(): \n");

  printf("  executing Statement WildcardE test. \n");
  numTestCases = sizeof(correctStmtWildcardETestCases) / sizeof(char *);
  numErrors += TestParseHelper(correctStmtWildcardETestCases, numTestCases);

  printf("  executing Declaration WildcardT test. \n");
  numTestCases = sizeof(correctDeclWildcardTestCases) / sizeof(char *);
  numErrors += TestParseHelper(correctDeclWildcardTestCases, numTestCases);

  if (numErrors == 0) {
    printf("Passed TestParseStringWithWildcards() \n");
    return TRUE;
  }
  else {
    printf("%d errors in TestParseStringWithWildcards() \n", numErrors);
    return FALSE;
  }

}


char* badWildcardStrings[] = {
  "",
  "121",
  "x + %q",
  "double double;",
  "if x + 32;",
  "for (int i = 0; i < 10; i++) {}",
  "for (i = 0; int i < < 10; ) {x+=i;}",
  "for (i = 0; i < 10; i +++) {x +=i;}",
  "for (324kjf) {x += i;}",
  
};

/* Tests to make sure bad strings fail to parse. */
GLOBAL Bool TestParseBadWildcards() {

  int numErrors = 0;
  int numTestCases = 0;

  int i = 0;
  printf("Running TestParseBadWildcards(): \n");

  if (ParseWildcardString(NULL) != NULL) {
    numErrors++;
  }

  numTestCases = sizeof(badWildcardStrings) / sizeof(char *);
  for (i = 0; i < numTestCases; i++) {
    if (ParseWildcardString(badWildcardStrings[i]) != NULL) {
      numErrors++;
      printf("Error: %s should return NULL when parsed. \n",
	     badWildcardStrings[i]);
    }
  }


  if (numErrors == 0) {
    printf("Passed TestParseBadWildcards() \n");
    return TRUE;
  }
  else {
    printf("%d errors in TestParseBadWildcards()  \n", numErrors);
    return FALSE;
  }

}

/* Non-automated test for checking ParseWildcardString */
/*  Run diff on ParseWildOut.txt and ParseWildCorrect.txt
    Output is correct if there are no differences */
void TestParseWildcardString() {
  Node *e1 =(Node*) ParseStringIntoASTNode("312;");
  Node *e2 =(Node*) ParseStringIntoASTNode("(21 + 12);");
  Node *e3 =(Node*) ParseStringIntoASTNode("3<4;");
  Node *e4 =(Node*) ParseStringIntoASTNode("f(3);");
  Node *e5 =(Node*) ParseStringIntoASTNode("x;");

  Node *t1 = MakePrimCoord(EMPTY_TQ, Sint, UnknownCoord);
  Node *t2 = MakePrim(EMPTY_TQ, Double);
  Node *t3 = MakePrim(EMPTY_TQ, Char);
  /*  Node *t4 = MakePrim(EMPTY_TQ, Ulong);*/
  /* Node *t5 = MakePrim(EMPTY_TQ Void);*/

  List* answer = NULL;

  FILE *output, *correctOutput;

  char* inputString;
  int testCaseNum = 0;

  output = fopen("ParseWildOut.txt", "w");
  correctOutput = fopen("ParseWildCorrect.txt", "w");
  
  printf("Beginning TestParseWildcardString() \n");


  testCaseNum++;
  inputString =  "23 + %e;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  printf("\n");
  fprintf(correctOutput, "23+312;\n");


  testCaseNum++;
  inputString =  "x[%e] - %e;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e2, e3);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  printf("\n");
  fprintf(correctOutput, "x[(21+12)]-(3<4);\n");


  testCaseNum++;
  inputString =  "%t x = 23;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  printf("\n");
  fprintf(correctOutput, "int x=23;\n");

  testCaseNum++;
  inputString =  "%t x = %e;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t2, e4);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "double x=f(3);\n");
  
  testCaseNum++;
  inputString =  "int f(%t x, int y);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t2);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int f(double x,int y);\n");

  testCaseNum++;
  inputString =  "%t g(int x, double y);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t3, e4);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "char g(int x,double y);\n");


  testCaseNum++;
  inputString =  "%e + %e; if (%e) {f(%e);} else {x = 23;}";
  printf("Test case %d %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1, e2, e3, e4);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "312+(21+12);if (3<4) {f(f(3));}else {x=23;}\n");

  testCaseNum++;
  inputString =  "%e + %e;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1, e2);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "312+(21+12);\n");

  testCaseNum++;
  inputString =  "(%e->%d).%d;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e5, "field1", "field2");
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "(x->field1).field2;\n");

  testCaseNum++;
  inputString =  "int %d = 10;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString,"qwer");
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int qwer=10;\n");
  
  testCaseNum++;
  inputString =  "%t %d = %e;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t3, "qwer", e2);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "char qwer=(21+12);\n");

  testCaseNum++;
  inputString =  "void f(int x);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "void f(int x);\n");

  testCaseNum++;
  inputString =  "%e1 - %e2 + %e3*(%e1-%e2);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1, e2, e3);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "312-(21+12)+(3<4)*(312-(21+12));\n");


  testCaseNum++;
  inputString =  "%e1 - %e2 + %e5.%d + %e3*(%e1-%e2);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1, e2, e5, "q", e3);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "312-(21+12)+x.q+(3<4)*(312-(21+12));\n");

  testCaseNum++;
  inputString =  "{%e1 + %e21; {%e3;} {{%e4;}}}";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, e1, e2, e3, e4);
  OutputStmtList(output, answer);
  OutputStmtList(stdout, answer);  
  fprintf(correctOutput, "{312+(21+12);{3<4;}{{f(3);}}}\n");


  testCaseNum++;
  inputString = "%t *%d;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "someVal");
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int*someVal;\n");

  

  testCaseNum++;
  inputString = "%t %d[10];";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "someVal");
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int someVal[10];\n");
  
  testCaseNum++;
  inputString = "%t %d[%e];";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "someVal", e1);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int someVal[312];\n");

  testCaseNum++;
  inputString = "%t *%d[%e];";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "someVal", e1);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int*someVal[312];\n");

  testCaseNum++;
  inputString = "%t1 %d1(%t2 %d2, %t3 *%d3, %t1 **%d4, %t1 %d5[], %t1 **%d6[]);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "a", t2, "b", t3, "c", "d", "e", "f");
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int a(double b,char*c,int**d,int*e,int***f);\n");

  testCaseNum++;
  inputString = "%t1* %d1(%t2 %d2, %t3 *%d3, %t1 **%d4, %t1 %d5[], %t1 **%d6[]);";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1, "a", t2, "b", t3, "c", "d", "e", "f");
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int*a(double b,char*c,int**d,int*e,int***f);\n");

  
  testCaseNum++;
  inputString = "%t1 x = (%t2) y;";
  printf("Test case %d: %s \n", testCaseNum, inputString);
  answer = ParseWildcardString(inputString, t1,  t2);
  OutputProgram(output, answer);
  OutputProgram(stdout, answer);  
  fprintf(correctOutput, "int x=(double)y;\n");

  
  /****  The list of strings that don't work or that
   *        work in funny ways.....
   *
   *  1. "int f(int x) {return (x+10);}"
   *
   *      Any function declaration with a body seems to
   *      break the program - actually, the output procedure.
   *       in OutputDeclList.  The procedure which is parsed
   *       this way I think doesn't have the right flag set.
   *
   *      Actually, this example doesn't fail if cilk2c is run
   *       with the -notrans flag.
   *      If you call transform on the function, and don't use the
   *       notrans flag, when you use OutputProgram,
   *       you get extra lines, 
   *          #include <cilk-cilk2c.h>
   *
   *          #undef CILK_WHERE_AM_I
   *          #define CILK_WHERE_AM_I IN_C_CODE
   *
   *  2. "%t g(int x,double y){return (%e+x+y);"
   *      Same problems as #1 - because of the function body.
   *
   */
  
/*    testCaseNum++; */
/*    inputString =  "int f(int x){return (x+10);}"; */
/*    printf("Test case %d: %s \n", testCaseNum, inputString); */
/*    answer = ParseWildcardString(inputString); */
/*    if (Transform) { */
/*      answer = TransformProgram(answer); */
/*    } */
/*    OutputProgram(output, answer); */
/*    OutputProgram(stdout, answer);   */
/*    fprintf(correctOutput, "int f(int x){return (x+10);}"); */

 
/*    testCaseNum++; */
/*    inputString =  "%t g(int x,double y){return (%e+x+y);}"; */
/*    printf("Test case %d: %s \n", testCaseNum, inputString); */
/*    answer = ParseWildcardString(inputString, t1, e1); */
/*    if (Transform) { */
/*      answer = TransformProgram(answer); */
/*    } */
/*    OutputProgram(output, answer); */
/*    OutputProgram(stdout, answer);   */
/*    fprintf(correctOutput, "int g(int x,double y){return (312+x+y);}"); */

  fclose(output);
  fclose(correctOutput);
}






