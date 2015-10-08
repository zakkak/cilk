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

#ifndef _WILDCARD_TABLE_H
#define _WILDCARD_TABLE_H


typedef struct wildcardTableStruct WildcardTable;


GLOBAL WildcardTable *NewWildcardTable(int tableKey);

/* Clears the contents of the wildcard table */
GLOBAL void ResetWildcardTable(WildcardTable *table);

/* Looks up NAME in the wildcard table, and returns 
   the corresponding node (or NULL if not found) */
GLOBAL Node* LookupWildcard(WildcardTable *table, const char *name);


/* Inserts wildcard with specified NAME into the TABLE.  Any entry with
   the same name gets replaced. */
GLOBAL void InsertWildcard(WildcardTable *table, const char *name, Node* wildcardValue);


GLOBAL void PrintWildcardTable(FILE *out, WildcardTable *table);

#endif  /* _WILDCARD_TABLE_H */

