#include <opencv2\highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
//#include <cv.h> - original include... not enough information
//#include <highgui.h> - original include... not enough information

// Solution found - https://stackoverflow.com/questions/15693900/how-to-determine-a-region-of-interest-and-then-crop-an-image-using-opencv 
// As it is now... It doesn't do anything


int main(int argc, char* argv[])
{
	//cv::Mat img = cv::imread(argv[1]); - original code
	cv::Mat img = cv::imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");
	std::cout << "Original image size: " << img.size() << std::endl;

	// Convert RGB Mat to GRAY
	cv::Mat gray;
	cv::cvtColor(img, gray, CV_BGR2GRAY);
	std::cout << "Gray image size: " << gray.size() << std::endl;

	// Erode image to remove unwanted noises
	int erosion_size = 5;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	cv::erode(gray, gray, element);

	// Scan the image searching for points and store them in a vector
	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = gray.begin<uchar>();
	cv::Mat_<uchar>::iterator end = gray.end<uchar>();
	for (; it != end; it++)
	{
		if (*it)
			points.push_back(it.pos());
	}

	// From the points, figure out the size of the ROI
	int left, right, top, bottom;
	for (int i = 0; i < points.size(); i++)
	{
		if (i == 0) // initialize corner values
		{
			left = right = points[i].x;
			top = bottom = points[i].y;
		}

		if (points[i].x < left)
			left = points[i].x;

		if (points[i].x > right)
			right = points[i].x;

		if (points[i].y < top)
			top = points[i].y;

		if (points[i].y > bottom)
			bottom = points[i].y;
	}
	std::vector<cv::Point> box_points;
	box_points.push_back(cv::Point(left, top));
	box_points.push_back(cv::Point(left, bottom));
	box_points.push_back(cv::Point(right, bottom));
	box_points.push_back(cv::Point(right, top));

	// Compute minimal bounding box for the ROI
	// Note: for some unknown reason, width/height of the box are switched.
	cv::RotatedRect box = cv::minAreaRect(cv::Mat(box_points));
	std::cout << "box w:" << box.size.width << " h:" << box.size.height << std::endl;

	// Draw bounding box in the original image (debugging purposes)
	//cv::Point2f vertices[4];
	//box.points(vertices);
	//for (int i = 0; i < 4; ++i)
	//{
	//    cv::line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(0, 255, 0), 1, CV_AA);
	//}
	//cv::imshow("Original", img);
	//cv::waitKey(0);

	// Set the ROI to the area defined by the box
	// Note: because the width/height of the box are switched, 
	// they were switched manually in the code below:
	cv::Rect roi;
	roi.x = box.center.x - (box.size.height / 2);
	roi.y = box.center.y - (box.size.width / 2);
	roi.width = box.size.height;
	roi.height = box.size.width;
	std::cout << "roi @ " << roi.x << "," << roi.y << " " << roi.width << "x" << roi.height << std::endl;

	// Crop the original image to the defined ROI
	cv::Mat crop = img(roi);

	// Display cropped ROI
	cv::imshow("Cropped ROI", crop);
	cv::waitKey(0);

	return 0;
}