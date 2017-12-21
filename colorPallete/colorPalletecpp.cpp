// @author Denis Trofimov
// OpenCv hsv color pallete picker
// Used to detect circles of concrete color 
// https://gist.github.com/derofim/7de67a23a7db66092a516edfeee80341

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <tesseract\baseapi.h> // don't have this I don't think
#include <leptonica\allheaders.h> // don't have this I don't think

#include "tinyxml2.h" // don't have this I don't think

#include <Windows.h>

#include <iostream>
#include <string>
#include <set>
#include <chrono>
#include <mutex>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <map>
#include <functional>
#include <algorithm>
#include <random>
#include <iterator>
#include <limits>
#include <cstddef>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// @author Denis Trofimov
// OpenCv hsv color pallete picker
// Used to detect circles of concrete color 

#include <iostream>
#include <cmath>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

VideoCapture setCamera(int camID = 0)
{
	VideoCapture stream1(camID);   //0 is the id of video device.0 if you have only one camera.

	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}
	return stream1;
}

Mat getCameraImage(VideoCapture& stream1)
{
	Mat cameraFrame;
	stream1.read(cameraFrame);
	return cameraFrame;
}

Mat getDiskImage(char * path)
{
	Mat image;

	image = imread(path, IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		exit(-1);
	}
	return image;
}

bool circlesMessageSpammed = false;

