/* This file contains YUV to RGB transformation functions.                */

#include "stdlib.h"
#include "yuv2rgb.h"
#include<iostream>
#include<stdio.h>

#define u_int8_t	unsigned __int8 

using namespace std;

static float YUV_RGB1402[256];
static float YUV_RGB0344[256];
static float YUV_RGB0714[256];
static float YUV_RGB1772[256];

int YUV2RGB(int X_DIM, int Y_DIM, void *y_in, void *u_in, void *v_in, unsigned char *rgb_out, int flip)
{
	static int init_done = 0;
	int size = X_DIM * Y_DIM;

	unsigned char *y_buffer = (unsigned char *)y_in;
	unsigned char *sub_u_buf = (unsigned char *)u_in;
	unsigned char *sub_v_buf = (unsigned char *)v_in;
	
	unsigned char *u_buffer = (unsigned char *)malloc(size);
	unsigned char *v_buffer = (unsigned char *)malloc(size);
	
	unsigned char *y, *u, *v;

	if (init_done == 0)
	{
		InitLookupTable();
		init_done = 1;
	}

	if ((X_DIM % 2) || (Y_DIM % 2)) return 1;

	if (!(u_buffer && v_buffer))
	{
		if (u_buffer) free(u_buffer);
		if (v_buffer) free(v_buffer);
		return 2;
	}
	
	//upsample sub_u, sub_v to u_buffer, v_buffer;
	int idx_row = 0, idx_col = 0, pos0, pos1, pos2, pos3, pos;
	for(int row = 0; row < Y_DIM / 2; row++){
		for(int col = 0; col < X_DIM / 2; col++){
			pos = col + X_DIM / 2 * row;
			pos0 = idx_col + idx_row * X_DIM;
			pos1 = idx_col + 1 + idx_row * X_DIM; 
			pos2 = idx_col + (idx_row + 1) * X_DIM;
			pos3 = idx_col + 1 + (idx_row + 1) * X_DIM;
			
			u_buffer[pos0] = sub_u_buf[pos];
			u_buffer[pos1] = sub_u_buf[pos];
			u_buffer[pos2] = sub_u_buf[pos];
			u_buffer[pos3] = sub_u_buf[pos];
			
			v_buffer[pos0] = sub_v_buf[pos];
			v_buffer[pos1] = sub_v_buf[pos];
			v_buffer[pos2] = sub_v_buf[pos];
			v_buffer[pos3] = sub_v_buf[pos];
			
			if(idx_col + 2 == X_DIM){
				idx_row += 2;
				idx_col = 0; 
			} 
			else{
				idx_col += 2; 
			}
		}
	}  

	y = y_buffer;
	u = u_buffer;
	v = v_buffer;
	float tem;
	for(int i = 0, j = 0; i < size; i++){
		tem = (*y) + YUV_RGB1402[*v];
		rgb_out[j + 2] = tem > 255 ? 255 : (tem < 0 ? 0 : (unsigned char)tem);
		tem = (*y) - YUV_RGB0344[*u] - YUV_RGB0714[*v];
		rgb_out[j + 1] = tem > 255 ? 255 : (tem < 0 ? 0 : (unsigned char)tem);
		tem = (*y) + YUV_RGB1772[*u];
		rgb_out[j] = tem > 255 ? 255 : (tem < 0 ? 0 : (unsigned char)tem);
		y++; u++; v++;
		j += 3;
	}
	if (!(u_buffer && v_buffer))
	{
		if (u_buffer) free(u_buffer);
		if (v_buffer) free(v_buffer);
	}
	return 0;
}

// initialize table
void InitLookupTable()
{
	int i;
	for(i = 0; i < 256; i++) YUV_RGB1402[i] = (float)1.402 * (i - 128);
	for(i = 0; i < 256; i++) YUV_RGB0344[i] = (float)0.344 * (i - 128);
	for(i = 0; i < 256; i++) YUV_RGB0714[i] = (float)0.714 * (i - 128);
	for(i = 0; i < 256; i++) YUV_RGB1772[i] = (float)1.772 * (i - 128);
}


