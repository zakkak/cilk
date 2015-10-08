#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "wildcard.h"

/* Returns the type of wildcard corresponding to
   the passed in character */
PRIVATE WildcardType WildcardLetterType(char c) {
  switch(c) {
  case 'e':
    return WildcardE;
  case 't':
    return WildcardT;
  case 'd':
    return WildcardD;
  default:
/*      printf("Error: Encountering invalid wildcard letter. \n"); */
    return NonWildcard;
  }
}




/* Prints the letter corresponding to the WildcardType
   t to file, or "-" if not a wildcard. */
PRIVATE char GetWCTypeLetter(WildcardType t) {
  switch(t) {
  case WildcardD:
    return 'd';
  case WildcardE:
    return 'e';
  case WildcardT:
    return 't';
  default:
    return '-';
  }
}

/* Prints the letter corresponding to the WildcardType
   t to file, or "-" if not a wildcard. */
GLOBAL void PrintTypeLetter(FILE *out, WildcardType t) {
  switch(t) {
  case WildcardD:
    fprintf(out, "d");
    break;
  case WildcardE:
    fprintf(out, "e");
    break;
  case WildcardT:
    fprintf(out, "t");
    break;
  default:
    fprintf(out, "-");
  }
}




/* Returns the type of wildcard given the string name. */
GLOBAL WildcardType WildcardTypeFromName(const char* wcName) {
  if ((wcName == NULL) || (strlen(wcName) < 2)) {
    return NonWildcard;
  }

  if (wcName[0] != '%') {
    return NonWildcard;
  }
  return WildcardLetterType(wcName[1]);
}


/* Scans through INPUT_STRING, from left to right,
   and returns a List of the WildcardType's found. */
GLOBAL List* GetWildcardTypes(char* inputString) {
  char currentChar, previousChar;
  int i = 0;
  int length;
  WildcardType *currentType;
  List* answer = NULL;
  
  
  if (inputString == NULL) {
    return NULL;
  }

  length = strlen(inputString);
  
  currentChar = ' ';
  for (i = 0; i < length; i++) {
    previousChar = currentChar;
    currentChar = inputString[i];

    if ((currentChar == '%') && (previousChar != '%')) {
      if (i < length - 1) {
	currentType = (WildcardType *)malloc(sizeof(WildcardType));
	*currentType = WildcardLetterType(inputString[i+1]);
	if (*currentType != NonWildcard) {
	  answer = AppendItem(answer, currentType);
	}
      }
    }
  }
  return answer;
}


/************************************************************/
/* The two following procedures are now defined in c4.l. */
   

/* The name we give the first new wildcard. */
/*#define FIRST_NEW_WC_NAME 100  */
/*#define NUM_EXTRA_DIGITS 3*/

/* This procedure takes all the unnamed*/
/* wildcards in the string and assigns them*/
/* unique names.  */

/*  GLOBAL char* NameAllWildcards(const char *inputString) { */
/*    char currentChar, previousChar; */
/*    int i = 0; */
/*    int length; */
/*    char* newStringName; */
/*    int newIndex; */
/*    int wildcardNum = 0; */
/*    if (inputString == NULL) { */
/*      return NULL; */
/*    } */

/*    // Scan once to find out how much space to allocate  */
/*    length = strlen(inputString); */
/*    currentChar = ' '; */
/*    for (i = 0; i < length; i++) { */
/*      previousChar = currentChar; */
/*      currentChar = inputString[i]; */

/*      //    printf("prev and current pair: %c and %c \n", previousChar, currentChar); */
/*      if ((currentChar == '%') && (previousChar != '%')) { */
/*        if (i < length - 1) { */
/*  	if (WildcardLetterType(inputString[i+1]) != NonWildcard) { */
/*  	  //	  printf("looking ahead, see %c. incrementing \n", inputString[i+1]); */
/*  	  wildcardNum++; */
/*  	  //	  printf("Found a wildcard %c\n", inputString[i+1]); */
/*  	} */
/*        }   */
/*      } */
/*    } */

