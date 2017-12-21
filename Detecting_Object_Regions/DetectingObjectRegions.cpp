#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(){ // this doesn't run

	const int channels[] = { 0, 1, 2 };
	const int histSize[] = { 32, 32, 32 };
	const float rgbRange[] = { 0, 256 };
	const float* ranges[] = { rgbRange, rgbRange, rgbRange };

	Mat hist;
	Mat im32fc3, backpr32f, backpr8u, backprBw, kernel;

	Mat im = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");

	im.convertTo(im32fc3, CV_32FC3);
	calcHist(&im32fc3, 1, channels, Mat(), hist, 3, histSize, ranges, true, false);
	calcBackProject(&im32fc3, 1, channels, hist, backpr32f, ranges);

	double minval, maxval;
	minMaxIdx(backpr32f, &minval, &maxval);
	threshold(backpr32f, backpr32f, maxval / 32, 255, THRESH_TOZERO);
	backpr32f.convertTo(backpr8u, CV_8U, 255.0 / maxval);
	threshold(backpr8u, backprBw, 10, 255, THRESH_BINARY);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

	dilate(backprBw, backprBw, kernel);
	morphologyEx(backprBw, backprBw, MORPH_CLOSE, kernel, Point(-1, -1), 2);

	backprBw = 255 - backprBw;

	morphologyEx(backprBw, backprBw, MORPH_OPEN, kernel, Point(-1, -1), 2);
	erode(backprBw, backprBw, kernel);

	Mat mask(backpr8u.rows, backpr8u.cols, CV_8U);

	mask.setTo(GC_PR_BGD);
	mask.setTo(GC_PR_FGD, backprBw);

	Mat bgdModel, fgdModel;
	grabCut(im, mask, Rect(), bgdModel, fgdModel, GC_INIT_WITH_MASK);

	Mat fg = mask == GC_PR_FGD;

	imshow("fg", fg);

	waitKey(0); // Wait for a keystroke in the window
	return 0;


	// returns the green color space... pretty well also
}