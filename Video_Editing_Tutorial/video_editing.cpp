// Awesome Tutorial here - https://solarianprogrammer.com/2015/06/04/opencv-video-editing-tutorial/

// Source code here - https://github.com/sol-prog/opencv-video-editing

// Check out the License and README

// The Big Buck Bunny movie used in the tutorial is (c) copyright 2008, Blender Foundation 
// www.bigbuckbunny.org you can get a copy from http://mirror.bigbuckbunny.de/peach/bigbuckbunny_movies/big_buck_bunny_720p_surround.avi

// // OpenCV accessing video camera
#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    // Open the first camera attached to your computer
    cv::VideoCapture cap(0);
    if(!cap.isOpened()) {
        std::cout << "Unable to open the camera\n";
        std::exit(-1);
    }

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

    return 0;
}




// OpenCV editing a video and saving the result
#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
	// Open a video file:
	cv::VideoCapture cap("big_buck_bunny_720p_h264.mov");
	if (!cap.isOpened()) {
		std::cout << "Unable to open the camera\n";
		std::exit(-1);
	}

	// Get the width/height and the FPS of the vide
	int width = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	double FPS = cap.get(CV_CAP_PROP_FPS);

	// Open a video file for writing (the MP4V codec works on OS X and Windows)
	cv::VideoWriter out("output.mov", CV_FOURCC('m', 'p', '4', 'v'), FPS, cv::Size(width, height));
	if (!out.isOpened()) {
		std::cout << "Error! Unable to open video file for output." << std::endl;
		std::exit(-1);
	}

	cv::Mat image;
	cv::Mat panel_middle, panel_right;
	int delta = width / 3;

	while (true) {
		cap >> image;
		if (image.empty()) {
			std::cout << "Can't read frames from your camera\n";
			break;
		}

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