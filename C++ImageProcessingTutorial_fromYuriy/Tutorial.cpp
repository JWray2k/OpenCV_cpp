#include "PgmIO.h"
#include "PpmIO.h"
#include <

// Can't get the code saved because there is Unicode below...

// Tutorial - Image processing with C / C++ - Marc Schlipsing and Jan Salmen - https://www.ppgia.pucpr.br/~facon/ComputerVisionBooks/2010ImageProcessingWithCC++.pdf 

// In general, two - dimensional images are stored in a linear memory buffer, so that all
// image rows are placed in memory consecutively.Depending on the size of the image
// and the bit - resolution for each pixel one will need to allocate an adequate amount of
// memory.The position of the image in memory is stored in a pointer which refers to the
// first pixel.In the case of an 8 - bit gray - value image one may use a pointer of the unsigned
// 8 - bit type.

// readPGM definition.... I think that this is probably within the header file I am having trouble implementing.
bool readPGM(const char ∗ fileName, unsigned char ∗∗ ppData, int &width, int &height);

// to receive an image from a .pgm-file. Given a file name, the method will try opening
// the file, allocate the right amount of memory, return the image position by setting our
// pointer pImage and the size parameters. This call shows two ways in which a function
// can write on the input parameters.For the memory position we pass the address to our
// pointer. The sizes width and height are handed over by - reference.In both cases one
// can write the outside variable from inside the function.


static void filterGauss3x1(unsigned char ∗ pImgDst, const unsigned char ∗ pImgSrc, const int width, const int height)
{
	for(int y = 0; y < height; ++y)
	{
		unsigned char ∗ pDst = pImgDst + y ∗ width + 1;
		const unsigned char ∗ pSrc = pImgSrc + y ∗ width;
		
			for(int x = 1; x < width−1; ++x)
			{
			const unsigned int sum = (int) pSrc[0] + 2∗(int) pSrc[1] + (int) pSrc[2];
			pDst = sum / 4;
			++pDst;
			++pSrc;
			}
		}
	};

static void filterGauss3x1(unsigned char ∗ pImgDst, const unsigned char ∗ pImgSrc, const int width, const int height)
{
	for(int y = 1; y < height −1; ++y)
		{
		unsigned char ∗ pDst = pImgDst + y ∗ width;
		const unsigned char ∗ pSrc = pImgSrc + y ∗ width;
		
			for(int x = 0; x < width; ++x)
			{
				const unsigned int sum = (int) pSrc[−width] + 2∗(int) pSrc[0] + (int) pSrc[width];
				∗pDst = sum / 4;
				++pDst;
				++pSrc;
			}
		}
	};

// //////////////// /// // // // /// // // // /// // // // /// // // /// // // // /// // // // /// /
// F i l t e r image w i t h 3 x3 Gau ss ian k e r n e l ( s e p a r a t e d i n 3 x1 and 1 x3 )
// //////////////// /// // // // /// // // // /// // // // /// // // /// // // // /// // // // /// /
static void filterGauss3x1(unsigned char ∗ pImg, const int width, const int height)
{
	unsigned char ∗ pFltY = new unsigned char[width ∗ height];
	
	// Beginning with the main(), a pointer is created and initialized with 0.
	// unsigned char ∗ pImage = 0;
	
	filterGauss3x1(pFltY, pImg, width, height);
	
		// cop y b o r d e r
		memcpy(pFltY, pImg, width);
		memcpy(pFltY + width ∗ (height −1), pImg + width ∗(height −1), w idth);
	
		filterGauss3x1(pImg, pFltY, width, height);
	
		delete pFltY;
	};


