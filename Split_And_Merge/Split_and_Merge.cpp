#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

// Split and Merge Tutorial - http://opencvexamples.blogspot.com/2013/10/split-and-merge-functions.html

/*
Split and Merge functions

- void split(InputArray m, OutputArrayOfArrays mv)
		- Divides a multi-channel array into several single-channel arrays.

- Parameters:
	- src – input multi-channel array.
	- mv – output array or vector of arrays; in the first variant of the function the number of arrays must match src.channels(); 
	  the arrays themselves are reallocated, if needed.

- void merge(InputArrayOfArrays mv, OutputArray dst)
	- Creates one multichannel array out of several single-channel ones.

- Parameters:
	- mv – input array or vector of matrices to be merged; all the matrices in mv must have the same size and the same depth.
	- dst – output array of the same size and the same depth as mv[0]; The number of channels will be the total number of channels in the matrix array.

*/

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1);
	namedWindow("src", 1); imshow("src", src);

	// Split the image into different channels
	vector<Mat> rgbChannels(3);
	split(src, rgbChannels);

	// Show individual channels
	Mat g, fin_img;
	g = Mat::zeros(Size(src.cols, src.rows), CV_8UC1);

	// Showing Red Channel
	// G and B channels are kept as zero matrix for visual perception
	{
		vector<Mat> channels;
		channels.push_back(g);
		channels.push_back(g);
		channels.push_back(rgbChannels[2]);

		/// Merge the three channels
		merge(channels, fin_img);
		namedWindow("R", 1); imshow("R", fin_img);
	}

	// Showing Green Channel
	{
		vector<Mat> channels;
		channels.push_back(g);
		channels.push_back(rgbChannels[1]);
		channels.push_back(g);
		merge(channels, fin_img);
		namedWindow("G", 1); imshow("G", fin_img);
	}

	// Showing Blue Channel
	{
		vector<Mat> channels;
		channels.push_back(rgbChannels[0]);
		channels.push_back(g);
		channels.push_back(g);
		merge(channels, fin_img);
		namedWindow("B", 1); imshow("B", fin_img);
	}

	waitKey(0);
	return 0;
}
