#include<stdlib.h>
#include<iostream>
#include<string>
#include<windows.h>
#include"utils.h"
#include"bmp2yuv.h"
#include"huffman_code.h"

using namespace std;
typedef unsigned char BYTE;

int main() {
	FILE *bmp_file, *dpcm_file, *predict_file, *debug_file;
	char input_name[256],dpcm_name[256], predict_name[256];

	getName(input_name, dpcm_name, predict_name);
	cout << input_name << " " << dpcm_name << " " << predict_name << endl;

	bmp_file = fopen(input_name, "rb");
	dpcm_file = fopen(dpcm_name, "wb");
	predict_file = fopen(predict_name, "wb");
	debug_file = fopen("debug.yuv", "wb");

	DWORD height, width;
	getHW(bmp_file, &height, &width);
	fclose(bmp_file);

	DWORD size = height * width;

	bmp_file = fopen(input_name, "rb");
	BYTE *y = (BYTE*)malloc(height * width);
	BYTE *u = (BYTE*)malloc(height * width / 4);
	BYTE *v = (BYTE*)malloc(height * width / 4);
	bmp2yuv(bmp_file, y, u, v);

	/*fwrite(y, 1, size, debug_file);
	fwrite(u, 1, size/4, debug_file);
	fwrite(v, 1, size/4, debug_file);
	fclose(debug_file);
	system("pause");*/
	/*开辟两个空间*/
	
	BYTE *diff_buffer, *predic_buffer;
	

	/*DPCM*/
	cout << "请选择量化类型(0:均匀量化|1:非均匀量化)："; int a, op; cin >> a;
	if (a == 0) {
		cout << "请选择量化比特数：";
		cin >> op;
		switch (op)
		{
		case 4:
			diff_buffer = (BYTE*)malloc(height * width / 2);
			predic_buffer = (BYTE*)malloc(height * width);
			dpcmCode_4(y, diff_buffer, predic_buffer, height, width);
			/*输出*/
			for (int i = 0; i < (int)(height * width / 4); i++) {
				u[i] = 128; v[i] = 128;
			}
			fwrite(predic_buffer, 1, height * width, predict_file);
			fwrite(u, 1, height * width / 4, predict_file);
			fwrite(v, 1, height * width / 4, predict_file);

			fwrite(diff_buffer, 1, height * width / 2, dpcm_file);
			fwrite(u, 1, height * width / 4, dpcm_file);
			fwrite(v, 1, height * width / 4, dpcm_file);

			fclose(dpcm_file);
			fclose(predict_file);
			fclose(bmp_file);

			break;
		case 8:
			diff_buffer = (BYTE*)malloc(height * width);
			predic_buffer = (BYTE*)malloc(height * width);
			dpcmCode_8(y, diff_buffer, predic_buffer, height, width);

			/*输出*/
			fwrite(predic_buffer, 1, height * width, predict_file);
			fwrite(u, 1, height * width / 4, predict_file);
			fwrite(v, 1, height * width / 4, predict_file);

			for (int i = 0; i < (int)(height * width / 4); i++) {
				u[i] = 128; v[i] = 128;
			}

			fwrite(diff_buffer, 1, height * width, dpcm_file);
			fwrite(u, 1, height * width / 4, dpcm_file);
			fwrite(v, 1, height * width / 4, dpcm_file);

			fclose(dpcm_file);
			fclose(predict_file);
			fclose(bmp_file);

			break;
		default:
			break;
		}
	}
	
	else if (a == 1) {
		diff_buffer = (BYTE*)malloc(height * width);
		predic_buffer = (BYTE*)malloc(height * width);
		dpcmCode_Non_uni_Qtz(y, diff_buffer, predic_buffer, height, width);

		/*输出*/
		fwrite(predic_buffer, 1, height * width, predict_file);
		fwrite(u, 1, height * width / 4, predict_file);
		fwrite(v, 1, height * width / 4, predict_file);

		for (int i = 0; i < (int)(height * width / 4); i++) {
			u[i] = 128; v[i] = 128;

		}

		fwrite(diff_buffer, 1, height * width / 2, dpcm_file);
		fwrite(u, 1, height * width / 4, dpcm_file);
		fwrite(v, 1, height * width / 4, dpcm_file);

		fclose(dpcm_file);
		fclose(predict_file);
		fclose(bmp_file);
	}
	
	/*计算峰值信噪比*/
	cal_PSNR(y, predic_buffer, (int)(height * width));

	/*输出y分量*/
	FILE *y_file, *y_diff;
	y_file = fopen("y_file.y", "wb");
	y_diff = fopen("y_diff.y", "wb");
	fwrite(y, 1, height * width, y_file);
	if (a == 0) {
		if(op == 8)
			fwrite(diff_buffer, 1, height * width, y_diff);
		else if(op == 4)
			fwrite(diff_buffer, 1, height * width / 2, y_diff);
	}
	else {
		fwrite(diff_buffer, 1, height * width / 2, y_diff);
	}
	system("pause");
	return 0;
}