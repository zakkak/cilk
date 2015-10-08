dnl This file contains autoconf macros for use with Cilk
dnl
dnl ### Selecting which language to use for testing
dnl AC_LANG_CILK()
AC_DEFUN([AC_LANG_CILK],
[define([AC_LANG], [CILK])dnl
ac_ext=cilk
# CFLAGS is not in ac_cpp because -g, -O, etc. are not valid cpp options.
ac_cpp='$CPP $CPPFLAGS'
ac_compile='${CILK-ilk} -c $CILKFLAGS $CPPFLAGS conftest.$ac_ext 1>&AC_FD_CC'
ac_link='${CILK-cilk} -o conftest${ac_exeext} $CILKFLAGS $CPPFLAGS $LDFLAGS conftest.$ac_ext $LIBS 1>&AC_FD_CC'
cross_compiling=$ac_cv_prog_cilk_cross
])
dnl AC_PROG_CILK_WORKS
AC_DEFUN([AC_PROG_CILK_WORKS],
[AC_MSG_CHECKING([whether the cilk compiler ($CILK $CFLAGS $LDFLAGS) works])
AC_LANG_SAVE
AC_LANG_CILK
AC_TRY_COMPILER([cilk int main(int argc, char *argv[]){sync; return(0);}], ac_cv_prog_cilk_works, ac_cv_prog_cilk_cross)
AC_LANG_RESTORE
AC_MSG_RESULT($ac_cv_prog_cilk_works)
if test $ac_cv_prog_cilk_works = no; then
  AC_MSG_ERROR([installation or configuration problem: cilk compiler cannot create executables.])
fi
AC_MSG_CHECKING([whether the C compiler ($CILK $CFLAGS $LDFLAGS) is a cross-compiler])
AC_MSG_RESULT($ac_cv_prog_cilk_cross)
cross_compiling=$ac_cv_prog_cilk_cross
])
dnl AC_PROG_CILK_G,
AC_DEFUN([AC_PROG_CILK_G],
[AC_CACHE_CHECK(whether ${CILK-cilk} accepts -g, ac_cv_prog_cilk_g,
[echo 'cilk void f(void){}' > conftest.cilk
if test -z "`${CILK-cilk} -g -c conftest.cilk 2>&1`"; then
  ac_cv_prog_cilk_g=yes
else
  ac_cv_prog_cilk_g=no
fi
rm -f conftest*
])])
dnl AC_PROG_CILK
AC_DEFUN([AC_PROG_CILK],
[AC_BEFORE([$0], [AC_PROG_CILKCPP])dnl
AC_CHECK_PROGS(CILK, $CILK cilk, cilk)

AC_PROG_CILK_WORKS
AC_PROG_CILK_G

dnl Check whether -g works, even if CILKFLAGS is set, in case the package
dnl plays around with CILKFLAGS (such as to build both debugging and
dnl normal versions of a library), tasteless as that idea is.
ac_test_CILKFLAGS="${CILKFLAGS+set}"
ac_save_CILKFLAGS="$CILKFLAGS"
CILKFLAGS=
if test "$ac_test_CILKFLAGS" = set; then
  CILKFLAGS="$ac_save_CILKFLAGS"
elif test $ac_cv_prog_cilk_g = yes; then
  CILKFLAGS="-g -O2"
else
  CILKFLAGS="-O2"
fi
])
AC_SUBST(CILKFLAGS)dnl
