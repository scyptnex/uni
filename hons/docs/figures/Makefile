# Automated Makefile
TEXFILES := $(wildcard *.tex)

AUXFILES := $(patsubst %.tex,%.aux,$(TEXFILES))
DVIFILES := $(patsubst %.tex,%.dvi,$(TEXFILES))
LOGFILES := $(patsubst %.tex,%.log,$(TEXFILES))
PSFILES := $(patsubst %.tex,%.ps,$(TEXFILES))
PDFFILES := $(patsubst %.tex,%.pdf,$(TEXFILES))



all: $(PDFFILES)

%.pdf: %.tex
	mv $< temp.tex
	./picwrap.sh temp.tex > $<
	latex $<
	latex $<
	latex $<
	dvips $(patsubst %.tex,%.dvi,$<)
	ps2pdf $(patsubst %.pdf,%.ps,$@)
	mv temp.tex $<

clean:
	rm -f *.aux
	rm -f *.log
	rm -f *.dvi
	rm -f *.ps
	rm -f *.pdf
