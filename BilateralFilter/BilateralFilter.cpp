#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

/*
Smoothing (Blurring) by Bilateral
So far, we reviewed 3 image filters. Bilateral blurring is one of the most advanced filter to smooth an image and reduce noise. 
The other three filters will smooth away the edges while removing noises, however, this filter can reduce noise of the image 
while preserving the edges. The drawback of this type of filter is that it takes longer to filter the input image.

void bilateralFilter(InputArray src, OutputArray dst, int d, double sigmaColor, double sigmaSpace, int borderType=BORDER_DEFAULT )
Parameters:
	src - Source 8-bit or floating-point, 1-channel or 3-channel image.
	dst - Destination image of the same size and type as src.
	d - Diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
	sigmaColor - Filter sigma in the color space. A larger value of the parameter means that farther colors within the pixel neighborhood 
	will be mixed together, resulting in larger areas of semi-equal color.
	sigmaSpace - Filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other as long as their 
	colors are close enough. When d>0 , it specifies the neighborhood size regardless of sigmaSpace . Otherwise, d is proportional to sigmaSpace.
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
		bilateralFilter(src, dst, i, 200, 200); // refer to http://docs.opencv.org/3.0-beta/modules/imgproc/doc/filtering.html --- i represents the size

		//show the blurred image with the text
		imshow("Bilateral Filter", dst);

		//wait for 5 seconds
		waitKey(5000);
	}
}