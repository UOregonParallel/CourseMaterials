#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;

//----Method Declarations
static void ungarbleVideo();
//----END Method Declarations


/**----------------------------------------------------------------------------
 * Main method to call the ungarble method
 */
int main(int argc, char* argv[])
{	
	ungarbleVideo();
	return 0;
}


//read next frame function
void readFrame(VideoCapture &input_cap, Mat& readFrame, int *outOfFrames)
{
	if(!input_cap.read(readFrame))
		(*outOfFrames) = 1;
}


//filter to decrease the contrast
void decreaseContrastFilter(Mat &inFrame, Mat &outFrame) 
{
	//decrease the contrast 1/1.025
	inFrame.convertTo(outFrame, -1, .975, 0); 
} //END decreaseContrastFilter


//filter to decrease the brightness
void decreaseBrightnessFilter(Mat &frame, Mat &outFrame)
{				
		//decrease the brightness by 100 units
		outFrame =  frame - Scalar(100, 100, 100); 
} //END decreaseBrightnessFilter


//filter to rotate a frame
void rotateMatFilter(Mat &frame, Mat &outFrame) 
{
	int iAngle = 340;
	Mat matRotation = getRotationMatrix2D(Point(frame.cols/2, frame.rows/2), (iAngle-180), 1);	 
	warpAffine( frame, outFrame, matRotation, frame.size());
			
} //END rotateMatFilter()


//filter to rearange the pixels in a frame
void rearangePixelsFilter(Mat &frame, Mat &outFrame) 
{
	outFrame = frame.clone();
		for (int y = 0; y < frame.rows; y++) {
			for (int x = 0; x < frame.cols; x++) {
				int r, g, b;
				r = frame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[0];
				g = frame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[1];
				b = frame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[2];
				outFrame.at<cv::Vec3b>(y,x)[0] = g;
				outFrame.at<cv::Vec3b>(y,x)[1] = b;
				outFrame.at<cv::Vec3b>(y,x)[2] = r;
			}
		}	
} //END rearangePixelsFilter()


//function to write a frame
void writeFrame(VideoWriter &output_cap, Mat &outFrame)
{
	output_cap.write(outFrame);
} //END writeFrame()


/**----------------------------------------------------------------------------
 * Method to 'ungargle' a video. This method performs the reverse operations
 * of those used to 'gargle' the video. The output video will be near exact
 * to the original, allowing for the true visualization to be seen.
 *	
 * @warning Two of these operations must be done in the exact reverse order 
 * that they were performed in the transformation in order to retrieve the 
 * correct image (rotate and pixel location change)..
 *
 * @param void
 * @return void
 */
void ungarbleVideo() 
{
	// Load input video
	VideoCapture input_cap("../interceptedSignal.avi");
	if (!input_cap.isOpened())
	{
		std::cout << "Input video could not be opened" << std::endl;
		exit(13);
	}

	// Setup output video
	VideoWriter output_cap("ungarble.avi",  
							input_cap.get(CV_CAP_PROP_FOURCC),
							input_cap.get(CV_CAP_PROP_FPS), 
							cv::Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), 
							input_cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

	if (!output_cap.isOpened())
	{
		std::cout << "Output video could not be opened" << std::endl;
		exit(13);
	}

	Mat frame, brightFrameReturn, contrastFrameReturn;
	Mat pixelsFrameReturn, rotateFrameReturn;
	int outOfFrames = 0;
	while(true) {
		readFrame(input_cap, frame, &outOfFrames);
		
		if(outOfFrames == 1) 
			break;
		
		decreaseBrightnessFilter(frame, brightFrameReturn);
		decreaseContrastFilter(brightFrameReturn, contrastFrameReturn);
		rearangePixelsFilter(contrastFrameReturn, pixelsFrameReturn);
		rotateMatFilter(pixelsFrameReturn, rotateFrameReturn);
		writeFrame(output_cap, rotateFrameReturn);
	}

	// Release capture interfaces   
	input_cap.release();
	output_cap.release();

	std::cout << "Video was ungarbled..." << std::endl;
} //END ungarbleVideo()


