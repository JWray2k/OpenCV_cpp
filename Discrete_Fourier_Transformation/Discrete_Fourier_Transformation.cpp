// Video 1 - OpenCV Basics - 06 - Taking the DFT of a Grayscale Image - https://www.youtube.com/watch?v=x7FPKU97fgs&index=6&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP
// Video 2 - OpenCVBasics - 07 - Visualizing the DFT - https://www.youtube.com/watch?v=MyX9UPu4cAA&index=7&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP
// Video 3 - OpenCV Basics - 08 - Recenter the DFT - https://www.youtube.com/watch?v=yKxbhXsOUy4&index=8&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP
// Video 4 - OpenCVBasics - 09 - Inverting the DFT - https://www.youtube.com/watch?v=PRoHcorBNgI&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP 
// Video 5 - OpenCV Basics - 10 - 2D Gaussian - https://www.youtube.com/watch?v=mr2LcT4jhCw&list=PLAp0ZhYvW6XbEveYeefGSuLhaPlFML9gP&t=1 

#include "opencv2\opencv.hpp"
#include <stdint.h>

using namespace cv;
using namespace std;

void takeDFT(Mat& source, Mat& destination)
{
	Mat originalComplex[2] = { source, Mat::zeros(source.size(), CV_32F) };

	Mat dftReady;

	merge(originalComplex, 2, dftReady);

	Mat dftOfOriginal;

	dft(dftReady, dftOfOriginal, DFT_COMPLEX_OUTPUT);

	destination = dftOfOriginal;
}

// Create this to realign the DFT
void recenterDFT(Mat& source) 
{
	// Create cetner points for the image... where we will split the image into quadrants.
	int centerX = source.cols / 2;
	int centerY = source.rows / 2;

	// Define quadrants
	Mat q1(source, Rect(0, 0, centerX, centerY));
	Mat q2(source, Rect(centerX, 0, centerX, centerY)); // Begins by centerX
	Mat q3(source, Rect(0, centerY, centerX, centerY));
	Mat q4(source, Rect(centerX, centerY, centerX, centerY));

	Mat swapMap;

	// Recenters the DFT
	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);

	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);

} // low frequency (high amplitude) information will now be in the center and high frequency (low amplitude) information will be in the corners

void showDFT(Mat& source) // This is just so we have something the look at as human beings...
{
	Mat splitArray[2] = { Mat::zeros(source.size(), CV_32F), Mat::zeros(source.size(), CV_32F) };

	split(source, splitArray);

	Mat dftMagnitude;

	magnitude(splitArray[0], splitArray[1], dftMagnitude);

	dftMagnitude += Scalar::all(1); // adds 1 to every single element in that object to make the Log funciton properly

	log(dftMagnitude, dftMagnitude); // take the log of the input and output it with the same variable name

	// Values are still well beyond 0 and 1, so we want to normalize to get that range.
	normalize(dftMagnitude, dftMagnitude, 0, 1, CV_MINMAX); // CV_MINMAX - type of normalization

	recenterDFT(dftMagnitude);

	imshow("DFT", dftMagnitude);
	waitKey(); // goes into a loop until we press a key

}

void invertDFT(Mat& source, Mat& destination) // pass a mat object by reference called source.
{
	Mat inverse;

	// could use idft() - which means inverse dft...
	dft(source, inverse, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE); // able to chain flags together using the bitwise OR operator

	destination = inverse;
}

void createGaussian(Size& size, Mat& output, int uX, int uY, float sigmaX, float sigmaY, float amplitude = 1.0f)
{
	Mat temp = Mat(size, CV_32F);

	for (int r = 0; r < size.height; r++) // rows
	{
		for (int c = 0; c < size.width; c++) // columns
		{
			float x = ((c - uX)*((float)c - uX)) / (2.0f + sigmaX * sigmaX); // square the term - cast a float values to prevent dealing with integers
			float y = ((r - uY)*((float)r - uY)) / (2.0f + sigmaY * sigmaY);
			float value = amplitude * exp(-(x + y)); // pass in a negative exponent to negate the value
			temp.at<float>(r, c) = value;
		}
	}
	// get the range that we visualized in OpenCV
	normalize(temp, temp, 0.0f, 1.0f, NORM_MINMAX);
	output = temp;

}

int main(int argv, char** argc)
{
	Mat output;
	createGaussian(Size(256, 256), output, 256 / 2, 256 / 2, 10, 10); //using 10 as sigma values... leaving amplitude at 1, so don't declare it
	imshow("Gaussian", output);
	waitKey();


	/* DFT main() - 
	Mat original = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", CV_LOAD_IMAGE_GRAYSCALE);

	Mat originalFloat;

	original.convertTo(originalFloat, CV_32FC1, 1.0 / 255.0);

	Mat dftOfOriginal;

	takeDFT(originalFloat, dftOfOriginal);

	showDFT(dftOfOriginal);

	Mat invertedDFT;

	invertDFT(dftOfOriginal, invertedDFT);

	imshow("InvertDFT Result", invertedDFT);
	waitKey();
	*/
}