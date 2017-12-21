// SCENE TEXT SEGMENTATION BASED ON THRESHOLDING - http://upcommons.upc.edu/bitstream/handle/2117/78023/BachelorThesis_AlejandroPerez.pdf?sequence=1

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <dirent.h> // https://github.com/tronkko/dirent - maybe necessary?

using namespace cv;
using namespace std;
#define CONTOUR_AREA 100
#define CONTOUR_AREA_MAX 50000
bool process_image(std::string name, std::string src_folder, std::string
	dst_folder) {

	// Read image and transform to grayscale
	cv::Mat image = cv::imread(src_folder + name, CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty()) return false;
	image.convertTo(image, CV_8U);

	// Blur the image
	Mat blurred;
	GaussianBlur(image, blurred, Size(3, 3), 1.5);

	// Threshold
	Mat otsu_threshold;
	cv::threshold(blurred, otsu_threshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// find contours
	vector<vector<Point>> contours; vector<Vec4i> hierarchy;
	cv::findContours(otsu_threshold, contours, hierarchy, CV_RETR_CCOMP,
		CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(otsu_threshold.size(), CV_8UC3);
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	/*
	// Find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(otsu_threshold, contours, hierarchy, CV_RETR_CCOMP,
		CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(otsu_threshold.size(), CV_8UC3);
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	*/

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
	{
		// calculate parameters for filter the letters
		approxPolyDP(Mat(contours[idx]), contours_poly[idx], 3, true);
		boundRect[idx] = boundingRect(Mat(contours_poly[idx]));
		float occupyrate;
		occupyrate = (contourArea(contours[idx]) / (boundRect[idx].width *
			boundRect[idx].height));
		float aspectratio;
		aspectratio = max(boundRect[idx].height, boundRect[idx].width) /
			min(boundRect[idx].height, boundRect[idx].width);
		float perimeter;
		perimeter = arcLength(contours[idx], true);
		float compactness;
		compactness = contourArea(contours[idx]) / (perimeter * perimeter);
		// filter contours by region areas and parameters and draw
		RNG rng(12345);
		{
			if ((contourArea(contours[idx]) > CONTOUR_AREA) &
				(contourArea(contours[idx]) <= CONTOUR_AREA_MAX))
			{
				if ((occupyrate >= 0.0025) & (occupyrate <= 0.95)){
					if (aspectratio <= 15){
						if (compactness > 0.005 & compactness <=
							0.95){
							Scalar color(rand() & 255, rand() & 255,
								rand() & 255);
							drawContours(drawing, contours, idx,
								color, CV_FILLED, 8, hierarchy);
						}
					}
				}
			}
		}
	}
	cv::imwrite(dst_folder + name, drawing);
	return true;
}