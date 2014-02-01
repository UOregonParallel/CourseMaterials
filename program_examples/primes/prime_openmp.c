/**
 * @author James Kress
 * @date February 1, 2014
 * @version 1.0.0
 *
 * Usage: ./prime_cilkplus 
 *
 * Note: Number of processors to use must be set in the environemnt 
 * 		i.e.: export OMP_NUM_THREADS=$counter
 *
 * For Further Information: 
 *       Send queries or bug reports to jkress@cs.uoregon.edu
 */
# include <stdlib.h>
# include <stdio.h>
# include <omp.h>


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
  int total = 0;

# pragma omp parallel \
  shared ( n ) \
  private ( i, j, prime )
  

# pragma omp for reduction ( + : total )
  for ( i = 2; i <= n; i++ )
  {
    prime = 1;

    for ( j = 2; j < i; j++ )
    {
      if ( i % j == 0 )
      {
        prime = 0;
        break;
      }
    }
    total = total + prime;
  }

  return total;
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

  printf ( "\n   Call PRIME_NUMBER to count the primes from 1 to N.\n" );
  printf ( "\n" );
  printf ( "         N        Pi          Time\n" );
  printf ( "\n" );

  n = n_lo;

  while ( n <= n_hi )
  {
    wtime = omp_get_wtime ( );

    primes = prime_number ( n );

    wtime = omp_get_wtime ( ) - wtime;

    printf ( "  %8d  %8d  %14f\n", n, primes, wtime );

    n = n * n_factor;
  }
 
  return;
}


/******************************************************************************
 *   This program calls a version of PRIME_NUMBER that includes
 *   OpenMP directives for parallel processing.
 */
int main ( int argc, char *argv[] )
{
  int n_factor;
  int n_hi;
  int n_lo;

  printf ( "\n\nPRIME_OPENMP\n" );
  printf ( "  C/OpenMP version\n" );

  printf ( "\n" );
  printf ( "  Number of processors available = %d\n", omp_get_num_procs ( ) );
  printf ( "  Number of threads =              %d\n", omp_get_max_threads ( ) );

  n_lo = 1;
  n_hi = 131072;
  n_factor = 2;
  prime_number_sweep ( n_lo, n_hi, n_factor );

  n_lo = 5;
  n_hi = 500000;
  n_factor = 10;
  prime_number_sweep ( n_lo, n_hi, n_factor );

  printf ( "\nPRIME_OPENMP\n" );
  printf ( "  Normal end of execution.\n\n\n" );

  return 0;
}

/* vim: set ts=4: */
//END ALL

