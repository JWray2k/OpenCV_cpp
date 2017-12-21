//I'm having a hell of a time dealing with this x86 and x64 nonsense... I should download content cot VS2013 x64
#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning - must come before #includes
#include <iostream> //had an issue with included directories... make sure not to VC++ and C++ show up as different messages.
#include <opencv2/opencv.hpp>
//#include <initializer_list>
//#include <iterator>
#include <opencv2/opencv_modules.hpp>

//referenced from https://github.com/opencv/opencv/blob/master/modules/cudaarithm/src/cuda/norm.cu 
// #ifndef HAVE_OPENCV_CUDEV

// #error "opencv_cudev is required"

// #else

// #include "opencv2/cudaarithm.hpp"
// #include "opencv2/cudev.hpp"
// #include "opencv2/core/private.cuda.hpp"


// Image Segmentation with Distance Transform and Watershed Algorithm - http://docs.opencv.org/3.1.0/d2/dbd/tutorial_distance_transform.html 


using namespace std;
using namespace cv;
using namespace cv::cuda;

/*
inline void mix_channels(cv::Mat const &src, cv::Mat &dst, std::initializer_list<int> from_to)
{
cv::mixChannels(&src, 1, &dst, 1, std::begin(from_to), from_to.size() / 2);
}
*/

// Global variables - From Erosion & Dialation
Mat src, erosion_dst, dilation_dst; // src, was here before

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
void Erosion(int, void*);
void Dilation(int, void*);

