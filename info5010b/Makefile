.PHONY: clean all

COMPONENTS=$(wildcard *.tex)
FIGURES=$(wildcard figures/*.tex)

NAME=presentation
OUTP=$(NAME).pdf
INP=$(NAME).tex

$(OUTP): $(INP)
	pdflatex $(INP)
	bibtex $(patsubst %.tex,%.aux,$(INP))
	pdflatex $(INP)
	pdflatex $(INP)
	#dvips $(patsubst %.tex,%.dvi,$(INP))
	#ps2pdf $(patsubst %.tex,%.ps,$(INP))

clean:
	rm -f *.pdf
	rm -f *.log
	rm -f *.nav
	rm -f *.out
	rm -f *.aux
	rm -f *.snm
	rm -f *.toc
	rm -f *.bbl
	rm -f *.blg
	rm -f *.dvi
	rm -f *.ps
