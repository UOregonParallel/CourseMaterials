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
#include <sys/time.h>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

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
matrix_dump(double D[ORDER][ORDER])
{
	int i, j;

	// C[N][M]
	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			printf("%.0f\t", D[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void
matrix_init(void)
{
	int i, j;

	// A[N][P]
	A[:][:] = AVAL;

	// B[P][M]
	B[:][:] = BVAL;

	// C[N][M]
	C[:][:] = 0.0;

	if (ORDER < 20) {
		matrix_dump(A);
		matrix_dump(B);
		matrix_dump(C);
	}
}

double
matrix_multiply(void)
{
	timeval t0, t1;
	double start, end;

	gettimeofday(&t0, NULL);

	cilk_for (int i=0; i<N; i++) {
		cilk_for (int j=0; j<M; j++) {
			C[i][j] = __sec_reduce_add(A[i][:] * B[:][j]);
		}
	}

	gettimeofday(&t1, NULL);

	start = t0.tv_sec + t0.tv_usec / 1000000.0;
	end   = t1.tv_sec + t1.tv_usec / 1000000.0;

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


	if (correct) {
		printf("\n Hey, it worked");
	} else {
		printf("\n Errors in multiplication");
	}

	printf("\n all done \n");

	return 0;
}
