.PHONY: clean all

COMPONENTS=$(wildcard *.tex)
FIGURES=$(wildcard figures/*.tex)

THESISMAIN=thesis.tex
BIBLIOMAIN=biblio.bib
SYMBOLMAIN=txSymbols.tex

thesis.pdf: $(COMPONENTS) $(FIGURES)
	latex $(THESISMAIN)
	bibtex $(patsubst %.tex,%.aux,$(THESISMAIN))
	latex $(THESISMAIN)
	latex $(THESISMAIN)
	dvips $(patsubst %.tex,%.dvi,$(THESISMAIN))
	ps2pdf $(patsubst %.tex,%.ps,$(THESISMAIN))

check:
	echo $(COMPONENTS)
	echo $(FIGURES)
	
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
