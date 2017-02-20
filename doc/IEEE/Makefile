TARGETS=ICPP
BUILD_ENGINE=pdflatex
SPELL_CHECK_TARGETS = ICPP.tex section/*

all: $(TARGETS)

$(TARGETS):
	cd graphics && make
	$(BUILD_ENGINE) $@
	bibtex $@
	$(BUILD_ENGINE) $@
	$(BUILD_ENGINE) $@

spell:
	for f in $(SPELL_CHECK_TARGETS) ; do \
		LC_ALL=en_US.utf8 aspell -c $$f ; \
	done

clean:
	rm -rf  $(BUILD_DIR)
	rm -rf *.log *.aux *.dvi *~ *.bbl  *.bl *.out
	rm -rf *.lo* *.toc *.blg 
	rm -rf *.pdf
	cd graphics && make clean
