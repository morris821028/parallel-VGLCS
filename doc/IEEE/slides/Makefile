TEX_FILE = $(wildcard *.tex)
SPELL_CHECK_TARGETS=$(TEX_FILE)
LATEX_ENGINE=pdflatex

.PHONY: thesis clean spellcheck

thesis: thesis.tex
	$(LATEX_ENGINE) thesis.tex
	$(LATEX_ENGINE) thesis.tex

spellcheck: $(TEX_FILE)
	@for f in $(TEX_FILE); do \
		aspell --mode=tex --personal=./.aspell.en.pws check $$f; \
	done

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
