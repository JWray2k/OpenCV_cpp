/*
Histogram Calculation -
http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
Goal

In this tutorial you will learn how to:
	Use the OpenCV function split to divide an image into its correspondent planes.
	To calculate histograms of arrays of images by using the OpenCV function calcHist
	To normalize an array by using the function normalize

Note In the last tutorial (Histogram Equalization) we talked about a particular kind of histogram called Image histogram. Now we will considerate it in its more general concept. Read on!

What are histograms?
Histograms are collected counts of data organized into a set of predefined bins

When we say data we are not restricting it to be intensity values (as we saw in the previous Tutorial). The data collected can be whatever feature you find useful to describe your image.

Let’s see an example. Imagine that a Matrix contains information of an image (i.e. intensity in the range 0-255):

../../../../../_images/Histogram_Calculation_Theory_Hist0.jpg
What happens if we want to count this data in an organized way? Since we know that the range of information value for this case is 256 values, we can segment our range in subparts (called bins) like:

\begin{array}{l}
[0, 255] = { [0, 15] \cup [16, 31] \cup ....\cup [240,255] } \\
range = { bin_{1} \cup bin_{2} \cup ....\cup bin_{n = 15} }
\end{array}

and we can keep count of the number of pixels that fall in the range of each bin_{i}. Applying this to the example above we get the image below ( axis x represents the bins and axis y the number of pixels in each of them).

../../../../../_images/Histogram_Calculation_Theory_Hist1.jpg
This was just a simple example of how an histogram works and why it is useful. An histogram can keep count not only of color intensities, but of whatever image features that we want to measure (i.e. gradients, directions, etc).

Let’s identify some parts of the histogram:

	dims: The number of parameters you want to collect data of. In our example, dims = 1 because we are only counting the intensity values of each pixel (in a greyscale image).
	bins: It is the number of subdivisions in each dim. In our example, bins = 16
	range: The limits for the values to be measured. In this case: range = [0,255]
What if you want to count two features? In this case your resulting histogram would be a 3D plot (in which x and y would be bin_{x} and bin_{y} for each feature and z would be the number of counts for each combination of (bin_{x}, bin_{y}). The same would apply for more features (of course it gets trickier).

What OpenCV offers you
For simple purposes, OpenCV implements the function calcHist, which calculates the histogram of a set of arrays (usually images or image planes). It can operate with up to 32 dimensions. We will see it in the code below!

Code
What does this program do?

	Loads an image
	Splits the image into its R, G and B planes using the function split
	Calculate the Histogram of each 1-channel plane by calling the function calcHist
	Plot the three histograms in a window

*/
/**
* @function calcHist_Demo.cpp
* @brief Demo code to use the function calcHist
* @author
*/

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/**
* @function main
*/
int main(int argc, char** argv)
{
	Mat src, dst;

	/// Load image
	// String imageName("../data/lena.jpg"); // by default
	String imageName("C:/Users/wray/Pictures/times-square.jpg");

	if (argc > 1)
	{
		imageName = argv[1];
	}

	src = imread(imageName, IMREAD_COLOR);

	if (src.empty())
	{
		return -1;
	}

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	waitKey(0);

	return 0;

}
