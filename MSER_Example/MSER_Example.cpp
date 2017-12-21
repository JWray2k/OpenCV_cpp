#include <iostream>  
#include <opencv2\opencv.hpp>  
#include <opencv2\highgui.hpp>  
#include <opencv2\imgcodecs.hpp>  
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\text.hpp>
#include <opencv2/features2d.hpp>

// 6/12/2015 - OpenCV MSER example (opencv 300 ) - http://study.marearts.com/2015/06/opencv-mser-example-opencv-300.html

// Results seem underwhelming for this one.

#ifdef _DEBUG             
#pragma comment(lib, "opencv_core330d.lib")     
#pragma comment(lib, "opencv_highgui330d.lib")  
#pragma comment(lib, "opencv_imgcodecs330d.lib")
#pragma comment(lib, "opencv_text330d.lib")
#pragma comment(lib, "opencv_features2d330d.lib")
#pragma comment(lib, "opencv_imgproc330d.lib")

#else     
#pragma comment(lib, "opencv_core330.lib")     
#pragma comment(lib, "opencv_highgui330.lib")  
#pragma comment(lib, "opencv_imgcodecs330.lib")  
#pragma comment(lib, "opencv_text330.lib")
#pragma comment(lib, "opencv_features2d330.lib")
#pragma comment(lib, "opencv_imgproc330d.lib")
#endif      

using namespace std;
using namespace cv;
void main()
{

	Mat inImg = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");

	Mat textImg;
	cvtColor(inImg, textImg, CV_BGR2GRAY);
	//Extract MSER

	vector< vector< Point> > contours;
	vector< Rect> bboxes;
	Ptr< MSER> mser = MSER::create(21, (int)(0.00002*textImg.cols*textImg.rows), (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);
	mser->detectRegions(textImg, contours, bboxes);

	for (int i = 0; i < bboxes.size(); i++)
	{
		rectangle(inImg, bboxes[i], CV_RGB(0, 255, 0));
	}


	namedWindow("t");
	imshow("t", inImg);
	waitKey(0);

}