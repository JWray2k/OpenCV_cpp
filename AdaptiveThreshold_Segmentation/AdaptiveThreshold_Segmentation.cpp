// SCENE TEXT SEGMENTATION BASED ON THRESHOLDING - http://upcommons.upc.edu/bitstream/handle/2117/78023/BachelorThesis_AlejandroPerez.pdf?sequence=1
#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning 

// Scene Segmentation from this doesn't give that great of results...

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "dirent.h" // what is this?

using namespace cv;
using namespace std;
#define CONTOUR_AREA 75
#define CONTOUR_AREA_MAX 25000

void pause()   {
	system("pause"); // Beautiful implementation that doesn't need openCV's waitkey funcion - https://stackoverflow.com/questions/193469/how-to-make-visual-studio-pause-after-executing-a-console-app-in-debug-mode 
}

bool process_image(std::string name, std::string src_folder, std::string
	dst_folder) {
	// Read image and transform to grayscale
	cv::Mat image = cv::imread(src_folder + name, CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty()) return false;
	image.convertTo(image, CV_8U);
	// Blur the image
	Mat blurred;
	GaussianBlur(image, blurred, Size(3, 3), 1.5);
	// Filter opening
	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat open_result;
	cv::morphologyEx(blurred, open_result,
		cv::MORPH_OPEN, structure_elem);
	// Threshold
	cv::Mat image_threshold;
	cv::adaptiveThreshold(open_result, image_threshold, 255,
		CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 51, 10);
	// Filter closing
	cv::Mat const structure_elem2 = cv::getStructuringElement(
		cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::Mat close_result;
	cv::morphologyEx(image_threshold, close_result,
		cv::MORPH_CLOSE, structure_elem2);
	// Find contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(close_result, contours, hierarchy, CV_RETR_CCOMP,
		CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing = Mat::zeros(close_result.size(), CV_8UC3);
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

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
				if ((occupyrate >= 0.03) & (occupyrate <= 0.95)){
					if (aspectratio <= 6){
						if (compactness > 0.003 & compactness <= 0.95){
							Scalar color(rand() & 255, rand() & 255, rand()
								& 255);
							drawContours(drawing, contours, idx, color,
								CV_FILLED, 8, hierarchy);
						}
					}
				}
			}
		}
	}
	cv::imwrite(dst_folder + name, drawing);
	return true;
}

int wmain(int argc, wchar_t* argv[])
{
	// Read images and save results in folders
	DIR *directory;
	struct dirent *entry;
	std::vector<std::string> names;
	// Original:
	// std::string src = "C:/icdar/icdar2013/";
	// std::string dst = "C:/Final/Adaptativethreshold/";
	std::string src = "C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/"; // DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png
	std::string dst = "C:/Users/wray/Pictures/AdaptiveThreshold/"; // should deposit the new picture in my picture library
	if ((directory = opendir(src.c_str())) == NULL) {
		std::cerr << "Could not open directory" << std::endl;
		atexit(pause);
		return 1;
	}
	while ((entry = readdir(directory)) != NULL) {
		std::string name = entry->d_name;
		if (name != "." && name !=
			"..") names.push_back(name);
		atexit(pause);
	}
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++) {
		if (!process_image(*it, src, dst)) std::cout << "Problems processing image: " <<
			*it << std::endl;
		atexit(pause);
	}
	return 0;
}