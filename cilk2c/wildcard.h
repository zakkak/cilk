#ifndef _WILDCARD_H
#define _WILDCARD_H

#include <stdio.h>
#include "basics.h"


typedef enum {
  WildcardD,    /* Identifier wildcard  %d */
  WildcardE,    /* Expression wildcard  %e */ 
  WildcardT,    /* Type wildcard        %t */
  NonWildcard   /* A normal node.          */
} WildcardType;

/* Prints the letter corresponding to the WildcardType
   to file, or "-" if not a wildcard. */
GLOBAL void PrintTypeLetter(FILE *out, WildcardType t);

/* Returns the type of wildcard given the string name */
GLOBAL WildcardType WildcardTypeFromName(const char* wcName);



/* Scans through INPUT_STRING, from left to right,
   and returns a List of the WildcardType's found. */
GLOBAL List* GetWildcardTypes(char* inputString);


/* Scans through INPUT_STRING, from left to right,
   and returns a List of strings corresponding
   to the names of the wildcards found. */
GLOBAL List* GetWildcardNames(const char *str);

/* This procedure takes all the unnamed
   wildcards in the string and assigns them
   unique names.  (named and unnamed wildcards
   should not be mixed in the same string).  See
   implementation in c4.l */
GLOBAL char* NameAllWildcards(const char *inputString);


/**** Test case code **/

/* Testing PrintTypeLetter */
GLOBAL Bool TestWildcardLetterFunctions();

/* Goes through a set of strings, calls GetWildcardTypes(),
   and prints the the output.
   Returns TRUE if all tests are passed. */
GLOBAL Bool TestGetWildcardTypeCases();


/* Goes through a set of strings, calls GetWildcardTypes(),
   and prints the the output.
   Not an automated test. */
GLOBAL Bool TestGetWildcardNameCases();

/* Testing PrintTypeLetter */
GLOBAL Bool TestWildcardLetterFunctions();
     
/* This is an automated test, but it will break if
   we change FIRST_NEW_WC_NAME and NUM_EXTRA_DIGITS from
   * 100 and 3, respectively. */
GLOBAL Bool TestNameAllWildcards();

#endif /* _WILDCARD_H */



