/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 * 
 *************************************************************************/
/*
 * Copyright (c) 1994-2002 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2002 Bradley C. Kuszmaul
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

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/symbol.c $ $LastChangedBy: bradley $ $Rev: 1469 $ $Date: 2004-08-02 07:27:43 -0400 (Mon, 02 Aug 2004) $");

/* TABLE_SIZE must be a power of two */
#define TABLE_SIZE 16
#define TABLE_MASK (TABLE_SIZE - 1)

typedef struct {
     short level;
     short version;
} Scope;

typedef struct symbolstruct Symbol;
struct symbolstruct {
     const char *name;
     Generic *var;
     Scope scope;
     Symbol *next;
     Symbol *shadow;
     Symbol *scope_next;
};

struct tablestruct {
     Symbol *table[TABLE_SIZE];
     const char *table_name;
     TableType kind;
     Symbol *scopes[MAX_SCOPE_DEPTH];
     ShadowProc shadow;
     ExitscopeProc exitscope;
     struct tablestruct *next_table;
};

GLOBAL short Level = 0;
GLOBAL Bool TrackScopeExits = TRUE;
extern Bool TrackInsertSymbol;	/* main.c */

PRIVATE short current_version[MAX_SCOPE_DEPTH];
PRIVATE SymbolTable *table_list = NULL;

/***********************************************************************\
* Symbol Table Hash Function
\***********************************************************************/

PRIVATE inline unsigned hash(const char *name)
{
     return (unsigned) (((unsigned long) (name) >> 2) & TABLE_MASK);
}

/***********************************************************************\
* Symbol memory management
\***********************************************************************/

PRIVATE Symbol *free_list = NULL;

PRIVATE Symbol *new_symbol(void)
{
     static const unsigned int blocks = 4096 / (sizeof(Symbol));
     Symbol *sym;

     sym = free_list;
     if (sym == NULL) {
	 unsigned int i;

	  free_list = HeapNewArray(Symbol, blocks);
	  sym = free_list;
	  for (i = 0; i < blocks - 1; i++) {
	       sym->next = &free_list[i];
	       sym = sym->next;
	  }
	  sym->next = NULL;
	  sym = free_list;
     }
     free_list = free_list->next;
     return (sym);
}

PRIVATE void free_symbol(Symbol *sym)
{
     assert(sym != NULL);
     /* fprintf(stderr, "FREE %s (%d,%d)\n", sym->name, sym->scope.level,
      * sym->scope.version); */
     sym->next = free_list;
     free_list = sym;
}

/***********************************************************************\
* NewSymbolTable
\***********************************************************************/

GLOBAL SymbolTable *NewSymbolTable(const char *name, TableType kind,
				   ShadowProc shadow,
				   ExitscopeProc exitscope)
{
     SymbolTable *create;

     create = HeapNew(SymbolTable);
     create->table_name = name;
     create->kind = kind;
     create->shadow = shadow;
     create->exitscope = exitscope;

     /* this table to the list of tables */
     create->next_table = table_list;
     table_list = create;

     return (create);
}

/***********************************************************************\
* ResetSymbolTable
\***********************************************************************/

GLOBAL void ResetSymbolTable(SymbolTable *table)
{
     int i;
     Symbol *chain, *next;

     for (i = 0; i < TABLE_SIZE; i++) {
	  for (chain = table->table[i]; chain != NULL; chain = next) {
	       if (table->exitscope != NULL)
		    (*table->exitscope) (chain->var);
	       next = chain->next;
	       free_symbol(chain);
	  }
	  table->table[i] = NULL;
     }
}

/***********************************************************************\
* Enter/Exit nested scope
\***********************************************************************/

GLOBAL void EnterScope()
{
     Level++;
     if (Level == MAX_SCOPE_DEPTH) {
	  fprintf(stderr, "Internal Error: out of nesting levels!\n");
	  abort();
     }
     current_version[Level]++;
}

GLOBAL void ExitScope()
{
     if (Level == 0) {
	  SyntaxError("missing '{' detected");
     } else {
	  /* notify dead Generics */
	  SymbolTable *table;
	  for (table = table_list; table != NULL; table = table->next_table) {
	       if (table->exitscope != NULL) {
		    Symbol *var;
		    for (var = table->scopes[Level];
			 var != NULL; var = var->scope_next)
			 (*table->exitscope) (var->var);
		    table->scopes[Level] = NULL;	/* reset Generic list */
	       }
	  }
	  Level--;
     }
     /* PrintSymbolTable(stderr, Identifiers); */
}

