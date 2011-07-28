.PHONY: clean all generic

META=thesis.tex biblio.bib txSymbols.tex
TEMPFI=thesis.temp.tex

introduction.pdf: tIntroduction.tex $(META)

background.pdf: tBackground.tex $(META)
	rm -f $(TEMPFI)
	echo "\input{tBackground}" > $(TEMPFI)
	cp thesis.tex background.tex
	pdflatex background.tex
	bibtex background.aux
	pdflatex background.tex
	pdflatex background.tex
	rm background.tex

example.pdf: tExample.tex $(META)

systems.pdf: tSystems.tex $(META)

model.pdf: tModel.tex $(META)

hardness.pdf: tHardness.tex $(META)

algos.pdf: tAlgos.tex $(META)

experiment.pdf: tExperiment.tex $(META)

related.pdf: tRelated.tex $(META)
	rm -f $(TEMPFI)
	echo "\input{tRelated}" > $(TEMPFI)
	cp thesis.tex related.tex
	pdflatex related.tex
	bibtex related.aux
	pdflatex related.tex
	pdflatex related.tex
	rm related.tex

conclusion.pdf: tConclusion.tex

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
	rm -f $(TEMPFI)
