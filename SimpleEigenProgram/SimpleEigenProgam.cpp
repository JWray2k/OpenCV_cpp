#include <iostream>
// #include <Eigen/Dense>
#include "C:/Users/wray/Desktop/EIGENDIR/Eigen/Dense" // this file pulls in a bunch of other header files... look below for contents -
using Eigen::MatrixXd;

void pause()   {
	system("pause"); // Beautiful implementation that doesn't need openCV's waitkey funcion - https://stackoverflow.com/questions/193469/how-to-make-visual-studio-pause-after-executing-a-console-app-in-debug-mode 
}

int main()
{
	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;
	atexit(pause);
}

/*
Dense Contains the following: 

		#include "Core"
		#include "LU"
		#include "Cholesky"
		#include "QR"
		#include "SVD"
		#include "Geometry"
		#include "Eigenvalues"

*/