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

#ifndef _SYMBOL_H_
#define _SYMBOL_H_
FILE_IDENTITY(symbol_h_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/symbol.h $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

/* two kinds of symbol tables: nested scope or one large flat scope */
typedef enum {
     Nested, Flat
} TableType;

GLOBAL extern short Level;	/* scope depth, initially zero */

typedef void (*ConflictProc) (Generic *orig, Generic *create);
typedef void (*ShadowProc) (Generic *create, Generic *shadowed);
typedef void (*ExitscopeProc) (Generic *dead);

/* create a create symbol table:
 * `shadow' is called when one symbol shadows another,
 * if `shadow' is NULL then no action is taken;
 * `exitscope' is called when an entry becomes dead,
 * if it is NULL then no action is taken */
GLOBAL SymbolTable *NewSymbolTable(const char *name, TableType kind,
				   ShadowProc, ExitscopeProc);

GLOBAL void ResetSymbolTable(SymbolTable *table);

GLOBAL void PrintSymbolTable(FILE *out, SymbolTable *table);

GLOBAL void EnterScope(void);
GLOBAL void ExitScope(void);

GLOBAL Bool LookupSymbol(SymbolTable *, const char *name, Generic **var);

GLOBAL Generic *InsertSymbol(SymbolTable *, const char *name, Generic *var,
			     ConflictProc);

GLOBAL void MoveToOuterScope(SymbolTable *, const char *name);

GLOBAL const char *InsertUniqueSymbol(SymbolTable *table, Generic *var, const char *root);

typedef struct {
     SymbolTable *table;
     int i;
     void *chain;
} SymbolTableMarker;

GLOBAL void IterateSymbolTable(SymbolTableMarker *, SymbolTable *);
GLOBAL Bool NextInSymbolTable(SymbolTableMarker *, const char **name, GenericREF itemref);

#endif				/* ifndef _SYMBOL_H_ */
