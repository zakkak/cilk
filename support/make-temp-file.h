/* Function declarations for libiberty (cut down to just use make_temp_file)

   Copyright 2001, 2002 Free Software Foundation, Inc.
   
   Note - certain prototypes declared in this header file are for
   functions whoes implementation copyright does not belong to the
   FSF.  Those prototypes are present in this file for reference
   purposes only and their presence in this file should not construed
   as an indication of ownership by the FSF of the implementation of
   those functions in any way or form whatsoever.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
   
   Written by Cygnus Support, 1994.

   The libiberty library provides a number of functions which are
   missing on some operating systems.  We do not declare those here,
   to avoid conflicts with the system header files on operating
   systems that do support those functions.  In this file we only
   declare those functions which are specific to libiberty.  */

extern char *make_temp_file (const char *);
