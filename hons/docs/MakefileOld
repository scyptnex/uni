.PHONY: clean all generic

META=thesis.tex biblio.bib txSymbols.tex
TEMPFI=thesis.temp.tex

thesis.pdf: tIntroduction.tex tBackground.tex tExample.tex tSystems.tex tModel.tex tHardness.tex tAlgos.tex tExperiment.tex tRelated.tex tConclusion.tex $(META)
	rm -f $(TEMPFI)
	echo "\input{tIntroduction.tex}\n\input{tBackground.tex}\n\input{tExample.tex}\n\input{tSystems.tex}\n\input{tModel.tex}\n\input{tHardness.tex}\n\input{tAlgos.tex}\n\input{tExperiment.tex}\n\input{tRelated.tex}\n\input{tConclusion.tex}" > $(TEMPFI)
	pdflatex thesis.tex
	bibtex thesis.aux
	pdflatex thesis.tex
	pdflatex thesis.tex

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
	rm -f $(TEMPFI)
	echo "\input{tModel}" > $(TEMPFI)
	cp thesis.tex model.tex
	pdflatex model.tex
	pdflatex model.tex
	pdflatex model.tex
	rm model.tex

hardness.pdf: tHardness.tex $(META)
	rm -f $(TEMPFI)
	echo "\input{tHardness}" > $(TEMPFI)
	cp thesis.tex hardness.tex
	pdflatex hardness.tex
	pdflatex hardness.tex
	pdflatex hardness.tex
	rm hardness.tex

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
