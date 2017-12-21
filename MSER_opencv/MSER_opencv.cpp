// VasanthKumarB/MSER_opencv.cpp - https://gist.github.com/VasanthKumarB/588944f47e6a1664b43e

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	Mat img = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1); // reads an image in grayscale ... wasa argv[1]

	Ptr<MSER> ms = MSER::create();
	vector<vector<Point> > regions;
	vector<cv::Rect> mser_bbox;
	ms->detectRegions(img, regions, mser_bbox); // mser_box also

	for (int i = 0; i < regions.size(); i++)
	{
		rectangle(img, mser_bbox[i], CV_RGB(0, 255, 0)); // this is why I am getting lame rectangles.
	}

	imshow("mser", img);
	waitKey(0);
	return 0;
}