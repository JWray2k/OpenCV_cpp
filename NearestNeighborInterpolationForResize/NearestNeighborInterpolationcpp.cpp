// Scaäe an image using nearest neighbor interpolation.... http://answers.opencv.org/question/68507/cvresize-incorrect-interpolation-with-inter_nearest/


#include <iostream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("C:/Users/wray/Pictures/Checkered.png");
	Mat outImage;
	resize(image, outImage, Size(256, 256), INTER_NEAREST);

	imshow("orig", image);
	imshow("resized", outImage);

	waitKey(0);

	return 0;
}