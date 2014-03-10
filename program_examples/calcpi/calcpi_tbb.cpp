/*
 * Author: Stephanie Labasan
 * Date: 7 March 2014
 * Version: 1.0.0

 * File: calcpi_tbb.cpp
 * Usage: ./omp <numThreads> <numIntervals>
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <math.h>

#include "tbb/tbb.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace tbb;

class calcPiTBB {
	double *const x_1;
	double *const n_1;
	double *const pi_1;

	public:
	calcPiTBB (double *x_2, double *n_2, double *pi_2) : x_1(x_2), n_1(n_2), pi_1(pi_2) { } 

	void operator() ( const blocked_range<int>& r ) const {
		for ( int i = r.begin(); i != r.end(); i++) {
			*x_1 = (*n_1) * ((double)i - 0.5);
			*pi_1 += 4.0 / (1.0 + (*x_1) * (*x_1));
			printf("*pi_1 = %16lf\n", *pi_1);
		}
	}
};
	
void printError(int argc, char *argv[]) {
	printf("Usage: %s <numThreads> <numIntervals>\n", argv[0]);
	exit(0);
}

int main (int argc, char *argv[]) {
	struct timeval start, stop;
    double theoreticalPi = 3.141592653589793238462643;
	int numThreads, intervals;
	int i; 
	double n, x, pi = 0.0; 

	if (argc != 3) {
		printError(argc, argv);
	}

	intervals = atoi(argv[2]);
	numThreads = atoi(argv[1]);

	gettimeofday(&start, NULL);

	printf("Begin CalculatePi_TBB\n");
	printf("  Num Threads = %d\n", numThreads);
	printf("  Num Intervals = %d\n", intervals);

	n = 1.0 / (double)intervals;
	//printf("n = %.16lf\n", n);

	calcPiTBB c(&x, &n, &pi);
	blocked_range<int> r(0, intervals);

	/* Parallel loop with reduction for calculating pi*/ 
	parallel_for(r, c);

	pi *= n; 
	
	gettimeofday(&stop, NULL);
	
	double timeElapsed = (stop.tv_sec+(stop.tv_usec/(double)1000000)) - (start.tv_sec+(start.tv_usec/(double)1000000));

	printf("\n");
	printf("  Pi         %.12lf\n", pi);
	printf("  Error      %.16f\n", fabs(pi-theoreticalPi));
	printf("  Time       %.6fs\n", timeElapsed);
	printf("\nEnd CalculatePi_TBB\n");
}
