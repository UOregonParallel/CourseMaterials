/**
 * @author James Kress
 * @date February 1, 2014
 * @version 1.0.0
 *
 * Usage: ./prime_tbb <int - num avail proc.> <int - proc. to use>
 *
 * For Further Information: 
 *       Send queries or bug reports to jkress@cs.uoregon.edu
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/partitioner.h"
 
using namespace std;
using namespace tbb;
 

/*****************************************************************************
 *  prime_tbb returns the number of primes between 1 and N. This method
 * 	uses the tbb parallel for loop. 
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
*/
class prime_tbb
{
	public:
		prime_tbb(atomic<int> *count) : count(count) { }
			
 
		void operator() (const blocked_range<size_t> &r) const
		{
		    for (size_t i = r.begin(); i < r.end(); i++)
		    {
		    	++(*count);
		    	
		        for ( int j = 2; j < i; j++ )
				{
					if ( i % j == 0 )
					{
						--(*count);
						break;
					}
				}
		    }
		}

 		atomic<int> *count;
};


/******************************************************************************
 * PRIME_NUMBER_SWEEP does repeated calls to PRIME_TBB.
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
	static tick_count t_start, t_end;
	
	printf ( "\n   Call PRIME_NUMBER to count the primes from 1 to N.\n" );
	printf ( "\n" );
	printf ( "         N        Pi          Time\n" );
	printf ( "\n" );

	n = n_lo;

	while ( n <= n_hi )
	{
		atomic<int> count;
		count = 0;
		
		prime_tbb p2(&count);
		blocked_range<size_t> r(2, n+1);
		
		t_start = tick_count::now();
		parallel_for(r, p2);
		t_end = tick_count::now();
		
		printf ( "  %8d  %8d  %14f\n", n, (int)count, (t_end - t_start).seconds() );
		n = n * n_factor;
	}	 
	return;
}

/******************************************************************************
 *   This program calls a version of PRIME_NUMBER that includes
 *   tbb directives for parallel processing.
 */
int main(int argc, char *argv[] )
{
    int n_factor;
	int n_hi;
	int n_lo;
	
	printf ( "\n\nPRIME_TBB\n" );
	printf ( "  C++/tbb version\n" );
	
	//Check for right num of args, else exit
	if(argc != 3) {
		fprintf(stderr, "Usage: <int> num available processors "
						"<int> num threads to run\n\n");
		exit(13);
	}
	
	printf ( "\n" );
	printf ( "  Number of processors available = %i\n", atoi(argv[1]) );
	printf ( "  Number of threads =              %i\n", atoi(argv[2]) );
	
	task_scheduler_init init(atoi(argv[2]));	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	
	n_lo = 1;
	n_hi = 131072;
	n_factor = 2;
	prime_number_sweep ( n_lo, n_hi, n_factor );
	
	n_lo = 5;
	n_hi = 500000;
	n_factor = 10;
	prime_number_sweep ( n_lo, n_hi, n_factor );

	printf ( "\nPRIME_tbb\n" );
	printf ( "  Normal end of execution.\n\n\n" );

	return 0;
}

/* vim: set ts=4: */
//END ALL

