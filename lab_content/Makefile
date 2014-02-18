############################################################################
# @file Makefile
# @Author James Kress
# @date 2/1/2014
# @version 1.00
# @description This is the top level makefile for the class slides.
#				This file will make the C program necessary to produce
#				the slides with latex. The C program is used to reduce
#				screen clutter and only report errors from pdflatex.
#
############################################################################
prog:=makeSlides
objs:=$(patsubst %.c,%.o,$(wildcard *.c))

cc=gcc
ccflags= 
ldflags= 


all: $(prog) MAPLAB 

%.o: %.c
	$(cc) $(ccflags) -c $<

$(prog): $(objs)
	$(cc) $(ccflags) -o $@ $^ $(ldflags)


.PHONY: clean veryClean MAPLAB

MAPLAB:
	make -C maplab

clean:
	make -C maplab clean
	rm -rf $(objs) *.d  *~ $(prog) 


#END ALL

