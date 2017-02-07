all: fig-VGLCSex.pdf fig-VGLCSex2.pdf fig-interval-decomposition.pdf fig-sparse-table.pdf fig-cartesian-encoding.pdf fig-bst-encoding.pdf

fig-VGLCSex.pdf: fig-VGLCSex.tex
	xelatex fig-VGLCSex.tex
fig-VGLCSex2.pdf: fig-VGLCSex2.tex
	xelatex fig-VGLCSex2.tex

fig-interval-decomposition.pdf: fig-interval-decomposition.tex
	xelatex fig-interval-decomposition.tex

fig-sparse-table.pdf: fig-sparse-table.tex
	xelatex fig-sparse-table.tex

fig-cartesian-encoding.pdf: fig-cartesian-encoding.tex
	xelatex fig-cartesian-encoding.tex

fig-bst-encoding.pdf: fig-bst-encoding.tex
	xelatex fig-bst-encoding.tex

clean:
	rm -f *.aux *.log *.nav *.out *.pdf *.snm *.toc *.vrb
