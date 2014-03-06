############################################################################
# @file Makefile
# @Author James Kress
# @date 2/1/2014
# @version 1.00
# @description This is an innerlevel makefile. It depends on the C program
#				makeSlides to be built on the top level.
#
############################################################################
SRC         := $(wildcard *.tex)

Lecture_10.pdf: $(SRC)
	pdflatex $(SRC) | .././makeSlides $(SRC) 
	
clean:
	rm -rf *.aux *.log *.toc *.nav *.out *.snm *.pdf *~
	
#END ALL

