#include <iostream> //had an issue with included directories... make sure not to VC++ and C++ show up as different messages.
#include <opencv2/opencv.hpp>


/*	Image Segmentation with Distance Transform and watershed Agorithm

Goal

In this tutorial you will learn how to:
-	Use the OpenCV function cv::filter2D in order to perform some laplacian filtering for image sharpening.
-	Use the OpenCV function cv::distanceTransform in order to obtain the derived representation of a binary
image, where the value of each pixel replaced by its distance to the nearest background pixel.
-	Use the OpenCV function cv::watershed in order to isolate objects in the image from the background.
*/


using namespace std;
using namespace cv;

int main()
{
	// Load the image
	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); 

	// Check if everything was fine
	if (!src.data)
		return -1;

	// Show source image
	imshow("Source Image", src);
	waitKey(1);
	// Change the background from white to black, since that will help later to extract
	// better results during the use of Distance Transform
	for (int x = 0; x < src.rows; x++) {
		for (int y = 0; y < src.cols; y++) {
			if (src.at<Vec3b>(x, y) == Vec3b(255, 255, 255)) {
				src.at<Vec3b>(x, y)[0] = 0;
				src.at<Vec3b>(x, y)[1] = 0;
				src.at<Vec3b>(x, y)[2] = 0;
			}
		}
	}

	// Show output image
	imshow("Black Background Image", src);

	// Create a kernel that we will use for accuting/sharpening our image
	Mat kernel = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1); // an approximation of second derivative, a quite strong kernel

	// do the laplacien filtering as it is
	// well, we need to convert everything in something more deeper than CV_8U
	// beacuase the kernel has some negative values,
	// and we can expect in general to have a palacien image with negative values,
	// BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
	// so the possible negative number will be truncated
	Mat imgLaplacian;
	Mat sharp = src; // copy source image to another temorary one
	filter2D(sharp, imgLaplacian, CV_32F, kernel);
	src.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - imgLaplacian;

	// convert back to 8bits gray scale
	imgResult.convertTo(imgResult, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

	// imshow( "Laplace Filtered Image", imgLaplacian );
	imshow("New Sharped Image", imgResult);

	src = imgResult; // copy back

	// Create binary image from source image
	Mat bw;
	cvtColor(src, bw, CV_BGR2GRAY);
	threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("Bonary Image", bw);

	// Perform the distance transform algorithm
	Mat dist;
	distanceTransform(bw, dist, CV_DIST_L2, 3);

	// Normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	normalize(dist, dist, 0, 20., NORM_MINMAX);
	imshow("Distance Transform Image", dist);

	// Threshold to obtain the peaks
	// This will be the markers for the foreground objects
	threshold(dist, dist, .4, 1., CV_THRESH_BINARY);

	// Dilate a bit the dist image
	Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
	dilate(dist, dist, kernel1);
	imshow("Peaks", dist);

	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);

	// Find total markers
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(dist.size(), CV_32SC1);

	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

	// Draw the background marker
	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("Markers", markers * 10000);

	// Perform the watershed algorithm
	watershed(src, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);

	// imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
	// image looks like at that point
	// Generate random colors
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int b = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0, 255);
		int r = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// Create the result image
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);


	Mat dst_hnh = Mat::zeros(markers.size(), CV_8UC3);


	// Below is Yuriy's data structure...
	cv::Mat src_cs;
	src_cs = src.clone();
	src_cs.convertTo(src_cs, CV_32F);
	src_cs /= 255.0;

	// Structure for values w.r.t. index;
	std::vector < std::vector<cv::Vec3f>> values_index(contours.size());

	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				dst.at<Vec3b>(i, j) = colors[index - 1];
				values_index[index - 1].push_back(src_cs.at<cv::Vec3f>(i, j));
			}
			else
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}

	std::vector<cv::Vec3f> mean_values(values_index.size());

	for (size_t i = 0; i < values_index.size(); i++)
	{
		auto mean_val = (cv::mean(values_index[i]));
		mean_values[i] = (cv::Vec3f(mean_val[0], mean_val[1], mean_val[2]));
	}

	std::vector<float> norm_sum(values_index.size());




	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				cv::Vec3f p1 = src_cs.at<cv::Vec3f>(i, j); // instead of taking dst, he takes src  (changed the src_cs to hsv_v) ... changed back, it breaks
				cv::Vec3f p2 = mean_values[index - 1];

				norm_sum[index - 1] += cv::norm(p1, p2, cv::NORM_L2);
			}
		}
	}

	for (size_t i = 0; i < values_index.size(); i++)
	{
		norm_sum[i] /= ((float)values_index[i].size());
		std::cout << norm_sum[i] << std::endl;
	}



	// Declare Homogeneous Threshold
	const float homogeneous_threshold = 0.18f;

	int count = 0;

	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++) // THESE ARE MARKER ROWS... NOT POXELS!
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()) && norm_sum[index - 1] <= homogeneous_threshold && count % 5 == 0){ //this mod operator can be changed for different alrogithms
				dst_hnh.at<Vec3b>(i, j) = colors[index - 1];
				count++;
			}
			else
			//if (count % 5 == 0){
			//	dst_hnh.at<Vec3b>(i, j) = colors[25]; // --- no color segmentations?
			//	}
			//count++;
			dst_hnh.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			count++;
		}
		count++;
	}

	// Visualize the final image
	imshow("Final Result", dst_hnh);

	waitKey(0);
	return 0;
}

/*


const float homogeneous_threshold = 0.18f;

int count = 0;

for (int i = 0; i < markers.rows; i++)
{
for (int j = 0; j < markers.cols; j++)
{
int index = markers.at<int>(i, j);
if (index > 0 && index <= static_cast<int>(contours.size()) && norm_sum[index - 1] <= homogeneous_threshold) //  && count % 5 == 0 (can be added for flip)
{
dst_hnh.at<Vec3b>(i, j) = colors[65]; // dst_hnh has been marked with zeroes... hence why I'm still just getting two colors
count++;
}
else
{
if (count % 5 == 0){
dst_hnh.at<Vec3b>(i, j) = colors[25]; // --- no color segmentations?
count++;
}
count++;
}

}
}


*/