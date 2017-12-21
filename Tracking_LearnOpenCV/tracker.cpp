// Referenced from Object Tracking using OpenCV (C++/Python) - https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
// code - https://github.com/spmallick/learnopencv/tree/master/tracking

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>


using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
	(std::ostringstream() << std::dec << x)).str()



int main(int argc, char **argv)
{
	// List of tracker types in OpenCV 3.2
	// NOTE : GOTURN implementation is buggy and does not work.
	const char *types[] = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN" }; // unsure how to run through this, hardcore the create for now...
	vector <string> trackerTypes(types, std::end(types));

	// Create a tracker
	// string trackerType = trackerTypes[2]; // This literally just pulls a name from the types string array
	string trackerType = "BOOSTING";
	// Ptr<Tracker> tracker = TrackerKCF::create(); // KCF syntax
	// Ptr< TrackerBoosting > 	create(); //Boosting syntax
	Ptr<TrackerBoosting> tracker = TrackerBoosting::create(); // not terrible performance
	// Ptr<Tracker> tracker = Tracker::create("BOOSTING"); // unable to use create for some reason...

	// Read video
	VideoCapture video("C:/Users/wray/videos/chaplin.mp4");

	// Exit if video is not opened
	if (!video.isOpened())
	{
		cout << "Could not read video file" << endl;
		return 1;

	}

	// Read first frame
	Mat frame;
	bool ok = video.read(frame);

	// Define initial boundibg box
	Rect2d bbox(287, 23, 86, 320);

	// Uncomment the line below to select a different bounding box
	// bbox = selectROI(frame, false);

	// Display bounding box.
	rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	imshow("Tracking", frame);

	tracker->init(frame, bbox);

	while (video.read(frame))
	{

		// Start timer
		double timer = (double)getTickCount();

		// Update the tracking result
		bool ok = tracker->update(frame, bbox);

		// Calculate Frames per second (FPS)
		float fps = getTickFrequency() / ((double)getTickCount() - timer);


		if (ok)
		{
			// Tracking success : Draw the tracked object
			rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
		}
		else
		{
			// Tracking failure detected.
			putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}

		// Display tracker type on frame
		putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display FPS on frame
		putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display frame.
		imshow("Tracking", frame);

		// Exit if ESC pressed.
		int k = waitKey(1);
		if (k == 27)
		{
			break;
		}

	}



}