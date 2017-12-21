#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/plot.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// GLCM texture analysis using OpenCV C++ - http://answers.opencv.org/question/173045/glcm-texture-analysis-using-opencv-c/

void glcm(const Mat img, vector<double> &vec_energy)
{
	float energy = 0, contrast = 0, homogenity = 0, IDM = 0, entropy = 0, mean1 = 0;
	int row = img.rows, col = img.cols;
	cv::Mat gl = cv::Mat::zeros(256, 256, CV_32FC1);

	//creating glcm matrix with 256 levels,radius=1 and in the horizontal direction
	for (int i = 0; i<row; i++)
	for (int j = 0; j<col - 1; j++)
		gl.at<float>(img.at<uchar>(i, j), img.at<uchar>(i, j + 1)) = gl.at<float>(img.at<uchar>(i, j), img.at<uchar>(i, j + 1)) + 1;

	// normalizing glcm matrix for parameter determination
	gl = gl + gl.t();
	gl = gl / sum(gl)[0];


	for (int i = 0; i<256; i++)
	for (int j = 0; j<256; j++)
	{
		energy = gl.at<float>(i, j)*gl.at<float>(i, j);
		vec_energy.push_back(energy);

		//finding parameters
		contrast = contrast + (i - j)*(i - j)*gl.at<float>(i, j);
		homogenity = homogenity + gl.at<float>(i, j) / (1 + abs(i - j));
		if (i != j)
			IDM = IDM + gl.at<float>(i, j) / ((i - j)*(i - j));                      //Taking k=2;
		if (gl.at<float>(i, j) != 0)
			entropy = entropy - gl.at<float>(i, j)*log10(gl.at<float>(i, j));
		mean1 = mean1 + 0.5*(i*gl.at<float>(i, j) + j*gl.at<float>(i, j));
	}

	/*  for (int i = 0; i<256; i++)
	{
	for (int j = 0; j<256; j++)
	cout << a[i][j] << "\t";
	cout << endl;
	}*/


	cout << "energy=" << energy << endl;
	cout << "contrast=" << contrast << endl;
	cout << "homogenity=" << homogenity << endl;
	cout << "IDM=" << IDM << endl;
	cout << "entropy=" << entropy << endl;
	cout << "mean=" << mean1 << endl;
}


int main(int argc, char** argv)
{


	Mat img = cv::imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png", CV_LOAD_IMAGE_UNCHANGED); // was argv[1]

	if (img.empty())
	{
		cout << "can not load " << argv[1];
		return 1;
	}
	imshow("Image", img);

	vector<double> vec_energy;
	glcm(img, vec_energy);   //call to glcm function

	Mat mat_energy = Mat(vec_energy);

	Mat plot_result;

	Ptr<plot::Plot2d> plot = plot::Plot2d::create(mat_energy);
	plot->setShowGrid(false);
	plot->setShowText(false);
	plot->render(plot_result);

	imshow("plot", plot_result);
	cv::waitKey(0);
	return 0;
}






//double glcm(Mat &image)
//{
//	float energy = 0, contrast = 0, homogenity = 0, IDM = 0, entropy = 0, mean1 = 0;
//	int row = img.rows, col = img.cols;
//	cv::Mat gl = cv::Mat::zeros(256, 256, CV_32FC1);
//
//	//creating glcm matrix with 256 levels,radius=1 and in the horizontal direction
//	for (int i = 0; i<row; i++)
//	for (int j = 0; j<col - 1; j++)
//		gl.at<float>(img.at<uchar>(i, j), img.at<uchar>(i, j + 1)) = gl.at<float>(img.at<uchar>(i, j), img.at<uchar>(i, j + 1)) + 1;
//
//	// normalizing glcm matrix for parameter determination
//	gl = gl + gl.t();
//	gl = gl / sum(gl)[0];
//
//	double asm = 0;  //angular second moment
//	for (int i = 0; i<256; i++)
//	for (int j = 0; j<256; j++)
//	{
//		asm = asm + gl.at<float>(i, j)*gl.at<float>(i, j); //!!!!!
//		energy = gl.at<float>(i, j)*gl.at<float>(i, j);
//		a[i][j] = energy;
//		//        cout<<energy;
//		//finding parameters
//		contrast = contrast + (i - j)*(i - j)*gl.at<float>(i, j);
//		homogenity = homogenity + gl.at<float>(i, j) / (1 + abs(i - j));
//		if (i != j)
//			IDM = IDM + gl.at<float>(i, j) / ((i - j)*(i - j));                      //Taking k=2;
//		if (gl.at<float>(i, j) != 0)
//			entropy = entropy - gl.at<float>(i, j)*log10(gl.at<float>(i, j));
//		mean1 = mean1 + 0.5*(i*gl.at<float>(i, j) + j*gl.at<float>(i, j));
//	}
//	energy = sqrt(asm); //!!!!!
//
//	for (int i = 0; i<256; i++)
//	{
//		for (int j = 0; j<256; j++)
//			cout << a[i][j] << "\t";
//		cout << endl;
//	}
//
//
//
//
//
//
//	cout << "energy=" << energy << endl;
//	cout << "contrast=" << contrast << endl;
//	cout << "homogenity=" << homogenity << endl;
//	cout << "IDM=" << IDM << endl;
//	cout << "entropy=" << entropy << endl;
//	cout << "mean=" << mean1 << endl;
//}
//
//void MainWindow::on_pushButton_2_clicked()
//{
//	Mat im = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE); //!!!! GLCM works ONLY on grayscale images
//	Mat dest(im.rows, im.cols, CV_64F);
//	for (int y = 8; y<im.rows - 8; y++)
//	for (int x = 8; x<im.cols - 8; x++){
//		Mat subimg = Mat(img, Rect(x - 8, y - 8, 16, 16));  //take a 16x16 subimage
//		double feature = GLCM(subimg);  //get the energy (or other feature) for this window
//		dest.at<double>(y, x) = feaure;
//	}
//
//	glcm(img);                                                                      //call to glcm function
//	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE);
//	imshow("Image", img);
//	cv::Mat d = cv::Mat(256, 256, CV_8U, &a, 2); //this is just a try to convert a as mat
//	cv::namedWindow("Result", CV_WINDOW_AUTOSIZE);
//	cv::imshow("Result", d);
//
//	cv::waitKey(1);
//}