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
	DWORD bfSize; //�ļ���С(������14�ֽ�)
	WORD bfReserved1; //�����֣�������
	WORD bfReserved2; //�����֣�ͬ��
	DWORD bfOffBits; //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER;

typedef struct targetBITMAPINFOHEADER {
	DWORD   biSize;             //ָ���˽ṹ��ĳ��ȣ�Ϊ40
	LONG    biWidth;            //λͼ��
	LONG    biHeight;           //λͼ��
	WORD    biPlanes;           //ƽ������Ϊ1
	WORD    biBitCount;         //������ɫλ����������1��2��4��8��16��24���µĿ�����32
	DWORD   biCompression;      //ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
	DWORD   biSizeImage;        //ʵ��λͼ����ռ�õ��ֽ���
	LONG    biXPelsPerMeter;    //X����ֱ���
	LONG    biYPelsPerMeter;    //Y����ֱ���
	DWORD   biClrUsed;          //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
	DWORD   biClrImportant;     //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
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