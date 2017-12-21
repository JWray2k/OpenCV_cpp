#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

// referenced from FILTERS A - AVERAGE AND GAUSSIANBLUR - http://www.bogotobogo.com/OpenCV/opencv_3_tutorial_imgproc_gausian_median_blur_bilateral_filter_image_smoothing.php
/*void blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
	The parameters are:

	src - input image; it can have any number of channels, which are processed independently, but the depth should be CV_8U, CV_16U, CV_16S, CV_32F or CV_64F.
	dst - output image of the same size and type as src.
	ksize - blurring kernel size.
	anchor - anchor point; default value Point(-1,-1) means that the anchor is at the kernel center.
	borderType - border mode used to extrapolate pixels outside of the image.
*/
// Also known as The filter used here the most simplest one called homogeneous smoothing or box filter.

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
		//smooth the image in the "src" and save it to "dst"
		blur(src, dst, Size(i, i));

		//show the blurred image with the text
		imshow("Smoothing by avaraging", dst);

		//wait for 3 seconds
		waitKey(3000);
	}
}