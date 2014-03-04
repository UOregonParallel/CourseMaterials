#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

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
	int i, j;
	unsigned char pixel;
	unsigned char min, max;
	int rows = (image.rows + R_STEP - 1) / R_STEP;
	int cols = (image.cols + C_STEP - 1) / C_STEP;

	unsigned char *aa = new unsigned char[rows*cols];

	min = max = image.at<unsigned char>(0, 0);
	for (i=0; i<image.rows; i++) {
		for (j=0; j<image.cols; j++) {
			pixel = image.at<unsigned char>(i, j);
			if (pixel > max) max = pixel;
			if (pixel < min) min = pixel;
		}
	}

	for (i=0; i<image.rows; i+=R_STEP) {
		for (j=0; j<image.cols; j+=C_STEP) {
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
	for (i=0; i<rows; i++) {
		for (j=0; j<cols; j++) {
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
		cout << "Usage: aa <input> <output>" << endl;
		return -1;
	}

	input  = argv[1];
	output = argv[2];

	image = imread(input, 0);

	ascii_art(image, output);

	return 0;
}
