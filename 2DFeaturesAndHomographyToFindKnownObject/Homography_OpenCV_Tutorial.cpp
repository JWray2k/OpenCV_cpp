/*
In this tutorial you will learn how to:

	- Use the function findHomography to find the transform between matched keypoints.
	- Use the function perspectiveTransform to map the points.
*/
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

#include <iostream>
#include <string>
#include <vector>

// Updated Version from https://gist.github.com/derofim/d3539ba2c4e1a7fb113bc9ec05348e0b

using namespace cv;
using namespace cv::xfeatures2d;

/** @function main */
int main(int argc, char** argv)
{
	std::string fileImgA = "C:/Users/wray/Pictures/box.png";
	std::string fileImgB = "C:/Users/wray/Pictures/box_in_scene.png";

	Mat img_object = cv::imread(fileImgA, IMREAD_GRAYSCALE);
	Mat img_scene = imread(fileImgB, IMREAD_GRAYSCALE);

	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}


	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	// Use Ptr as in https://github.com/kyamagu/mexopencv/issues/154
	Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector->detect(img_object, keypoints_object);
	detector->detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	// Based on http://docs.opencv.org/3.2.0/d9/d97/tutorial_table_of_content_features2d.html
	Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();

	Mat descriptors_object, descriptors_scene;

	extractor->compute(img_object, keypoints_object, descriptors_object);
	extractor->compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img_matches);
	// scale image to fit on screen
	resizeWindow("image", 800, 600);

	waitKey(0);
	return 0;
}

/*

Description

Features2D + Homography to find a known object http://docs.opencv.org/3.2.0/d9/d97/tutorial_table_of_content_features2d.html ported to OpenCV 3.2.0

Setup

git clone -b 3.2.0 --depth 1 --recursive https://github.com/opencv/opencv_contrib.git
git clone -b 3.2.0 --depth 1 --recursive https://github.com/opencv/opencv.git
Create bat script (Windows):
@echo off
REM git clone -b 3.2.0 --depth 1 --recursive https://github.com/opencv/opencv_contrib.git
REM OR Download https://github.com/opencv/opencv_contrib/archive/3.2.0.zip
REM
REM git clone -b 3.2.0 --depth 1 --recursive https://github.com/opencv/opencv.git
REM OR Download https://github.com/opencv/opencv/archive/3.2.0.zip

REM # Create build directory
call cmake -E make_directory "build"

REM # Create build generator files
call cmake -E chdir "build" cmake -E time cmake --clean "D:\OpenCV\opencv-3\opencv" -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DOPENCV_EXTRA_MODULES_PATH="D:\OpenCV\opencv-3\opencv_contrib\modules" -DWITH_CUDA=OFF

REM # Build project
call cmake -E chdir "build" cmake -E time cmake --build . --config Release
You can change -DWITH_CUDA=OFF to -DWITH_CUDA=ON You can change path "D:\OpenCV\opencv-3\opencv" to yours You can change path "D:\OpenCV\opencv-3\opencv_contrib\modules" to yours (note "modules" in path)

*/