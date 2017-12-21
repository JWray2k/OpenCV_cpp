// Referenced from Learning Image Processing with OpenCV - https://books.google.de/books?id=Y_irBwAAQBAJ&pg=PA117&lpg=PA117&dq=cvtcolor+cielab+opencv+c%2B%2B&source=bl&ots=xlcxF9D28w&sig=FGGa93oFRbWsSWOPl_6a--kmomU&hl=en&sa=X&ved=0ahUKEwiYpejt3rHWAhVBuRQKHV-6DFgQ6AEIaDAJ#v=onepage&q=cvtcolor%20cielab%20opencv%20c%2B%2B&f=false

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char** argv){

	//Load the image
	Mat image = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");
	imshow("Picture", image);

	//transform to CIE Lab
	cvtColor(image, image, COLOR_BGR2Lab);

	vector<Mat> channels;

	split(image, channels);

	//show channels in gray scale
	namedWindow("L channel (gray)", WINDOW_AUTOSIZE);
	imshow("L channel (gray)", channels[0]);
	namedWindow("L channel (gray)", WINDOW_AUTOSIZE);
	imshow("a channel (gray)", channels[1]);
	namedWindow("L channel (gray)", WINDOW_AUTOSIZE);
	imshow("b channel (gray)", channels[2]);

	namedWindow("CIE Lab image (all channels)", WINDOW_AUTOSIZE);
	imshow("CIE Lab image", image);

	waitKey(0);

	return 0;
}