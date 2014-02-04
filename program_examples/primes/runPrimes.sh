#!/bin/bash -l
###############################################################################
# Parallel Program Example Script
#
# @author James Kress
# @date January 2014
# @version 1.0.0
#	
# Usage: ./primeTests
#
# Script to compile and run the prime number examples 'prime_openmp.c', 
# prime_tbb.cpp, and prime_cilkplus.c with a varying number of threads. 
# The output of the program will be sent to both the console and a 
# file called 'prime_output.txt'.. 
#
# For Further Information: 
#       Send queries or bug reports to jkress@cs.uoregon.edu
###############################################################################

#------------------------------------------------------------------------------
#  Compile the first two programs with GCC and G++ and the third with icpc
#------------------------------------------------------------------------------
gcc -fopenmp prime_openmp.c -o prime_omp
g++ prime_tbb.cpp -ltbb -lrt -o prime_tbb
icpc prime_cilkplus.cpp -o prime_cilkplus

#------------------------------------------------------------------------------
# MIST - different requirements on MIST, uncomment these and comment the above
#------------------------------------------------------------------------------
#module load intel/14.0.1
#export LD_LIBRARY_PATH=/usr/local/packages/intel_xe/14.0.1/tbb/lib/intel64/gcc4.1
#g++ prime_tbb.cpp -L/usr/local/packages/intel_xe/14.0.1/tbb/lib/intel64/gcc4.1 -ltbb -o prime_tbb
#gcc -fopenmp prime_openmp.c -o prime_omp
#icpc prime_cilkplus.cpp -o prime_cilkplus


#------------------------------------------------------------------------------
# Rename the executable and remove any old output files
#------------------------------------------------------------------------------
rm -rf prime_local_output.txt


#------------------------------------------------------------------------------
# Start by running the program at one thread per
# processor, and then decrement by 2
#
# i.e. 32, 30, 28, ...
#------------------------------------------------------------------------------
counter=$(grep -c "^processor" /proc/cpuinfo)
until [ $counter -eq 0 ]; do
	echo "Run with $counter threads."
	export OMP_NUM_THREADS=$counter
	./prime_omp 2>&1 | tee -a prime_output.txt
	
	./prime_tbb $(grep -c "^processor" /proc/cpuinfo) $counter  \
				2>&1 | tee -a prime_output.txt
	
	./prime_cilkplus $(grep -c "^processor" /proc/cpuinfo) $counter  \
				2>&1 | tee -a prime_output.txt
	
	if [ $counter -le 2 ]; then
		let counter=counter-1
	else
		let counter=counter-2
	fi
done 


#------------------------------------------------------------------------------
#  Discard the executable file and exit
#-----------------------------------------------------------------------------
rm prime_omp prime_tbb prime_cilkplus
echo "Program output written to prime_local_output.txt"

#vim: set ts=4:
#END ALL

