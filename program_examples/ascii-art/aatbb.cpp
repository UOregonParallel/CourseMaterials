#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>

#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>

using namespace std;
using namespace cv;
using namespace tbb;

#ifndef C_STEP
#define C_STEP 4
#endif

#ifndef R_STEP
#define R_STEP 6
#endif

class MinMax {
	Mat myImage;

public:
	unsigned char min, max;

	void operator()(const blocked_range<int>& r) {
		Mat image = myImage;

		for (int i=r.begin(); i!=r.end(); ++i) {
			for (int j=0; j<image.cols; j++) {
				unsigned char pixel = image.at<unsigned char>(i, j);
				if (pixel > max) max = pixel;
				if (pixel < min) min = pixel;
			}
		}
	}

	void join(const MinMax& x) {
		if (x.min < min) min = x.min;
		if (x.max > max) max = x.max;
	}

	MinMax(MinMax& x, tbb::split):myImage(x.myImage),min(x.min),max(x.max) {}

	MinMax(Mat image):myImage(image) {
		min = max = image.at<unsigned char>(0, 0);
	}
};

class AsciiArt {
	Mat myImage;
	unsigned char min, max;
	static const char g2c[8];
	unsigned char *aa;
	
public:	
	int cols, rows;

	void operator() (const blocked_range<int>& r) const {
		Mat image = myImage;

		for (int i=r.begin(); i!=r.end(); ++i) {
			for (int j=0; j<image.cols; j+=C_STEP) {
				float gray = 0;
				int count = 0;

				for (int m=0; m<R_STEP; m++) {
					for (int n=0; n<C_STEP; n++) {
						if ((i*R_STEP+m < image.rows) && (j+n < image.cols)) {
							gray += image.at<unsigned char>(i*R_STEP+m, j+n);
							count++;
						}
					}
				}

				gray /= count;
				gray = 7 * (gray - min) / (max - min);
				aa[i*cols + j/C_STEP] = g2c[(int)gray];
			}
		}
	}

	void dump(char *output) {
		ofstream art;
		art.open(output);
		for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
				art << aa[i*cols+j];
			}
			art << endl;
		}
	}

	AsciiArt(Mat image) : myImage(image) {
		MinMax mm(image);
		parallel_reduce(blocked_range<int>(0,image.rows), mm, auto_partitioner());

		min = mm.min;
		max = mm.max;

		rows = (image.rows + R_STEP - 1) / R_STEP;
		cols = (image.cols + C_STEP - 1) / C_STEP;

		aa = new unsigned char[rows*cols];
	}
};

const char AsciiArt::g2c[8] = {'@', '8', 'O', 'o', ':', '.', ' ', ' '};

int
main(int argc, char **argv)
{
	Mat image;
	char *input, *output;

	if (argc != 3) {
		cout << "Usage: aatbb <input> <output>" << endl;
		return -1;
	}

	input  = argv[1];
	output = argv[2];

	task_scheduler_init init;

	image = imread(input, 0);

	AsciiArt aa(image);

	parallel_for(blocked_range<int>(0, aa.rows), aa, auto_partitioner());
	
	aa.dump(output);

	return 0;
}
