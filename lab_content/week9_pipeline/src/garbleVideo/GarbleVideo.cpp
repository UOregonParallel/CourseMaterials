#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;

//----Method Declarations
static void garbleVideo();
static void ungarbleVideo();
//----END Method Declarations


/**----------------------------------------------------------------------------
 * Main method to call the garble and ungarble methods
 */
int main(int argc, char* argv[])
{	
	garbleVideo();
	ungarbleVideo();
	return 0;
}


/**----------------------------------------------------------------------------
 * Method to take an input video and 'gargle' it. The input video has is
 * brightness, contrast, rgb pixel locations, and rotation all changed. These
 * changes can then be reveresed, within a certain margin of error owing to 
 * the data loss fo the 8-bit image, to retrun the original image.
 * 
 * @param void
 * @return void
 */
void garbleVideo()
{
	// Load input video
	VideoCapture input_cap("217pin.mp4");
	if (!input_cap.isOpened())
	{
		std::cout << "Input video could not be opened" << std::endl;
		exit(13);
	}

	// Setup output video
	VideoWriter output_cap(	"output.avi",  
							input_cap.get(CV_CAP_PROP_FOURCC),
							input_cap.get(CV_CAP_PROP_FPS), 
							cv::Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), 
							input_cap.get(CV_CAP_PROP_FRAME_HEIGHT))
							);

	if (!output_cap.isOpened())
	{
		std::cout << "Output video could not be opened" << std::endl;
		exit(13);
	}

	// Loop to read frames from the input and write to the output
	Mat frame;
	int iAngle = 20;
	while (true)
	{       
		if (!input_cap.read(frame))             
			break;
	
		//increase the brightness by 100 units
		Mat imgH = frame + Scalar(100, 100, 100);
		
		//increase the contrast 1.025x 
		imgH.convertTo(imgH, -1, 1.025, 0); 

		//rotate image
		Mat matRotation = getRotationMatrix2D( 	Point(imgH.cols / 2, imgH.rows / 2), 
												(iAngle - 180), 
												1 
												);	 
		warpAffine( imgH, imgH, matRotation, imgH.size());

		//rearange rgb values and move pixel placement locations
		Mat outFrame = imgH.clone();
		for (int y = 0; y < imgH.rows; y++) {
			for (int x = 0; x < imgH.cols; x++) {
				int r, g, b;
				g = imgH.at<cv::Vec3b>(y,x)[0];
				b = imgH.at<cv::Vec3b>(y,x)[1];
				r = imgH.at<cv::Vec3b>(y,x)[2];

				outFrame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[0] = r;
				outFrame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[1] = g;
				outFrame.at<cv::Vec3b>((y+375)%(outFrame.rows),(x+475)%(outFrame.cols))[2] = b;
			}
		}		

		//write video to output
		output_cap.write(outFrame);
	}

	// Release capture interfaces   
	input_cap.release();
	output_cap.release();

	std::cout << "Video was garbled..." << std::endl;
} //END gargleVideo()


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
	VideoCapture input_cap("output.avi");
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


