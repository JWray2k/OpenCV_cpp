/*
Histogram Equalization -
http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_equalization/histogram_equalization.html
Goal
In this tutorial you will learn:
	- What an image histogram is and why it is useful
	- To equalize histograms of images by using the OpenCV function:equalize_hist:equalizeHist <>

Theory
What is an Image Histogram?
	- It is a graphical representation of the intensity distribution of an image.
	- It quantifies the number of pixels for each intensity value considered.

What is Histogram Equalization?
	- It is a method that improves the contrast in an image, in order to stretch out the intensity range.
	- To make it clearer, from the image above, you can see that the pixels seem clustered around the middle 
	  of the available range of intensities. What Histogram Equalization does is to stretch out this range. 
	  Take a look at the figure below: The green circles indicate the underpopulated intensities. After 
	  applying the equalization, we get an histogram like the figure in the center. The resulting image 
	  is shown in the picture at right.

How does it work?
	- Equalization implies mapping one distribution (the given histogram) to another distribution (a wider and 
	  more uniform distribution of intensity values) so the intensity values are spreaded over the whole range.

	- To accomplish the equalization effect, the remapping should be the cumulative distribution function (cdf) 
	  (more details, refer to Learning OpenCV). For the histogram H(i), its cumulative distribution H^{'}(i) is:

			 H^{'}(i) = \sum_{0 \le j < i} H(j) (review webpage)

	  To use this as a remapping function, we have to normalize H^{'}(i) such that the maximum value is 255 
	  ( or the maximum value for the intensity of the image ). From the example above, the cumulative function is:

	- Finally, we use a simple remapping procedure to obtain the intensity values of the equalized image:

			equalized( x, y ) = H^{'}( src(x,y) ) (review webpage)

	- What does this program do?

		- Loads an image
		- Convert the original image to grayscale
		- Equalize the Histogram by using the OpenCV function EqualizeHist - http://docs.opencv.org/2.4/modules/imgproc/doc/histograms.html?highlight=equalizehist#equalizehist
		- Display the source and equalized images in a window.
*/


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/**  @function main */
int main(int argc, char** argv)
{
	Mat src, dst;

	char* source_window = "Source image";
	char* equalized_window = "Equalized Image";

	/// Load image
	// src = imread(argv[1], 1); // Figure out what that 1 represents - original
	src = imread("C:/Users/wray/Pictures/faded.jpg"); // this image works awesome!!! There is a bit of noise and the corners could be enhanced a bit more though..
	// src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1); // This causes the stacked bunnies to blend a bit too much.

	if (!src.data)
	{
		cout << "Usage: ./Histogram_Demo <path_to_image>" << endl;
		return -1;
	}

	/// Convert to grayscale
	cvtColor(src, src, CV_BGR2GRAY);

	/// Apply Histogram Equalization
	equalizeHist(src, dst);

	/// Display results
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	namedWindow(equalized_window, CV_WINDOW_AUTOSIZE);

	imshow(source_window, src);
	imshow(equalized_window, dst);

	/// Wait until user exits the program
	waitKey(0);

	return 0;
}