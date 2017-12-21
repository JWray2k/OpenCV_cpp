#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
/// #include <opencv2\nonfree\features2d.hpp> //pre-3.0
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <iostream>
#include "opencv2/xfeatures2d.hpp"
#include <list>
#include <vector>

// from https://morf.lv/complete-guide-using-surf-feature-detector
// review source in download directory...

using namespace cv;
using namespace std;

//read an image	
Mat sceneP = imread("hdd_rotated.jpg");
Mat outImg;

void findObject(Mat objectP, int minHessian, Scalar color);

int main() {

	Mat find = imread("hdd.jpg");
	Mat label = imread("label1.jpg");
	Mat label2 = imread("label2.jpg");
	outImg = sceneP;


	if (find.empty() && sceneP.empty())
	{
		cout << "Could not load the image!" << endl;
		exit(0);
	}
	else
	{
		cout << "Images loaded succesfully" << endl;
	}

	findObject(label, 2500, Scalar(255, 0, 0));
	findObject(label2, 1500, Scalar(0, 255, 0));
	findObject(find, 1500, Scalar(0, 0, 255));

	namedWindow("Match");
	imshow("Match", outImg);


	waitKey(0);

	return 1;
}

void findObject(Mat objectP, int minHessian, Scalar color)
{
	//vector of keypoints	
	vector<cv::KeyPoint> keypointsO;
	vector<cv::KeyPoint> keypointsS;
	cout << "Looking for object...\n";
	//Start the timer
	double duration;
	duration = static_cast<double>(cv::getTickCount());



	SurfFeatureDetector surf(minHessian);
	surf.detect(objectP, keypointsO);
	surf.detect(sceneP, keypointsS);


	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute(objectP, keypointsO, descriptors_object);
	extractor.compute(sceneP, keypointsS, descriptors_scene);



	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	//BFMatcher matcher(NORM_L1);
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 150;
	double dist;

	//Quick calculation of min and max distances between keypoints
	for (int i = 0; i<descriptors_object.rows; i++)
	{
		dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	/* cout << "-- Max Dist: " << max_dist << endl;
	cout << "-- Min Dist: " << min_dist << endl;*/

	vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
			good_matches.push_back(matches[i]);
	}

	//drawMatches(objectP,keypointsO,sceneP,keypointsS,matches,outImg,Scalar::all(-1), Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/*drawKeypoints(objectP,keypointsO,objectP,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("SURF");
	imshow("SURF",objectP);*/

	//-- Localize the object

	if (good_matches.size() >= 8 && good_matches.size() <= 30)
	{
		cout << "OBJECT FOUND!" << endl;
		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		for (unsigned int i = 0; i < good_matches.size(); i++)
		{
			//-- Get the keypoints from the good matches
			obj.push_back(keypointsO[good_matches[i].queryIdx].pt);
			scene.push_back(keypointsS[good_matches[i].trainIdx].pt);
		}

		Mat H = findHomography(obj, scene, CV_RANSAC);



		//-- Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> obj_corners(4);
		obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(objectP.cols, 0);
		obj_corners[2] = cvPoint(objectP.cols, objectP.rows); obj_corners[3] = cvPoint(0, objectP.rows);
		std::vector<Point2f> scene_corners(4);

		perspectiveTransform(obj_corners, scene_corners, H);

		//-- Draw lines between the corners (the mapped object in the scene - image_2 )

		line(outImg, scene_corners[0], scene_corners[1], Scalar(255, 255, 0), 2); //TOP line
		line(outImg, scene_corners[1], scene_corners[2], color, 2);
		line(outImg, scene_corners[2], scene_corners[3], color, 2);
		line(outImg, scene_corners[3], scene_corners[0], color, 2);
	}
	else cout << "OBJECT NOT FOUND!" << endl;

	duration = static_cast<double>(cv::getTickCount()) - duration;
	duration = (duration / cv::getTickFrequency()) * 1000;



	//cout <<  << endl;
	cout << "Good matches found: " << good_matches.size() << endl;
	cout << "Algorithm duration: " << duration << endl << "--------------------------------------" << endl;


	// drawing the results
	namedWindow("matches");
	Mat img_matches;
	drawMatches(objectP, keypointsO, sceneP, keypointsS, good_matches, img_matches);
	imshow("matches", img_matches);
	waitKey(100);

}