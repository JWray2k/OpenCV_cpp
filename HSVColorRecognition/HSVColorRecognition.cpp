#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

/*
HSV image in openCV for color recognition
https://stackoverflow.com/questions/35522599/hsv-image-in-opencv-for-color-recognition
I converted it to HSV (I know imshow() doesn't understand the HSV and will print it as BGR)

Now, I want to get the hue value and know in which range it lies to recognize the color

how to calculate the number or value of hue?
how to specify ranges? using scalar method gives me bgr ranges

*/

int main()
{
	Mat image;
	image = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", CV_LOAD_IMAGE_COLOR);

	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Create a new matrix to hold the HSV image
	Mat HSV;

	// convert RGB image to HSV
	cvtColor(image, HSV, CV_BGR2HSV);

	namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", image);

	namedWindow("Result window", CV_WINDOW_AUTOSIZE);
	imshow("Result window", HSV);


	vector<Mat> hsv_planes;
	split(HSV, hsv_planes);
	Mat h = hsv_planes[0]; // H channel
	Mat s = hsv_planes[1]; // S channel
	Mat v = hsv_planes[2]; // V channel

	namedWindow("hue", CV_WINDOW_AUTOSIZE);
	imshow("hue", h);
	namedWindow("saturation", CV_WINDOW_AUTOSIZE);
	imshow("saturation", s);
	namedWindow("value", CV_WINDOW_AUTOSIZE);
	imshow("value", v);



	//// red color range
	// Scalar hsv_l(170, 150, 150);
	// Scalar hsv_h(180, 255, 255);
	// Mat bw;
	// inRange(HSV, hsv_l, hsv_h, bw);
	// imshow("Specific Colour", bw);
	////

	//// red color rangeS
	int redRange = 10;
	int n = 30;

	Scalar hsv_upper_l(180 - n, 150, 150); // 20 was n
	Scalar hsv_upper_h(180, 255, 255);
	Mat red_hue_upper;
	inRange(HSV, hsv_upper_l, hsv_upper_h, red_hue_upper);

	Scalar hsv_lower_l(0, 150, 150);
	Scalar hsv_lower_h(0 + n, 255, 255); // 20 was n
	Mat red_hue_lower;
	inRange(HSV, hsv_lower_l, hsv_lower_h, red_hue_lower);

	cv::Mat whole_red_hue = red_hue_lower | red_hue_upper;

	imshow("Specific Colour", whole_red_hue);


	//define ranges - For different colors it is easier (since they aren't placed around circle's start/end):
	int hueCenter = 15; // any hue value of the color ...... this was just "..."
	int hueRange = 10; // or any other range
	cv::Mat hueMask;
	inRange(HSV, Scalar(hueCenter - hueRange, 150, 150), Scalar(hueCenter + hueRange, 255, 255), hueMask);

	// if you want to mask-out more of the white/black areas you'll have to increase the lower saturation range bound to 200 and the lower value range bound to 230

	waitKey(0);
	return 0;
}