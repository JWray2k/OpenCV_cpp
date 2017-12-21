/*
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct userdata{
	Mat im;
	vector<Point2f> points;
};


void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		userdata *data = ((userdata *)data_ptr);
		circle(data->im, Point(x, y), 3, Scalar(0, 255, 255), 5, CV_AA);
		imshow("Image", data->im);
		if (data->points.size() < 4)
		{
			data->points.push_back(Point2f(x, y));
		}
	}

}



int main(int argc, char** argv)
{

	// Read in the image.
	Mat im_src = imread("C:/Users/wray/Pictures/first-image.jpg");
	Size size = im_src.size();

	// Create a vector of points.
	vector<Point2f> pts_src;
	pts_src.push_back(Point2f(0, 0));
	pts_src.push_back(Point2f(size.width - 1, 0));
	pts_src.push_back(Point2f(size.width - 1, size.height - 1));
	pts_src.push_back(Point2f(0, size.height - 1));



	// Destination image
	Mat im_dst = imread("C:/Users/wray/Pictures/times-square.jpg");


	// Set data for mouse handler
	Mat im_temp = im_dst.clone();
	userdata data;
	data.im = im_temp;


	//show the image
	imshow("Image", im_temp);

	cout << "Click on four corners of a billboard and then press ENTER" << endl;
	//set the callback function for any mouse event
	setMouseCallback("Image", mouseHandler, &data);
	waitKey(0);

	// Calculate Homography between source and destination points
	Mat h = findHomography(pts_src, data.points);

	// Warp source image
	warpPerspective(im_src, im_temp, h, im_temp.size());

	// Extract four points from mouse data
	Point pts_dst[4];
	for (int i = 0; i < 4; i++)
	{
		pts_dst[i] = data.points[i];
	}

	// Black out polygonal area in destination image.
	fillConvexPoly(im_dst, pts_dst, 4, Scalar(0), CV_AA);

	// Add warped source image to destination image.
	im_dst = im_dst + im_temp;

	// Display image.
	imshow("Image", im_dst);
	waitKey(0);

	return 0;
}
*/

/*
// Homography 2 - not sure why this exists - https://github.com/spmallick/learnopencv/blob/master/Homography/homography2.cpp 
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct userdata{
Mat im;
vector<Point2f> points;
};


void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
if  ( event == EVENT_LBUTTONDOWN )
{
userdata *data = ((userdata *) data_ptr);
circle(data->im, Point(x,y),3,Scalar(0,255,255), 5, CV_AA);
imshow("Image", data->im);
if (data->points.size() < 4)
{
data->points.push_back(Point2f(x,y));
}
}

}



int main( int argc, char** argv)
{

// Read in the image.
Mat im_src = imread(argv[1]);
Size size = im_src.size();

// Create a vector of points.
vector<Point2f> pts_src;
pts_src.push_back(Point2f(0,0));
pts_src.push_back(Point2f(size.width - 1, 0));
pts_src.push_back(Point2f(size.width - 1, size.height -1));
pts_src.push_back(Point2f(0, size.height - 1 ));



// Destination image
Mat im_dst = imread(argv[2]);




//Create a window
namedWindow("Image", 1);

Mat im_temp = im_dst.clone();

userdata data;
data.im = im_temp;



//set the callback function for any mouse event
setMouseCallback("Image", mouseHandler, &data);

//show the image
imshow("Image", im_temp);
waitKey(0);

Mat tform = findHomography(pts_src, data.points);
warpPerspective(im_src, im_temp, tform, im_temp.size());

Point pts_dst[4];
for( int i = 0; i < 4; i++)
{
pts_dst[i] = data.points[i];
}


fillConvexPoly(im_dst, pts_dst, 4, Scalar(0), CV_AA);

im_dst = im_dst + im_temp;

imshow("Image", im_dst);
waitKey(0);

return 0;
}

*/


//Perspective Correction - https://github.com/spmallick/learnopencv/blob/master/Homography/perspective-correction.cpp
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

struct userdata{
	Mat im;
	vector<Point2f> points;
};

// This one creates a user interface where one must select the corners of the image. The order is topL > topR > bottomR > bottomL


void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		userdata *data = ((userdata *)data_ptr);
		circle(data->im, Point(x, y), 3, Scalar(0, 255, 255), 5, CV_AA);
		imshow("Image", data->im);
		if (data->points.size() < 4)
		{
			data->points.push_back(Point2f(x, y));
		}
	}

}



int main(int argc, char** argv)
{

	// Read in the image.
	Mat im_src = imread("C:/Users/wray/Pictures/Painting_Resized.jpg");

	// Destination image
	Size size(1294, 800);
	Mat im_dst = Mat::zeros(size, CV_8UC3);


	// Create a vector of points.
	vector<Point2f> pts_dst;



	pts_dst.push_back(Point2f(0, 0));
	pts_dst.push_back(Point2f(size.width - 1, 0));
	pts_dst.push_back(Point2f(size.width - 1, size.height - 1));
	pts_dst.push_back(Point2f(0, size.height - 1));



	//Create a window
	namedWindow("Image", 1);

	Mat im_temp = im_src.clone();

	userdata data;
	data.im = im_temp;



	//set the callback function for any mouse event
	setMouseCallback("Image", mouseHandler, &data);

	//show the image
	imshow("Image", im_temp);
	waitKey(0);

	Mat tform = findHomography(data.points, pts_dst);
	warpPerspective(im_src, im_dst, tform, size);

	imshow("Image", im_dst);
	waitKey(0);

	return 0;
}






/* The following takes the image on a book and moves it to another book via a hard-coded homography using the 4 points in a rectangle.
#include "opencv2/opencv.hpp" 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Read source image.
	Mat im_src = imread("C:/Users/wray/Pictures/book2.jpg");
	// Four corners of the book in source image
	vector<Point2f> pts_src;
	pts_src.push_back(Point2f(141, 131));
	pts_src.push_back(Point2f(480, 159));
	pts_src.push_back(Point2f(493, 630));
	pts_src.push_back(Point2f(64, 601));


	// Read destination image.
	Mat im_dst = imread("C:/Users/wray/Pictures/book1.jpg");
	// Four corners of the book in destination image.
	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(318, 256));
	pts_dst.push_back(Point2f(534, 372));
	pts_dst.push_back(Point2f(316, 670));
	pts_dst.push_back(Point2f(73, 473));

	// Calculate Homography
	Mat h = findHomography(pts_src, pts_dst);

	// Output image
	Mat im_out;
	// Warp source image to destination based on homography
	warpPerspective(im_src, im_out, h, im_dst.size());

	// Display images
	imshow("Source Image", im_src);
	imshow("Destination Image", im_dst);
	imshow("Warped Source Image", im_out);

	waitKey(0);
}
*/