#pragma once
#ifndef RGB2YUV_H
#define RGB2YUV_H
#include<windows.h>

void bmp2yuv(FILE *bmpFile, BYTE *y_out, BYTE *u_out, BYTE *v_out);

#endif