// Bicubic interpolation using openCV - https://stackoverflow.com/questions/9613335/bicubic-interpolation-using-opencv 
// Need to rework cvResize()

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "iostream"

using namespace std;
int main(int argc, char* argv) {
	IplImage* img = NULL;
	IplImage* new_img = NULL;
	img = cvLoadImage("C:/Users/wray/Pictures/church01.jpg");
	if (!img){
		return -1;
	}
	new_img = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
	cvResize(img, new_img, size(), 2.0f, 0.0f, CV_INTER_CUBIC);
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvShowImage("original", img);
	cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	cvShowImage("result", new_img);

	CvSize dim = cvGetSize(new_img);
	cout << " dimensions:: height:" << dim.height << " width:" << dim.width << endl;

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&new_img);
	cvDestroyWindow("result");
	return 0;
}