TEX_FILE = $(wildcard *.tex)
SPELL_CHECK_TARGETS=$(TEX_FILE)
LATEX_ENGINE=pdflatex

SAMPLES = \
	slide.tex \

PDF = ${SAMPLES:%.tex=%.pdf}

.PHONY: all clean spellcheck

all:  ${PDF}

%.pdf: %.tex
	$(LATEX_ENGINE) $<
	$(LATEX_ENGINE) $<

spellcheck: $(TEX_FILE)
	@for f in $(TEX_FILE); do \
		aspell --mode=tex --personal=./.aspell.en.pws check $$f; \
	done

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
