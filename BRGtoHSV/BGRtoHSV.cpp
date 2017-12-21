#define _CRT_SECURE_NO_DEPRECATE //https://stackoverflow.com/questions/14386/fopen-deprecated-warning - must come before #includes
#include <iostream>

#include <opencv2/opencv.hpp>

// BGR to HSV and back again - https://stackoverflow.com/questions/35737942/bgr-to-hsv-and-back-again

/* Original source - didn't work
cv::Vec3b HSVtoBGR(const cv::Vec3f& hsv)
{
	cv::Vec3f hsvAdjusted = hsv; // If we use this directly, the output is (0,0,1) which is very wrong
	//hsvAdjusted[0] *= 360.; // If we do this to bring all of the values into the range (0,1), the output is (0,1,0), which is also very wrong

	// If we do this to bring all of the values into the range (0,255), the output is (0,0,200), which is still very wrong
	hsvAdjusted[1] *= 255. / 360.;
	hsvAdjusted[1] *= 255.;
	hsvAdjusted[2] *= 255.;

	cv::Mat_<cv::Vec3f> hsvMat(hsvAdjusted);

	cv::Mat_<cv::Vec3f> bgrMat;

	cv::cvtColor(hsvMat, bgrMat, CV_HSV2BGR);

	cv::Vec3b bgr = static_cast<cv::Vec3b>(bgrMat(0, 0));
	return bgr;
}
*/
cv::Vec3b HSVtoBGR(const cv::Vec3f& hsv)
{
	cv::Mat_<cv::Vec3f> hsvMat(hsv);
	cv::Mat_<cv::Vec3f> bgrMat;

	cv::cvtColor(hsvMat, bgrMat, CV_HSV2BGR);

	bgrMat *= 255; // Upscale after conversion

	// Conversion to Vec3b is handled by OpenCV, no need to static_cast
	return bgrMat(0);
}

/** Input 0 <= B <= 255, 0 <= G <= 255, 0 <= R <= 255
* Output 0 <= H <= 360, 0 <= S <= 1, 0 <= V <= 1  */
cv::Vec3f BGRtoHSV(const cv::Vec3b& bgr)
{
	cv::Mat3f bgrMat(static_cast<cv::Vec3f>(bgr));

	bgrMat *= 1. / 255.;

	cv::Mat3f hsvMat;
	cv::cvtColor(bgrMat, hsvMat, CV_BGR2HSV);

	cv::Vec3f hsv = hsvMat(0, 0);

	return hsv;
}

int main()
{
	// Create a BGR color
	cv::Vec3b bgr(5, 100, 200);
	std::cout << "bgr: " << bgr << std::endl;

	// Convert BGR to HSV
	cv::Vec3f hsv = BGRtoHSV(bgr);
	std::cout << "hsv: " << hsv << std::endl; // outputs // (29.23, .976, .7843), which seems correct

	// Convert back from HSV to BGR
	cv::Vec3b bgr2 = HSVtoBGR(hsv);
	std::cout << "bgr2: " << bgr2 << std::endl;

	return 0;
}