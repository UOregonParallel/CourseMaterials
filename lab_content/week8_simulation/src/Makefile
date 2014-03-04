
sim_init:
	icpc -o sim_init main_init.cpp network.cpp Vehicle.cpp Location.cpp ControlledVehicle.cpp

sim_openmp:
	icpc -o sim_openmp main_openmp.cpp network.cpp Vehicle.cpp Location.cpp ControlledVehicle.cpp

sim_cilk:
	icpc -o sim_cilk main_cilk.cpp network.cpp Vehicle.cpp Location.cpp ControlledVehicle.cpp -fcilkplus 

sim_tbb:
	icpc -std=c++0x -o sim_tbb main_tbb.cpp network.cpp Vehicle.cpp Location.cpp ControlledVehicle.cpp -L/usr/local/packages/intel/14.0/tbb/lib/intel64/gcc4.4 -ltbb -I/usr/local/packages/intel/14.0/tbb/include
