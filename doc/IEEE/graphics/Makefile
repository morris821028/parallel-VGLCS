all: fig-VGLCSex.pdf \
	fig-VGLCSex2.pdf \
	fig-VGLCS-dp-naive.pdf \
	fig-VGLCS-dp.pdf \
	fig-VGLCS-dp-rmq.pdf \
	fig-interval-decomposition-origin.pdf \
	fig-interval-decomposition.pdf \
	fig-sparse-table-origin.pdf \
	fig-sparse-table.pdf \
	fig-cartesian-encoding.pdf \
	fig-cartesian-encoding-static.pdf \
	fig-bst-encoding.pdf \
	fig-parallel-n.pdf \
	fig-parallel-p.pdf \
	fig-ISMQ.pdf \

fig-bst-encoding.pdf: fig-bst-n1.tex \
	fig-bst-n2.tex \
	fig-bst-n3.tex

%.pdf: %.tex
	pdflatex $^

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