/*    //  printf("Found %d wildcards in %s \n", wildcardNum, inputString); */

/*    // Allocate space for extra numbers.  */
/*    newStringName = (char *)malloc(length + NUM_EXTRA_DIGITS*wildcardNum + 1); */


/*    // Scan again to do the replacement.  */
  
/*    wildcardNum = FIRST_NEW_WC_NAME; */

/*    newIndex = 0; */
/*    currentChar = ' '; */
/*    i = 0; */
/*    while (i <= length) { */
/*      previousChar = currentChar; */
/*      currentChar = inputString[i]; */

/*      if ((currentChar == '%') */
/*  	&& (previousChar != '%') */
/*  	&& (i < length - 1) */
/*  	&& (WildcardLetterType(inputString[i+1]) != NonWildcard) */
/*  	&& (  ((i < length - 2) && (!isdigit(inputString[i+2]))) */
/*  	      || (i == length - 2))) { */
/*        int j; */
/*        int tempWildNum = wildcardNum; */
/*        //      printf("The wildcard is originally %c \n", inputString[i+1]); */
/*        //      printf("we are eplcaing. \n"); */
/*        newStringName[newIndex] = currentChar; */
/*        //      printf("copied %c into %d \n", newStringName[newIndex], newIndex); */
/*        newStringName[newIndex+1] = inputString[i+1]; */
/*        //      printf("copied %c into %d \n", newStringName[newIndex+1], newIndex+1); */

/*        for (j = NUM_EXTRA_DIGITS - 1; j >= 0; j--) { */
/*  	newStringName[newIndex+2+j] = (char)((tempWildNum%10)  + '0'); */
/*  	//	      printf("copied %c into %d\n", newStringName[newIndex+ 2 + j], newIndex + 2 + j); */
/*  	tempWildNum /= 10; */
/*        } */
/*        newIndex = newIndex + 2 + NUM_EXTRA_DIGITS; */
/*        i = i + 2; */
/*        wildcardNum = (wildcardNum + 1) % ((int)pow(10, NUM_EXTRA_DIGITS)); */
/*      } */
/*      else { */
/*        newStringName[newIndex] = currentChar; */
/*        //      printf("copied %c into %d\n", newStringName[newIndex], newIndex); */
/*        newIndex++; */
/*        i++; */
/*      } */
/*    } */

/*    printf("My string is %s \n", newStringName); */
  
/*    return newStringName; */
/*  } */


/* Scans through INPUT_STRING, from left to right,
   and returns a List of strings corresponding
   to the names of the wildcards found. */
/*  GLOBAL List* GetWildcardNamesOld(const char* inputString) { */
/*    char currentChar, previousChar; */
/*    int i = 0; */
/*    int length = strlen(inputString); */
/*    char* currentName; */

/*    int startWild; */
/*    int endWild; */

/*    List *answer = NULL; */



/*    if (inputString == NULL) { */
/*      return NULL; */
/*    } */
  
/*    currentChar = ' '; */
/*    while (i < length) { */
/*      previousChar = currentChar; */
/*      currentChar = inputString[i]; */
/*      if ( ((currentChar == '%') && (previousChar != '%')) */
/*  	 && (i < length - 1) */
/*  	 && (WildcardLetterType(inputString[i+1]) != NonWildcard)) { */
      
/*  	  startWild = i; */
/*  	  i = i + 2; */
/*  	  while ((i < length) && (isdigit(inputString[i]))) { */
/*  	    i++; */
/*  	  } */
/*  	  endWild = i; */

	 
/*  	  currentName = (char *)malloc(endWild - startWild + 1); */
/*  	  memcpy(currentName, &inputString[startWild], endWild-startWild); */
/*  	  currentName[endWild - startWild] = '\0'; */
	  
/*  	  //	  printf("Found the wildcard string %s \n", currentName); */
/*  	  answer = AppendItem(answer, currentName); */

