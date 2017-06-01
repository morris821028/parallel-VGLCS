#
# Makefile for NTU
#

.PHONY:	all

PDF = thesis.pdf

TEXFLAG = --pool-size=5000000 --save-size=80000 

all: trans.log ${PDF}

trans.log: ../partial/*.tex
	cd ../partial && ./ntu.sh 2>&1 1>../NTU/trans.log

%.pdf: %.tex ntu.sty ntuvars.tex
	cd ../graphics && make
	xelatex $(TEXFLAG) $<
	- bibtex $*
	xelatex $(TEXFLAG) $<
	xelatex $(TEXFLAG) $<
	while ( grep -q '^LaTeX Warning: Label(s) may have changed' $*.log) \
	do xelatex $(TEXFLAG) $<; done

thesis.pdf: \
	../partial/ntu/*.tex \
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
