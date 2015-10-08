/* Don't include file_identity here.  It is too gruesome
#include <compiler-dep.h>
FILE_IDENTITY(ident_gcc_builtin_h,
	      "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/gcc-builtin.h $ $LastChangedBy: bradley $ $Rev: 1631 $ $Date: 2004-09-21 16:43:48 -0400 (Tue, 21 Sep 2004) $");
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

/*
 * this file contains definitions of variables that are built
 * into gcc but not into cilk2c.
 */

#ifdef __GNUC__

__nooutput__ typedef void *__builtin_va_list;

extern __nooutput__ void __builtin_stdarg_start (void *, void *);
extern __nooutput__ void __builtin_va_end (void *);

extern __nooutput__ const char *__FUNCTION__;
extern __nooutput__ const char *__PRETTY_FUNCTION__;
extern __nooutput__ const char *__func__;

extern __nooutput__ int __builtin_constant_p();
extern __nooutput__ void *__builtin_alloca();
extern __nooutput__ int __builtin_abs(int);
extern __nooutput__ float __builtin_fabsf(float);
extern __nooutput__ long double __builtin_fabsl(long double);
extern __nooutput__ double __builtin_fabs(double);
extern __nooutput__ long __builtin_labs(long);
extern __nooutput__ void *__builtin_saveregs();
extern __nooutput__ void *__builtin_memcpy();
extern __nooutput__ void *__builtin_memset();
extern __nooutput__ int __builtin_classify_type();
extern __nooutput__ void *__builtin_next_arg();
extern __nooutput__ int __builtin_args_info();
extern __nooutput__ void *__builtin_return_address(unsigned int);
extern __nooutput__ long __builtin_expect(long,long);

#else

#ifdef sgi
extern __nooutput__ void __synchronize();
extern __nooutput__ int __lock_test_and_set(); /* The correct signature for __lock_test_and_set depends on the context, but we only use it to return an int. This could cause a problem if a user were to call lock_test_and_set directly. */
extern __nooutput__ void *__builtin_alloca();

#endif

#endif
