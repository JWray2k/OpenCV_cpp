// OpenCV Basics - 05 - Split and Merge - https://www.youtube.com/watch?v=VjYtoL0wZMc&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP&index=5

#include "opencv2\opencv.hpp"
#include <stdint.h>


using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
	// Mat original = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", CV_LOAD_IMAGE_COLOR);
	Mat original = imread("C:/Users/wray/Pictures/times-square.jpg", CV_LOAD_IMAGE_COLOR);

	Mat splitChannels[3]; // An array of three channels... one for each color channel

	imshow("B", splitChannels[0]); // blue is always first
	imshow("G", splitChannels[1]);
	imshow("R", splitChannels[2]);

	splitChannels[2] = Mat::zeros(splitChannels[2].size(), CV_8UC1); // remove the red channel... it was the index [2] in the array
	// U symbolizes unsigned, C1 symbolizes 1 channel

	// Mat::zeros() is a conveinient way to create a matrix of nothing but zeros.

	Mat output;

	merge(splitChannels, 3, output);
	imshow("Merged", output);
	waitKey();

	//failing... Assertion failed <size.width>0 ub cv::imshow
}