PRIVATE inline Bool stale(Scope scope)
{
     return ((scope.level > Level) ||
	     (current_version[scope.level] > scope.version));
}

/***********************************************************************\
* Symbol creation
\***********************************************************************/

PRIVATE Symbol *make_symbol(const char *name, Generic *var,
			    short level, short version)
{
     Symbol *sym = new_symbol();
     sym->name = name;
     sym->var = var;
     sym->scope.level = level;
     sym->scope.version = version;
     sym->shadow = sym->next = NULL;
     return (sym);
}

/***********************************************************************\
* InsertSymbol
\***********************************************************************/

/* returns the final version of the inserted Generic.  It differs from
 * the input `var' only when there is a redeclaration, in which case
 * the value is determined by the `conflict' procedure.
 * 
 * The `conflict' procedure is called when the create Generic conflicts
 * with an existing (current scope) symbol.  It is expected to modify
 * the `orig' Generic to represent the merger of the two versions.
 * For example, it may just declares a syntax error and leave the original
 * alone. */

GLOBAL Generic *InsertSymbol(SymbolTable *table, const char *name,
			     Generic *var,
			     ConflictProc conflict)
{
     unsigned int bucket;
     Symbol *chain, *sym;
     Symbol **handle;
     short level;

     if (TrackInsertSymbol)
	  fprintf(stderr, "InsertSymbol(%s, %s) %s scope=(%d,%d)\n",
		  table->table_name, name,
		  table->kind == Nested ? "Nested" : "Flat",
		  Level, current_version[Level]);

     if (table->kind == Nested) {
	  sym = make_symbol(name, var, Level, current_version[Level]);
	  level = Level;
     } else {
	  /* all symbols in a flat table have scope (0,0) and thus
	   * can never be stale */
	  sym = make_symbol(name, var, 0, 0);
	  level = 0;
     }

     if (table->exitscope != NULL) {
	  /* add to list of Generics at this level */
	  sym->scope_next = table->scopes[level];
	  table->scopes[level] = sym;
     }
     bucket = hash(name);
     chain = table->table[bucket];
     handle = &(table->table[bucket]);	/* *handle == chain */

     while (chain != NULL) {
	  if (chain->name == name) {
	       Symbol *next = chain->next;
	       while (chain != NULL && stale(chain->scope)) {
		    Symbol *tmp = chain->shadow;
		    free_symbol(chain);
		    chain = tmp;
	       }
	       if (chain == NULL) {
		    *handle = sym;
		    sym->next = next;
		    assert(sym->shadow == NULL);
		    return (var);
	       } else {
		    assert(!stale(chain->scope));
		    if (chain->scope.level == level) {
			 /* patch table in case we removed stale symbols */
			 *handle = chain;
			 chain->next = next;
			 /* resolve conflict */
			 if (conflict == NULL) {
			      FAIL("InsertSymbol(%s) conflicts -- NULL conflict function\n" /* FIX to include name of function */ );
			 }
			 (*conflict) (chain->var, var);
			 return (chain->var);	/* return resolved Generic */
		    } else {
			 assert(chain->scope.level < level);
			 *handle = sym;
			 sym->shadow = chain;
			 sym->next = next;
			 if (table->shadow)
			      (*table->shadow) (sym->var, chain->var);
			 return (var);
		    }
	       }
	       /* unreachable */
	  }
	  handle = &(chain->next);
	  chain = chain->next;	/* == *handle */
     }

     /* insert at front */
     sym->next = table->table[bucket];
     assert(sym->shadow == NULL);
     table->table[bucket] = sym;
     return (var);
}

/***********************************************************************\
* MoveToOuterScope
\***********************************************************************/

GLOBAL void MoveToOuterScope(SymbolTable *table, const char *name)
{
     unsigned int bucket;
     Symbol *chain;

     if (TrackInsertSymbol)
	  fprintf(stderr, "Moving to outer scope %s\n", name);

     assert(Level > 0);
     bucket = hash(name);
     chain = table->table[bucket];

     while (chain != NULL) {
	  if (chain->name == name) {
	       assert(chain->scope.level == Level);
	       assert(chain->scope.version == current_version[Level]);
	       chain->scope.level--;
	       chain->scope.version = current_version[Level - 1];
	       return;
	  }
	  chain = chain->next;
     }
     /* the symbol was not found! */
     assert(FALSE);
}

