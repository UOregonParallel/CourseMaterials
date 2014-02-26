/*
 * Matrix Multiply.
 *
 * This is a simple matrix multiply program which will compute the product
 *
 *                C  = A * B
 *
 * A ,B and C are both square matrix. They are statically allocated and
 * initialized with constant number, so we can focuse on the parallelism.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define ORDER 1000   // the order of the matrix
#define AVAL  3.0    // initial value of A
#define BVAL  5.0    // initial value of B
#define TOL   0.001  // tolerance used to check the result

#define N ORDER
#define P ORDER
#define M ORDER

double A[N][P];
double B[P][M];
double C[N][M];

void
matrix_dump(void)
{
	int i, j;

	// C[N][M]
	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			printf("%.0f\t", C[i][j]);
		}
		printf("\n");
	}
}

void
matrix_init(void)
{
	int i, j;

	// A[N][P]
	#pragma omp parallel for private(i,j)
	for (i=0; i<N; i++) {
		for (j=0; j<P; j++) {
			A[i][j] = AVAL;
		}
	}

	// B[P][M]
	#pragma omp parallel for private(i,j)
	for (i=0; i<P; i++) {
		for (j=0; j<M; j++) {
			B[i][j] = BVAL;
		}
	}

	// C[N][M]
	#pragma omp parallel for private(i,j)
	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			C[i][j] = 0.0;
		}
	}
}

double
matrix_multiply(void)
{
	int i, j, k;
	double start, end;

	start = omp_get_wtime(); 

	#pragma omp parallel for private(i,j,k)
	for (i=0; i<N; i++){
		for(k=0; k<P; k++){
			for (j=0; j<M; j++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	end = omp_get_wtime();

	return end - start;
}

int
check_result(void)
{
	int i, j;

	double e  = 0.0;
	double ee = 0.0;
	double v  = AVAL * BVAL * ORDER;

	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			e = C[i][j] - v;
			ee += e * e;
		}
	}

	if (ee > TOL) {
		return 0;
	} else {
		return 1;
	}
}

int
main(int argc, char **argv)
{
	int correct;
	double run_time;
	double mflops;

	matrix_init();

	run_time = matrix_multiply();

	correct  = check_result();

	mflops = (2.0 * N * P * M) / (1000000.0 * run_time);

	printf("Order %d multiplication in %f seconds \n", ORDER, run_time);
	printf("Order %d multiplication at %f mflops\n", ORDER, mflops);

	if (ORDER <= 20) matrix_dump(); 

	if (correct) {
		printf("\n Hey, it worked");
	} else {
		printf("\n Errors in multiplication");
	}

	printf("\n all done \n");

	return 0;
}
