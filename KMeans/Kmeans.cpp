// K-Means Clustering - http://answers.opencv.org/question/27808/how-can-you-use-k-means-clustering-to-posterize-an-image-using-c/
// Doesn't work... come back to later

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(int argc, char** argv)
{
	Mat src = imread(argv[1], 1);
	// Mat src = imread("C: / Users / wray / Pictures / DragonAndBunnies / DragonsAndBunnies_5x5_ap3 / dragons - 01.png");
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	for (int z = 0; z < 3; z++)
		samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y, x)[z];


	int clusterCount = 15;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);


	Mat new_image(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
	for (int x = 0; x < src.cols; x++)
	{
		int cluster_idx = labels.at<int>(y + x*src.rows, 0);
		new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
		new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
		new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
	}
	imshow("clustered image", new_image);
	waitKey(0);
}


// BELOW FOR CLUSTER GENERATION EXAMPLE: 









#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

//An example on K-means clustering - http://docs.opencv.org/3.1.0/de/d63/kmeans_8cpp-example.html 

using namespace cv;
using namespace std;

// static void help()
// {
//     cout << "\nThis program demonstrates kmeans clustering.\n"
//             "It generates an image with random points, then assigns a random number of cluster\n"
//             "centers and uses kmeans to move those cluster centers to their representitive location\n"
//             "Call\n"
//             "./kmeans\n" << endl;
// }
int main(int /*argc*/, char** /*argv*/)
{
	const int MAX_CLUSTERS = 5;
	Scalar colorTab[] =
	{
		Scalar(0, 0, 255),
		Scalar(0, 255, 0),
		Scalar(255, 100, 100),
		Scalar(255, 0, 255),
		Scalar(0, 255, 255)
	};
	Mat img(500, 500, CV_8UC3);
	//Mat img("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); //arguement type... (const char [81])... doesn't work
	RNG rng(12345);
	for (;;)
	{
		int k, clusterCount = rng.uniform(2, MAX_CLUSTERS + 1);
		int i, sampleCount = rng.uniform(1, 1001);
		Mat points(sampleCount, 1, CV_32FC2), labels;
		clusterCount = MIN(clusterCount, sampleCount);
		Mat centers;
		/* generate random sample from multigaussian distribution */
		for (k = 0; k < clusterCount; k++)
		{
			Point center;
			center.x = rng.uniform(0, img.cols);
			center.y = rng.uniform(0, img.rows);
			Mat pointChunk = points.rowRange(k*sampleCount / clusterCount,
				k == clusterCount - 1 ? sampleCount :
				(k + 1)*sampleCount / clusterCount);
			rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
		}
		randShuffle(points, 1, &rng);
		kmeans(points, clusterCount, labels,
			TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);
		img = Scalar::all(0);
		for (i = 0; i < sampleCount; i++)
		{
			int clusterIdx = labels.at<int>(i);
			Point ipt = points.at<Point2f>(i);
			circle(img, ipt, 2, colorTab[clusterIdx], FILLED, LINE_AA);
		}
		imshow("clusters", img);
		char key = (char)waitKey();
		if (key == 27 || key == 'q' || key == 'Q') // 'ESC'
			break;
	}
	return 0;
}


/*
K-means & Image Segmentation - Computerphile - https://www.youtube.com/watch?v=yR7k19YBqiw 

Dr. Mike's Code:
% This script is the one mentioned during the Computerphile Image
% Segmentation video. I chose matlab because it's a popular tool for
% quickly prototyping things. Matlab licenses are pricey, if you don't have
% one (or, like me, work for an organisation that does) try Octave as a
% good free alternative. This code should work in Octave too.

% Load in an input image
im = imread('C:\Path\Of\Input\Image.jpg');

% In matlab, K-means operates on a 2D array, where each sample is one row,
% and the features are the columns. We can use the reshape function to turn
% the image into this format, where each pixel is one row, and R,G and B
% are the columns. We are turning a W,H,3 image into W*H,3

% We also cast to a double array, because K-means requires it in matlab
imflat = double(reshape(im, size(im,1) * size(im,2), 3));

% I specify that initialisation shuold sample points at
% random, rather than anything complex like kmeans++ initialisation.
% Kmeans++ takes a long time if you are using 256 classes.

% Perform k-means. This function returns the class IDs assigned to each
% pixel, and in this case we also want the mean values for each class -
% what colour is each class. This can take a long time if the value for K
% is large, I've used the sampling start strategy to speed things up.

% While KMeans is running, it will show you the iteration count, and the
% number of pixels that have changed class since last iteration. This
% number should get lower and lower, as the means settle on appropriate
% values. For large K, it's unlikely that we will ever reach zero movement
% (convergence) within 150 iterations.
K = 3
[kIDs, kC] = kmeans(imflat, K, 'Display', 'iter', 'MaxIter', 150, 'Start', 'sample');

% Matlab can output paletted images, that is, grayscale images where the
% colours are stored in a separate array. This array is kC, and kIDs are
% the grayscale indices.
colormap = kC / 256; % Scale 0-1, since this is what matlab wants

% Reshape kIDs back into the original image shape
imout = reshape(uint8(kIDs), size(im,1), size(im,2));

% Save file out, you need to subtract 1 from the image classes, since once
% stored in the file the values should go from 0-255, not 1-256 like matlab
% would do.
imwrite(imout - 1, colormap, 'C:\Path\Of\Output\Image.png');
*/