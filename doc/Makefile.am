MANUAL_SRC = defs.tex impl-notes.tex manual.tex cactus.eps intro.tex	\
misc.tex fib-compile.fig lanref.tex model.eps people.tex cilk.bib	\
fib.tex library-spec.tex model.fig prog-guide.tex copyright.tex		\
fib2.tex supertech.sty version.tex cactus.pdf model.pdf

EXTRA_DIST = manual.pdf $(MANUAL_SRC) version.tex.in

manual.pdf: $(MANUAL_SRC)
	pdflatex manual
	bibtex manual
	pdflatex manual
	pdflatex manual

clean-local:
	rm -f *.dvi *~ *.log *.aux *.bbl *.blg *.ps *.toc *.lof nohup.out
	rm -f manual.pdf