int main(int, char** argv) //I can't even get through step 1 for displaying this image... That's what I'll focus on... It was because I didn't set up the wait key.
{
	// STEP 1
	// Load the image
	Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); //somehow I deleted this image from my picture library
	// Mat src = imread("C:/Users/wray/Pictures/cards.png"); //file location had to be modified... it's not just libraries/pictures - Step 2 works with this
	// Check if everything was fine
	if (!src.data)
		return -1;
	// Show source image
	imshow("Source Image", src);


	// STEP 2 - 

	// Change the background from white to black, since that will help later to extract
	// better results during the use of Distance Transform
	for (int x = 0; x < src.rows; x++) {
		for (int y = 0; y < src.cols; y++) {
			if (src.at<Vec3b>(x, y) == Vec3b(255, 255, 255)) {
				src.at<Vec3b>(x, y)[0] = 0; // the value 255 was changed to 0 for each of these
				src.at<Vec3b>(x, y)[1] = 0;
				src.at<Vec3b>(x, y)[2] = 0;
			}
		}
	}
	// Show output image
	imshow("Black Background Image", src);

	// STEP 3 - got results first run... nice... 

	// Create a kernel that we will use for accuting/sharpening our image 
	Mat kernel = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1); // an approximation of second derivative, a quite strong kernel

	// do the laplacian filtering as it is
	// well, we need to convert everything in something more deeper then CV_8U
	// because the kernel has some negative values,
	// and we can expect in general to have a Laplacian image with negative values
	// BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
	// so the possible negative number will be truncated
	Mat imgLaplacian;
	Mat sharp = src; // copy source image to another temporary one
	filter2D(sharp, imgLaplacian, CV_32F, kernel);
	src.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - imgLaplacian;

	// convert back to 8bits gray scale
	imgResult.convertTo(imgResult, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
	// imshow( "Laplace Filtered Image", imgLaplacian );
	imshow("New Sharped Image", imgResult);

	// I was missing this step from before.... big mistake
	src = imgResult; // copy back


	erode(src, erosion_dst, getStructuringElement(MORPH_CROSS,
		Size(2 * 1 + 1, 2 * 1 + 1),
		Point(1, 1)));
	// imshow("Erosion", erosion_dst);

	// STEP 4 - worked first try
	// Create binary image from source image
	Mat bw;
	/*cvtColor(src, bw, CV_BGR2GRAY);
	threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("Binary Image", bw);
*/

	//Erosion
	cvtColor(erosion_dst, bw, CV_BGR2GRAY);
	threshold(bw, bw, 20, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// STEP 5 - works first try... looks good on the cards, there's a strange glow for the dragon through, this step may need optimized
	// Perform the distance transform algorithm
	Mat dist;
	distanceTransform(bw, dist, CV_DIST_L2, 3);
	// Normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	normalize(dist, dist, 0, 60., NORM_MINMAX);
	// normalize(dist, dist, 0, 20., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	imshow("Distance Transform Image", dist);

	// STEP 6 - runs first try with the cards... my black area looks a bit more rounded than it should be though... ugh yeah, my dragon is a blob
	// Threshold to obtain the peaks
	// This will be the markers for the foreground objects
	threshold(dist, dist, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	// Dilate a bit the dist image
	Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
	dilate(dist, dist, kernel1);
	imshow("Peaks", dist); // I don't really understand what dist is vs dst

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
	// Mat dst = Mat::zeros(markers.size(), CV_8UC3);

	// Visualize the watershed image
	imshow("Watershed", dist);

	// Create the result image
	Mat dst = Mat::zeros(markers.size(), CV_8UC3);

	Mat dst_hnh = Mat::zeros(markers.size(), CV_8UC3);

	//// Create a new matrix to hold the HSV image
	//Mat HSV, g_HSV, fin_HSV;
	//vector<Mat> HSV_channels;

	//// convert RGB image to HSV
	//cv::cvtColor(src, HSV, CV_BGR2HSV);
	//g_HSV = Mat::zeros(Size(HSV.rows, HSV.cols), CV_8UC1);

	//HSV_channels.push_back(g_HSV); // push the first channel off the stack
	//HSV_channels.push_back(g_HSV); // push the second channel off the stack
	//HSV_channels.push_back(src);

	//cv::merge(HSV_channels, fin_HSV);

	//namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	//imshow("Display window", src);

	//namedWindow("HSV Result window", CV_WINDOW_AUTOSIZE);
	//imshow("HSV Result window", fin_HSV);



	//// CIELab color channels
	//Mat CIELab;

	////transform to CIE Lab
	//cvtColor(src, CIELab, COLOR_BGR2Lab);

	//vector<Mat> CIE_channels;

	//split(CIELab, CIE_channels);
	//namedWindow("CIELab Result window", CV_WINDOW_AUTOSIZE);
	//imshow("CIELab Result window", CIELab);




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



	//// Show individual channels
	//Mat g, fin_img;
	//g = Mat::zeros(Size(dist.cols, dist.rows), CV_8UC1); // Instead of CV_8UC1, use CV_BGR2HSV (actally, that crashes the program...)

	//// Showing Red Channel
	//// G and B channels are kept as zero matrix for visual perception
	//{
	//	vector<Mat> channels;
	//	channels.push_back(g);
	//	channels.push_back(g);
	//	// channels.push_back(rgbChannels[2]);
	//	channels.push_back(hsvChannels[2]);

	//	/// Merge the three channels
	//	merge(channels, fin_img);
	//	namedWindow("R", 1); imshow("R", fin_img);
	//}


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


	const float homogeneous_threshold = 0.18f;

	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()) && norm_sum[index - 1] <= homogeneous_threshold)
			{
				dst_hnh.at<Vec3b>(i, j) = colors[0]; // dst_hnh has been marked with zeroes... hence why I'm still just getting two colors
			}
			else
				dst_hnh.at<Vec3b>(i, j) = colors[1]; // --- no color segmentations?
		}
	}

	// Maybe I should merge my two images together... the segmentation and the non-segmentation...


	for (size_t i = 0; i < norm_sum.size(); i++)
	{
		if (norm_sum[i] <= homogeneous_threshold)
		{
			std::cout << to_string(i) + "is a homogeneous region" << endl;
			// colors[i] = color for a homogeneous region = Vec3b(255,255, 0);
		}
		else {
			//for (norm_sum[i]){
			//norm_sum[i].push_back(src_cs.at<cv::Vec3f>(i, j));
			//}
			// I want to convert the norm_sum pixel contents to black if they aren't homogeneous... or I want to take the specific regions that
			// are homogeneous into a texture... maybe from another image example

			//std::vector<cv::Vec3f> mean_values(values_index.size());

			// colors[i] = color for a non-homogeneous region = Vec3b(255, 0, 255);

			std::vector<cv::Vec3f> non_homogeneousy(values_index.size());
			mean_values[i] = (cv::Vec3f(0, 0, 0));
			/*for (int i = 0; i < norm_sum.rows; i++)
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
			}*/
		}
	}

	// Visualize the final image
	imshow("Final Result", dst_hnh);


	// Let's convert the original image to HSV

	// cv::cvtColor(src, src_hsv, cv::COLOR_BGR2HSV);

	//     // And split the channels
	//     cv::Mat hsv_ch[3];
	//     cv::split(src_hsv, hsv_ch);
	//
	//     // Take a channel with a "value" in HSV space
	//     cv::Mat hsv_v = hsv_ch[2].clone();


	waitKey(0);
	return 0;


	/*

	// STEP 7 - this is where it crashes... (it was dll files... now it runs... yay!)
	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);

	// Find total markers
	vector<vector<Point> > contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //this line crashes the program... it was crashing because I was loading in debug dll files for the release
	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(dist.size(), CV_32SC1);
	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
	drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

	// Draw the background marker
	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("Markers", markers * 10000); //nothing happens here with the dragon... it auto closes... whereas the cards work... hmmm



	// STEP 8
	// Perform the watershed algorithm
	watershed(src, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);
	imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
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
	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
	for (int j = 0; j < markers.cols; j++)
	{
	int index = markers.at<int>(i, j);
	if (index > 0 && index <= static_cast<int>(contours.size()))
	dst.at<Vec3b>(i, j) = colors[index - 1];
	else
	dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
	}
	}
	// Visualize the final image
	imshow("Final Result", src); //my result doesn't show up right... I didn't alter anything... so that's strange.
	waitKey(0);
	return 0;
	*/


	// Below I was working with color spaces and normalizion... some references in place


	// for (i = 0; i < )


	/*
	// - Keeps separating my colorspace into RGB
	// Separate color spaces
	// Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", 1);
	namedWindow("src", 1); imshow("src", src);

	// Split the image into different channels
	// vector<Mat> rgbChannels(3);
	vector<Mat> hsvChannels(3);
	// split(dst, rgbChannels);
	split(src, hsvChannels);

	// Show individual channels
	Mat g, fin_img;
	g = Mat::zeros(Size(src.cols, src.rows), CV_8UC1); // Instead of CV_8UC1, use CV_BGR2HSV (actally, that crashes the program...)

	// Showing Red Channel
	// G and B channels are kept as zero matrix for visual perception
	{
	vector<Mat> channels;
	channels.push_back(g);
	channels.push_back(g);
	// channels.push_back(rgbChannels[2]);
	channels.push_back(hsvChannels[2]);

	/// Merge the three channels
	merge(channels, fin_img);
	namedWindow("R", 1); imshow("R", fin_img);
	}

	// Showing Green Channel
	{
	vector<Mat> channels;
	channels.push_back(g);
	// channels.push_back(rgbChannels[1]);
	channels.push_back(hsvChannels[1]);
	channels.push_back(g);
	merge(channels, fin_img);
	namedWindow("G", 1); imshow("G", fin_img);
	}

	// Showing Blue Channel
	{
	vector<Mat> channels;
	// channels.push_back(rgbChannels[0]);
	channels.push_back(hsvChannels[0]);
	channels.push_back(g);
	channels.push_back(g);
	merge(channels, fin_img);
	namedWindow("B", 1); imshow("B", fin_img);
	}
	*/


	/*
	// Automatic Threshold using Hue channel - http://answers.opencv.org/question/22442/automatic-threshold-using-hue-channel/
	Mat src2 = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");
	cv::Mat hsv;
	cv::cvtColor(src2, hsv, CV_BGR2HSV);
	cv::Mat hue(src2.size(), CV_8U);
	//the third arguments are two number a pair, (0, 0) means copy the data of channels 0(hsv) to channels 0(hue)
	mix_channels(hsv, hue, { 0, 0 });
	cv::Mat otsuMat;
	cv::threshold(hue, otsuMat, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

	cv::imshow("after hsv split", otsuMat);
	*/

	/*
	Matrix Reductions - http://docs.opencv.org/3.0-beta/modules/cudaarithm/doc/reductions.html
	double cuda::norm(InputArray src1, InputArray src2, int normType = NORM_L2);

	Parameters:
	src1 – Source matrix. Any matrices except 64F are supported.
	src2 – Second source matrix (if any) with the same size and type as src1.
	normType – Norm type. NORM_L1 , NORM_L2 , and NORM_INF are supported for now.
	mask – optional operation mask; it must have the same size as src1 and CV_8UC1 type.
	buf – Optional buffer to avoid extra memory allocations. It is resized automatically.

	double cuda::norm(src, src2, NORM_L2);
	*/

	// Mat AVG?

	/*
	double 	cv::norm (InputArray src1, int normType=NORM_L2, InputArray mask=noArray())
	Calculates an absolute array norm, an absolute difference norm, or a relative difference norm. More...

	double 	cv::norm (InputArray src1, InputArray src2, int normType=NORM_L2, InputArray mask=noArray())

	double 	cv::norm (const SparseMat &src, int normType)

	void 	cv::normalize (InputArray src, InputOutputArray dst, double alpha=1, double beta=0, int norm_type=NORM_L2, int dtype=-1, InputArray mask=noArray())
	Normalizes the norm or value range of an array. More...

	void 	cv::normalize (const SparseMat &src, SparseMat &dst, double alpha, int normType)

	*/

	// The below came from http://docs.opencv.org/3.2.0/d2/de8/group__core__array.html 
	// norm(src, NORM_L2); // NormL2... not sure what this does exactly

	// cv::imshow("after Norm_L2", src); // This does nothing...


	/*
	Normalize documentation - http://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html#normalize

	void cv::normalize	(	InputArray 			src,
	InputOutputArray 	dst,
	double 				alpha = 1,
	double 				beta = 0,
	int 				norm_type = NORM_L2,
	int 				dtype = -1,
	InputArray 			mask = noArray()
	)


	void cv::normalize	(	const SparseMat & 	src,
	SparseMat & 		dst,
	double 				alpha,
	int 				normType
	)

	The InputOutputArray is "placeholder" type. Parameters of this type are modified in place, inside the function.

	There are other 2 types:

	InputArray which is read-only inside the function. This parameters are used as source data for the function, but are not modified by the function.

	OutputArray which is write-only. Parameters of this type are modified inside the function. Usually this means they are created inside the function.
	This means that data in the array are replaced by the data generated by the function.

	This principle is used so the function can "return" more than one value.
	https://stackoverflow.com/questions/29594048/the-meaning-of-inputoutputarray-in-cvstereocalibrate-declaration

	You may want to modify the code further after this point - (specifically in regards to InputOutputArray)
	http://www.emgu.com/wiki/files/3.0.0/document/html/77bd35bc-cdb4-3ed0-bcaf-9cf66a95f0cc.htm //actually this isn't openCV
	*/

	// I can't use cvNormalize cv::Mat to cvArr doesn't exist)

	// It seems that my InputOutputArray should be something of an existing array already... but I'm not sure what kind exactly... just getting back blackness

	// normalize(src, dst, 0, NORM_L2); //lower case normalize function accepted

	// cv::imshow("after other type of norm", dst); // This does nothing... - dst results as a blackened image, nothing changes to src

	//  cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1) - https://stackoverflow.com/questions/12023958/what-does-cvnormalize-src-dst-0-255-norm-minmax-cv-8uc1

	// cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	// cv::imshow("after third norm", dst); // looks like nothing happened... perhaps normalization needs to happen elsewhere... what did Yuriy point to with the summation?


	/*
	Mat temp, rp;

	for (i = descriptors1.rows - 1; i >= 0; i--)
	{
	normalize(descriptors1.row(i), temp);  -- The descriptors1 above is a Mat.
	rp = normd1.rowRange(i, i+1);
	temp.copyTo(rp);
	}
	https://stackoverflow.com/questions/10673715/how-to-normalize-rows-of-an-opencv-mat-without-a-loop
	*/
}