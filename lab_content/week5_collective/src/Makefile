projCC=gcc
default: serial
CFLAGS=-c -g -Wall
serial: serial.o key.o
	gcc serial.o -o serial

openmp: openmp.o key.o
	gcc openmp.o -o openmp -fopenmp 

openmp.o: openmp.c 
	gcc $(CFLAGS) openmp.c -fopenmp  

serial.o: serial.c 
	gcc $(CFLAGS) serial.c 

key.o: key.c
	gcc $(CFLAGS) key.c

clean:
	rm -rf *.o serial
