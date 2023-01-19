#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"

#define PI 3.1415926
#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4);
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long LONG;

typedef struct targetBITMAPFILEHEADER {
	DWORD bfSize; //文件大小(包含这14字节)
	WORD bfReserved1; //保留字，不考虑
	WORD bfReserved2; //保留字，同上
	DWORD bfOffBits; //实际位图数据的偏移字节数，即前三个部分长度之和
} BITMAPFILEHEADER;

typedef struct targetBITMAPINFOHEADER {
	DWORD   biSize;             //指定此结构体的长度，为40
	LONG    biWidth;            //位图宽
	LONG    biHeight;           //位图高
	WORD    biPlanes;           //平面数，为1
	WORD    biBitCount;         //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
	DWORD   biCompression;      //压缩方式，可以是0，1，2，其中0表示不压缩
	DWORD   biSizeImage;        //实际位图数据占用的字节数
	LONG    biXPelsPerMeter;    //X方向分辨率
	LONG    biYPelsPerMeter;    //Y方向分辨率
	DWORD   biClrUsed;          //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
	DWORD   biClrImportant;     //重要颜色数，如果为0，则表示所有颜色都是重要的
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;


FILE* file, * targetFile;
int rotateX, rotateY;
int write_rotateX, write_rotateY;
BITMAPFILEHEADER bmpFile, writeBmpFile;
BITMAPINFOHEADER bmpInfo, writeBmpInfo;

int angle;
double thelta;
char fileName[20];
WORD bfType;
int dwidth, dheight;

#endif // !IMAGE_H