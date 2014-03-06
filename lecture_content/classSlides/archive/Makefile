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


all: $(prog) WEEK1 WEEK2 WEEK3 WEEK4 WEEK5 WEEK6 WEEK7 WEEK8 WEEK9 WEEK10 

%.o: %.c
	$(cc) $(ccflags) -c $<

$(prog): $(objs)
	$(cc) $(ccflags) -o $@ $^ $(ldflags)


.PHONY: clean veryClean WEEK1 WEEK2 WEEK3 WEEK4 WEEK5 WEEK6 WEEK7 WEEK8 WEEK9 WEEK10 

WEEK1:
	make -C lecture1
	make -C lecture2

WEEK2:
	make -C lecture3
	make -C lecture4

WEEK3:
	make -C lecture5
	make -C lecture6

WEEK4:
	make -C lecture7
	make -C lecture8

WEEK5:
	make -C lecture9
	make -C lecture10

WEEK6:
	make -C lecture11
	make -C lecture12

WEEK7:
	make -C lecture13
	make -C lecture14

WEEK8:
	make -C lecture15
	make -C lecture16

WEEK9:
	make -C lecture17
	make -C lecture18

WEEK10:
	make -C lecture19
	make -C lecture20
	
clean:
	make -C lecture1 clean
	make -C lecture2 clean
	make -C lecture3 clean
	make -C lecture4 clean
	make -C lecture5 clean
	make -C lecture6 clean
	make -C lecture7 clean
	make -C lecture8 clean
	make -C lecture9 clean
	make -C lecture10 clean
	make -C lecture11 clean
	make -C lecture12 clean
	make -C lecture13 clean
	make -C lecture14 clean
	make -C lecture15 clean
	make -C lecture16 clean
	make -C lecture17 clean
	make -C lecture18 clean
	make -C lecture19 clean
	make -C lecture20 clean
	rm -rf $(objs) *.d  *~ $(prog) 


#END ALL