/*  	  //	  currentChar = inputString[endWild]; */
/*  	  currentChar = ' '; */
/*      } */
/*      else { */
/*            i++; */
/*      } */
/*    } */

/*    return answer; */
  
/*  } */


/************************************************************/
/** Test code ***/


GLOBAL Bool TestWildcardLetterFunctions() {
  int numErrors = 0;

  numErrors += !(GetWCTypeLetter(WildcardE) == 'e');
  numErrors += !(GetWCTypeLetter(WildcardD) == 'd');
  numErrors += !(GetWCTypeLetter(WildcardT) == 't');
  numErrors += !(GetWCTypeLetter(NonWildcard) == '-');

  numErrors += !(WildcardLetterType('e')  == WildcardE);
  numErrors += !(WildcardLetterType('d')  == WildcardD);
  numErrors += !(WildcardLetterType('t')  == WildcardT);
  numErrors += !(WildcardLetterType('-')  == NonWildcard); 

  printf("\n");
  PrintTypeLetter(stdout, WildcardE);
  PrintTypeLetter(stdout, WildcardT);
  PrintTypeLetter(stdout, WildcardD);
  PrintTypeLetter(stdout, NonWildcard);
  printf("\n");
  
  return (numErrors == 0);
}






/* This is an automated test, but it will break if
   we change WC_NUM_START and WC_EXTRA_DIGITS from
   * 100 and 3, respectively. */  
GLOBAL Bool TestNameAllWildcards() {

  int numErrors = 0;

  int i = 0;
  char* inputStrings[] = {
    "%e + %e;", "%e100 + %e101;",
    "%e - %e", "%e100 - %e101",
    "%t + %", "%t100 + %",
    " %t + %", " %t100 + %",
    "%%t + %e - %d", "%%t + %e100 - %d101",
    "12 + 342 - 3;", "12 + 342 - 3;",
    "% + %", "% + %",
    "% + %t12", "% + %t12",
    "% + %e", "% + %e100",
  };
  char* answer;

  
  int numTestCases = sizeof(inputStrings)/(2*(sizeof (char *)));

  if (NameAllWildcards(NULL) != NULL) {
    numErrors++;
    printf("Failed: NameAllWildcards(NULL) is not NULL.\n");
  }
  
  printf("Number of test cases: %d \n", numTestCases);
  for (i = 0; i < numTestCases; i++) {
    answer = NameAllWildcards(inputStrings[2*i]);
    
    if (strcmp(answer, inputStrings[2*i+1]) != 0) {
      numErrors++;
      printf("Failed NameAllWildcards on %s: returned %s instead of %s\n",
	     inputStrings[2*i],
	     answer,
	     inputStrings[2*i+1]);
    }
  }
		     
  return (numErrors == 0);
}




/* Helper function for TestGetWildcardTypeCases(),
   for testing one string. */
GLOBAL char* TestGetWildcardType(char* inputString) {
  List* testValue = GetWildcardTypes(inputString);

  ListMarker listwalk;
  WildcardType *currentType;
  int i = 0;
  int numWildcards = ListLength(testValue);

  char* answer = (char *)malloc(numWildcards + 1);

  answer[numWildcards] = '\0';

  IterateList(&listwalk, testValue);
  
  /*  printf("Wildcards for %s: ", inputString);*/
  while (!EndOfList(&listwalk)) {
    NextOnList(&listwalk, (GenericREF)&currentType);

    answer[i] = GetWCTypeLetter(*currentType);
    i++;
  }

  /*  printf("%s \n", answer);*/
  return answer;
}



/* Goes through a set of strings, calls GetWildcardTypes(),
   and prints the the output.
   Returns TRUE if all tests are passed. */
