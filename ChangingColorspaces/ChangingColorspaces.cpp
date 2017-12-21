#include <opencv2\opencv.hpp>
// import numpy as np
/*
Numpy is the core library for scientific computing in Python. It provides a high-performance multidimensional array object, 
and tools for working with these arrays. If you are already familiar with MATLAB, you might find this tutorial 
http://scipy.github.io/old-wiki/pages/NumPy_for_Matlab_Users useful to get started with Numpy.

http://www.numpy.org/

*/

// Reading and Writing Video - VideoCapture - http://docs.opencv.org/3.0-beta/modules/videoio/doc/reading_and_writing_video.html 


using namespace cv;

int main(int, char**)
{
	// Original Python code - cap = cv2.VideoCapture(0) 
	VideoCapture cap("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-%02d.png"); // open the default camera - no camera available however... nothing shows up

	// Original Python code -  while (1) - this is a loop which keeps going while its set to true... perhaps for the vid feed?
	if (!cap.isOpened())  // check if we succeeded - this checks if the video capture has succeeded.
		return -1;

	Mat HSV;
	namedWindow("HSV", 1);
	for (;;)
	{
		Mat frame;  // This makes a new Mat for every run of the for loop

		// Take each frame
		// Original Python code -  _, frame = cap.read()
		cap >> frame; // get a new frame from camera

		// Convert BGR to HSV
		// Original Python code - hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
		cvtColor(frame, HSV, COLOR_BGR2HSV);

		// define range of blue color in HSV
		// Original Python code - lower_blue = np.array([110, 50, 50])
		Scalar hsv_lower_blue(110, 50, 50); // Referenced from https://stackoverflow.com/questions/35522599/hsv-image-in-opencv-for-color-recognition 

		// Original Python code - upper_blue = np.array([130, 255, 255])
		Scalar hsv_upper_blue(130, 255, 255);

		// Threshold the HSV image to get only blue colors
		// Original Python code - mask = cv2.inRange(hsv, lower_green, upper_green)
		inRange(frame, hsv_lower_blue, hsv_upper_blue, HSV);

		/*
		GaussianBlur()
		C++: void GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )

		Parameters:
			- src – input image; the image can have any number of channels, which are processed independently, but the depth should be 
					CV_8U, CV_16U, CV_16S, CV_32F or CV_64F.
			- dst – output image of the same size and type as src.
			- ksize – Gaussian kernel size. ksize.width and ksize.height can differ but they both must be positive and odd. Or, they 
					  can be zero’s and then they are computed from sigma*.
			- sigmaX – Gaussian kernel standard deviation in X direction.
			- sigmaY – Gaussian kernel standard deviation in Y direction; if sigmaY is zero, it is set to be equal to sigmaX, if both 
					   sigmas are zeros, they are computed from ksize.width and ksize.height , respectively (see getGaussianKernel() for details); 
					   to fully control the result regardless of possible future modifications of all this semantics, it is recommended to specify 
					   all of ksize, sigmaX, and sigmaY.
			- borderType – pixel extrapolation method (see borderInterpolate() for details).

		The function convolves the source image with the specified Gaussian kernel. In-place filtering is supported.
		http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html?highlight=gaussianblur#gaussianblur
		*/
	
		GaussianBlur(frame, HSV, Size(7, 7), 1.5, 1.5); // Blurs an image using a Gaussian filter.

		/* Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

		   The Canny Edge detector was developed by John F. Canny in 1986. Also known to many as the optimal detector, Canny algorithm aims to satisfy three main criteria:
				- Low error rate: Meaning a good detection of only existent edges.
				- Good localization: The distance between edge pixels detected and real edge pixels have to be minimized.
				- Minimal response: Only one detector response per edge.
		*/

		Canny(frame, HSV, 0, 30, 3);

		imshow("HSV", HSV);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}






	// Bitwise - AND mask and original image
	// Original Python code - res = cv2.bitwise_and(frame, frame, mask = mask)

	// Original Python code - cv2.imshow('frame', frame)
	// Original Python code - cv2.imshow('mask', mask)
	// Original Python code - cv2.imshow('res', res)
	// Original Python code - k = cv2.waitKey(5) & 0xFF
	// Original Python code - if k == 27:
	// Original Python code - break

	// Original Python code - cv2.destroyAllWindows()