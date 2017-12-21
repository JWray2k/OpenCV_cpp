#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
//#include <opencv2/opencv.hpp>
//#include <iostream>

// Threshold Tutorial used - http://www.learnopencv.com/opencv-threshold-python-cpp/ 

using namespace cv;

int main(){
	// Read image
	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", IMREAD_GRAYSCALE); //need grayscale for thresholding
	Mat dst;

	// Set threshold and maxValue
	double thresh = 150; //my current test is most interesting when using values between 120 - 150
	double maxValue = 255; //this may change based on inverse...

	// 1) Binary Threshold - objects appear black
	// threshold(src, dst, thresh, maxValue, THRESH_BINARY);

	// 2) Inverse Binary Threshold - background appears black... objects appear white... this of course would only work with lower gray values for the background
	// threshold(src, dst, thresh, maxValue, THRESH_BINARY_INV);

	// 3) Truncate Thresholding - objects appear grayscale
	// In this type of thresholding, the destination pixel is set to the threshold ( thresh ) if the source pixel
	// value is greater than the threshold. Otherwise it is set to the source pixel value. maxValue is ignored.
	// threshold(src, dst, thresh, maxValue, THRESH_TRUNC);

	// 4) Threshold to Zero - objects appear smoothed grayscale if thresh value is low enough
	// In this kind of thresholding, the destination pixel value is set to the corresponding source pixel value 
	// if the source pixel value is greater than the threshold.Otherwise it is set to zero.maxValue is ignored.
	// threshold(src, dst, thresh, maxValue, THRESH_TOZERO); //thresh value between 60 and 100 seem pretty good

	// 5) Inverted Threshold to Zero - if thresh value is around 80, it appears like an X_Ray... higher thresh takes it to smooth grayscale
	// In this kind of thresholding, the destination pixel value is set to zero if the source pixel value is 
	// greater than the threshold. Otherwise it is set to the source pixel value. maxValue is ignored.
	// threshold(src, dst, thresh, maxValue, THRESH_TOZERO_INV);

	// BONUS 1) Adaptive Thresholding - Gives a coloring book edge detection effect
	// http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
	// void adaptiveThreshold(InputArray src, OutputArray dst, double maxValue, int adaptiveMethod, int thresholdType, int blockSize, double C)
	/*
	Parameters:
	src – Source 8-bit single-channel image.

	dst – Destination image of the same size and the same type as src .

	maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. See the details below.

	adaptiveMethod – Adaptive thresholding algorithm to use, ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C . See the details below.

	- ADAPTIVE_THRESH_MEAN_C , the threshold value T(x,y) is a mean of the \texttt{blockSize} \times \texttt{blockSize} neighborhood of (x, y) minus C.

	- ADAPTIVE_THRESH_GAUSSIAN_C , the threshold value T(x, y) is a weighted sum (cross-correlation with a Gaussian window) of the \texttt{blockSize}
	\times \texttt{blockSize} neighborhood of (x, y) minus C . The default sigma (standard deviation) is used for the specified blockSize.
	See getGaussianKernel() - http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#Mat - getGaussianKernel(int ksize, double sigma, int ktype)

	thresholdType – Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV.

	blockSize – Size of a pixel neighborhood that is used to calculate a threshold value for the pixel: 3, 5, 7, and so on. (for dragon, 7 is good)

	C – Constant subtracted from the mean or weighted mean (see the details below). Normally, it is positive but may be zero or negative as well. (for dragon, 8 is good)
	*/
	adaptiveThreshold(src, dst, maxValue, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 8); // Pretty good results with these settings... higherblock size = thicker lines

	// BONUS 2) OTSU Thresholding
	// http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#threshold 
	/*
	cv::threshold(im_gray, img_bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	Paramaters:
	- im_gray is a source 8-bit image,
	- img_bw is a result,
	- 0 means threshold level which actually is omitted because we used CV_THRESH_OTSU flag,
	- 255 is a value that is going to be assigned to respectively pixels in the result (namely, to all pixels which value in the source is greater then
	computed threshold level)
	- CV_THRESH_BINARY | CV_THRESH_OTSU is a required flag to perform Otsu thresholding. Because in fact we would like to perform binary thresholding,
	so we use CV_THRESH_BINARY (you can use any of 5 flags opencv provides) combined with CV_THRESH_OTSU
	- The point made about THRESH_<type> is critical... I got some interesting results with THRESH_TOZERO and THRESH_TRUNC
	*/
	// threshold(src, dst, 0, 255, THRESH_TRUNC | THRESH_OTSU); //I can't really tell what this one does as much... more research needed

	// BONUS 3) Flood Fill
	// http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html?highlight=floodfill
	// C++: int floodFill(InputOutputArray image, Point seedPoint, Scalar newVal, Rect* rect = 0, Scalar loDiff = Scalar(), Scalar upDiff = Scalar(), int flags = 4)
	// C++: int floodFill(InputOutputArray image, InputOutputArray mask, Point seedPoint, Scalar newVal, Rect* rect = 0, Scalar loDiff = Scalar(), Scalar upDiff = Scalar(), int flags = 4)
	/*
	Parameters:
	- image – Input/output 1- or 3-channel, 8-bit, or floating-point image. It is modified by the function unless the FLOODFILL_MASK_ONLY flag is set in
	the second variant of the function. See the details below.

	- mask –
	Operation mask that should be a single-channel 8-bit image, 2 pixels wider and 2 pixels taller than image. Since this is both an input and output
	parameter, you must take responsibility of initializing it. Flood-filling cannot go across non-zero pixels in the input mask. For example, an edge
	detector output can be used as a mask to stop filling at edges. On output, pixels in the mask corresponding to filled pixels in the image are set
	to 1 or to the a value specified in flags as described below. It is therefore possible to use the same mask in multiple calls to the function to
	make sure the filled areas do not overlap.

	- Note Since the mask is larger than the filled image, a pixel  (x, y) in image corresponds to the pixel  (x+1, y+1) in the mask.

	- seedPoint – Starting point.

	- newVal – New value of the repainted domain pixels.

	- loDiff – Maximal lower brightness/color difference between the currently observed pixel and one of its neighbors belonging to the component,
	or a seed pixel being added to the component.

	- upDiff – Maximal upper brightness/color difference between the currently observed pixel and one of its neighbors belonging to the component,
	or a seed pixel being added to the component.

	- rect – Optional output parameter set by the function to the minimum bounding rectangle of the repainted domain.

	- flags –
	Operation flags. The first 8 bits contain a connectivity value. The default value of 4 means that only the four nearest neighbor pixels (those
	that share an edge) are considered. A connectivity value of 8 means that the eight nearest neighbor pixels (those that share a corner) will be
	considered. The next 8 bits (8-16) contain a value between 1 and 255 with which to fill the mask (the default value is 1). For example, 4 |
	( 255 << 8 ) will consider 4 nearest neighbours and fill the mask with a value of 255. The following additional options occupy higher bits and
	therefore may be further combined with the connectivity and mask fill values using bit-wise or (|):

	- FLOODFILL_FIXED_RANGE If set, the difference between the current pixel and seed pixel is considered. Otherwise, the difference between neighbor
	pixels is considered (that is, the range is floating).

	- FLOODFILL_MASK_ONLY If set, the function does not change the image ( newVal is ignored), and only fills the mask with the value specified in
	bits 8-16 of flags as described above. This option only make sense in function variants that have the mask parameter.


	*/


	imshow("New Threshold Image", dst);
	waitKey(0);
	return 0;
}