GLOBAL Bool TestGetWildcardTypeCases() {
  /* The array of test cases.  The first string in each
     pair is parsed, and the 2nd is the expected answer.
     (TestGetWildcardType parses the List that is
     returned and generates the answer )
  */
  char* inputStrings[] = {
    "23 + %e;", "e",
    "x[%e] - %e;", "ee", 
    "%t x = 23;", "t",
    "%t x = %e;", "te", 
    "int f(%t x, int y);", "t",
    "%t g(int x, double y) { return %e + x + y;}", "te",
    "%e + %e; if (%e) {f(%e);} else {x = 23;}", "eeee",
    "%e;", "e",
    "x + %e;", "e",
    "%e + f(%e);", "ee",
    "x[%e];",  "e",
    "x || %e;", "e",
    "%e & 3;",  "e",
    "x > %e;", "e",
    "y = %e;", "e",
    "x += %e;", "e",
    "if (%e) { %e;} else { %e;}", "eee",
    "if (%e) { %e;} else { %e;} ", "eee",
    "{ 34; %e; 234; }", "e",
    "while (%e) {%e; }", "ee",
    "for (i = %e; i < %e; %e) {%e;}", "eeee",
    "do {%e;} while (%e );", "ee",
    "switch(%e) {case 1: %e; break; case %e: break; default: break;}", "eee",
    "%e + (%e + (%e + (%e + (%e))));", "eeeee",
    "x > 234;", "",
    "x +=4;",  "",
    "5*sizeof(struct {int x; double y;});", "",
    "sizeof(struct {int x; double y;});", "", 
    "sizeof(struct {double x; int y;});", "", 
    "sizeof(struct {double y;  int x;});", "",
    "(12 + (x + (12 - 32)));",    "",
    "sizeof(qwer[32] );", "",
    "for (i = 0; i < 10; i++) { x = x + i; }", "",
    "while (x < 0) {x--;}", "",
    "{32;} ", "",
    "{32;}", "",
    "switch(i) { case 0: i = 3; break; case 1: i = 2; break; default: i = 12; } ", "",
    "(int) x;", "",
    "", "",
    "%d->%e", "de",
    "%t %d  - %q", "td",
    "%t %d %d21  %e23", "tdde",
    "%% - %t + %%t", "t",
    "% + 32 - %e32%d", "ed",
  };
  int numTestCases = sizeof(inputStrings) / (2*sizeof(char *));
  int i;
  char* answer;
  int numErrors = 0;

  if (GetWildcardTypes(NULL) != NULL) {
    numErrors++;
  }

  for (i = 0; i < numTestCases; i++) {
    answer = TestGetWildcardType(inputStrings[2*i]);
    if (strcmp(answer, inputStrings[2*i+1]) != 0) {
      printf("Error:  GetWildcardType on %s. Returned %s instead of %s\n",
	     inputStrings[2*i], answer, inputStrings[2*i+1]);
      numErrors++;
    }
    else {
	/*       printf("Passed:  GetWildcardType on %s. Returned %s\n",*/
	/*	     inputStrings[2*i], answer);*/
    }
  }

  if (numErrors > 0) {
    printf("Error: failed TestGetWildcardTypeCases() \n");
  }
  else {
    printf("Passed all TestGetWildcardTypeCases(); \n");
  }

  return (numErrors == 0);
}




GLOBAL char* TestGetWildcardNames(char* inputString) {
  List* testValue = GetWildcardNames(inputString);

  ListMarker listwalk;
  char *currentName;
  int length = strlen(inputString);
  
  char *newOutput = (char *)malloc(length + 1);
  
  strcpy(newOutput, "");

   IterateList(&listwalk, testValue);
   /*  printf("Wildcard names for %s: ", inputString);*/
  while (!EndOfList(&listwalk)) {
    NextOnList(&listwalk, (GenericREF)&currentName);
    /*   printf("%s  ", currentName);*/
    newOutput = strcat(newOutput, currentName);
  }
  /*  printf("Returning %s \n", newOutput);*/

  return newOutput;
}

/* Goes through a set of strings, calls GetWildcardTypes(),
   and prints the the output.
   Returns TRUE if all tests are passed. */
