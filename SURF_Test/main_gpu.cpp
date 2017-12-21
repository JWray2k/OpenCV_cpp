#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\gpu\gpu.hpp>
#include <iostream>


using namespace cv;
using namespace std;
using namespace cv::gpu;

int main() {

	//vector of keypoints	
	vector<cv::KeyPoint> keypointsO;
	vector<cv::KeyPoint> keypointsS;

	//read an image
	Mat objectP = imread("hdd_sm.jpg");
	Mat sceneP = imread("scene.jpg");
	Mat outImg;

	gpu::GpuMat dst, src, src2;

	if (objectP.empty())
	{
		cout << "Could not load the image!" << endl;
	}
	else
	{
		cout << "Image loaded succesfully" << endl;
	}

	GpuMat keypoints1, keypoints2, desc1, desc2;
	//Start the timer
	double duration;
	duration = static_cast<double>(cv::getTickCount());

	src.upload(objectP);
	src2.upload(sceneP);
	SURF_GPU surfGPU;
	surfGPU(src, GpuMat(), keypoints1, desc1);
	surfGPU(src2, GpuMat(), keypoints2, desc2);

	BruteForceMatcher_GPU< L2<float> > matcher;
	GpuMat trainIdx, distance;

	matcher.matchSingle(desc1, desc2, trainIdx, distance);

	vector< DMatch > matches;
	vector<float> descriptors_object, descriptors_scene;

	surfGPU.downloadKeypoints(keypoints1, keypointsO);
	surfGPU.downloadKeypoints(keypoints2, keypointsS);
	surfGPU.downloadDescriptors(desc1, descriptors_object);
	surfGPU.downloadDescriptors(desc2, descriptors_scene);
	BruteForceMatcher_GPU< L2<float> >::matchDownload(trainIdx, distance, matches);

	/*SurfFeatureDetector surf(2500);
	surf.detect(objectP,keypointsO);
	surf.detect(sceneP,keypointsS);


	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute( objectP, keypointsO, descriptors_object );
	extractor.compute( sceneP, keypointsS, descriptors_scene );

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );


	//reduces the count of matches to 25
	nth_element(matches.begin(),matches.begin()+24,matches.end());
	matches.erase(matches.begin()+25,matches.end());*/

	drawMatches(objectP, keypointsO, sceneP, keypointsS, matches, outImg, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);



	/*drawKeypoints(objectP,keypointsO,objectP,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("SURF");
	imshow("SURF",objectP);*/

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypointsO[matches[i].queryIdx].pt);
		scene.push_back(keypointsS[matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(objectP.cols, 0);
	obj_corners[2] = cvPoint(objectP.cols, objectP.rows); obj_corners[3] = cvPoint(0, objectP.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(outImg, scene_corners[0] + Point2f(objectP.cols, 0), scene_corners[1] + Point2f(objectP.cols, 0), Scalar(0, 255, 0), 4);
	line(outImg, scene_corners[1] + Point2f(objectP.cols, 0), scene_corners[2] + Point2f(objectP.cols, 0), Scalar(0, 255, 0), 4);
	line(outImg, scene_corners[2] + Point2f(objectP.cols, 0), scene_corners[3] + Point2f(objectP.cols, 0), Scalar(0, 255, 0), 4);
	line(outImg, scene_corners[3] + Point2f(objectP.cols, 0), scene_corners[0] + Point2f(objectP.cols, 0), Scalar(0, 255, 0), 4);


	duration = static_cast<double>(cv::getTickCount());
	duration /= cv::getTickFrequency();

	namedWindow("Match");
	imshow("Match", outImg);
	cout << "Macthes found: " << matches.size() << endl;
	cout << "Algorithm duration: " << duration << endl;

	waitKey(0);

	return 1;
}