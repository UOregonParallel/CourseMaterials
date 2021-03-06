/**
 * @author James Kress
 * @date February 1, 2014
 * @version 1.0.0
 *
 * Usage: ./prime_cilkplus <int - num avail proc.> <int - proc. to use>
 *
 * For Further Information: 
 *       Send queries or bug reports to jkress@cs.uoregon.edu
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>


/*****************************************************************************
 *  PRIME_NUMBER returns the number of primes between 1 and N. This method
 * 	uses the openmp parallel for loop. 
 *
 *    A naive algorithm is used.
 *
 *                N  PRIME_NUMBER
 *
 *               1           0
 *              10           4
 *             100          25
 *           1,000         168
 *          10,000       1,229
 *         100,000       9,592
 *       1,000,000      78,498
 *      10,000,000     664,579
 *     100,000,000   5,761,455
 *   1,000,000,000  50,847,534
 *
 * @param n		- the maximum number to check.
 * @return int 	- number of prime numbers up tp n
*/
int prime_number ( int n )
{
	int i;
	int j;
	int prime;
	cilk::reducer_opadd<int> total;

	cilk_for(i = 2; i <= n; i++) 
	{
		total+=1;

		for ( j = 2; j < i; j++ )
		{
		  if ( i % j == 0 )
		  {
		    total-=1;
		    break;
		  }
		}
	}
	return total.get_value();
}


/******************************************************************************
 * PRIME_NUMBER_SWEEP does repeated calls to PRIME_NUMBER.
 *
 * @param n_lo		- the first value of n
 * @param n_hi		- the last value of n
 * @param n_factor	- factor by which to increase n after each iteration
 * @return void
 */
void prime_number_sweep ( int n_lo, int n_hi, int n_factor )
{
  int i;
  int n;
  int primes;
  double wtime;
  struct timeval start,end;

  printf ( "\n   Call PRIME_NUMBER to count the primes from 1 to N.\n" );
  printf ( "\n" );
  printf ( "         N        Pi          Time\n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    gettimeofday(&start,NULL); //Start timing the computation

    primes = prime_number ( n );

    gettimeofday(&end,NULL); //Stop timing the computation
    wtime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

    printf ( "  %8d  %8d  %14f\n", n, primes, wtime );

    n = n * n_factor;
  }
  return;
}


/******************************************************************************
 *   This program calls a version of PRIME_NUMBER that includes
 *   cilk plus directives for parallel processing.
 */
int main ( int argc, char *argv[] )
{
  int n_factor;
  int n_hi;
  int n_lo;

  printf ( "\n\nPRIME_cilklus\n" );
  printf ( "  C/Cilk Plus version\n" );

	if(argc != 3) {
		fprintf(stderr, "Usage: ./executablename <int> num available processors "
						"<int> num threads to run\n\n");
		exit(13);
	}
	
	printf ( "\n" );
	printf ( "  Number of processors available = %i\n", atoi(argv[1]) );
	printf ( "  Number of threads =              %i\n", atoi(argv[2]) );;

	//Set the number of cilk threads to use for this program
	if (0!= __cilkrts_set_param("nworkers", argv[2]))
	{
		printf("Failed to set worker count\n");
		return 1;
	}

  n_lo = 1;
  n_hi = 131072;
  n_factor = 2;
  prime_number_sweep ( n_lo, n_hi, n_factor );

  n_lo = 5;
  n_hi = 500000;
  n_factor = 10;
  prime_number_sweep ( n_lo, n_hi, n_factor );

  printf ( "\nPRIME_cilkplus\n" );
  printf ( "  Normal end of execution.\n\n\n" );

  return 0;
}

/* vim: set ts=4: */
//END ALL

