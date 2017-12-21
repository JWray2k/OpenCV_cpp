// Demhist - http://docs.opencv.org/3.2.0/d2/dcb/demhist_8cpp-example.html
// Really great example, but it converts the image to grayscale

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
int _brightness = 100;
int _contrast = 100;
Mat image;
/* brightness/contrast callback function */
static void updateBrightnessContrast(int /*arg*/, void*)
{
	int histSize = 64;
	int brightness = _brightness - 100;
	int contrast = _contrast - 100;
	/*
	* The algorithm is by Werner D. Streidt
	* (http://visca.com/ffactory/archives/5-99/msg00021.html)
	*/
	double a, b;
	if (contrast > 0)
	{
		double delta = 127.*contrast / 100;
		a = 255. / (255. - delta * 2);
		b = a*(brightness - delta);
	}
	else
	{
		double delta = -128.*contrast / 100;
		a = (256. - delta * 2) / 255.;
		b = a*brightness + delta;
	}
	Mat dst, hist;
	image.convertTo(dst, CV_8U, a, b);
	imshow("image", dst);
	calcHist(&dst, 1, 0, Mat(), hist, 1, &histSize, 0);
	Mat histImage = Mat::ones(200, 320, CV_8U) * 255;
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, CV_32F);
	histImage = Scalar::all(255);
	int binW = cvRound((double)histImage.cols / histSize);
	for (int i = 0; i < histSize; i++)
		rectangle(histImage, Point(i*binW, histImage.rows),
		Point((i + 1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
		Scalar::all(0), -1, 8, 0);
	imshow("histogram", histImage);
}
static void help()
{
	std::cout << "\nThis program demonstrates the use of calcHist() -- histogram creation.\n"
		<< "Usage: \n" << "demhist [image_name -- Defaults to ../data/baboon.jpg]" << std::endl;
}
const char* keys =
{
	"{help h||}{@image|../data/baboon.jpg|input image file}"
};
int main(int argc, const char** argv)
{
	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help"))
	{
		help();
		return 0;
	}
	string inputImage = parser.get<string>(0);
	// Load the source image. HighGUI use.
	image = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 0); //read inputImage before
	if (image.empty())
	{
		std::cerr << "Cannot read image file: " << inputImage << std::endl;
		return -1;
	}
	namedWindow("image", 0);
	namedWindow("histogram", 0);
	createTrackbar("brightness", "image", &_brightness, 200, updateBrightnessContrast);
	createTrackbar("contrast", "image", &_contrast, 200, updateBrightnessContrast);
	updateBrightnessContrast(0, 0);
	waitKey();
	return 0;
}





/*

// #include <cv.h>
#include <opencv/cv.h>
// #include <highgui.h>
#include <opencv/highgui.h>

// the above two where missing imread, calcHist... ect...
// CalcHist - http://docs.opencv.org/2.4/modules/imgproc/doc/histograms.html?highlight=calchist#calchist

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;

int main(int argc, char** argv)
{
	Mat src, hsv;
	// if (argc != 2 || !(src = imread(argv[1], 1)).data)
	if (argc != 2 || !(src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1)).data) //doesn't work..
		return -1;

	cvtColor(src, hsv, CV_BGR2HSV);

	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };

	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins * 10, CV_8UC3);

	for (int h = 0; h < hbins; h++)
	for (int s = 0; s < sbins; s++)
	{
		float binVal = hist.at<float>(h, s);
		int intensity = cvRound(binVal * 255 / maxVal);
		rectangle(histImg, Point(h*scale, s*scale),
			Point((h + 1)*scale - 1, (s + 1)*scale - 1),
			Scalar::all(intensity),
			CV_FILLED);
	}

	namedWindow("Source", 1);
	imshow("Source", src);

	namedWindow("H-S Histogram", 1);
	imshow("H-S Histogram", histImg);
	waitKey();
}

*/