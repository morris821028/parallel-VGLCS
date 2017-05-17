#
# Makefile for NTU
#

.PHONY:	all

PDF = thesis.pdf

TEXFLAG = --pool-size=5000000 --save-size=80000 

all: ${PDF}

%.pdf: %.tex
	cd ../graphics && make
	cd ../partial && ./ntu.sh
	xelatex $(TEXFLAG) $<
	- bibtex $*
	xelatex $(TEXFLAG) $<
	- makeindex -s gind.ist -o $*.ind $*.idx
	- makeindex -s gglo.ist -o $*.gls $*.glo
	xelatex $(TEXFLAG) $<
	while ( grep -q '^LaTeX Warning: Label(s) may have changed' $*.log) \
	do xelatex $(TEXFLAG) $<; done

thesis.pdf: \
	../partial/*.tex \
	../algorithms/*.tex \
	../codes/* \
	../formulas/*.tex \
	../graphics/*.tex \
	../tables/*.tex

clean:
	$(RM) *.log *.aux \
	*.cfg *.glo *.idx *.toc \
	*.ilg *.ind *.out *.lof \
	*.lot *.bbl *.blg *.gls *.cut *.hd \
	*.dvi *.ps *.thm *.tgz *.zip *.rpi
