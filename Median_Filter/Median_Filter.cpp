#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

// FILTERS B - MEDIANBLUR AND BILATERAL - http://www.bogotobogo.com/OpenCV/opencv_3_tutorial_imgproc_gausian_median_blur_bilateral_filter_image_smoothing_B.php

/* void medianBlur(InputArray src, OutputArray dst, int ksize)
The parameters are
	src - input 1-, 3-, or 4-channel image; when ksize is 3 or 5, the image depth should be CV_8U, CV_16U, or CV_32F, for larger aperture sizes, it can only be CV_8U.
	dst - destination array of the same size and type as src.
	ksize - aperture linear size; it must be odd and greater than 1, for example: 3, 5, 7 ...

	Median filter
	The Median filter is a common technique for smoothing. Median smoothinging is widely used in edge detection algorithms because under certain 
	conditions, it preserves edges while removing noise.
*/
using namespace cv;

int main(int argc, char** argv)
{
	namedWindow("Before", CV_WINDOW_AUTOSIZE);

	// Load the source image
	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1);

	// Create a destination Mat object
	Mat dst;

	// display the source image
	imshow("Before", src);

	for (int i = 1; i<51; i = i + 2)
	{
		// smooth the image in the "src" and save it to "dst"
		// blur(src, dst, Size(i,i));

		// Gaussian smoothing
		// GaussianBlur( src, dst, Size( i, i ), 0, 0 );

		// Median smoothing
		medianBlur(src, dst, i);

		// show the blurred image with the text
		imshow("Median filter", dst);

		// wait for 5 seconds
		waitKey(5000);
	}
}