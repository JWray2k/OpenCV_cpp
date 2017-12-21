/*
I'm trying to create a small program that allows you to capture frames from 2 to 2 seconds of a web camera in real time.     
My code is working, the problem is to record the images, only one image is recorded. My idea was to have several files 
stored in the directory of images, with the name years months days hours min sec It is possible to do this with cvSaveImage, 
or is otherwise more effective to do this
*/

sprintf(file, "%s", "/home/ricardo/Imagens/");
sprintf(file, "%d%d%d_%d%d%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
cvSaveImage(file, frame, 0);



#include stdio.h
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include<time.h>


int main() {


	char file[256];


	time_t mytime;
	mytime = time(NULL);
	struct tm tm = *localtime(&mytime);

	printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	sprintf(file, "%d%d%d_%d%d%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	/*
	"only one image is recorded"

	the problem is here:

	sprintf(file,"%d%d%d_%d%d%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	it has a minimum resolution of 1 second, so all images taken in that timespan get saved under the same name.

	so, either

	1. Add the clock() time:

		sprintf(file,"%d%d%d_%d%d%d_%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, clock() );
	
	2. Or maybe a counter:

		// somwhere out of the loop

		int counter = 0;

		// in the loop:

		sprintf(file,"%d%d%d_%d%d%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, counter); 
		counter++;


	Each of your sprintf calls overwrites your previous attempt, it does not work like strcat you've got to put it all into one call:

	sprintf(file,"%s%d%d%d_%d%d%d.pgm\n","D:/Projects/vs2010/text11/" tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	*/


	printf("%s", file);

	getchar();

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if (!capture) {
		fprintf(stderr, "ERROR: capture is NULL \n");
		getchar();
		return -1;
	}

	cvNamedWindow("mywindow", CV_WINDOW_AUTOSIZE);
	while (1) {
		IplImage* frame = cvQueryFrame(capture);
		if (!frame) {

			fprintf(stderr, "ERROR: frame is null...\n");
			getchar();
			break;
		}
		cvShowImage("mywindow", frame);
		sprintf(file, "%s", "/home/ricardo/Imagens/");
		sprintf(file, "%d%d%d_%d%d%d.pgm\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		cvSaveImage(file, frame, 0);

		//sleep(2);
		// Do not release the frame!
		//If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
		//remove higher bits using AND operator
		if ((cvWaitKey(10) & 255) == 27) break;
	}
	// Release the capture device housekeeping
	cvReleaseCapture(&capture);
	cvDestroyWindow("mywindow");
	return 0;
}