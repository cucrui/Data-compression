#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<fstream>
#include<cmath>
#define uc unsigned char
const double eps = 1e-8;
using namespace std;

int r_cnt[256], g_cnt[256], b_cnt[256];
double p_r[256], p_g[256], p_b[256];
double H_R = 0, H_G = 0, H_B = 0;

void write_txt(){
        freopen("r_cnt.txt", "w", stdout);
        for (int i = 0; i < 256; i++)
        {
            cout << i << " " << p_r[i] << endl;
        }
        freopen("g_cnt.txt", "w", stdout);
        for (int i = 0; i < 256; i++)
        {
            cout << i << " " << p_g[i] << endl;
        }
        freopen("b_cnt.txt", "w", stdout);
        for (int i  = 0; i < 256; i++)
        {
            cout << i << " " << p_b[i] << endl;
        }
}

double cal_p_log_p(double p){
    if(fabs(p - 0.0) < eps) return 0;
    else return -p * log(p);
}

int main()
{
    FILE *rgb_file = fopen("test.rgb", "rb");
    int size = 256 * 256;
    uc *r, *g, *b, *rgb_buf;
    rgb_buf = (uc*)malloc(size*3);
    r = (uc *)malloc(size);
    g = (uc *)malloc(size);
    b = (uc *)malloc(size);
    if (fread(rgb_buf, 1, size * 3, rgb_file))
    {
        for (int i = 0, j = 0; i < size; i++)
        {
            b[i] = rgb_buf[j];
            g[i] = rgb_buf[j + 1];
            r[i] = rgb_buf[j + 2];
            j += 3;
        }
        memset(r_cnt, 0, sizeof(r_cnt));
        memset(b_cnt, 0, sizeof(b_cnt));
        memset(g_cnt, 0, sizeof(g_cnt));
        for (int i = 0; i < size; i++)
        {
            r_cnt[r[i]]++;
            g_cnt[g[i]]++;
            b_cnt[b[i]]++;
        }
        for(int i = 0; i < 256; i++){
            p_r[i] = (double)r_cnt[i] / (256.0*256.0);
            p_g[i] = (double)g_cnt[i] / (256.0*256.0);
            p_b[i] = (double)b_cnt[i] / (256.0*256.0);
        }
        for(int i = 0; i < 256; i++){
            H_R += cal_p_log_p(p_r[i]);
            H_G += cal_p_log_p(p_g[i]);
            H_B += cal_p_log_p(p_b[i]);
        }
        write_txt();
    }
    
    return 0;
}
