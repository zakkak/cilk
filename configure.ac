dnl Process this file with autoconf to produce a configure script.
AC_INIT([cilk], [5.4.6])
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h runtime/cilk-sysdep.h])
AC_PREREQ(2.53)
AM_INIT_AUTOMAKE
AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

AC_ARG_WITH(perfctr,[  --with-perfctr         use the virtualized version of the timestamp counter to measure the critical path and work (requires the perfctr kernel patch)], with_perfctr=$withval, with_perfctr=no)

dnl Checks for required programs.
if test "$GCC" != "yes"; then
   AC_ERROR("I cannot find gcc.  gcc is required for Cilk to work.")
fi

dnl Checks for libraries.
AC_CHECK_LIB(m, sqrt)

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([alloca.h fcntl.h limits.h malloc.h stddef.h stdlib.h string.h strings.h sys/time.h unistd.h pthread.h sys/wait.h signal.h sched.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

dnl Checks for library functions.
AC_CHECK_FUNCS(memalign)
AC_CHECK_FUNCS(strchr)
AC_CHECK_FUNCS(strtoul)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(gethrtime)
AC_CHECK_FUNCS(gethrvtime)
AC_CHECK_FUNCS(sched_yield)
AC_CHECK_FUNCS(clone)
AC_CHECK_FUNCS(yield)
AC_CHECK_FUNCS(sginap)
AC_CHECK_FUNCS(sched_setaffinity)
ACX_PTHREAD([THREADLIBS="$PTHREAD_LIBS "])

dnl check for perfctr
PERFCTR_LIBS=""
if test "$with_perfctr" = "yes"; then
  AC_DEFINE(CILK_USE_PERFCTR, 1, "Use perfctr to measure critical path and work.")
  PERFCTR_LIBS=-lperfctr
fi
AC_SUBST(PERFCTR_LIBS)

AC_DEFINE_UNQUOTED(CILKC_PTHREAD_LIBS,"$PTHREAD_LIBS",[The pthread libraries])
AC_DEFINE_UNQUOTED(CILKC_PTHREAD_CFLAGS,"$PTHREAD_CFLAGS",[To be passed to the C compiler when compiling code for pthreads])
AC_DEFINE_UNQUOTED(CILKC_PTHREAD_CC,"$PTHREAD_CC",[The C compiler to use when compiling code for pthreads])

if test "$with_gnu_ld" == "yes"; then
   AC_DEFINE(WITH_GNU_LD,1,[Define if you are using the GNU linker])
fi

AC_CONFIG_FILES([FAQ/Makefile
                 Makefile
                 cilk2c/Makefile
                 doc/Makefile
		 doc/version.tex
                 examples/Makefile
                 runtime/Makefile
                 support/Makefile])

AC_OUTPUT
