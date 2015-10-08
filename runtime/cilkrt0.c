/*
 * startup file to invoke Cilk.
 */

/*
 * This file is linked with Cilk programs, and it is not part
 * of libcilk.a.  In this way, libcilk does not depend on the
 * user program and it can be compiled as a shared library.
 */

/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 *  USA.
 *
 */

#include <cilk-sysdep.h>

FILE_IDENTITY(ident,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/cilkrt0.c $ $LastChangedBy: bradley $ $Rev: 1875 $ $Date: 2005-02-15 09:57:07 -0500 (Tue, 15 Feb 2005) $");

#include <stdlib.h>

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

/* Sivan and Ofra 19 June 2003:
   the code in the #else block tries to generate a main that calls the user's 
   cilk_main when there is no main() but there is a cilk_main(), and should not
   be used by the linker when the user supplies a main() earlier in the link command.

   For some reason, this does not work. The linker uses this main() even when the 
   user supplies another.
*/

#define OFRA_MAIN_ATTEMPT

#ifndef OFRA_MAIN_ATTEMPT
int main(int argc, char *argv[])
{
     extern int Cilk_start(int (*main)(void *const ws, int argc, char *argv[]),
			   int argc, char *argv[]);
     extern int cilk_main(void *const ws, int argc, char *argv[]);

     return Cilk_start(cilk_main, argc, argv);
}
#else
#include <cilk.h>
#undef main
extern  int EXPORT(cilk_main)(CilkContext *const context, int argc, char *argv[]);

/* This is the main that the OS calls, in cilkrt0 */
int main(int argc, char *argv[])
{

    CilkContext * context;

    context = Cilk_init(&argc, argv);

    /* null context also when using -help flag */
    if(context == NULL)
	return 0;
    
    EXPORT(cilk_main)(context, argc, argv);

    Cilk_terminate(context);

    return 0;
}
#endif
