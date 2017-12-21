// #include <cv.h>
// #include <highgui.h>
#include <opencv\cv.h>
#include <opencv2\highgui.hpp>
// #include <opencv\highgui.h>
// using namespace cv; - ambiguous for some reason

/// Global Variables
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// Matrices to store images
cv::Mat src1;
cv::Mat src2;
cv::Mat dst;

/**
* @function on_trackbar
* @brief Callback for trackbar
*/
void on_trackbar(int, void*)
{
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = (1.0 - alpha);

	addWeighted(src1, alpha, src2, beta, 0.0, dst);

	imshow("Linear Blend", dst);
}

int main(int argc, char** argv)
{
	/// Read image ( same size, same type )
	// src1 = imread("../../images/LinuxLogo.jpg");
	// src2 = imread("../../images/WindowsLogo.jpg");

	src1 = cv::imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); // works for the dragon... but not the rubic cube... strange
	src2 = cv::imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-17.png"); 

	if (!src1.data) { printf("Error loading src1 \n"); return -1; }
	if (!src2.data) { printf("Error loading src2 \n"); return -1; }

	/// Initialize values
	alpha_slider = 0;

	/// Create Windows
	cv::namedWindow("Linear Blend", 1);

	/// Create Trackbars
	char TrackbarName[50];
	sprintf(TrackbarName, "Alpha x %d", alpha_slider_max);

	cv::createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);

	/// Show some stuff
	on_trackbar(alpha_slider, 0);

	/// Wait until user press some key
	cv::waitKey(0);
	return 0;
}