/***********************************************************************\
* LookupSymbol
\***********************************************************************/

/* returns TRUE if symbol found, *var set to the found var
 * (*var valid only if return value is TRUE */
GLOBAL Bool LookupSymbol(SymbolTable *table,
			 const char *name,
			 GenericREF var)
{
     unsigned int bucket;
     Symbol *chain;
     Symbol **handle;

     bucket = hash(name);
     handle = &(table->table[bucket]);
     chain = *handle;		/* loop invariant */

     while (chain != NULL) {
	  if (chain->name == name) {
	       Symbol *next = chain->next;
	       while (chain != NULL && stale(chain->scope)) {
		    Symbol *tmp = chain->shadow;
		    free_symbol(chain);
		    chain = tmp;
	       }
	       if (chain == NULL) {
		    *handle = next;
		    return (FALSE);
	       } else {
		    assert(!stale(chain->scope));
		    *handle = chain;
		    chain->next = next;
		    *var = chain->var;
		    return (TRUE);
	       }
	       /* unreachable */
	  }
	  handle = &(chain->next);
	  chain = chain->next;	/* == *handle */

     }
     return (FALSE);
}

/***********************************************************************\
* PrintSymbolTable
\***********************************************************************/

GLOBAL void PrintSymbolTable(FILE *out, SymbolTable *table)
{
     Symbol *chain, *shadow;
     int i, entries = 0, length, worst = 0, depth = 0;

     assert(table != NULL);
     fprintf(out, "\nSymbolTable: %s\n", table->table_name);

     for (i = 0; i < TABLE_SIZE; i++) {
	  length = 0;
	  for (chain = table->table[i]; chain != NULL; chain = chain->next) {
	       length++;
	       fprintf(out, "\t%s:", chain->name);
	       for (shadow = chain; shadow != NULL; shadow = shadow->shadow) {
		    fprintf(out, " (%d,%d)",
			    (int) shadow->scope.level,
			    (int) shadow->scope.version);
	       }
	       fputc('\n', out);
	  }
	  entries += length;
	  depth += (length + 1) * length / 2;	/* sum of 1 to length */
	  if (length > worst)
	       worst = length;
     }
     fprintf(out, "End of symbol table %s\n", table->table_name);
     fprintf(out, "\t%d entries\n", entries);
     fprintf(out, "\tAverage depth for a successful search: %.2g\n",
	     depth / (entries + 1e-6));
     fprintf(out, "\tAverage depth for a failed search: %.2g\n",
	     entries / (float) TABLE_SIZE);
     fprintf(out, "\tLongest chain: %d\n", worst);
}

/***********************************************************************\
* InsertUniqueSymbol
\***********************************************************************/

/* Creates unique valid C identifier for Generic and inserts
 * it into symbol table.  At most 16 characters from root are used for
 * prefix of identifier.  Returns identifier chosen. */

GLOBAL const char *InsertUniqueSymbol(SymbolTable *table, Generic *var, const char *root)
{
     char buf[33];
     const char *name;
     static unsigned counter = 0;
     Generic *existing;

     do {
	  sprintf(buf, "%.16s%d", root, ++counter);
	  name = UniqueString(buf);
     }
     while (LookupSymbol(table, name, &existing));

     InsertSymbol(table, name, var, NULL);	/* NULL conflict procedure should
						 * never be called */
     return name;
}

/***********************************************************************\
* Iteration over symbol table
\***********************************************************************/

GLOBAL void IterateSymbolTable(SymbolTableMarker * marker, SymbolTable *table)
{
     marker->table = table;
     marker->i = -1;
     marker->chain = NULL;
}

GLOBAL Bool NextInSymbolTable(SymbolTableMarker * marker, const char **name,
			      GenericREF var)
{
#define CHAIN ((Symbol *)marker->chain)

     if (marker->i == TABLE_SIZE)
	  return FALSE;
     if (marker->chain != NULL)
	  marker->chain = CHAIN->next;
     while (marker->chain == NULL)
	  if (++marker->i == TABLE_SIZE)
	       return FALSE;
	  else
	       marker->chain = marker->table->table[marker->i];

     *name = CHAIN->name;
     *var = CHAIN->var;
     return TRUE;
}
