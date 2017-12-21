#include <cstdio>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src;
	Mat hsv_image;
	// src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1); - crap result basd on hue I think
	src = imread("C:/Users/wray/Pictures/lunar.jpg", 1); // pretty solid result, but doesnät do a good job making lines precise.
	cvtColor(src, hsv_image, COLOR_BGR2HSV);

	Mat imgThreshold1, imgThreshold2, imgThreshold;
	inRange(hsv_image,
		Scalar(0, 0, 0),
		Scalar(48, 207, 74),
		imgThreshold1);

	inRange(hsv_image,
		Scalar(140, 0, 0),
		Scalar(180, 207, 114),
		imgThreshold2);

	imgThreshold = max(imgThreshold1, imgThreshold2); // combining the two thresholds

	Mat element_erode = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element_dilate = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	/// Apply the erosion and dilation operations
	erode(imgThreshold, imgThreshold, element_erode);
	dilate(imgThreshold, imgThreshold, element_dilate);

	GaussianBlur(imgThreshold, imgThreshold, Size(9, 9), 2, 2);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(imgThreshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(src, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
	}

	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", imgThreshold);
	imshow("Final result", src);

	waitKey(0);

	return 0;
}