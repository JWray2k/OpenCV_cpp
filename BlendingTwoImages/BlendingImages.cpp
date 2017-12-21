/* Adding (blending) two images using OpenCV - http://docs.opencv.org/2.4/doc/tutorials/core/adding_images/adding_images.html 

Goal
In this tutorial you will learn:
	what is linear blending and why it is useful;
	how to add two images using addWeighted

Theory
Note The explanation below belongs to the book Computer Vision: Algorithms and Applications by Richard Szeliski

From our previous tutorial, we know already a bit of Pixel operators. An interesting dyadic (two-input) operator is the linear blend operator:

g(x) = (1 - \alpha)f_{0}(x) + \alpha f_{1}(x)

By varying \alpha from 0 \rightarrow 1 this operator can be used to perform a temporal cross-dissolve between two images or videos, as seen in 
slide shows and film productions (cool, eh?)

*/

// #include <cv.h>
#include <opencv\cv.h>
// #include <highgui.h>
#include <opencv\highgui.h>
#include <iostream>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;

int main(int argc, char** argv)
{
	double alpha = 0.5; double beta; double input;

	Mat src1, src2, dst;

	/// Ask the user enter alpha
	std::cout << " Simple Linear Blender " << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "* Enter alpha [0-1]: ";
	std::cin >> input;

	/// We use the alpha provided by the user if it is between 0 and 1
	if (input >= 0.0 && input <= 1.0)
	{
		alpha = input;
	}

	/// Read image ( same size, same type )
	src1 = imread("C:/Users/wray/Pictures/church01.jpg");
	src2 = imread("C:/Users/wray/Pictures/church03.jpg");

	if (!src1.data) { printf("Error loading src1 \n"); return -1; }
	if (!src2.data) { printf("Error loading src2 \n"); return -1; }

	/// Create Windows
	namedWindow("Linear Blend", 1);

	beta = (1.0 - alpha);
	addWeighted(src1, alpha, src2, beta, 0.0, dst);

	imshow("Linear Blend", dst);

	waitKey(0);
	return 0;
}