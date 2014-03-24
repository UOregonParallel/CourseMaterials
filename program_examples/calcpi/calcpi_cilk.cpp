/*
 * Author: Stephanie Labasan
 * Date: 7 March 2014
 * Version: 1.0.0

 * File: calcpi_cilk.cpp
 * Usage: ./cilk <numThreads> <numIntervals>
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <math.h>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>

void printError(int argc, char *argv[]) {
	printf("Usage: %s <numThreads> <numIntervals>\n", argv[0]);
	exit(0);
}

int main (int argc, char *argv[]) {
	struct timeval start, stop;
    double theoreticalPi = 3.141592653589793238462643;
	int numThreads, intervals;
	int i; 
	double n, x, sumPi = 0.0; 
	cilk::reducer_opadd<double> pi;

	if (argc != 3) {
		printError(argc, argv);
	}

	intervals=atoi(argv[2]);
	numThreads = atoi(argv[1]);

	if (0 != __cilkrts_set_param("nworkers", argv[1])) {
		printf("Failed to set worker count\n");
		return 1;
	}	
	
	gettimeofday(&start, NULL);

	printf("Begin CalculatePi_CilkPlus\n");
	printf("  Num Threads = %d\n", numThreads);
	printf("  Num Intervals = %d\n", intervals);

	n = 1.0 / (double)intervals;

	/* Parallel loop with reduction for calculating pi*/ 
	cilk_for (i = 0; i < intervals; i++) {
		x = n * ((double)i - 0.5);
		pi += 4.0 / (1.0 + x * x);
	}
	sumPi = pi.get_value() * n;

	gettimeofday(&stop, NULL);
	
	double timeElapsed = (stop.tv_sec+(stop.tv_usec/(double)1000000)) - (start.tv_sec+(start.tv_usec/(double)1000000));

	printf("\n");
	printf("  Pi         %.12lf\n", sumPi);
	printf("  Error      %.16f\n", fabs(sumPi-theoreticalPi));
	printf("  Time       %.6fs\n", timeElapsed);
	printf("\nEnd CalculatePi_CilkPlus\n");
}
