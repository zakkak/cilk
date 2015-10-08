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
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/warning.c $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

#ifndef NO_STDARG
   #include <stdarg.h>
   #define VA_START(ap,fmt)   va_start(ap,fmt)
#else
   /* for many older Unix platforms: use varargs */
   #include <varargs.h>
   extern int	vfprintf(FILE *, const char *, void *);
   #define VA_START(ap,fmt)   va_start(ap)
#endif

GLOBAL int Line = 1, Errors = 0, Warnings = 0;
GLOBAL int LineOffset = 0;

GLOBAL void Fail(const char *file, int line, const char *msg) /* __attribute__((noreturn)) */
{
     fprintf(stderr, "Assertion failed in %s, line %d\n", file, line);
     fprintf(stderr, "\t%s\n", msg);

     if (strcmp(PhaseName, "Parsing") == 0) {
	  fprintf(stderr, "Input: %s, line %d\n", Filename, Line);
     }
#if 0
     exit(10);
#endif
     abort();
}

GLOBAL void SyntaxError(const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     Errors++;
     fprintf(stderr, "%s:%d: ", Filename, Line);
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}

GLOBAL void Warning(int level, const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     if (level > WarningLevel)
	  return;
     Warnings++;
     fprintf(stderr, "%s:%d: Warning: ", Filename, Line);
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}

GLOBAL void Hint(const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     fprintf(stderr, "%s:%d: Hint: ", Filename, Line);
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}

GLOBAL void SyntaxErrorCoord(Coord c, const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     Errors++;
     PRINT_COORD(stderr, c);
     fprintf(stderr, ": ");
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}

GLOBAL void WarningCoord(int level, Coord c, const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     if (level > WarningLevel)
	  return;
     Warnings++;
     PRINT_COORD(stderr, c);
     fprintf(stderr, ": Warning: ");
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}

GLOBAL void HintCoord(Coord c, const char *fmt,...)
{
     va_list ap;
     VA_START(ap, fmt);

     PRINT_COORD(stderr, c);
     fprintf(stderr, ": hint: ");
     vfprintf(stderr, fmt, ap);
     fputc('\n', stderr);
     va_end(ap);
}
