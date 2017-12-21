#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main()
{
	// Random initialize A with values in [-100, 100]
	Mat1d A(1000, 1000);
	randu(A, Scalar(-100), Scalar(100));

	// B initialized with some constant (5) value
	Mat1d B(A.rows, A.cols, 5.0);

	// Operation: B(A>0) = 0;

	{
		// Using mask

		double tic = double(getTickCount());
		B.setTo(0, A > 0);
		double toc = (double(getTickCount()) - tic) * 1000 / getTickFrequency();

		cout << "Mask: " << toc << endl;
	}
	{
		// Using for loop

		double tic = double(getTickCount());
		for (int r = 0; r < B.rows; ++r)
		{
			double* pA = A.ptr<double>(r);
			double* pB = B.ptr<double>(r);
			for (int c = 0; c < B.cols; ++c)
			{
				if (pA[c] > 0.0) pB[c] = 0.0;
			}
		}
		double toc = (double(getTickCount()) - tic) * 1000 / getTickFrequency();

		cout << "Loop: " << toc << endl;
	}


	getchar();
	return 0;
}

/* Same code in Matlab
Matlab Code

% Random initialize A with values in[-100, 100]
A = (rand(1000) * 200) - 100;
% B initialized with some constant(5) value
B = ones(1000) * 5;

tic
B(A>0) = 0;
toc
*/
