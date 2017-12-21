#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning - must come before #includes
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>


using namespace std;
using namespace cv;
// using namespace cv::cuda; //not sure what is using this... I think I got it elsewhere...

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
	// was Mat src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");
	src = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); //somehow I deleted this image from my picture library
	// Mat src = imread("C:/Users/wray/Pictures/cards.png"); //file location had to be modified... it's not just libraries/pictures - Step 2 works with this
	// Check if everything was fine
	if (!src.data)
		return -1;
	// Show source image
	// imshow("Source Image", src);

	// STEP 2 - 

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
	// imshow("Black Background Image", src); // This does nothing... is it something to do with another quality?

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
	// imshow("New Sharped Image", imgResult);

	// I was missing this step from before.... big mistake
	src = imgResult; // copy back

	// C++: void dilate(InputArray src, OutputArray dst, InputArray kernel, Point anchor=Point(-1,-1), 
	// int iterations=1, int borderType=BORDER_CONSTANT, const Scalar& borderValue=morphologyDefaultBorderValue() )

	// Mat dst;

	// void dialate(src, dst, 1, anchor = Point(-1, -1), iterations = 1, )
	dilate(src, dilation_dst, getStructuringElement(MORPH_CROSS,
		Size(2 * 1 + 1, 2 * 1 + 1),
		Point(1, 1)));
	// imshow("Dilation", dilation_dst);


	erode(src, erosion_dst, getStructuringElement(MORPH_CROSS,
		Size(2 * 1 + 1, 2 * 1 + 1),
		Point(1, 1)));
	// imshow("Erosion", erosion_dst);


	// STEP 4 - worked first try
	// Create binary image from source image
	Mat bw, bwa, bwb, bw1, bw1a, bw1b;
	
	//Standard
	cvtColor(src, bw, CV_BGR2GRAY);
	threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result
	// imshow("Binary Image - standard", bw);

	//Standard
	cvtColor(src, bwa, CV_BGR2GRAY);
	threshold(bwa, bwa, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result
	// imshow("Binary Image - standard a", bwa);

	//Standard
	cvtColor(src, bwb, CV_BGR2GRAY);
	threshold(bwb, bwb, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result	
	// imshow("Binary Image - standard b", bwb);

	//Dialation
	//cvtColor(dilation_dst, bw1, CV_BGR2GRAY);
	//threshold(bw1, bw1, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	// imshow("Binary Image - Dialation", bw1);

	//Erosion
	cvtColor(erosion_dst, bw1, CV_BGR2GRAY);
	threshold(bw1, bw1, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result
	// imshow("Binary Image - Erosion", bw1);

	//Erosion
	cvtColor(erosion_dst, bw1a, CV_BGR2GRAY);
	threshold(bw1a, bw1a, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result
	// imshow("Binary Image - Erosion a", bw1a);

	//Erosion
	cvtColor(erosion_dst, bw1b, CV_BGR2GRAY);
	threshold(bw1b, bw1b, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	
	// Hidden for final result
	// imshow("Binary Image - Erosion b", bw1b);


	// STEP 5 - works first try... looks good on the cards, there's a strange glow for the dragon through, this step may need optimized
	// Perform the distance transform algorithm
	Mat dist, dista, distb, dist1, dist1a, dist1b;
	
	// Standard
	distanceTransform(bw, dist, CV_DIST_L2, 3);

	// Standard a
	distanceTransform(bwa, dista, CV_DIST_L2, 3);

	// Standard b
	distanceTransform(bwb, distb, CV_DIST_L2, 3);

	// Dialation
	// distanceTransform(bw1, dist1, CV_DIST_L2, 3);

	// Erosion
	distanceTransform(bw1, dist1, CV_DIST_L2, 3);

	// Erosion a
	distanceTransform(bw1a, dist1a, CV_DIST_L2, 3);

	// Erosion b
	distanceTransform(bw1b, dist1b, CV_DIST_L2, 3);


	// Add mats to compare normalization...
	Mat distaa, distbb, dist1aa, dist1bb, dist2a, dist2b;

	// Normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	normalize(dist, dist, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	
	// Hidden for final result
	// imshow("Distance Transform Image", dist);

	normalize(dista, distaa, 0, 14., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	
	// Hidden for final result
	// imshow("Distance Transform Image a 14", dista);

	//normalize(distb, distbb, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image b 30", distb);

	//normalize(dist, distbb, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image b 30", distbb);

	//normalize(dist, distaa, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image b 30", distaa);

	//normalize(dista, distaa, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image aa 5", distaa);

	//normalize(distb, distbb, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image bb 5", distbb);


	//// Dialation
	//normalize(dist1, dist1, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Dialation", dist1);

	//normalize(dist1, dist1a, 0, 9., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Dialation a", dist1a);

	//normalize(dist1, dist1b, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Dialation b", dist1b);

	// Erosion
	normalize(dist1, dist1, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	
	// Hidden for final result
	// imshow("Distance Transform Image - Erosion", dist1);

	//normalize(dist1, dist1aa, 0, 9., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Erosion a", dist1a);

	normalize(dist1, dist1bb, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	
	// Hidden for final result
	// imshow("Distance Transform Image - Erosion b norm 30", dist1bb);

	//normalize(dist1a, dist1aa, 0, 9., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Erosion a norm 8", dist1aa);

	//normalize(dist1b, dist1bb, 0, 30., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Erosion dist1bb norm 30", dist1bb);

	//normalize(dist1a, dist1aa, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Erosion 5 - aa", distaa);

	//normalize(dist1b, dist1bb, 0, 5., NORM_MINMAX); //this is very important... it allows me to normalize my values between a low and high threshold... why 30?
	//imshow("Distance Transform Image - Erosion 5 - bb", distbb);


	// At this point, Dialation seems to be doing work... Erosion seems to make it better though... I'll step through one more time
	// Normal is much sharper than Erosion still... that makes me question if IÄm on the right track or not.

	// STEP 6 - runs first try with the cards... my black area looks a bit more rounded than it should be though... ugh yeah, my dragon is a blob
	// Threshold to obtain the peaks
	// This will be the markers for the foreground objects

	// working with dist1, dist1bb, dist, dista

	threshold(dist, dist, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	// Dilate a bit the dist image
	Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
	dilate(dist, dist, kernel1);

	// Hidden for final result
	// imshow("Peaks - standard, 5 thresh", dist); // I don't really understand what dist is vs dst

	// This result wasn't good enough to keep
	//threshold(dista, dista, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	//// Dilate a bit the dist image
	//Mat kernel1a = Mat::ones(3, 3, CV_8UC1);
	//dilate(dista, dista, kernel1a);
	//imshow("Peaks - standard, 14 thresh", dista); // I don't really understand what dist is vs dst

	threshold(dist1, dist1, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	// Dilate a bit the dist image
	Mat kernel1b = Mat::ones(3, 3, CV_8UC1); // Single channel
	dilate(dist1, dist1, kernel1b);
	
	// Hidden for final result
	// imshow("Peaks - erosion, 5 thresh", dist1); // I don't really understand what dist is vs dst

	threshold(dist1bb, dist1bb, .4, 1., CV_THRESH_BINARY); //this is needed before I can identify contours
	// Dilate a bit the dist image
	Mat kernel1c = Mat::ones(3, 3, CV_8UC1);
	dilate(dist1bb, dist1bb, kernel1c);

	// Hidden for final result
	// imshow("Peaks - erosion, 30 thresh", dist1bb); // I don't really understand what dist is vs dst



	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U); // Standard

	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat dist_8u_Erosion5;
	dist1.convertTo(dist_8u_Erosion5, CV_8U); // Erosion 5

	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	Mat dist_8u_Erosion30;
	dist1bb.convertTo(dist_8u_Erosion30, CV_8U); // Erosion 30 ************** By making this dist_8u_Erosion30, all of my v2 markser sifted


	// Find total markers
	vector<vector<Point> > contours; // look at how vectors are declared.. vector<vector<point> > may be a declaration
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Find total markers - Erosion 5
	vector<vector<Point> > contours_E5;
	findContours(dist_8u_Erosion5, contours_E5, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Find total markers - Erosion 30
	vector<vector<Point> > contours_E30;
	findContours(dist_8u_Erosion30, contours_E30, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);



	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(dist.size(), CV_32SC1);

	// Create the marker image for the watershed algorithm
	Mat markers_E5 = Mat::zeros(dist1.size(), CV_32SC1);

	// Create the marker image for the watershed algorithm
	Mat markers_E30 = Mat::zeros(dist1bb.size(), CV_32SC1);




	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

	// Draw the foreground markers E5
	for (size_t i = 0; i < contours_E5.size(); i++)
		drawContours(markers_E5, contours_E5, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);

	// Draw the foreground markers E30
	for (size_t i = 0; i < contours_E30.size(); i++)
		drawContours(markers_E30, contours_E30, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);


	// Draw the background marker
	circle(markers, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("Markers", markers * 10000);

	// Draw the background marker E5
	circle(markers_E5, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("Markers E5", markers_E5 * 10000);

	// Draw the background marker E30
	circle(markers_E30, Point(5, 5), 3, CV_RGB(255, 255, 255), -1);
	imshow("Markers E30", markers_E30 * 10000);




	// Perform the watershed algorithm
	watershed(src, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);

	// Perform the watershed algorithm
	watershed(src, markers_E5);
	Mat mark_5 = Mat::zeros(markers_E5.size(), CV_8UC1);
	markers_E5.convertTo(mark_5, CV_8UC1);
	bitwise_not(mark_5, mark_5);

	// Perform the watershed algorithm
	watershed(src, markers_E30);
	Mat mark_30 = Mat::zeros(markers_E30.size(), CV_8UC1);
	markers_E30.convertTo(mark_30, CV_8UC1);
	bitwise_not(mark_30, mark_30);



	imshow("Markers_v2", mark); // uncomment this if you want to see how the mark

	imshow("Markers_v2_E5", mark_5);

	imshow("Markers_v2_E30", mark_30);



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
	Mat dst_E5 = Mat::zeros(markers_E5.size(), CV_8UC3);
	Mat dst_E30 = Mat::zeros(markers_E30.size(), CV_8UC3);


	// Fill labeled objects with random colors
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
				dst.at<Vec3b>(i, j) = colors[index - 1]; // maybe I have something I must do in with colors... I doubt it though...
			else
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}

	// Fill labeled objects with random colors E5
	for (int i = 0; i < markers_E5.rows; i++)
	{
		for (int j = 0; j < markers_E5.cols; j++)
		{
			int index = markers_E5.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours_E5.size()))
				dst_E5.at<Vec3b>(i, j) = colors[index - 1];
			else
				dst_E5.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}


	// Fill labeled objects with random colors E30
	for (int i = 0; i < markers_E30.rows; i++)
	{
		for (int j = 0; j < markers_E30.cols; j++)
		{
			int index = markers_E30.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours_E30.size()))
				dst_E30.at<Vec3b>(i, j) = colors[index - 1];
			else
				dst_E30.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}


	// Visualize the final image
	imshow("Final Result", dst);

	// Visualize the final image
	imshow("Final Result E5", dst_E5);

	// Visualize the final image
	imshow("Final Result E3ß", dst_E30);



	/// Create windows
	// namedWindow("Erosion Demo", CV_WINDOW_AUTOSIZE);
	// namedWindow("Dilation Demo", CV_WINDOW_AUTOSIZE);
	// cvMoveWindow("Dilation Demo", src.cols, 0); - this is pretty nice... allows you to move an area

	/*/// Create Erosion Trackbar
	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
		&erosion_elem, max_elem,
		Erosion);

	createTrackbar("Kernel size:\n 2n +1", "Erosion Demo",
		&erosion_size, max_kernel_size,
		Erosion);
*/
	/// Create Dilation Trackbar
	/*createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
		&dilation_elem, max_elem,
		Dilation);

	createTrackbar("Kernel size:\n 2n +1", "Dilation Demo",
		&dilation_size, max_kernel_size,
		Dilation);
		*/
	/// Default start
	// Erosion(0, 0);
	// Dilation(0, 0);

	waitKey(0);
	return 0;
}

/**  @function Erosion  */
void Erosion(int, void*)
{
	int erosion_type;
	if (erosion_elem == 0){ erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1){ erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(src, erosion_dst, element); // needed source to be a global variable to be used like this
	imshow("Erosion Demo", erosion_dst);
}
/*
The function that performs the erosion operation is erode. As we can see, it receives three arguments:

src: The source image

erosion_dst: The output image

element: This is the kernel we will use to perform the operation. If we do not specify, the default is a simple 3x3 matrix. Otherwise, we can specify its shape.
For this, we need to use the function getStructuringElement:
*/


/** @function Dilation */
void Dilation(int, void*)
{
	int dilation_type;
	if (dilation_elem == 0){ dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1){ dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(src, dilation_dst, element);
	imshow("Dilation Demo", dilation_dst);
}