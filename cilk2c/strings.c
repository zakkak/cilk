
/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 * 
 *************************************************************************/
/*
 * Copyright (c) 1949-2002 Massachusetts Institute of Technology
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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/strings.c $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

#define TABLE_SIZE  231

typedef struct eT {
     char *string;
     struct eT *next;
} entryType;

PRIVATE entryType *hash_table[TABLE_SIZE];

/*  copy_string:
 * Copy string to create location, and return the create location.
 */
PRIVATE char *copy_string(const char *string)
{
     char *new_string;

     new_string = HeapNewArray(char, strlen(string) + 1);
     return strcpy(new_string, string);
}

/* hash table function */
PRIVATE short hash_function(const char *string)
{
     unsigned short i, k;
     unsigned long val;

     assert(string != NULL);

     val = (short) string[0] + 1;
     for (i = 1; i < 8; i++) {
	  if (string[i] == 0)
	       break;
	  k = string[i] & 0x3f;
	  val *= k + 7;
     }
     return ((short) (val % TABLE_SIZE));
}

GLOBAL char *UniqueString(const char *string)
{
     short bucket = hash_function(string);
     entryType *entry;

     for (entry = hash_table[bucket]; entry != NULL; entry = entry->next)
	  if (strcmp(string, entry->string) == 0)
	       return (entry->string);

     /* not found */
     entry = HeapNew(entryType);

     entry->string = copy_string(string);
     entry->next = hash_table[bucket];
     hash_table[bucket] = entry;

     return (entry->string);
}
