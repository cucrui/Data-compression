#ifndef YUV2RGB_H
#define YUV2RGB_H

int YUV2RGB(int x_dim, int y_dim, void *y_in, void *u_out, void *v_out, unsigned char *rgb_out, int flip);

void InitLookupTable();

#endif

