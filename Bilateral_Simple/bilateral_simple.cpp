#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

// referenced from  - Applying Bilateral Filter - http://opencvexamples.blogspot.com/2013/10/applying-bilateral-filter.html


// referenced from https://stackoverflow.com/questions/28515084/opencv-3-0-0-mser-binary-mask
void mserExtractor(const Mat& image, Mat& mserOutMask){
	Ptr<MSER> mserExtractor = MSER::create();

	vector<vector<cv::Point>> mserContours;
	vector<KeyPoint> mserKeypoint;
	vector<cv::Rect> mserBbox;
	mserExtractor->detectRegions(image, mserContours, mserBbox);

	for (vector<cv::Point> v : mserContours){
		for (cv::Point p : v){
			mserOutMask.at<uchar>(p.y, p.x) = 255;
		}
	}
}

int main(int argc, char** argv)
{

	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1);
	Mat dst; // this is apparently being called more than once....

	//Apply bilateral filter
	bilateralFilter(src, dst, 19, 80, 80);
	imshow("source", src);
	imshow("result", dst);

	Mat HSV;

	// convert RGB image to HSV
	cvtColor(dst, HSV, CV_BGR2HSV);

	vector<Mat> hsv_planes;
	split(HSV, hsv_planes);
	Mat h = hsv_planes[0]; // H channel
	Mat s = hsv_planes[1]; // S channel
	Mat v = hsv_planes[2]; // V channel

	namedWindow("hue", CV_WINDOW_AUTOSIZE);
	imshow("hue", h);
	namedWindow("saturation", CV_WINDOW_AUTOSIZE);
	imshow("saturation", s);
	namedWindow("value", CV_WINDOW_AUTOSIZE);
	imshow("value", v);

	Mat mser; // mser outmask....... breaking right now... not enough brain power to keep going.

 	mserExtractor(dst, mser); // https://stackoverflow.com/questions/28515084/opencv-3-0-0-mser-binary-mask

	namedWindow("mser", CV_WINDOW_AUTOSIZE);
	imshow("mser", mser); // need to get a better understanding of the MSER outmask

	//Mat dist;
	//distanceTransform(src, dist, CV_DIST_L2, 3);
	//// Normalize the distance image for range = {0.0, 1.0}
	//// so we can visualize and threshold it
	//normalize(dist, dist, 0, 9., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image", dist);

	//// STEP 6 - runs first try with the cards... my black area looks a bit more rounded than it should be though... ugh yeah, my dragon is a blob
	//// Threshold to obtain the peaks
	//// This will be the markers for the foreground objects
	//threshold(dist, dist, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	//// Dilate a bit the dist image
	//Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
	//dilate(dist, dist, kernel1);
	//imshow("Peaks", dist); // I don't really understand what dist is vs dst

	//// Create the CV_8U version of the distance image
	//// It is needed for findContours()
	//Mat dist_8u;
	//dist.convertTo(dist_8u, CV_8U);

	//// Find total markers
	//vector<vector<Point> > contours;
	//findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//// Create the marker image for the watershed algorithm
	//Mat markers = Mat::zeros(dist.size(), CV_32SC1);

	//// Draw the foreground markers
	//for (size_t i = 0; i < contours.size(); i++)
	//	drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

	//// Draw the background marker
	//circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	//imshow("Markers", markers * 10000);

	//// Perform the watershed algorithm
	//watershed(src, markers);
	//Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	//markers.convertTo(mark, CV_8UC1);
	//bitwise_not(mark, mark);

	//// imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
	//// image looks like at that point
	//// Generate random colors
	//vector<Vec3b> colors;
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	int b = theRNG().uniform(0, 255);
	//	int g = theRNG().uniform(0, 255);
	//	int r = theRNG().uniform(0, 255);
	//	colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	//}

	//// Create the result image
	//Mat dst = Mat::zeros(markers.size(), CV_8UC3);

	//// Fill labeled objects with random colors
	//for (int i = 0; i < markers.rows; i++)
	//{
	//	for (int j = 0; j < markers.cols; j++)
	//	{
	//		int index = markers.at<int>(i, j);
	//		if (index > 0 && index <= static_cast<int>(contours.size()))
	//			dst.at<Vec3b>(i, j) = colors[index - 1];
	//		else
	//			dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
	//	}
	//}

	//// Visualize the final image
	//imshow("Final Result", dst);


	waitKey(0);
	return 0;
}