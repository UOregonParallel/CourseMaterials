#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

struct pixel {
	double red;
	double green;
	double blue;
	
	pixel(double r, double g, double b) : red(r), green(g), blue(b) {};
};

void gaussian_kernel(const int rows, const int cols, const double stddev, double * const kernel) {
	const double denom = 2.0 * stddev * stddev;
	const double g_denom = M_PI * denom;
	const double g_denom_recip = (1.0/g_denom);
	double sum = 0.0;
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			const double row_dist = i - (rows/2);
			const double col_dist = j - (cols/2);
			const double dist_sq = (row_dist * row_dist) + (col_dist * col_dist);
			const double value = g_denom_recip * exp((-dist_sq)/denom);
			kernel[i + (j*rows)] = value;
			sum += value;
		}
	}
	// Normalize
	const double recip_sum = 1.0 / sum;
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			kernel[i + (j*rows)] *= recip_sum;
		}		
	}
}

void apply_stencil(const int radius, const double stddev, const int rows, const int cols, pixel * const in, pixel * const out) {
	const int dim = radius*2+1;
	double kernel[dim*dim];
	gaussian_kernel(dim, dim, stddev, kernel);
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			const int out_offset = i + (j*rows);
			// For each pixel, do the stencil
			for(int x = i - radius, kx = 0; x <= i + radius; ++x, ++kx) {
				for(int y = j - radius, ky = 0; y <= j + radius; ++y, ++ky) {
					if(x >= 0 && x < rows && y >= 0 && y < cols) {
						const int in_offset = x + (y*rows);
						const int k_offset = kx + (ky*dim);
						out[out_offset].red   += kernel[k_offset] * in[in_offset].red;
						out[out_offset].green += kernel[k_offset] * in[in_offset].green;
						out[out_offset].blue  += kernel[k_offset] * in[in_offset].blue;
					}
				}
			}
		}
	}
}

int main( int argc, char* argv[] ) {

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " imageName" << std::endl;
	}

	// Read image
	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(!image.data ) {
		std::cout <<  "Error opening " << argv[1] << std::endl;
		return -1;
	}
	
	// Get image into C array of doubles for processing
	const int rows = image.rows;
	const int cols = image.cols;
	pixel * imagePixels = (pixel *) malloc(rows * cols * sizeof(pixel));
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			Vec3b p = image.at<Vec3b>(i, j);
			imagePixels[i + (j*rows)] = pixel(p[0]/255.0,p[1]/255.0,p[2]/255.0);
		}
	}
	
	// Create output array
	pixel * outPixels = (pixel *) malloc(rows * cols * sizeof(pixel));
	for(int i = 0; i < rows * cols; ++i) {
		outPixels[i].red = 0.0;
		outPixels[i].green = 0.0;
		outPixels[i].blue = 0.0;
	}
	
	// Do the stencil
	apply_stencil(3, 32.0, rows, cols, imagePixels, outPixels);
	
	// Create an output image (same size as input)
	Mat dest(rows, cols, CV_8UC3);
	// Copy C array back into image for output
	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			const size_t offset = i + (j*rows);
			dest.at<Vec3b>(i, j) = Vec3b(floor(outPixels[offset].red * 255.0),
										 floor(outPixels[offset].green * 255.0),
										 floor(outPixels[offset].blue * 255.0));
		}
	}
	
	imwrite("out.jpg", dest);
	
	
	free(imagePixels);
	free(outPixels);
	return 0;
}
