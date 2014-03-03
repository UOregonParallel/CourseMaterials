#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_min.h>

using namespace std;
using namespace cv;
using namespace cilk;

#ifndef C_STEP
#define C_STEP 4
#endif

#ifndef R_STEP
#define R_STEP 6
#endif

static char g2c[8] = {'@', '8', 'O', 'o', ':', '.', ' ', ' '};

int
ascii_art(Mat &image, char *output)
{
	unsigned char min, max;
	int rows = (image.rows + R_STEP - 1) / R_STEP;
	int cols = (image.cols + C_STEP - 1) / C_STEP;

	reducer_max<unsigned char> r_max;
	reducer_min<unsigned char> r_min;

	unsigned char *aa = new unsigned char[rows*cols];

	cilk_for (int i=0; i<image.rows; i++) {
		cilk_for (int j=0; j<image.cols; j++) {
			unsigned char pixel = image.at<unsigned char>(i, j);

			r_max.calc_max(pixel);
			r_min.calc_min(pixel);
		}
	}

	max = r_max.get_value();
	min = r_min.get_value();

	cilk_for (int i=0; i<image.rows; i+=R_STEP) {
		cilk_for (int j=0; j<image.cols; j+=C_STEP) {
			int m, n;
			float gray = 0;
			int count = 0;

			for (m=0; m<R_STEP; m++) {
				for (n=0; n<C_STEP; n++) {
					if ((i+m < image.rows) && (j+n < image.cols)) {
						gray += image.at<unsigned char>(i+m, j+n);
						count++;
					}
				}
			}
			
			gray /= count;
			gray = 7 * (gray - min) / (max - min);
			aa[i*cols/R_STEP + j/C_STEP] = g2c[(int)gray];
		}
	}


	ofstream art;
	art.open(output);
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			art << aa[i*cols+j];
		}
		art << endl;
	}

	delete[] aa;

	return 0;
}

int
main(int argc, char **argv)
{
	Mat image;
	char *input, *output;

	if (argc != 3) {
		cout << "Usage: aacilk <input> <output>" << endl;
		return -1;
	}

	input  = argv[1];
	output = argv[2];

	image = imread(input, 0);

	ascii_art(image, output);

	return 0;
}
