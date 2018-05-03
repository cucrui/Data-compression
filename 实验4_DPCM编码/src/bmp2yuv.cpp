#include<windows.h>
#include"rgb2yuv.h"
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;

void bmp2yuv(FILE *bmpFile, BYTE *y_out, BYTE *u_out, BYTE *v_out) { 
	BITMAPFILEHEADER head;
	BITMAPINFOHEADER info;
	BYTE *rgbout;
	DWORD height, width;

	if (bmpFile != NULL) {
		fread(&head, 1, sizeof(head), bmpFile);
		fread(&info, 1, sizeof(info), bmpFile);

		height = info.biHeight;
		width = info.biWidth;
		DWORD size = height * width * 3;

		if (info.biBitCount == 24) {
			rgbout = (BYTE *)malloc(size);
			fread(rgbout, 1, size, bmpFile);
			RGB2YUV(width, height, rgbout, y_out, u_out, v_out, false);
			free(rgbout);
		}
		else if (info.biBitCount == 8) {
			BYTE plane_table[256][4];
			BYTE *plane = (BYTE *)malloc(256 * 4);
			fread(plane, 1, 256 * 4, bmpFile);
			for (int i = 0; i < 256; i++) {
				plane_table[i][0] = plane[0];
				plane_table[i][1] = plane[1];
				plane_table[i][2] = plane[2];
				plane_table[i][3] = plane[3];
				plane += 4;
			}

			BYTE *raw_data = (BYTE *)malloc(width * height);
			fread(raw_data, 1, width * height, bmpFile);
			rgbout = (BYTE *)malloc(width * height * 3);

			for (int i = 0, j = 0; i < (int)(width * height * 3); i += 3) {
				int index = raw_data[j];
				rgbout[i] = plane_table[index][0];
				rgbout[i + 1] = plane_table[index][1];
				rgbout[i + 2] = plane_table[index][2];
				j++;
			}

			RGB2YUV(width, height, rgbout, y_out, u_out, v_out, false);

			free(rgbout);
			free(raw_data);
		}
		
		else {
			cout << "not 8bit/24bit" << endl;
		}
	}
	else {
		cout << "can not open file" << endl;
	}
}