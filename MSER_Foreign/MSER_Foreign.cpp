
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
// Mser license plate target detection
std::vector <cv::Rect> mserGetPlate(cv::Mat srcImage)
{
	// HSV space conversion
	cv::Mat gray, gray_neg;
	cv::Mat hsi;
	cv::cvtColor(srcImage, hsi, CV_BGR2HSV);
	// channel separation
	std::vector <cv::Mat> channels;
	cv::split(hsi, channels);
	// extract the h channel
	gray = channels[1];
	// Grayscale conversion 
	cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
	// negate the value of gray
	gray_neg = 255 - gray;
	std::vector <std::vector <cv::Point>> regContours;
	std::vector <std::vector <cv::Point>> charContours;

	// Create an MSER object
	cv::Ptr <cv::MSER> mesr1 = cv::MSER::create(2, 10, 5000, 0.5, 0.3);
	cv::Ptr <cv::MSER> mesr2 = cv::MSER::create(2, 2, 400, 0.1, 0.3);


	std::vector <cv::Rect> bboxes1;
	std::vector <cv::Rect> bboxes2;
	// MSER + detection
	mesr1->detectRegions(gray, regContours, bboxes1);
	// MSER-operation
	mesr2->detectRegions(gray_neg, charContours, bboxes2);

	cv::Mat mserMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);
	cv::Mat mserNegMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);

	for (int i = (int)regContours.size() - 1; i= 0; i--)
	{
		// Generate mser + results based on the detection area point
		const std::vector <cv::Point> & r = regContours[i];
		for (int j = 0; j <(int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserMapMat.at <unsigned char>(pt) = 255;
		}
	}
	// MSER-detected
	for (int i = (int)charContours.size() - 1; i>= 0; i--)
	{
		// Generate mser-result based on the detection area point
		const std::vector <cv::Point> & r = charContours[i];
		for (int j = 0; j <(int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserNegMapMat.at <unsigned char>(pt) = 255;
		}
	}
	// mser results output
	cv::Mat mserResMat;
	// mser + with mser-bit with the operation
	mserResMat = mserMapMat & mserNegMapMat;
	cv::imshow("mserMapMat", mserMapMat);
	cv::imshow("mserNegMapMat", mserNegMapMat);
	cv::imshow("mserResMat", mserResMat);
	// Close the connection gap
	cv::Mat mserClosedMat;
	cv::morphologyEx(mserResMat, mserClosedMat,
		cv::MORPH_CLOSE, cv::Mat::ones(1, 20, CV_8UC1));
	cv::imshow("mserClosedMat", mserClosedMat);
	// Look for external contours
	std::vector <std::vector <cv::Point>> plate_contours;
	cv::findContours(mserClosedMat, plate_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	// Candle license plate area judgment output
	std::vector <cv::Rect> candidates;
	for (size_t i = 0; i!= plate_contours.size(); ++i)
	{
		// Solve the smallest outer rectangle
		cv::Rect rect = cv::boundingRect(plate_contours[i]); // had to change rect to Rect
		// aspect ratio
		double wh_ratio = rect.width / double(rect.height);
		// Does not meet the size of the conditions to judge
		if (rect.height> 20 && wh_ratio> 4 && wh_ratio <7)
			candidates.push_back(rect);
	}
	return candidates;
}

int main()
{
	cv::Mat srcImage =
		cv::imread("car.jpg");
	if (srcImage.empty())
		return-1;
	cv::imshow("src Image", srcImage);
	// Candle license plate area detection
	std::vector <cv::Rect> candidates;
	candidates = mserGetPlate(srcImage);
	// License plate area display
	for (int i = 0; i <candidates.size(); ++i)
	{
		cv::imshow("rect", srcImage(candidates[i]));
		cv::waitKey();
	}
	cv::waitKey(0);
	return 0;
}
