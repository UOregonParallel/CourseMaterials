targets      = aa aaomp aatbb aacilk

opencv_flags = -lopencv_core -lopencv_highgui
openmp_flags = -fopenmp
tbb_flags    = -ltbb

all: $(targets)

clean:
	rm -f $(targets) *.o *.txt

aa: aa.cpp
	g++ $(opencv_flags) -o $@ $^

aatbb: aatbb.cpp
	g++ $(opencv_flags) $(tbb_flags) -o $@ $^

aaomp: aaomp.cpp
	g++ $(opencv_flags) $(openmp_flags) -o $@ $^

aacilk: aacilk.cpp
	icpc $(opencv_flags) -o $@ $^

test: $(targets)
	$(foreach target,$(targets),./$(target) Lenna.png Lenna_$(target).txt;)	
