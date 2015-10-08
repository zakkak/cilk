EXTRA_DIST = bfnnconv.pl* html.refs m-html.pl m-lout.pl cilk-faq.bfnn	\
m-ascii.pl m-info.pl m-post.pl cilk-faq.ascii
PERL=perl -I$(srcdir)

all cilk-faq.ascii: cilk-faq.bfnn
	@echo converting 1 ...
	$(PERL) $(srcdir)/bfnnconv.pl $(srcdir)/cilk-faq.bfnn
	@echo converting 2 ...
	$(PERL) $(srcdir)/bfnnconv.pl $(srcdir)/cilk-faq.bfnn

clean-local:
	rm -f *~ core a.out *.lout *.ps *.info *.ascii *.xrefdb *.post
	rm -rf *.html
