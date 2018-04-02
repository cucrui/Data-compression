#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<cmath>
#include"RGB2YUV.h"

using namespace std;

//typedef unsigned long	DWORD;
//typedef unsigned short	WORD;
//typedef unsigned char BYTE;
//typedef long LONG;
//
//typedef struct tagbitmapfileheader{
//	WORD    bfType;
//	DWORD   bfSize;
//	WORD    bfReserved1;
//	WORD    bfReserved2;
//	DWORD   bfOffBits;
//} BITMAPFILEHEADER;
//
//typedef struct tagBITMAPINFOHEADER {
//	DWORD      biSize;
//	LONG       biWidth;
//	LONG       biHeight;
//	WORD       biPlanes;
//	WORD       biBitCount;
//	DWORD      biCompression;
//	DWORD      biSizeImage;
//	LONG       biXPelsPerMeter;
//	LONG       biYPelsPerMeter;
//	DWORD      biClrUsed;
//	DWORD      biClrImportant;
//} BITMAPINFOHEADER;

void write_yuvfile(BYTE *rgbout, DWORD height, DWORD width, FILE *yuvfile) {
	BYTE *y_buf = (BYTE *)malloc(width * height);
	BYTE *u_buf = (BYTE *)malloc(width * height / 4);
	BYTE *v_buf = (BYTE *)malloc(width * height / 4);

	RGB2YUV(width, height, rgbout, y_buf, u_buf, v_buf, false);

	fwrite(y_buf, 1, width * height, yuvfile);
	fwrite(u_buf, 1, width * height / 4, yuvfile);
	fwrite(v_buf, 1, width * height / 4, yuvfile);
	cout << "sucess" << endl;
	free(y_buf);
	free(u_buf);
	free(v_buf);
}

int main() {
	FILE *bmpfile;
	FILE *yuvfile;

	BITMAPFILEHEADER head;
	BITMAPINFOHEADER info;
	
	BYTE *rgbout;

	DWORD height, width;
	
	bmpfile = fopen("Lenna_8.bmp", "rb");
	yuvfile = fopen("Lenna_8.yuv", "wb");

	if (bmpfile != NULL) {
		fread(&head, 1, sizeof(head), bmpfile);
		fread(&info, 1, sizeof(info), bmpfile);
		
		height = info.biHeight; 
		width = info.biWidth;
		DWORD size = height * width * 3;
		
		if (info.biBitCount == 24) {
			rgbout = (BYTE *)malloc(size);
			fread(rgbout, 1, size, bmpfile);
			write_yuvfile(rgbout, height, width, yuvfile);
			free(rgbout);
		}
		else if (info.biBitCount == 8) {
			BYTE plane_table[256][4];
			BYTE *plane = (BYTE *)malloc(256 * 4);
			fread(plane, 1, 256 * 4, bmpfile);
			for (int i = 0; i < 256; i++) {
				plane_table[i][0] = plane[0];
				plane_table[i][1] = plane[1];
				plane_table[i][2] = plane[2];
				plane_table[i][3] = plane[3];
				plane += 4;
			}

			BYTE *raw_data = (BYTE *)malloc(width * height);
			fread(raw_data, 1, width * height, bmpfile);
			rgbout = (BYTE *)malloc(width * height * 3);
			
			for (int i = 0, j = 0; i < (int)(width * height * 3); i += 3) {
				int index = raw_data[j];
				rgbout[i] = plane_table[index][0];
				rgbout[i + 1] = plane_table[index][1];
				rgbout[i + 2] = plane_table[index][2];
				j++;
			}

			write_yuvfile(rgbout, height, width, yuvfile);

			free(rgbout);
			free(raw_data);
		}
		else if (info.biBitCount == 16) {

			int tem_size = width * height;
			
			BYTE pow2_5 = (BYTE)(pow(2, 5) - 1);
			BYTE pow2_6 = (BYTE)(pow(2, 6) - 1);

			rgbout = (BYTE *)malloc(size);
			WORD *raw_data = (WORD *)malloc(tem_size * 2);

			if (head.bfOffBits == 70) {
				WORD *kong = (WORD *)malloc(16);
				fread(kong, 1, 16, bmpfile);

				fread(raw_data, 2, tem_size, bmpfile);
				
				for (int i = 0, j = 0; i < (int)size; i += 3) {
					WORD tem = raw_data[j];
					rgbout[i + 2] = (((tem >> 11) & pow2_5) << 3);
					rgbout[i + 1] = (((tem >> 5) & pow2_6) << 2);
					rgbout[i] = ((tem & pow2_5) << 3);
					j++;
				}
			}
			
			else{
				fread(raw_data, 2, tem_size, bmpfile);

				for (int i = 0, j = 0; i < (int)size; i += 3) {
					WORD tem = raw_data[j];
					rgbout[i + 2] = (((tem >> 10) & pow2_5) << 3);
					rgbout[i + 1] = (((tem >> 5) & pow2_5) << 3);
					rgbout[i] = ((tem & pow2_5) << 3);
					j++;
				}
			}

			write_yuvfile(rgbout, height, width, yuvfile);
			free(rgbout);
			free(raw_data);
		}
		else {
			cout << "not 8bit/16bit/24bit" << endl;
			return 0;
		}
	}
	else {
		cout << "can not open file" << endl;
		return 0;
	}
	return 0;
}
