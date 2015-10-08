# Simulate CILK before installation
CILK=$(top_builddir)/support/cilkclocal

SUFFIXES = .cilk
NPROC = 4
PROGFLAGS = --nproc $(NPROC) --yield

CILKFLAGS=@CFLAGS@
CILKCOMPILE = $(CILK) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CILKFLAGS) $(CILKFLAGS)
CILKLD = $(CILK)
CILKLINK = $(CILKLD) $(AM_CILKFLAGS) $(CILKFLAGS) $(PERFCTR_LIBS) $(LDFLAGS) -o $@
LINK=$(CILKLINK)

noinst_PROGRAMS=hello fib fib-serial testall ck magic knapsack queens	\
cilksort fft test-locks bucket cholesky game nfib kalah matmul lu plu	\
heat test-errno strassen rectmul spacemul \
test-void-func \
velin \
cholesky-lib \
nfib-lib
MOSTLYCLEANFILES=pragma.E pragma.E.cilk pragma.E.c deprecated.log deprecated2.out deprecated2.log deprecated2.i

GETOPTIONS_SRC = getoptions.c getoptions.h

fib_serial_SOURCES = fib-serial.c
fib-serial$(EXEEXT): $(fib_serial_OBJECTS) $(fib_serial_DEPENDENCIES)
	@rm -f fib-serial
	$(CC) -o $@ $(fib_serial_OBJECTS) $(LIBS)

hello_SOURCES=hello.cilk
fib_SOURCES=fib.cilk
velin_SOURCES=velin.cilk
testall_SOURCES=testall.cilk rand.cilk
test_locks_SOURCES=test-locks.cilk
test_errno_SOURCES=test-errno.cilk
ck_SOURCES=ck.cilk rand.cilk $(GETOPTIONS_SRC)
magic_SOURCES=magic.cilk $(GETOPTIONS_SRC)
knapsack_SOURCES=knapsack.cilk $(GETOPTIONS_SRC)
queens_SOURCES=queens.cilk
cilksort_SOURCES=cilksort.cilk $(GETOPTIONS_SRC)
fft_SOURCES=fft.cilk $(GETOPTIONS_SRC)
bucket_SOURCES=bucket.cilk $(GETOPTIONS_SRC)
cholesky_SOURCES=cholesky.cilk rand.cilk $(GETOPTIONS_SRC)
cholesky_lib_SOURCES=cholesky-lib.cilk rand.cilk $(GETOPTIONS_SRC)
matmul_SOURCES=matmul.cilk rand.cilk $(GETOPTIONS_SRC)
game_SOURCES=game.cilk $(GETOPTIONS_SRC)
nfib_SOURCES=nfib.cilk
nfib_lib_SOURCES=nfib-lib.cilk
kalah_SOURCES=kalah.cilk
lu_SOURCES=lu.cilk  $(GETOPTIONS_SRC)
plu_SOURCES=plu.cilk  $(GETOPTIONS_SRC)
heat_SOURCES=heat.cilk  $(GETOPTIONS_SRC)
strassen_SOURCES=strassen.cilk  $(GETOPTIONS_SRC)
rectmul_SOURCES=rectmul.cilk  $(GETOPTIONS_SRC)
spacemul_SOURCES=spacemul.cilk  $(GETOPTIONS_SRC)
test_void_func_SOURCES=test-void-func.cilk

EXTRA_DIST = knapsack-example1.input knapsack-example2.input	\
knapsack-example3.input

check:  hello testall fft fib lu matmul cilksort heat test-locks test-errno
	@echo "Runnig Cilk tests.  This will take a while."
	./hello $(PROGFLAGS)
	./test-void-func $(PROGFLAGS)
	./test-locks $(PROGFLAGS)
	./test-errno $(PROGFLAGS)
	./testall $(PROGFLAGS)
	./fft $(PROGFLAGS) -c
	./fib $(PROGFLAGS) 35
	./lu $(PROGFLAGS) -n 1024
	./matmul $(PROGFLAGS) 1066
	./cilksort $(PROGFLAGS) -n 3000000
	./heat $(PROGFLAGS)
	./strassen $(PROGFLAGS)
	./velin $(PROGFLAGS)

.cilk.o:
	$(CILKCOMPILE) -c -o $@ $<
