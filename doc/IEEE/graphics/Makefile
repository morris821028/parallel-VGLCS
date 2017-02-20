all: fig-VGLCSex.pdf \
	fig-VGLCSex2.pdf \
	fig-interval-decomposition.pdf \
	fig-sparse-table.pdf \
	fig-cartesian-encoding.pdf \
	fig-bst-encoding.pdf

%.pdf: %.tex
	pdflatex $^

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
