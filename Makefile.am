SUBDIRS = cilk2c runtime support examples doc FAQ
EXTRA_DIST = THANKS bootstrap.sh

all-check:
	$(MAKE) clean
	$(MAKE) check
	$(MAKE) clean
	$(MAKE) check CILKCFLAGS='-O2 -cilk-debug'
	$(MAKE) clean
	$(MAKE) check CILKCFLAGS='-O2 -cilk-profile'
	$(MAKE) clean
	$(MAKE) check CILKCFLAGS='-O2 -cilk-profile -cilk-debug'


