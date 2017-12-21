//From https://gist.github.com/yiling-chen/6227670
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

	Mat m = Mat::ones(5, 5, CV_8UC1);
	cout << m << endl << endl;

	Mat integ;
	integral(m, integ);

	cout << integ << endl;

	return 0;
}