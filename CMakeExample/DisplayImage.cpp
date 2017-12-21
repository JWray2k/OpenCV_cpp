/*
Using OpenCV with gcc and CMake

- The easiest way of using OpenCV in your code is to use CMake. A few advantages (taken from the Wiki):
	1 - No need to change anything when porting between Linux and Windows
	2 - Can easily be combined with other tools by CMake( i.e. Qt, ITK and VTK )
- If you are not familiar with CMake, checkout the tutorial on its website.
*/

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	Mat image;
	image = imread(argv[1], 1);

	if (!image.data)
	{
		printf("No image data \n");
		return -1;
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	waitKey(0);

	return 0;
}