int main(int arg c, char∗ argv[])
{
	// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Read g r a y−v a l u e image
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		unsigned char ∗ pImage = 0;
		int width, height;
	
		bool readOk = readPGM("eyes dark.pgm", &pImage, width, height);
	
		if(!readOk)
		{
			printf("Reading image failed !\n");
			printf("Press any key to exit. \ n");
			getchar();
			return −1;
		}
	
		
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// S c a l e image b y 1/2
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		const int widthScl = width / 2;
		const int heightScl = height / 2;
		unsigned char ∗ pScaledImage = new unsigned char[widthScl ∗ heightScl];
	
		for(int y = 0; y < heightScl; y++)
		{
			for(int x = 0; x < widthScl; x++)
				{
					pScaledImage[x + y ∗ widthScl] = pImage[2∗x + 2∗y∗ width];
				}
		}
	
		writePGM("half.pgm", pScaledImage, widthScl, heightScl);
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// F i l t e r image w i t h 3 x3 Gau ss ian k e r n e l
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		filterGauss3x1(pScaledImage, widthScl, heightScl);
	
		writePGM("halfFiltered.pgm", pScaledImage, widthScl, heightScl);
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Compute h i s t o g r a m / c u t u pp e r and l o w e r 5% o f g r a y−v a l u e s
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		unsigned int histogram[256];
		memset(histogram, 0, 256 ∗ sizeof(unsigned int));
	
		// c a l c u l a t e h i s t o g r a m
		for(int i = 0; i < widthScl * heightScl; i++)
			++histogram[pScaledImage[i]];
	
		// d e t e rm i n e l o w e r and u pp e r bound f o r h i s t o g r a m s t r e t c h
		const float cutOffPercentage = 0.05;
		unsigned char lowerBound, upperBound;
		unsigned int histAccu = 0;
		const unsigned int lowerPercentile = cutOffPercentage ∗ widthScl ∗ heightScl;
		const unsigned int upperPercentile = (1 − cutOffPercentage) ∗ widthScl ∗ heightScl;
	
		for(int h = 0; h < 256; h++)
		{
		histAccu += histogram[h];
		if (histAccu <= lowerPercentile)
			{
			lowerBound = h;
			continue;
			}
		if(histAccu >= upperPercentile)
			{
			upperBound = h;
			 break;
			}
		}
	
		// a s s i g n new g r a y−v a l u e s from l i n e a r mapp ing b e t w e e n l o w e r and u pp e r bound
		const float histScale = 255. / (upperBound − lowerBound);
		for(int i = 0; i < widthScl ∗ heightScl; i++)
		{
			const int newVal = histScale ∗ ((int) pScaledImage[i] − lowerBound);
			pScaledImage[i] = std::min<int >(255, std::max<int>(0, newVal));
		}

		writePGM("histogram.pgm", pScaledImage, widthScl, heightScl);
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Compute image e n e r g y from g r a d i e n t s
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		unsigned char ∗ energy = new unsigned char[w i d t h S c l ∗ h e i g h t S c l];
		memset(energy, 0, widthScl ∗ heightScl);
	
		for(int y = 1; y < heightScl −1; ++y)
		{
		const int rowOffset = y ∗ widthScl;
		for(int x = 1; x < widthScl − 1; ++x)
			
			{
			const int gradX = pScaledImage[rowOffset + x + 1] − pScaledImage[r owO f f s e t + x −1];
			const int gradY = pScaledImage[rowOffset + x + widthScl] − pScaledImage[rowOffset + x − widthScl];
			energy[rowOffset + x] = sqrt((float) (gradX ∗ gradX + gradY ∗ gradY)) / sqrt(2.f);
			}
		}
	
		writePGM("energy.pgm", energy, widthScl, heightScl);
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Segmen t h i g h e n e r g y a r e a s b y T h r e s h o l d i n g
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		for(int i = 0; i < widthScl ∗ heightScl; i++)
		{
		if(energy[i] > 30)
			energy[i] = 255;
		else
			energy[i] = 0;
		}
		writePGM("energyThresh.pgm", energy, widthScl, heightScl);
	
		delete energy;
		delete pScaled Image;
		free(pImage);
		
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Read c o l o r (RGB) image
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		rtcvRgbaValue ∗ pRgbImage = 0;
		readOk = readPPM("eyescolor.ppm", &pRgbImage, width, height);
		// readOk = readPPM( ”HSV cone . ppm ” , &pRgbImage , w i d t h , h e i g h t ) ; // p r o o f o f c o n c e p t ; )
		
		if(!readOk)
		{
			printf("Reading color image faileD !\n");
			printf("Press any key to exit. \ n");
			getchar();
			return −1;
		}
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// D e t e rm in e t h e dom in an t c o l o r from a Hue−h i s t o g r a m
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		
		// Compute hue−h i s t o g r a m w i t h 8 b i n s
		unsigned int hueHist[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		for(int i = 0; i < width ∗ height; i++)
			++hueHist[pRgbImage[i].getHue() >> 5];
	
		// Find maximum i n h i s t o g r a m
		int maxHist = 0;
		int maxHue = −1;
		for(int h = 0; h < 8; h++)
			{
			if(hueHist[h] > maxHist)
				{
					maxHist = hueHist[h];
					maxHue = h;
				}
		}
	
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		// Segmen t dom in an t c o l o r ( and n e i g h b o r s ) i n HSV c o l o r s p a c e
		// //////////////// // /// // // // /// // // // /// // // // /// // // /// // // // /// // // // //
		unsigned char ∗ hueSeg = new unsigned char[width∗ height];
			for(int i = 0; i < width ∗ height; i++)
			{
				const unsigned char hue = pRgbImage[i].getHue() >> 5; // i n b i n s
				const unsigned char sat = pRgbImage[i].getSat();
				const unsigned char val = pRgbImage[i].getV();
				if(hue == maxHue && sat > 100 && val > 100)
					227 hueSeg[i] = 255;
				else
					hueSeg[i] = 0;
			}
		
		writePGM("hueSegmentation.pgm", hueSeg, width, height);
	
		delete hueSeg;
		free(pRgbImage);
	
		printf("Finished! Press any key. \ n");
		getchar();

		return 0;
	 }
