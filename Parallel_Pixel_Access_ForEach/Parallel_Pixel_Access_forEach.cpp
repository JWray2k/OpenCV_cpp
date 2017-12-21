// Include OpenCV header
#include <opencv2/opencv.hpp>

#include <inttypes.h> // I needed this to solve an error I ran into with - typedef Point3_<uint8_t> Pixel




// Parallel Pixel Access in OpenCV using forEach - http://www.learnopencv.com/parallel-pixel-access-in-opencv-using-foreach/

	// Let us first define a function complicatedThreshold.It takes in an RGB pixel value and applies a complicated threshold to it.


	// Define a pixel 
	//typedef Point3_<uint8_t> Pixel;

	// C - Type - What are uint8_t, uint16_t, uint32_t and uint64_t? - https://www.badprog.com/c-type-what-are-uint8-t-uint16-t-uint32-t-and-uint64-t
	// It turns out that they are equal respectively to: unsigned char, unsigned short, unsigned int and unsigned long long.


	// Source code from https://github.com/spmallick/learnopencv/blob/master/forEach/forEach.cpp
	// Use cv and std namespaces
	using namespace cv;
	using namespace std;

	// Define a pixel
	typedef Point3_<uint8_t> Pixel; //Point3_ was set up as uint8_t - it releases with int as the type, but it can't run the dragon

	// tic is called to start timer
	void tic(double &t)
	{
		t = (double)getTickCount();
	}

	// toc is called to end timer
	double toc(double &t)
	{
		return ((double)getTickCount() - t) / getTickFrequency();
	}

	void complicatedThreshold(Pixel &pixel)
	{
		if (pow(double(pixel.x) / 10, 2.5) > 100)
		{
			pixel.x = 255;
			pixel.y = 255;
			pixel.z = 255;
		}
		else
		{
			pixel.x = 0;
			pixel.y = 0;
			pixel.z = 0;
		}
	}



	// Parallel execution with function object.
	struct Operator
	{
		void operator ()(Pixel &pixel, const int * position) const
		{
			// Perform a simple threshold operation
			complicatedThreshold(pixel);
		}
	};


	int main(int argc, char** argv)
	{
		// Read image
		Mat image = imread("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png"); // breaks for this
		// Mat image = imread("C:/Users/wray/Pictures/cards.png"); // breaks for this also

		// Scale image 30x
		resize(image, image, Size(), 30, 30);

		// Print image size
		cout << "Image size " << image.size() << endl;

		// Number of trials
		int numTrials = 5;

		// Print number of trials
		cout << "Number of trials : " << numTrials << endl;

		// Make two copies
		Mat image1 = image.clone();
		Mat image2 = image.clone();
		Mat image3 = image.clone();

		// Start timer
		double t;
		tic(t);

		for (int n = 0; n < numTrials; n++)
		{
			// Naive pixel access
			// Loop over all rows
			for (int r = 0; r < image.rows; r++)
			{
				// Loop over all columns
				for (int c = 0; c < image.cols; c++)
				{
					// Obtain pixel at (r, c)
					Pixel pixel = image.at<Pixel>(r, c);
					// Apply complicatedTreshold
					complicatedThreshold(pixel);
					// Put result back
					image.at<Pixel>(r, c) = pixel;
				}

			}
		}

		cout << "Naive way: " << toc(t) << endl; //dragon took 113.995

		// The above method is considered inefficient because the location of a pixel in memory is being calculated every 
		// time we call the at method.This involves a multiplication operation.The fact that the pixels are located in a 
		// contiguous block of memory is not used.


		// Start timer
		tic(t);

		// image1 is guaranteed to be continous, but
		// if you are curious uncomment the line below
		// cout << "Image 1 is continous : " << image1.isContinuous() << endl;

		for (int n = 0; n < numTrials; n++)
		{
			// Get pointer to first pixel
			Pixel* pixel = image1.ptr<Pixel>(0, 0);

			// Mat objects created using the create method are stored
			// in one continous memory block.
			const Pixel* endPixel = pixel + image1.cols * image1.rows;

			// Loop over all pixels
			for (; pixel != endPixel; pixel++)
			{
				complicatedThreshold(*pixel);
			}


		}
		cout << "Pointer Arithmetic " << toc(t) << endl;
		tic(t);

		// In OpenCV, all pixels in a row are stored in one continuous block of memory.If the Mat object is created using the create, 
		// ALL pixels are stored in one contiguous block of memory.Since we are reading the image from disk and imread uses the create 
		// method, we can simply loop over all pixels using simple pointer arithmetic that does not require a multiplication.

		for (int n = 0; n < numTrials; n++)
		{
			image2.forEach<Pixel>(Operator());
		}
		cout << "forEach : " << toc(t) << endl;

#if __cplusplus >= 201103L || (__cplusplus < 200000 && __cplusplus > 199711L)
		tic(t);

		for (int n = 0; n < numTrials; n++)
		{
			// Parallel execution using C++11 lambda.
			image3.forEach<Pixel>
				(
				[](Pixel &pixel, const int * position) -> void
			{
				complicatedThreshold(pixel);
			}
			);
		}
		cout << "forEach C++11 : " << toc(t) << endl;

#endif

		return EXIT_SUCCESS;
	}
