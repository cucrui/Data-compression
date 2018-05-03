#ifndef UTILS_H
#define UTILS_H
#include<windows.h>
#include<stdio.h>
typedef unsigned char BYTE;

void cal_PSNR(BYTE *a, BYTE *b, int size);
void getHW(FILE *bmpFile, DWORD *h, DWORD *w);
void dpcmCode_8(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width);
void dpcmCode_4(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width);
void dpcmCode_Non_uni_Qtz(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width);
void getName(char *input, char *diff_name,char *predic_name);

#endif 
