/*************************************************************************
 *
 *  C-to-C Translator
 *
 *  Adapted from Clean ANSI C Parser
 *  Eric A. Brewer, Michael D. Noakes
 * 
 * Copyright (c) 1994-2002 MIT Laboratory for Computer Science
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
 *************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

FILE_IDENTITY(config_ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/cilk2c/config.h $ $LastChangedBy: bradley $ $Rev: 1341 $ $Date: 2004-06-08 08:20:40 -0400 (Tue, 08 Jun 2004) $");

#include <limits.h>

/* expected suffixes for input and output files */
#define INPUT_SUFFIX   ".cilk"
#define OUTPUT_SUFFIX  ".c"

/* preprocessor command lines */
/* #define DEFAULT_PREPROC       "gcc -E -xc"*/
/* #define ANSI_PREPROC          "gcc -E -ansi -xc"*/

/* maximum number of nested block scopes */
#define MAX_NESTED_SCOPES    100

/* default warning level */
#define WARNING_LEVEL 4

#if 0
/* These DEFAULT_... symbols are obsolete.  The default signedness for all
 * types but char is specified by ANSI C, and the logic in FinishPrimType
 * is hard-coded to follow ANSI.  For char, c-to-c treats unspecified chars 
 * as distinct types from signed char and unsigned char, so no default is
 * required. -- rcm */
/* basic types w/o signed or unsigned default to: */
#define DEFAULT_INT     Sint
#define DEFAULT_SHORT   Sshort
#define DEFAULT_LONG    Slong
#define DEFAULT_CHAR    Schar
#endif

/* host types used hold values of each target type;
   i.e. TARGET_INT resolves to the type of the host used to
   represent ints on the target */
typedef char            TARGET_CHAR;
typedef int             TARGET_INT;
typedef unsigned int    TARGET_UINT;
typedef long            TARGET_LONG;
typedef unsigned long   TARGET_ULONG;

/* target limits */
#define TARGET_MAX_UCHAR  256
#define TARGET_MAX_INT    INT_MAX
#define TARGET_MAX_UINT   UINT_MAX
#define TARGET_MAX_LONG   LONG_MAX
#define TARGET_MAX_ULONG  ULONG_MAX

/* Basic sizes and alignments */
#define CHAR_SIZE         sizeof(char)
#define CHAR_ALIGN        CHAR_SIZE

#define SHORT_SIZE        sizeof(short)
#define SHORT_ALIGN       SHORT_SIZE

#define INT_SIZE          sizeof(int)
#define INT_ALIGN         INT_SIZE

#define FLOAT_SIZE        sizeof(float)
#define FLOAT_ALIGN       FLOAT_SIZE

#define DOUBLE_SIZE       sizeof(double)
#define DOUBLE_ALIGN      DOUBLE_SIZE

#ifdef __GNUC__
#define LONGDOUBLE_SIZE   sizeof(long double)
#else
#define LONGDOUBLE_SIZE   sizeof(double)
#endif
#define LONGDOUBLE_ALIGN  LONGDOUBLE_SIZE

#define LONG_SIZE         sizeof(long)
#define LONG_ALIGN        LONG_SIZE

#ifdef __GNUC__
#define LONGLONG_SIZE     sizeof(long long)
#else
#define LONGLONG_SIZE     sizeof(long)
#endif
#define LONGLONG_ALIGN    LONGLONG_SIZE

#define POINTER_SIZE      sizeof(void *)
#define POINTER_ALIGN     POINTER_SIZE

#define BYTE_LENGTH   CHAR_BIT
#define WORD_LENGTH   (INT_SIZE * BYTE_LENGTH)

#endif				/* ifndef _CONFIG_H_ */
