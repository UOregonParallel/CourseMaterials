
pass_init:
	icpc -o pass_init pass_init.c -lcrypto

pass_openmp:
	icpc -o pass_openmp pass_openmp.c -lcrypto -fopenmp

pass_cilk:
	icpc -o pass_cilk pass_cilk.c -lcrypto -fcilkplus -lcilkrts

pass_tbb:
	icpc -o pass_tbb pass_tbb.c -lcrypto -ltbb