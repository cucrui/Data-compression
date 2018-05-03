#include"utils.h"
#include<windows.h>
#include<cmath>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<string>
using namespace std;

void cal_PSNR(BYTE *a, BYTE *b, int size) {
	const double eps = 1e-8;
	double MSE = 0, PSNR;
	for (int i = 0; i < size; i++) {
		MSE += (a[i] - b[i]) * (a[i] - b[i]);
	}
	MSE /= size;
	if (fabs(MSE - eps) < 0) {
		cout << "INF" << endl;
	}
	else {
		PSNR = 10 * log10(255 * 255 / MSE);
		cout << "PSNR = " << PSNR << endl;
	}
}

void getHW(FILE *bmpFile, DWORD *h, DWORD *w) {
	FILE *tem = bmpFile;
	BITMAPFILEHEADER head;
	BITMAPINFOHEADER info;
	fread(&head, 1, sizeof(head), tem);
	fread(&info, 1, sizeof(info), tem);
	*h = info.biHeight;
	*w = info.biWidth;
}

void dpcmCode_8(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width){
	int idx = 0;
	for (int i = 0; i < (int)height; i++) {
		for (int j = 0; j < (int)width; j++) {
			int pos_cur = i * width + j, pos_past = (i - 1) * width + j;
			BYTE dif;
			if (i == 0) {
				dif = (input[pos_cur] - 128 + 255) / 2;
				predic[pos_cur] = dif * 2 - 255 + 128;
			}
			else {
				dif = (input[pos_cur] - predic[pos_past] + 255) / 2;
				predic[pos_cur] = 2 * dif - 255 + predic[pos_past]; 
			}
			diff[pos_cur] = dif;
		}
	}
}

void dpcmCode_4(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width){
	int idx = 0, cnt = 0;
	for (int i = 0; i < (int)height; i++) {
		for (int j = 0; j < (int)width; j++) {
			int pos_cur = i * width + j, pos_past = (i - 1) * width + j;
			BYTE dif;
			if (i == 0) {
				dif = (input[pos_cur] - 128 + 255) / 32;
				int tem = dif * 32 - 255 + 128;
				if (tem < 0) tem = 0;
				else if (tem > 255) tem = 255;
				predic[pos_cur] = tem;
			}
			else {
				dif = (input[pos_cur] - predic[pos_past] + 255) / 32;
				int tem = dif * 32 - 255 + predic[pos_past];
				if (tem < 0) tem = 0;
				else if (tem > 255) tem = 255;
				predic[pos_cur] = tem; 
			}
			if(cnt % 2 == 0){
				diff[idx] = 0;
				diff[idx] |= dif;
				diff[idx] <<= 4;
				cnt++;
			}
			else{
				diff[idx] |= dif;
				cnt = 0;
				idx++;
			}
		}
	}
}

double Qunt(int dif) {
	const double eps = 1e-8;
	double x = (double)dif / 255, y;
	if (dif < 0) {
		y = -log(1.0 - 255 * x) / log(256.0);
	}
	else {
		y = log(1.0 + 255 * x) / log(256.0);
	}
	return y;
}

double Rev_Qunt(int in) {
	const double e = 2.71828;
	if ((in & 8) == 8) {//dif is positive
		in &= 7;
		double y = (double)(in * 32) / 255;
		double x = (pow(e, y * log(256)) - 1) / 255;
		return x;
	}
	else {//dif is negtive
		double y = (double)(in * 32) / 255;
		double x = (pow(e, y * log(256)) - 1) / 255;
		return -x;
	}
}

void dpcmCode_Non_uni_Qtz(BYTE *input, BYTE *diff, BYTE *predic, DWORD height, DWORD width) {
	int idx = 0, cnt = 0;
	//freopen("debug.txt", "w", stdout);

	for (int i = 0; i < (int)height; i++) {

		for (int j = 0; j < (int)width; j++) {
			int pos_cur = i * width + j, pos_past = (i - 1) * width + j;
			BYTE dif;
			double tem;
			if (i == 0) {
				tem = Qunt(input[pos_cur] - 128);
				dif = (BYTE)(abs(tem) * 8);
				if (tem > 0) {
					dif |= 8;
				}
				else {
					dif &= 7;
				}
				predic[pos_cur] = (BYTE)(Rev_Qunt(dif) * 255 + 128);
			}
			else {
				tem = Qunt(input[pos_cur] - predic[pos_past]);
				dif = (BYTE)(abs(tem) * 8);
				if (tem > 0) {
					dif |= 8;
				}
				else {
					dif &= 7;
				}
				predic[pos_cur] = (BYTE)(Rev_Qunt(dif) * 255 + predic[pos_past]);
			}
			//cout << idx << " " << (int)dif << endl;
			if (cnt % 2 == 0) {
				diff[idx] = 0;
				diff[idx] |= dif;
				diff[idx] <<= 4;
				cnt++;
			}
			else {
				diff[idx] |= dif;
				cnt = 0;
				idx++;
			}
		}
	}
	//system("pause");
}

void getName(char *input, char *diff_name, char *predic_name){
	string s, tem1, tem2; cout << "请输入文件名：" << endl; cin >> s;
	strcpy(input, s.c_str());
	int len = strlen(input), pos;
	for(int i = 0; i < len; i++){
		if(s[i] == '.'){
			pos = i; break;
		}
	}
	tem1 = s.erase(pos, 4);
	tem2 = s;
	tem1.append("_dcmp.yuv");
	tem2.append("_predict.yuv");
	strcpy(diff_name, tem1.c_str());
	strcpy(predic_name , tem2.c_str());
}
