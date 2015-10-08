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
#include "ast.h"
#include "symbol.h"
#include "wildcardTable.h"

/* Use a Symbol Table for a wildcard table */

struct wildcardTableStruct {
  SymbolTable *symtable;
  int tableKey;
};

GLOBAL WildcardTable *NewWildcardTable(int tableKey) {
  WildcardTable *create;

  create = HeapNew(WildcardTable);
  create->symtable = NewSymbolTable("Wildcards", Flat, NULL, NULL);
  create->tableKey = tableKey;

  return create;
}

/* Clears the contents of the wildcard table */
GLOBAL void ResetWildcardTable(WildcardTable *table) {
  if (table != NULL) {
    ResetSymbolTable(table->symtable);
  }
}



/* Looks up NAME in the wildcard table, and returns 
   the corresponding node (or NULL if not found) */
GLOBAL Node* LookupWildcard(WildcardTable *table, const char *name) {
  Node *answer;
  Bool found = FALSE;

  if (table != NULL) {
    found = LookupSymbol(table->symtable, UniqueString(name), (GenericREF) &answer);
  }

  if (found) {
      /*    printf("Found %s in table. \n", name);*/
    return answer;
  }
  else {
    return NULL;
  }
}


/* Simply replaces old token. */
PRIVATE void wildcard_conflict(Node *orig, Node *create)
{
  *orig = *create;
}


/* Inserts wildcard with specified NAME into the TABLE.  Any entry with
   the same name gets replaced. NULL values can not be
   inserted into the table. */
GLOBAL void InsertWildcard(WildcardTable *table, const char *name, Node* wildcardValue) {
  if ((table != NULL) && (wildcardValue != NULL)) {
    InsertSymbol(table->symtable, UniqueString(name), wildcardValue, (ConflictProc) wildcard_conflict);
  }
}


GLOBAL void PrintWildcardTable(FILE *out, WildcardTable *table) {

  SymbolTableMarker tableWalk;
  const char *symName;
  Node *currentNode;
  
  if (table == NULL) {
    fprintf(out, "\nNull Wildcard Table\n");
  }
  else {
    fprintf(out, "\nWildcard Table %d: \n", table->tableKey);
    IterateSymbolTable(&tableWalk, table->symtable);
    while (NextInSymbolTable(&tableWalk, &symName, (GenericREF)&currentNode)) {
      fprintf(out, "Wildcard %s: ", symName);
      /*      MyOutputExpr(out, currentNode);*/
    }
    fprintf(out, "\n");
    /*    PrintSymbolTable(out, table->symtable);*/
  }
}

