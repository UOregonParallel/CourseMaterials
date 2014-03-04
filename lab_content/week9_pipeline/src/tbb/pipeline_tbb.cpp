#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/partitioner.h"
#include "tbb/pipeline.h"

using namespace cv;
using namespace tbb;


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


//tbb parallel pipeline method
void videoPipe( VideoCapture &input_cap, VideoWriter &output_cap, int liveTokens ) {
    parallel_pipeline( /*max_number_of_live_token=*/liveTokens,       
        make_filter<void,Mat>(
            filter::serial_in_order,
            [&](flow_control& fc)-> Mat{
            	Mat readFrame;
               if(!input_cap.read(readFrame))
               	fc.stop();
               	
               return readFrame;
            }    
        ) &
        make_filter<Mat,Mat>(
            filter::parallel,
            [&](Mat x) {
    			Mat returnFrame;
            	decreaseBrightnessFilter(x, returnFrame);
            	return returnFrame;
            }
        ) &
        make_filter<Mat,Mat>(
            filter::parallel,
            [&](Mat x) {
    			Mat returnFrame;
            	decreaseContrastFilter(x, returnFrame);
            	return returnFrame;
            }
        ) &
        make_filter<Mat,Mat>(
            filter::parallel,
            [&](Mat x) {
    			Mat returnFrame;
            	rearangePixelsFilter(x, returnFrame);
            	return returnFrame;
            }
        ) &
        make_filter<Mat,Mat>(
            filter::parallel,
            [&](Mat x) {
    			Mat returnFrame;
            	rotateMatFilter(x, returnFrame);
            	return returnFrame;
            }
        ) &
        make_filter<Mat,void>(
            filter::serial_in_order,
            [&](Mat x) {
            	writeFrame(output_cap, x);
            	return output_cap;
            }
        )
    );
} //END videoPipe()


/**----------------------------------------------------------------------------
 * Method to 'ungarble' a video. This method performs the reverse operations
 * of those used to 'garble' the video. The output video will be near exact
 * to the original, allowing for the true visualization to be seen.
 *	
 * @warning Two of these operations must be done in the exact reverse order 
 * that they were performed in the transformation in order to retrieve the 
 * correct image (rotate and pixel location change)..
 *
 * @param void
 * @return void
 */
void ungarbleVideo(int liveTokenNum) 
{
	// Load input video
	VideoCapture input_cap = VideoCapture("../interceptedSignal.avi");
	if (!input_cap.isOpened())
	{
		std::cout << "Input video could not be opened" << std::endl;
		exit(13);
	}

	// Setup output video
	VideoWriter output_cap = VideoWriter("output.avi",  
			input_cap.get(CV_CAP_PROP_FOURCC),
			input_cap.get(CV_CAP_PROP_FPS), 
			cv::Size(input_cap.get(CV_CAP_PROP_FRAME_WIDTH), 
					input_cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

	if (!output_cap.isOpened())
	{
		std::cout << "Output video could not be opened" << std::endl;
		exit(13);
	}

	videoPipe(input_cap, output_cap, liveTokenNum);
	
	// Release capture interfaces   
	input_cap.release();
	output_cap.release();

	std::cout << "Video was ungarbled..." << std::endl;
} //END ungarbleVideo()


/**----------------------------------------------------------------------------
 * Main method to call the ungarble method
 */
int main(int argc, char* argv[])
{	
	if(argc < 2) { 
		std::cout << "./pipeline_tbb <num threads>	" << std::endl;
		exit(13);
	}
	
	ungarbleVideo(atoi(argv[1]));
	
	return 0;
} //END main()
//END ALL

