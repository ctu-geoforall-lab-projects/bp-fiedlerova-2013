PDFLATEX = pdflatex
FILE = posudek

all: 
	$(PDFLATEX) $(FILE).tex

clean:
	rm -f *.aux *.log *~ *.out

distclean:
	make clean
	rm -f $(FILE).pdf
