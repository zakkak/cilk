lib_LTLIBRARIES = libcilk.la libcilk.g.la libcilk.p.la libcilk.g.p.la
pkglib_LIBRARIES = libcilkrt0.a libcilkrt0g.a libcilkrt0p.a libcilkrt0gp.a

CILK_HFILES = cilk-cilk2c.h cilk.h cilk-lib.h cilk-lib.cilkh	\
gcc-builtin.h cilk-cilk2c-pre.h

CILK_CFILES = sysdep.c cilk.c sched.c hooks.c stats.c barrier.c		\
cmdline.c internal-malloc.c timing.c invoke-main.c malloc.c debug.c	\
workers.c mutex.c

pkginclude_HEADERS = $(CILK_HFILES) cilk-conf.h cilk-sysdep.h

CILK_SRC = cilk-internal.h $(CILK_CFILES) $(CILK_HFILES)
COMMON_LINK_FLAGS = -version-info 1:3:0 @PTHREAD_LIBS@

AM_CFLAGS = $(PTHREAD_CFLAGS)
CC = $(PTHREAD_CC)

CILK_DEBUG = -DCILK_DEBUG=1
CILK_PROFILE = -DCILK_TIMING=1 -DCILK_STATS=1 -DCILK_CRITICAL_PATH=1

# startup library
libcilkrt0_a_SOURCES = cilkrt0.c

libcilkrt0g_a_SOURCES = cilkrt0.c
libcilkrt0g_a_CPPFLAGS = $(CILK_DEBUG)

libcilkrt0p_a_SOURCES = cilkrt0.c
libcilkrt0p_a_CPPFLAGS = $(CILK_PROFILE)

libcilkrt0gp_a_SOURCES = cilkrt0.c
libcilkrt0gp_a_CPPFLAGS = $(CILK_PROFILE) $(CILK_DEBUG)

# runtime system library
libcilk_la_SOURCES = $(CILK_SRC)

libcilk_g_la_SOURCES = $(CILK_SRC)
libcilk_g_la_CPPFLAGS = $(CILK_DEBUG)

libcilk_p_la_SOURCES = $(CILK_SRC)
libcilk_p_la_CPPFLAGS = $(CILK_PROFILE)

libcilk_g_p_la_SOURCES = $(CILK_SRC)
libcilk_g_p_la_CPPFLAGS = $(CILK_PROFILE) $(CILK_DEBUG)

EXTRA_DIST = PROTOCOLS
