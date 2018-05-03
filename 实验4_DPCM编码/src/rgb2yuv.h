#pragma once
#ifndef RGB2YUV_H
#define RGB2YUV_H

int RGB2YUV(int x_dim, int y_dim, void *bmp, void *y_out, void *u_out, void *v_out, int flip);

void InitLookupTable();

#endif