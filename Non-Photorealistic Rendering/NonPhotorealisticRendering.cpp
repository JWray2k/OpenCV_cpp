/*
npr_demo.cpp

Author:
Siddharth Kerada <siddharthkherada27@gmail.com>

This tutorial demonstrates how to use OpenCV Non-Photoealistic Renduring Module.
1) Edge Preserve Smoothing
	-> Using Normalized convolution Filter
	-> Using Recursive Filter
2) Detail Enhancement
3) Pencil sketch/Colo Pencil Drawing
4) Stylization

referenced from https://www.youtube.com/watch?v=CsfhQcAFt6U

// This works very nicely... expecially the pencil and stylization
*/

#include <signal.h> // Review what this doses
#include "opencv2\photo.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	cv::CommandLineParser parser(argc, argv, "{help h||show help message}{@imgage||input image}");
	if (parser.has("help"))
	{
		parser.printMessage();
		exit(0);
	}
	/* Causes the program to break without having the imread(parser.get<string>("@image"));
	if (parser.get<string>("@image").empty())
	{
		parser.printMessage();
		exit(0);
	}
	*/

	// Mat I = imread(parser.get<string>("@image"));
	Mat I = imread("C:/Users/wray/Pictures/church03.jpg");

	int num, type;

	if (I.empty())
	{
		cout << "Image not found" << endl;
		exit(0);
	}

	Mat img;

	while (1){

		cout << endl;
		cout << " Edge Preserve Filter " << endl;
		cout << "----------------------" << endl;

		cout << "Options:" << endl;
		cout << endl;

		cout << "1) Edge Preserve Smoothing" << endl;
		cout << "	-> Using Normalized convolution Filter" << endl;
		cout << "	-> Using Recursive Filter" << endl;
		cout << "2) Detail Enhancement" << endl;
		cout << "3) Pencil sketch/Color Pencil Drawing" << endl;
		cout << "4) Stylization" << endl;
		cout << "5) Quit" << endl;
		cout << endl;

		cout << "Press number 1-5 to choose from above techniques: ";

		cin >> num;

		if (num == 1)
		{
			cout << endl;
			cout << "Press 1 for Normalized Convolution Filter and 2 for Recursive Filter: ";

			cin >> type;

			edgePreservingFilter(I, img, type); // Review what this does
			imshow("Detail Enhanced", img);

		}
		else if (num == 2)
		{
			detailEnhance(I, img); // Review what this does
			imshow("Detail Enhanced", img);
		}
		else if (num == 3)
		{
			Mat img1;
			pencilSketch(I, img1, img, 10, 0.1f, 0.03f); // Review what this does
			imshow("Pencil Sketch", img1);
			imshow("Color Pencil Sketch", img);
		}
		else if (num == 4)
		{
			stylization(I, img); // Review what this does
			imshow("Stylization", img);
		}
		else if (num == 5)
		{
			break; // I wonder how many console applications have a break for the quit option... that's pretty nice
		}
		else
		{
			puts("\nInvalid choice!!!");
		}
		waitKey(0);
	}
	return 0;
}