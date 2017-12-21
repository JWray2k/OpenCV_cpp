#include <opencv2\xfeatures2d\nonfree.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;

int main(){


	SiftDescriptorExtractor sift;

	vector<KeyPoint> keypoints; // keypoint storage
	Mat descriptors; // descriptor storage

	// manual keypoint grid

	int step = 10; // 10 pixels spacing between kp's

	for (int y = step; y<img.rows - step; y += step){
		for (int x = step; x<img.cols - step; x += step){

			// x,y,radius
			keypoints.push_back(KeyPoint(float(x), float(y), float(step)));
		}
	}

	// compute descriptors

	sift.compute(img, keypoints, descriptors);


}

// Compute Dense SIFT features in OpenCV 3.0 - http://answers.opencv.org/question/73165/compute-dense-sift-features-in-opencv-30/?answer=73178#post-id-73178
