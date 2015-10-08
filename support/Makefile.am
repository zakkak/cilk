

pkgdatadir = $(pkglibdir)
noinst_PROGRAMS = cilkclocal
bin_PROGRAMS = cilkc
PTHREAD_LIBS=@PTHREAD_LIBS@
RPATH=@HAVE_RPATH@

cilkclocal_SOURCES = cilkc.c make-temp-file.c make-temp-file.h
cilkclocal_CPPFLAGS=\
-DCILKHEADER_DIR="\"`(cd $(top_srcdir); pwd)`/runtime\"" \
-DCILK_RTSBUILD_DIR="\"`(cd $(top_builddir); pwd)`/runtime\"" \
-DCILK2C_DIR="\"`(cd $(top_builddir); pwd)`/cilk2c\""	     \
-DLIBS_DIR="\"`(cd $(top_builddir); pwd)`/runtime/.libs\""   \
-DLIBS2_DIR="\"`(cd $(top_builddir); pwd)`/runtime\""

cilkc_SOURCES = cilkc.c make-temp-file.c make-temp-file.h
cilkc_CPPFLAGS=-DCILKHEADER_DIR="\"$(prefix)/include/cilk\""	\
-DCILK2C_DIR="\"$(pkglibdir)\"" -DLIBS_DIR="\"$(libdir)\""	\
-DLIBS2_DIR="\"$(libdir)/cilk\""

EXTRA_DIST = cilk.m4 automake-cilk dotemacs.el

DISTCLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in
