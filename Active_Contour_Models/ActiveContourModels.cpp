// Active Contour Models in OpenCV 3.0 - https://stackoverflow.com/questions/32894542/active-contour-models-in-opencv-3-0

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

// config_eext represents a Matlab function to calculate External Energy
Mat config_eext(float wl, float we, float wt, Mat image) 
{
	Mat eline, gradx, grady, img_gray, eedge;

	//bitdepth defined as CV_32F
	image.convertTo(img_gray, bitdepth);

	//Convolution Kernels
	Mat m1, m2, m3, m4, m5;
	m1 = (Mat_<float>(1, 2) << 1, -1);
	m2 = (Mat_<float>(2, 1) << 1, -1);
	m3 = (Mat_<float>(1, 3) << 1, -2, 1);
	m4 = (Mat_<float>(3, 1) << 1, -2, 1);
	m5 = (Mat_<float>(2, 2) << 1, -1, -1, 1);

	img_gray.copyTo(eline);

	//Kernels de gradiente
	Mat kernelx = (Mat_<float>(1, 3) << -1, 0, 1);
	Mat kernely = (Mat_<float>(3, 1) << -1, 0, 1);

	//Gradiente em x e em y
	filter2D(img_gray, gradx, -1, kernelx);
	filter2D(img_gray, grady, -1, kernely);

	//Edge Energy como definido por Kass
	eedge = -1 * (gradx.mul(gradx) + grady.mul(grady));

	//Termination Energy Convolution
	Mat cx, cy, cxx, cyy, cxy, eterm(img_gray.rows, img_gray.cols, bitdepth), cxm1, den, cxcx, cxcxm1, cxcxcy, cxcycxy, cycycxx;
	filter2D(img_gray, cx, bitdepth, m1);
	filter2D(img_gray, cy, bitdepth, m2);
	filter2D(img_gray, cxx, bitdepth, m3);
	filter2D(img_gray, cyy, bitdepth, m4);
	filter2D(img_gray, cxy, bitdepth, m5);

	//element wise operations to find Eterm
	cxcx = cx.mul(cx);
	cxcx.convertTo(cxcxm1, -1, 1, 1);
	den = cxcxm1 + cy.mul(cy);
	cv::pow(den, 1.5, den);
	cxcxcy = cxcx.mul(cy);
	cxcycxy = cx.mul(cy);
	cxcycxy = cxcycxy.mul(cxy);
	cycycxx = cy.mul(cy);
	cycycxx = cycycxx.mul(cxx);
	eterm = (cxcxcy - 2 * cxcycxy + cycycxx);
	cv::divide(eterm, den, eterm, -1);

	//Image energy
	Mat eext;
	eext = wl*eline + we*eedge + wt*eterm;
	return eext;
}
/*
did you compare your gradient image with the matlab gradient image? – Micka Oct 1 '15 at 18:43

filter2D performs correlation in OpenCV. In MATLAB, conv2 performs convolution. The difference is that the kernel 
is not 180 degree rotated for filter2D where MATLAB does this. If you want to achieve the same results for filter2D 
and OpenCV, you need to rotate the kernel by 180 degrees in OpenCV. With regards to that, Micka is correct in that you
should compare your gradient images. They are probably not the same. imgradient uses the Sobel kernel for the gradient 
so make sure your kernel is correct in OpenCV. – rayryeng Oct 1 '15 at 18:51

The gradient image are indeed different. By rotating 180 degrees you mean using [1 -1] instead of [-1 1]? – Andrei Oct 1 '15 at 19:47

@Andrei Sorry to get back to you so late. Yes that's correct. In OpenCV, you can achieve a 180 degree rotation by transposing first, 
then flipping along the columns, so use cv::transpose, then cv::flip with the flipCode being set to 0. – rayryeng Aug 10 at 21:29
*/