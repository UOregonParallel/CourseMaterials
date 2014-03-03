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

SUBDIRS  := $(wildcard week*)

.PHONY: all clean subclean veryClean subdirs $(SUBDIRS) 

all: $(prog) subdirs

subdirs: $(SUBDIRS)

$(SUBDIRS):
	make -C $@

clean: subclean
	rm -rf $(objs) *.d  *~ $(prog) 

subclean:
	for dir in $(SUBDIRS); do    \
		make -C $$dir clean; \
	done

$(prog): $(objs)
	$(cc) $(ccflags) -o $@ $^ $(ldflags)

%.o: %.c
	$(cc) $(ccflags) -c $<

#END ALL

