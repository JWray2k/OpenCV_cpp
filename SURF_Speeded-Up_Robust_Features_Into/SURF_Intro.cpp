/*
Introduction to SURF (Speeded-Up Robust Features)

Goal

In this chapter,
	We will see the basics of SURF
	We will see SURF functionalities in OpenCV

Blarg... Python Tutorial - http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_feature2d/py_surf_intro/py_surf_intro.html 
*/
#include <opencv2\opencv.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <list>
#include <vector>

using namespace cv;

// OpenCV - Object matching using SURF descriptors and BruteForceMatcher
// https://stackoverflow.com/questions/7296915/opencv-object-matching-using-surf-descriptors-and-bruteforcematcher

int main(){
	Mat image1, outImg1, image2, outImg2;

	// vector of keypoints
	vector<KeyPoint> keypoints1, keypoints2;

	// Read input images
	image1 = imread("C://Google-Logo.jpg", 0);
	image2 = imread("C://Alex_Eng.jpg", 0);

	SurfFeatureDetector surf(50); //used to be 2500
	// You need to modify your Hessian, 2500 is too much. Try 50. When you use a big Hessian, the result is a lot of keypoints, 
	// resulting some unnecessary. Another information about SURF is that your marker need to be more rich, with more details.
	surf.detect(image1, keypoints1);
	surf.detect(image2, keypoints2);
	drawKeypoints(image1, keypoints1, outImg1, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(image2, keypoints2, outImg2, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("SURF detector img1");
	imshow("SURF detector img1", outImg1);

	namedWindow("SURF detector img2");
	imshow("SURF detector img2", outImg2);

	SurfDescriptorExtractor surfDesc;
	Mat descriptors1, descriptors2;
	surfDesc.compute(image1, keypoints1, descriptors1);
	surfDesc.compute(image2, keypoints2, descriptors2);

	/*

	The problem was in using Brute Force Matcher only, I found methods to obtain a set of good matches between two views at 
	"OpenCV 2 Computer Vision Application Programming Cookbook"

	Ch9: Matching images using random sample consensus

	They are using K-Nearest Neighbor and RANSAC

	And thanks

	==========================================================================================================================
	
	For removing outliers RANSAC + homography is a good method when comparing two planar images.

	Homography is the model that RANSAC will try to use to compare points from both images and it will find the best set of points 
	that better fit the projection model of the homography (the transformation from one plane to another).

	cv::findHomography(srcPoints,dstPoints, RANSAC, status);

	The function above will return an array status that has a 1 for indices considered inliers and 0 for indices considered outliers, 
	so you can remove outliers by checking this status array.

	==========================================================================================================================

		Using LMEDS (Calib3d.LMEDS on Android) gives better results to me, I do not know why, in my course project, RANSAC in Matlab 
		gave very nice results. But definetely, removing outliners is a must!
	
	*/


	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptors1, descriptors2, matches);

	nth_element(matches.begin(), matches.begin() + 24, matches.end());
	matches.erase(matches.begin() + 25, matches.end());

	Mat imageMatches;
	drawMatches(image1, keypoints1, image2, keypoints2, matches, imageMatches, Scalar(255, 255, 255));

	namedWindow("Matched");
	imshow("Matched", imageMatches);

	cv::waitKey();
	return 0;
}