Mat detectircles(Mat inImage, bool isImageGray = false)
{
	Mat resultAfterImage;
	inImage.copyTo(resultAfterImage);
	//cv::cvtColor(resultAfterImage, resultAfterImage, cv::COLOR_BGR2HSV);  

	if (!isImageGray) cv::cvtColor(resultAfterImage, resultAfterImage, cv::COLOR_BGR2GRAY);
	//cv::cvtColor(resultAfterImage, resultAfterImage, cv::COLOR_GRAY2BGR);
	std::vector<cv::Vec3f> circles;
	// smooth it, otherwise a lot of false circles may be detected
	GaussianBlur(resultAfterImage, resultAfterImage, Size(9, 9), 2, 2);
	//cv::HoughCircles(resultAfterImage, circles, CV_HOUGH_GRADIENT, 1, resultAfterImage.rows,     100,  20, 0, 0);
	cv::HoughCircles(resultAfterImage, circles, CV_HOUGH_GRADIENT, 2, resultAfterImage.rows / 4, 200, 70, 0, 0);
	cv::cvtColor(resultAfterImage, resultAfterImage, cv::COLOR_GRAY2BGR);
	// Loop over all detected circles and outline them on the original image
	if (circles.size() == 0 && !circlesMessageSpammed) {
		cout << "No circles" << std::endl;
		circlesMessageSpammed = !circlesMessageSpammed;
		//std::exit(-1);
	}

	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// draw the circle center
		circle(resultAfterImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// draw the circle outline
		circle(resultAfterImage, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/*for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
	cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
	int radius = std::round(circles[current_circle][2]);
	cv::circle(resultAfterImage, center, radius, cv::Scalar(0, 255, 0), 5);
	imshow("circles resultAfterImage", resultAfterImage);
	}	*/
	return resultAfterImage;
}

Scalar clr_l_picked(38, 50, 50);
Scalar clr_h_picked(81, 255, 100);

Mat processImage(const Mat original)
{
	cv::Mat result_image;
	Mat image;
	original.copyTo(image);

	cv::GaussianBlur(image, image, cv::Size(19, 19), 2, 2);
	cv::medianBlur(image, image, 3);
	//imshow("blurred", image);

	//morphological opening (remove small objects from the foreground)
	erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (fill small holes in the foreground)
	dilate(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	/*result_image = image; */

	//cv::cvtColor(image, image, cv::COLOR_RGB2HSV);
	cv::cvtColor(image, image, cv::COLOR_BGR2HSV);

	// internally, OpenCV stores a color image in the BGR format rather than RGB
	// OpenCV H has values from 0 to 180, S and V from 0 to 255. 
	// The red color, in OpenCV, has the hue values approximately in the range of 0 to 10 and 160 to 180.

	// color range
	float sensitivity = 29;
	Scalar clr_l(60 - sensitivity, 100 - sensitivity, 50 - sensitivity);
	Scalar clr_h(60 + sensitivity, 255, 255);
	//115 10: 1, 7, 6
	//	299 355 : 200, 248, 0
	// OpenCV uses H: 0 - 180, S: 0 - 255, V: 0 - 255
	Scalar clr_l_Red(38, 0, 0);
	Scalar clr_h_Red(81, 255, 255);

	//# For segmenting the image in RGB format.
	cv::inRange(image, clr_l_picked, clr_h_picked, result_image);

	/*Mat circles_image;
	result_image.copyTo(circles_image);
	std::vector<cv::Vec3f> circles;
	//cv::cvtColor(circles_image, circles_image, cv::COLOR_BGR2GRAY);
	cv::HoughCircles(circles_image, circles, CV_HOUGH_GRADIENT, 1, circles_image.rows, 100, 20, 0, 0);
	cv::cvtColor(circles_image, circles_image, cv::COLOR_GRAY2BGR);
	// Loop over all detected circles and outline them on the original image
	if (circles.size() == 0) {
	cout << "No circles" << std::endl;
	//std::exit(-1);
	}
	for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
	cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
	int radius = std::round(circles[current_circle][2]);
	cv::circle(circles_image, center, radius, cv::Scalar(0, 255, 0), 5);
	}
	imshow("circles", circles_image); */

	auto detectedCircles = detectircles(result_image, true);
	imshow("detectedCircles result_image", detectedCircles);

	return result_image;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	/*if (event == EVENT_LBUTTONDOWN)
	{
	cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
	cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
	cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
	cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}	*/
	if (event == EVENT_LBUTTONDOWN || event == EVENT_RBUTTONDOWN) {
		Mat* rgb = (Mat*)userdata;
		Mat hsv;
		rgb->copyTo(hsv);
		cv::cvtColor(hsv, hsv, COLOR_BGR2HSV);


		if (event == EVENT_LBUTTONDOWN) {
			clr_l_picked[0] = (double)(hsv).at<Vec3b>(y, x)[0];
			clr_l_picked[2] = (double)(hsv).at<Vec3b>(y, x)[2];

			printf("x=%d y=%d: h=%d, s=%d, v=%d\n",
				x, y,
				(int)(hsv).at<Vec3b>(y, x)[0],
				(int)(hsv).at<Vec3b>(y, x)[1],
				(int)(hsv).at<Vec3b>(y, x)[2]);
		}
		if (event == EVENT_RBUTTONDOWN) {
			clr_h_picked[2] = (double)(hsv).at<Vec3b>(y, x)[2];
			/*if (clr_l_picked[0] > clr_h_picked[0]) {
			clr_h_picked[0] = 255;
			clr_l_picked[0] = clr_h_picked[0];
			} */
			printf("x=%d y=%d: r=%d, g=%d, b=%d\n",
				x, y,
				(int)(*rgb).at<Vec3b>(y, x)[0],
				(int)(*rgb).at<Vec3b>(y, x)[1],
				(int)(*rgb).at<Vec3b>(y, x)[2]);
		}
	}
}

void showColorPallete() {
	Mat colorpallete = getDiskImage("color_pallete.jpg");
	imshow("colorpallete", colorpallete);
	cv::setMouseCallback("colorpallete", CallBackFunc, &colorpallete);
}

Mat colorpallete;

int main(int argc, char** argv)
{
	colorpallete = getDiskImage("color_pallete.jpg");
	//VideoCapture cameraStream = setCamera(0);
	imshow("colorpallete", colorpallete);
	while (true) {
		cv::setMouseCallback("colorpallete", CallBackFunc, &colorpallete);
		//Mat image = getCameraImage(cameraStream);

		// lego.jpg shapes_and_colors.jpg color_bulbs.jpg circles.jpg innerCircles.jpg
		// svetofor3.png color_wheel_730.png
		Mat image = getDiskImage("color_bulbs.jpg");
		imshow("original", image);

		colorpallete = getDiskImage("color_pallete.jpg");
		// Draw a rectangle ( 5th argument is not -ve)
		auto x1 = clr_l_picked[0] / 180 * colorpallete.size().width;
		auto x2 = clr_h_picked[0] / 180 * colorpallete.size().width;

		auto h1 = (clr_l_picked[2] / 255)*colorpallete.size().height;
		auto h2 = (clr_h_picked[2] / 255)*colorpallete.size().height;//(((clr_l_picked[2] / 255)) * colorpallete.size().height);
		rectangle(colorpallete, Point(x1, h1), Point(x2, h2), Scalar(0, 55, 255), +1, 4);
		imshow("colorpallete", colorpallete);


		auto detectedCircles = detectircles(image);
		imshow("detectedCircles image", detectedCircles);

		Mat resultImage = processImage(image);
		imshow("resultImage", resultImage);

		Mat resultAfterImage;
		cv::bitwise_and(image, image, resultAfterImage, resultImage);

		imshow("cam", resultAfterImage);

		waitKey(1);
	}

	waitKey(0); // Wait for a keystroke in the window
	cin.get();
	return 0;
}