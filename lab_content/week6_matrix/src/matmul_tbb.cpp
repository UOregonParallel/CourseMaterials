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

#include <tbb/tbb.h>

#define ORDER 1000   // the order of the matrix
#define AVAL  3.0    // initial value of A
#define BVAL  5.0    // initial value of B
#define TOL   0.001  // tolerance used to check the result

#define N ORDER
#define P ORDER
#define M ORDER

using namespace tbb;

class Matmul {
	double *A; // A[N][P];
	double *B; // B[P][M];
	double *C; // C[N][M];

	void matrix_init(void)
	{
		int i, j;

		// A[N][P]
		A = new double[N*P];
		for (i=0; i<N; i++) {
			for (j=0; j<P; j++) {
				A[N*i+j] = AVAL;
			}
		}

		// B[P][M]
		B = new double[P*M];
		for (i=0; i<P; i++) {
			for (j=0; j<M; j++) {
				B[P*i+j] = BVAL;
			}
		}

		// C[N][M]
		C = new double[N*M];
		for (i=0; i<N; i++) {
			for (j=0; j<M; j++) {
				C[N*i+j] = 0.0;
			}
		}
	}

public:
	void operator() (const blocked_range<int>& r) const 
	{
		int i, j, k;

		for (i=r.begin(); i!=r.end(); i++){
			for(k=0; k<P; k++){
				for (j=0; j<M; j++){
					C[N*i+j] += A[N*i+k] * B[P*k+j];
				}
			}
		}
	}

	int correctness(void)
	{
		int i, j;

		double e  = 0.0;
		double ee = 0.0;
		double v  = AVAL * BVAL * ORDER;

		for (i=0; i<N; i++) {
			for (j=0; j<M; j++) {
				e = C[N*i+j] - v;
				ee += e * e;
			}
		}

		if (ee > TOL) {
			return 0;
		} else {
			return 1;
		}
	}

	Matmul() {
		matrix_init();
	}
};

int
main(int argc, char **argv)
{
	int correct;
	double run_time;
	double mflops;

	tick_count start, end;

	Matmul mm;

	start = tick_count::now();

	parallel_for(blocked_range<int>(0, M), mm, auto_partitioner());

	end = tick_count::now();

	run_time = (end-start).seconds();

	correct  = mm.correctness();

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
