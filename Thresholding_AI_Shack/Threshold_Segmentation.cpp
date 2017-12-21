/*
Thresholding - http://aishack.in/tutorials/thresholding/

Introduction
Thresholding is one of the most basic techniques for what is called Image Segmentation. When you threshold an image, you get segments 
inside the image... each representing something. For example... complex segmentation algorithms might be able to segment out "house-like" 
structures in an image.

With thresholding, you can segment the image based on colour. For example, you can segment all red colour in an image.

Our Goal
Here's what we'll be making in this tutorial: an application that will threshold out the red regions in an image... something like this:

You might ask, exactly why go through all the trouble of thresholding an image? The reason for this is: The thresholded image is easier 
for the computer to analyse. It has got clear, stark boundaries, so the computer can easily find the boundary of each region (each of 
which represents a red patch).

Remember, thresholding isn't the best algorithm (as you'll see later)... but it gives reasonably good results for many images (as you 
can see in the image above).
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main()
{
	IplImage* img = cvLoadImage("C:/Users/wray/Pictures/DragonAndBunnies/DragonsAndBunnies_5x5_ap3/dragons-01.png");

	// Now, we'll create 3 grayscale images. These 3 images will hold the red, green and blue channels of the image img. 
	IplImage* channelRed = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* channelGreen = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* channelBlue = cvCreateImage(cvGetSize(img), 8, 1);
	// The cvCreateImage function allocates memory for an image. Currently, all these images are the same blank... each pixel is black.

	// Now, we'll copy each channel into these images... one at a time:
	cvSplit(img, channelBlue, channelGreen, channelRed, NULL);
	// The cvSplit command split the 3 channel image img into three different channels. The order is Blue, Green and Red because thats how its stored in memory.

	// We're interested in finding out the red regions... so we'll focus on the red channel

	// You'll notice that not just the red regions are bright in the red channel... even the white regions are bright (because white=red+green+blue... 
	// so white is seen in all three channels).

	// To extract actual red areas, we remove areas common with the green and blue channels... something like this: 
	// Just red = red channel - (green channel + blue channel). And we implement this in code using the following commands:

	cvAdd(channelBlue, channelGreen, channelGreen); // adds the green channel to the blue channel and stores the result in channelGreen

	cvSub(channelRed, channelGreen, channelRed); // removes the green channel from the red channel and then stores the result in channelRed

	// Basically, what we've done is add the blue and green channels, and store the result in the green channel. Then, subtract this green channel from 
	// the red channel. And store the result in red channel.

	// Now, we do the thresholding. 
	cvThreshold(channelRed, channelRed, 20, 255, CV_THRESH_BINARY);

	// See how the red region turn bright? This is segmentation. You create segments... White=red in the original image... black=non red.

	// Now, an analysis of what actually happens in the cvThreshold function. The function goes through each pixel of the image. If the value of 
	// the pixel is greater than 20 (the third parameter)... it changes it to a 255 (the fourth parameter). Thats the reason all the reds turn bright.

	// The first parameter is the source image. The second is the destination image.

	// And see the last parameter? (CV_THRESH_BINARY) This parameter decides the behaviour of this function. CV_THRESH_BINARY change the value to 255 
	// when the value is greater than 20. An other possible value is: CV_THRESH_BINARY_INV... which is the reverse: if the value is greater than 20, 
	// it is set it 0... else it is set to 255.

	// Now that we've thresholded the image, all we need to do is display it.

	cvNamedWindow("original");
	cvNamedWindow("red");
	cvShowImage("original", img);
	cvShowImage("red", channelRed);
	cvWaitKey(0);
	return 0;
}

// Thresholding isn't that great, but it lets you get through a lot of situations quite easily(and efficiently).

// You do get some greenish areas, but none of them isn't prominent enough.. and would probably be discarded as noise.

// In such situations, you could try converting the image into HSV colour space. And then, segmenting the H channel. You'd have a narrow range of 
// hues for green... to do that, you'd have to use the cvInRangeS function... which is described below.

/*
Advanced Thresholding
The cvThreshold function is good for simple purposes... places where you just need to check if a pixel's value is greater or less than a particular value.

More powerful functions are the cvCmp, cvCmpS, inRange and inRangeS function. The functions ending in an S let you compare the image against a 
particular value (like 20). The others let you compare the image against another image (so you could have different comparison values for different pixels).

The cvCmp function lets you specify the type of comparison (greater than, greater or equal, etc). Its syntaxes are:

cvCmp(src1, src2, dst, cmp_op);
cvCmpS(src, value, dst, cmp_op);

cmp_op can take these values:

CV_CMP_EQ - equal to
CV_CMP_GT - greater than
CV_CMP_GE - greater or equal
CV_CMP_LT - less than
CV_CMP_LE - less or equal
CV_CMP_NE - not equal to

The inRange function lets you specify a range of values (a minimum and a maximum) which is converted to a 255. Any value outside the range is 
set to 0. Its syntaxes are:

cvInRange(src1, lowersrc, uppersrc, dst);
cvInRangeS(src, scalarLower, scalarHigher, dst);

Pretty much self explanatory. This function can be used to threshold out an HSV image... where the H channel holds the colour image. 
I've written a tutorial that uses these function, you might be interested in reading it: Tracking coloured objects.
*/