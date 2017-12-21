#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

// Referenced from FILTERS A - AVERAGE AND GAUSSIANBLUR - http://www.bogotobogo.com/OpenCV/opencv_3_tutorial_imgproc_gausian_median_blur_bilateral_filter_image_smoothing.php
/* void GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )
Parameters:
		src - input image; the image can have any number of channels, which are processed independently, but the depth should be CV_8U, CV_16U, CV_16S, CV_32F or CV_64F.
		dst - output image of the same size and type as src.
		ksize - Gaussian kernel size. ksize.width and ksize.height can differ but they both must be positive and odd. Or, they can be zeroâs and then they are computed from sigma*.
		sigmaX - Gaussian kernel standard deviation in X direction.
		sigmaY - Gaussian kernel standard deviation in Y direction; if sigmaY is zero, it is set to be equal to sigmaX, if both sigmas are zeros, they are computed from ksize.width and ksize.height , respectively; to fully control the result regardless of possible future modifications of all this semantics, it is recommended to specify all of ksize, sigmaX, and sigmaY.
		borderType - pixel extrapolation method.
*/

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
		GaussianBlur(src, dst, Size(i, i), 0, 0); // this is an intersting algorithm... it continuously iterates until ending runtime.

		//show the blurred image with the text
		imshow("Gaussian filter", dst);

		//wait for 5 seconds
		waitKey(5000);
	}
}