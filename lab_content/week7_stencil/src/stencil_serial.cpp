#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main( int argc, char* argv[] ) {

	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " imageName" << std::endl;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if(!image.data ) {
		std::cout <<  "Error opening " << argv[1] << std::endl;
		return -1;
	}

	return 0;
}