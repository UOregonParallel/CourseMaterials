############################################################################
# 
# @file src/Makefile
# @Author James Kress
# @date 10/2/2013
# @version 1.00
# @copyright KressEnterprisesInternational - All Rights Reserved
# @description This is the src makefile for BSplines
#
# $Id: Makefile 122 2014-01-22 00:07:00Z kirk $
#
############################################################################

prog:=garbleVideo
objs:=$(patsubst %.c,%.o,$(wildcard *.cpp))

# desktop compile
#cc=g++
#ccflags= -MMD -g -Wall
#ldflags= -lopencv_core -lopencv_highgui -lopencv_imgproc

# MIST compile
# module load opencv
#cc=g++
#ccflags= -MMD -g -Wall -I/usr/local/packages/opencv/2.4.2/include
#ldflags= -lopencv_core -lopencv_highgui -lopencv_imgproc -L/usr/local/packages/opencv/2.4.2/lib



%.o: %.c
	$(cc) $(ccflags) -c $<

$(prog): $(objs)
	$(cc) $(ccflags) -o $@ $^ $(ldflags)


.PHONY: clean

clean:
	rm -f garbleVideo *.o *.d counts* *.output *~ $(prog) 
	
-include *.d

# vim: set ts=4: #
