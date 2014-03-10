#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <math.h>

void printError(int argc, char *argv[]) {
	printf("Usage: %s <numIntervals>\n", argv[0]);
	exit(0);
}

int main (int argc, char *argv[]) {
	struct timeval start, stop;
    double theoreticalPi = 3.141592653589793238462643;
	int intervals;
	int i; 
	double n, x, pi = 0.0; 

	if (argc != 2) {
		printError(argc, argv);
	}

	gettimeofday(&start, NULL);

	intervals=atoi(argv[1]);

	printf("Begin CalculatePi_Serial\n");
	printf("  Num Intervals = %d\n", intervals);

	n = 1.0 / (double)intervals;

	/* Parallel loop with reduction for calculating pi*/ 
	for (i = 0; i < intervals; i++) {
		x = n * ((double)i - 0.5);
		pi += 4.0 / (1.0 + x * x);
	}
	pi *= n; 
	
	gettimeofday(&stop, NULL);
	
	double timeElapsed = (stop.tv_sec+(stop.tv_usec/(double)1000000)) - (start.tv_sec+(start.tv_usec/(double)1000000));

	printf("\n");
	printf("  Pi         %.12lf\n", pi);
	printf("  Error      %.16f\n", fabs(pi-theoreticalPi));
	printf("  Time       %.6fs\n", timeElapsed);
	printf("\nEnd CalculatePi_Serial\n");
}
