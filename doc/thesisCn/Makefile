TEX_FILE = $(wildcard *.tex)
SPELL_CHECK_TARGETS=$(TEX_FILE)
LATEX_ENGINE=xelatex

.PHONY: thesis clean

thesis: thesis.tex
	$(LATEX_ENGINE) thesis.tex
	$(LATEX_ENGINE) thesis.tex

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
