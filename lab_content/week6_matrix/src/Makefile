CC      := gcc

matmul      : CFLAGS  := -fopenmp
matmul_omp  : CFLAGS  := -fopenmp
matmul_tbb  : LDFLAGS := -L/usr/local/packages/intel_xe/14.0.1/tbb/lib/intel64/gcc4.1/ -ltbb
matmul_tbb  : CXX := icpc
matmul_cilk : CXX := icpc

targets := matmul matmul_omp matmul_tbb matmul_cilk

.PHONY: all clean

all: ${targets}

clean:
	rm -f ${targets} *.o