GLOBAL Bool TestGetWildcardNameCases() {
   /* The array of test cases.  The first string in each
     pair is parsed, and the 2nd is the expected answer.
     (TestGetWildcardName parses the List that is
     returned and generates the answer )
  */
  char* inputStrings[] = {
    "23 + %e;", "%e",
    "x[%e] - %e;", "%e%e", 
    "%t x = 23;", "%t",
    "%t x = %e;", "%t%e", 
    "int f(%t x, int y);", "%t",
    "%t g(int x, double y) { return %e + x + y;}", "%t%e",
    "%e + %e; if (%e) {f(%e);} else {x = 23;}", "%e%e%e%e",
    "%e;", "%e",
    "x + %e;", "%e",
    "%e + f(%e);", "%e%e",
    "x[%e];",  "%e",
    "x || %e;", "%e",
    "%e & 3;",  "%e",
    "x > %e;", "%e",
    "y = %e;", "%e",
    "x += %e;", "%e",
    "if (%e) { %e;} else { %e;}", "%e%e%e",
    "if (%e) { %e;} else { %e;} ", "%e%e%e",
    "{ 34; %e; 234; }", "%e",
    "while (%e) {%e; }", "%e%e",
    "for (i = %e; i < %e; %e) {%e;}", "%e%e%e%e",
    "do {%e;} while (%e );", "%e%e",
    "switch(%e) {case 1: %e; break; case %e: break; default: break;}", "%e%e%e",
    "%e + (%e + (%e + (%e + (%e))));", "%e%e%e%e%e",
    "x > 234;", "",
    "x +=4;",  "",
    "5*sizeof(struct {int x; double y;});", "",
    "sizeof(struct {int x; double y;});", "", 
    "sizeof(struct {double x; int y;});", "", 
    "sizeof(struct {double y;  int x;});", "",
    "(12 + (x + (12 - 32)));",    "",
    "sizeof(qwer[32] );", "",
    "for (i = 0; i < 10; i++) { x = x + i; }", "",
    "while (x < 0) {x--;}", "",
    "{32;} ", "",
    "{32;}", "",
    "switch(i) { case 0: i = 3; break; case 1: i = 2; break; default: i = 12; } ", "",
    "(int) x;", "",
    "", "",
    "%d->%e", "%d%e",
    "%t %d  - %q", "%t%d",
    "%t %d %d21  %e23", "%t%d%d21%e23",
    "%% - %t + %%t", "%t",
    "% + 32 - %e32%d", "%e32%d",
    "%t1 %d2 = %e1.%d2", "%t1%d2%e1%d2",
    "%e %e - %e + %e123123121", "%e%e%e%e123123121",
    "%e1 + %e2 - %e3 + %q4 - %w", "%e1%e2%e3",
    "%e1 + %%d32 - %e + %f", "%e1%e",
  };
  int numTestCases = sizeof(inputStrings) / (2*sizeof(char *));
  int i;
  char* answer;
  int numErrors = 0;

  
  if (GetWildcardTypes(NULL) != NULL) {
    numErrors++;
  }
  
  for (i = 0; i < numTestCases; i++) {
    answer = TestGetWildcardNames(inputStrings[2*i]);
    if (strcmp(answer, inputStrings[2*i+1]) != 0) {
      printf("Error:  GetWildcardName on %s. Returned %s instead of %s\n",
	     inputStrings[2*i], answer, inputStrings[2*i+1]);
      numErrors++;
    }
    else {
	/*      printf("Passed:  GetWildcardNames on %s. Returned %s\n",*/
	/*      inputStrings[2*i], answer);*/
    }
  }

  if (numErrors > 0) {
    printf("Error: failed TestGetWildcardNameCases() with %d errors.\n", numErrors);
  }
  else {
    printf("Passed all TestGetWildcardNameCases(); \n");
  }

  return (numErrors == 0);
}











