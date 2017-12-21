// Introduction to OpenCV Tracker - http://docs.opencv.org/trunk/d2/d0a/tutorial_introduction_to_tracker.html

/*
Goal

In this tutorial you will learn how to

	Create a tracker object.
	Use the roiSelector function to select a ROI from a given image.
	Track a specific region in a given image.
*/
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
using namespace std;
using namespace cv;
int main(int argc, char** argv){

	// 1. Set up the input video
	// show help
	if (argc<2){
		cout <<
			" Usage: tracker <video_name>\n"
			" examples:\n"
			" example_tracking_kcf Bolt/img/%04d.jpg\n"
			" example_tracking_kcf faceocc2.webm\n"
			<< endl;
		return 0;
	}

	/*
	In this tutorial, you can choose between video or list of images for the program input. As written in the help, you should specify the 
	input video as parameter of the program. If you want to use image list as input, the image list should have formatted numbering as shown 
	in help. In the help, it means that the image files are numbered with 4 digits (e.g. the file naming will be 0001.jpg, 0002.jpg, and so on).

	You can find video samples in opencv_extra/testdata/cv/tracking https://github.com/opencv/opencv_extra/tree/master/testdata/cv/tracking
	*/


	// 2. declare all required variables
	Rect2d roi; // You need roi to record the bounding box of the tracked object. The value stored in this variable will be updated using the tracker object.
	Mat frame; // The frame variable is used to hold the image data from each frame of the input video or images list.



	// 3. create a tracker object
	Ptr<Tracker> tracker = TrackerKCF::create();
	/*
	There are at least 5 types of tracker algorithms that can be used:
		MIL
		BOOSTING
		MEDIANFLOW
		TLD
		KCF
	Each tracker algorithm has their own advantages and disadvantages, please refer the documentation of cv::Tracker for more detailed information.
	*/


	// set input video
	// std::string video = argv[1]; // Step 1 ...commented out to just get the next
	VideoCapture cap("C:/Users/wray/videos/chaplin.mp4"); // was just video


	// get bounding box
	cap >> frame;
	// 4. Select the tracked object
	roi = selectROI("tracker", frame);
	// Using this function, you can select the bounding box of the tracked object using a GUI. With default parameters, the selection is started from the 
	// center of the box and a middle cross will be shown.


	// quit if ROI was not selected
	if (roi.width == 0 || roi.height == 0)
		return 0;

	// 5. initialize the tracker
	tracker->init(frame, roi);
	// Tracker algorithm should be initialized with the provided image data as well as the bounding box of the tracked object. Make sure that the bounding 
	// box is not valid (size more than zero) to avoid the initialization process failed.

	// perform the tracking process
	printf("Start the tracking process, press ESC to quit.\n");
	for (;;){
		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if (frame.rows == 0 || frame.cols == 0)
			break;

		// 6. update the tracking result
		tracker->update(frame, roi); // This update function will perform the tracking process and pass the result to the roi variable.

		// draw the tracked object
		rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);

		// show image with the tracked object
		imshow("tracker", frame);

		//quit on ESC button
		if (waitKey(1) == 27)break;
	}
	return 0;
}