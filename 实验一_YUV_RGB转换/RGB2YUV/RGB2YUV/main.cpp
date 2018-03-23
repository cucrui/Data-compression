#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "rgb2yuv.h"
#include<iostream>
#include<io.h>
using namespace std;
#define u_int8_t	unsigned __int8  //8-bit integer
#define u_int		unsigned __int32 //32-bit integer
#define u_int32_t	unsigned __int32
#define FALSE		false
#define TRUE		true

/*
 * rgb2yuv
 * required arg1 should be the input RAW RGB24 file
 * required arg2 should be the output RAW YUV12 file
 */ 
/* int main(int argc, char** argv)*/
int main(int argc, char** argv)
{
	/* variables controlable from command line */
	u_int frameWidth = 352;			/* --width=<uint> */
	u_int frameHeight = 240;		/* --height=<uint> */
	bool flip = true;				/* --flip */
	//unsigned int i;

	/* internal variables */

	char* rgbFileName = NULL;
	char* yuvFileName = NULL;
	FILE* rgbFile = NULL;
	FILE* yuvFile = NULL;
	u_int8_t* rgbBuf = NULL;
	u_int8_t* yBuf = NULL;
	u_int8_t* uBuf = NULL;
	u_int8_t* vBuf = NULL;
	u_int32_t videoFramesWritten = 0;
	unsigned int i;

	/* begin process command line */
	/* point to the specified file names */
	rgbFileName = argv[1];
	yuvFileName = argv[2];

	frameWidth = atoi(argv[3]);//atoi() string to integer
	frameHeight = atoi(argv[4]);
	
	
	rgbFile = fopen(rgbFileName, "rb");

	if (rgbFile == NULL)
	{
		printf("cannot find rgb file\n");
		exit(1);
	}
	else
	{
		printf("The input rgb file is %s\n", rgbFileName);
	}

	/* open the RAW file */
	
	yuvFile = fopen(yuvFileName, "wb");
	if (yuvFile == NULL)
	{
		printf("cannot find yuv file\n");
		exit(1);
	}
	else
	{
		printf("The output yuv file is %s\n", yuvFileName);
	}

	/* get an input buffer for a frame */
	rgbBuf = (u_int8_t*)malloc(frameWidth * frameHeight * 3); //三个通道，每个通道大小宽*高

	/* get the output buffers for a frame */
	yBuf = (u_int8_t*)malloc(frameWidth * frameHeight);  //4 ：2 ： 0 采样格式
	uBuf = (u_int8_t*)malloc((frameWidth * frameHeight) / 4);
	vBuf = (u_int8_t*)malloc((frameWidth * frameHeight) / 4);

	if (rgbBuf == NULL || yBuf == NULL || uBuf == NULL || vBuf == NULL)
	{
		printf("no enought memory\n");
		return 0;
		//exit(1);
	}
	
	while (fread(rgbBuf, 1, frameWidth * frameHeight * 3, rgbFile)) 
		//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
	{
		if(RGB2YUV(frameWidth, frameHeight, rgbBuf, yBuf, uBuf, vBuf, flip))
		{
			printf("error");
			return 0;
		}

		for (i = 0; i < frameWidth*frameHeight; i++)
		{
			if (yBuf[i] < 16) yBuf[i] = 16;
			if (yBuf[i] > 235) yBuf[i] = 235;
		}

		for (i = 0; i < frameWidth*frameHeight/4; i++)
		{
			if (uBuf[i] < 16) uBuf[i] = 16;
			if (uBuf[i] > 240) uBuf[i] = 240;

			if (vBuf[i] < 16) vBuf[i] = 16;
			if (vBuf[i] > 240) vBuf[i] = 240;
		}

		fwrite(yBuf, 1, frameWidth * frameHeight, yuvFile);
		fwrite(uBuf, 1, (frameWidth * frameHeight) / 4, yuvFile);
		fwrite(vBuf, 1, (frameWidth * frameHeight) / 4, yuvFile);

		printf("\r...%d", ++videoFramesWritten);
	}

	printf("\n%u %ux%u video frames written\n", 
		videoFramesWritten, frameWidth, frameHeight);

	/* cleanup */
	free(yBuf);
	free(rgbBuf);
	free(uBuf);
	free(vBuf);

	fclose(rgbFile);
	fclose(yuvFile);
	system("pause");
	return 0;
}
