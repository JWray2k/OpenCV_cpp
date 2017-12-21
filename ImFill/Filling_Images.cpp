#include "opencv2/opencv.hpp"

// ImFill Tutorial - https://www.learnopencv.com/filling-holes-in-an-image-using-opencv-python-c/

/*
There is no imfill function in OpenCV, but we can surely write one! The idea is rather simple. We know the pixel (0,0) is 
connected to the background. So we can extract the background, by simply doing a floodfill operation from pixel (0, 0). 
Pixels that are not affected by the floodfill operation are necessarily inside the boundary. The flood-filled image when 
inverted and combined with the thresholded image gives the foreground mask!
*/

using namespace cv;

int main(int argc, char **argv)
{
	// Read image
	Mat im_in = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", IMREAD_GRAYSCALE); 
	
	// doesn't help...
	//Mat dst;

	////Apply bilateral filter
	//bilateralFilter(im_in, dst, 15, 80, 80);
	//imshow("source", im_in);
	//imshow("result", dst);

	//Mat gray_image;
	//cvtColor(dst, gray_image, CV_BGR2GRAY);

	// Threshold.
	// Set values equal to or above 220 to 0.
	// Set values below 220 to 255.
	Mat im_th;
	threshold(im_in, im_th, 120, 255, THRESH_BINARY_INV); //initial threshold (220) is too high... right about 120 is good

	// Floodfill from point (0, 0)
	Mat im_floodfill = im_th.clone();
	floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));

	// Invert floodfilled image
	Mat im_floodfill_inv;
	bitwise_not(im_floodfill, im_floodfill_inv);

	// Combine the two images to get the foreground.
	Mat im_out = (im_th | im_floodfill_inv);

	// Display images
	imshow("Thresholded Image", im_th);
	imshow("Floodfilled Image", im_floodfill);
	imshow("Inverted Floodfilled Image", im_floodfill_inv);
	imshow("Foreground", im_out);
	waitKey(0);

}