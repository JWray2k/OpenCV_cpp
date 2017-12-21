#include "opencv2/calib3d.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <list>
#include <vector>
#include <iostream> 

// Referenced -
// Awesome Tutorial here - https://solarianprogrammer.com/2015/06/04/opencv-video-editing-tutorial/
// Source code here - https://github.com/sol-prog/opencv-video-editing


// MSER Class Reference - http://docs.opencv.org/3.3.0/d3/d28/classcv_1_1MSER.html 

int main() {
	// Open a video file:
	std::string arg = "C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-%02d.png";
	cv::VideoCapture capture(arg); // was arg
	if (!capture.isOpened())
		capture.open(atoi(arg.c_str()));
	
	// Get the width/height and the FPS of the vide
	int width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	double FPS = capture.get(CV_CAP_PROP_FPS); // I can hardcode the FPS to a number

	// Open a video file for writing (the MP4V codec works on OS X and Windows)
	cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
	if (!capture.isOpened()) {
		std::cout << "Error! Unable to open video file for output." << std::endl;
		std::exit(-1);
	}
	// Variable for storing video frames / image sequence
	cv::Mat image;
	cv::Mat panel_middle, panel_right;
	int delta = width / 3;

	while (true) {
		capture >> image;
		if (image.empty()) {
			std::cout << "Can't read frames from your camera\n";
			break;
		}

		// mser = cv2.MSER_create() - Python call
		cv::Ptr<cv::MSER> create(int _delta = 5, int _min_area = 60, int _max_area = 14400,
									double _max_variation = 0.25, double _min_diversity = .2,
									int _max_evolution = 200, double _area_threshold = 1.01,
									double _min_margin = 0.003, int _edge_blur_size = 5			); // so now that it's been constructed, how do I pull from it?

		/*
		cv::MSER::create(int _delta = 5,
			int 	_min_area = 60,
			int 	_max_area = 14400,
			double 	_max_variation = 0.25,
			double 	_min_diversity = .2,
			int 	_max_evolution = 200,
			double 	_area_threshold = 1.01,
			double 	_min_margin = 0.003,
			int 	_edge_blur_size = 5
			)

		*/

		// Number of frames to capture
		int num_frames = 25; // I think this is handled elsewhere also...

		// What does ret and frame mean here? - ret, frame = cap.read() https://stackoverflow.com/questions/28773186/what-does-ret-and-frame-mean-here

		// Python:
		// while True: // looks to be a while loop to modify the image while it
		// ret, img = cam.read() - learned about that here - https://www.learnopencv.com/how-to-find-frame-rate-or-frames-per-second-fps-in-opencv-python-cpp/

		// Grab a few frames
		for (int i = 0; i < num_frames; i++)
		{
			capture >> image;
		}

		if (!capture.isOpened()){
			break;
		}
#
		cv::Mat gray;

		// Return here
		// gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) 
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
			// vis = img.copy()
			cv::Mat vis = gray; // copy result to a Mat that will be visualized

			// regions, _ = mser.detectRegions(gray)
			cv::MSER::detectRegions(image,
				std::vector< std::vector< Point > > & 	msers,
				std::vector< Rect > & 	bboxes
				)
			hulls = [cv2.convexHull(p.reshape(-1, 1, 2)) for p in regions]
			cv2.polylines(vis, hulls, 1, (0, 255, 0))

			cv2.imshow('img', vis)
		if cv2.waitKey(5) == 27:
		break
			cv2.destroyAllWindows()


		// Copy the middle panel from the image and convert it to gray
		panel_middle = image(cv::Rect(delta, 0, image.cols / 3, image.rows));
		cv::cvtColor(panel_middle, panel_middle, cv::COLOR_BGR2GRAY);
		cv::cvtColor(panel_middle, panel_middle, cv::COLOR_GRAY2BGR);

		// Copy the modified middle panel back to the source image
		panel_middle.copyTo(image(cv::Rect(delta, 0, image.cols / 3, image.rows)));

		// Copy the right panel from the image and convert it to gray
		panel_right = image(cv::Rect(2 * delta, 0, image.cols / 3, image.rows));
		cv::cvtColor(panel_right, panel_right, cv::COLOR_BGR2GRAY);

		// Apply a Canny edge detector to the right panel, convert the result to BGR
		cv::Canny(panel_right, panel_right, 100, 300, 3);
		cv::cvtColor(panel_right, panel_right, cv::COLOR_GRAY2BGR);

		// Copy the modified right panel back to the source image
		panel_right.copyTo(image(cv::Rect(2 * delta, 0, image.cols / 3, image.rows)));

		// Save frame to video
		out << image;

		cv::imshow("Modified video", image);

		// Stop the camera if the user presses the "ESC" key
		if (cv::waitKey(1000.0 / FPS) == 27) break;
	}

	return 0;
}

/* The following reads in each frame one by one from the capture... it also adjusts the FPS to 24
cv::Mat image;
double FPS = 24.0;
// Read camera frames (at approx 24 FPS) and show the result
while(true) {
cap >> image;
if(image.empty()) {
std::cout << "Can't read frames from your camera\n";
break;
}

cv::imshow("Camera feed", image);

// Stop the camera if the user presses the "ESC" key
if(cv::waitKey(1000.0/FPS) == 27) break;
}
*/


/*

#!/usr/bin/env python

'''
MSER detector demo
==================
Usage:
------
mser.py [<video source>]
Keys:
-----
ESC   - exit
'''

import numpy as np
import cv2
import video
import sys

if __name__ == '__main__':
try:
video_src = sys.argv[1]
except:
video_src = 0

cam = video.create_capture(video_src)
mser = cv2.MSER_create()

while True:
ret, img = cam.read()
if ret == 0:
break
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
vis = img.copy()

regions, _ = mser.detectRegions(gray)
hulls = [cv2.convexHull(p.reshape(-1, 1, 2)) for p in regions]
cv2.polylines(vis, hulls, 1, (0, 255, 0))

cv2.imshow('img', vis)
if cv2.waitKey(5) == 27:
break
cv2.destroyAllWindows()

*/