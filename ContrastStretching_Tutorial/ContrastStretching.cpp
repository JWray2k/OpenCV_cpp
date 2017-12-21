/*
Contrast Stretching using C++ and OpenCV: Image Processing
http://www.programming-techniques.com/2013/01/contrast-stretching-using-c-and-opencv.html

Theory

Contrast Stretching is one of the piecewise linear function. Contrast Stretching increases 
the dynamic range of the grey level in the image being processed.


Points (r1, s1) and (r2, s2) control the shape of the transformation. The selection of control 
points depends upon the types of image and varies from one image to another image. If r1 = s1 
and r2 = s2 then the transformation is linear and this doesn't affect the image. In other case 
we can calculate the intensity of output pixel, provided intensity of input pixel is x, as follows

for 0 <= x <= r1
	output = s1 / r1 * x
for r1 < x <= r2
	output = ((s2 - s1)/(r2 - r1))*(x - r1) + s1
for r2 < x <= L - 1
	output = ((L-1 - s2)/(L-1 - r2))*(x - r2) + s2

*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int computeOutput(int, int, int, int, int);

int main()
{
	Mat image = imread("C:/Users/wray/Pictures/faded.jpg");
	Mat new_image = image.clone();

	int r1, s1, r2, s2;
	cout << "Enter r1: " << endl; cin >> r1;
	cout << "Enter s1: " << endl; cin >> s1;
	cout << "Enter r2: " << endl; cin >> r2;
	cout << "Enter s2: " << endl; cin >> s2;

	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			for (int c = 0; c < 3; c++){
				int output = computeOutput(image.at<Vec3b>(y, x)[c], r1, s1, r2, s2);
				new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(output);
			}
		}
	}

	namedWindow("Original Image", 1);
	imshow("Original Image", image);

	namedWindow("New Image", 1);
	imshow("New Image", new_image);

	waitKey();

	return 0;
}

int computeOutput(int x, int r1, int s1, int r2, int s2)
{
	float result;
	if (0 <= x && x <= r1){
		result = s1 / r1 * x;
	}
	else if (r1 < x && x <= r2){
		result = ((s2 - s1) / (r2 - r1)) * (x - r1) + s1;
	}
	else if (r2 < x && x <= 255){
		result = ((255 - s2) / (255 - r2)) * (x - r2) + s2;
	}
	return (int)result;
}

// Output
// r1 = 70 s1 = 0 r2 = 140 s2 = 255 - this worked for the original image tested.... 
// but I would need to use something like GIMP to find the pixels I want to manage-