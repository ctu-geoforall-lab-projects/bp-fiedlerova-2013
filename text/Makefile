FILE=fiedlter_bp

all:
	pdflatex $(FILE)
	bibtex $(FILE)

clean:
	rm -f *~ *.aux *.log *.out

distclean:
	make clean
	rm -f $(FILE).pdf
