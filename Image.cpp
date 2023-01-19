#include "Image.h"
int RotImage()
{
	cout << "please input the rotate angle : ";
	cin >> angle;
	if (angle % 360 == 270) {
		angle++;
	}
	thelta = (double)(angle * PI / 180);
	// 整个变化中bmp的文件头是相似的，只有bfsize变化了
	fread(&bmpFile, 1, sizeof(BITMAPFILEHEADER), file);
	writeBmpFile = bmpFile;
	// 整个变换过程之中bmp的信息头也是相似的，这个则是biwidth、biheight，以及bisizeimage发生变化
	fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), file);
	writeBmpInfo = bmpInfo;

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;

	int newWidth = abs(width * cos(thelta) + height * sin(thelta));
	// int newHeight = 2000;
	int newHeight = abs(width * sin(thelta) + height * cos(thelta));
	// int newWidth = 2000;

	writeBmpInfo.biWidth = newWidth;
	writeBmpInfo.biHeight = newHeight;

	// 在计算实际占用的空间的时候我们需要将宽度为4byte的倍数
	int writewidth = WIDTHBYTES(newWidth * writeBmpInfo.biBitCount);
	writeBmpInfo.biSizeImage = writewidth * writeBmpInfo.biHeight;
	writeBmpFile.bfSize = 54 + writeBmpInfo.biSizeImage;
	fwrite(&writeBmpFile, 1, sizeof(BITMAPFILEHEADER), targetFile);
	fwrite(&writeBmpInfo, 1, sizeof(BITMAPINFOHEADER), targetFile);

	/**
	 * step 2 : 完成空间的申请与分配的准备工作
	*/
	// 在这里因为待处理的图片宽度应当是4byte的倍数，因此我们首先要完成对宽度进行完成4byte的倍数化
	int l_width = WIDTHBYTES(width * bmpInfo.biBitCount);
	int write_width = WIDTHBYTES(writeBmpInfo.biWidth * writeBmpInfo.biBitCount);
	rotateX = width / 2;
	rotateY = height / 2;
	write_rotateX = newWidth / 2;
	write_rotateY = newHeight / 2;

	// cout << "writeBmpInfo.biWidth : " << writeBmpInfo.biWidth << endl;
	// cout << "writeBmpInfo.biBitCount : " << writeBmpInfo.biBitCount << endl;
	// cout << "write_width" << write_width << endl;
	// cout << "writewidth" << writewidth << endl;


	// 准备工作完成之后，我们现在就要将bmp文件中的数据文件存放在一个数组中，因此我们需要申请空间创建数组来完成数据的存放
	BYTE* preData = (BYTE*)malloc(height * l_width);
	memset(preData, 0, height * l_width);

	BYTE* aftData = (BYTE*)malloc(newHeight * writewidth);
	memset(aftData, 0, newHeight * writewidth);


	// cout << "test!" << endl;
	int OriginalImg = l_width * height;
	int LaterImg = writewidth * newHeight;

	fread(preData, 1, OriginalImg, file);

	/**
	 * step 3 : 完成将图像信息的迁移
	*/
	for (int hnum = 0; hnum < newHeight; ++hnum) {
		// cout << "test " << hnum << endl;
		for (int wnum = 0; wnum < newWidth; ++wnum) {
			// 新数据的下标为index
			int index = hnum * writewidth + wnum * 3;
			// cout << "index " << index << endl;
			// 利用公式计算这个原来的点的地方
			double d_original_img_hnum = (wnum - write_rotateX) * sin(thelta) + (hnum - write_rotateY) * cos(thelta) + rotateY;
			double d_original_img_wnum = (wnum - write_rotateX) * cos(thelta) - (hnum - write_rotateY) * sin(thelta) + rotateX;

			if (d_original_img_hnum < 0 || d_original_img_hnum > height || d_original_img_wnum < 0 || d_original_img_wnum > width) {
				aftData[index] = 0; // 这个相当于是R
				aftData[index + 1] = 0; // 这个相当于是G
				aftData[index + 2] = 0;  // 这个相当于是B
				continue;
			}
			else {
				/**
				 * 我们在这里使用双线性插值法来完成对应
				*/
				int i_original_img_hnum = d_original_img_hnum;
				int i_original_img_wnum = d_original_img_wnum;
				double distance_to_a_X = d_original_img_wnum - i_original_img_wnum;
				double distance_to_a_Y = d_original_img_hnum - i_original_img_hnum;

				int original_point_A = i_original_img_hnum * l_width + i_original_img_wnum * 3;
				int original_point_B = i_original_img_hnum * l_width + (i_original_img_wnum + 1) * 3;
				int original_point_C = (i_original_img_hnum + 1) * l_width + i_original_img_wnum * 3;
				int original_point_D = (i_original_img_hnum + 1) * l_width + (i_original_img_wnum + 1) * 3;

				if (i_original_img_wnum == width - 1) {
					// cout << "test" << endl;
					original_point_A = original_point_B;
					original_point_C = original_point_D;
				}
				if (i_original_img_hnum == height - 1) {
					original_point_C = original_point_A;
					original_point_D = original_point_B;
				}

				aftData[index] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D];

				aftData[index + 1] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A + 1]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B + 1]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C + 1]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D + 1];

				aftData[index + 2] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A + 2]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B + 2]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C + 2]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D + 2];
			}
		}
	}
	fwrite(aftData, 1, LaterImg, targetFile);
	fclose(file);
	fclose(targetFile);
	return 0;
}

int ScaImage()
{
	cout << "please input the new width : ";
	cin >> dwidth;
	cout << "please input the new height : ";
	cin >> dheight;
	angle = 360;
	thelta = (double)(angle * PI / 180);
	// 整个变化中bmp的文件头是相似的，只有bfsize变化了
	fread(&bmpFile, 1, sizeof(BITMAPFILEHEADER), file);
	writeBmpFile = bmpFile;
	// 整个变换过程之中bmp的信息头也是相似的，这个则是biwidth、biheight，以及bisizeimage发生变化
	fread(&bmpInfo, 1, sizeof(BITMAPINFOHEADER), file);
	writeBmpInfo = bmpInfo;

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	// int newWidth = 1500;

	writeBmpInfo.biWidth = dwidth;
	writeBmpInfo.biHeight = dheight;

	// 在计算实际占用的空间的时候我们需要将宽度为4byte的倍数
	int writewidth = WIDTHBYTES(dwidth * writeBmpInfo.biBitCount);
	writeBmpInfo.biSizeImage = writewidth * writeBmpInfo.biHeight;
	writeBmpFile.bfSize = 54 + writeBmpInfo.biSizeImage;
	fwrite(&writeBmpFile, 1, sizeof(BITMAPFILEHEADER), targetFile);
	fwrite(&writeBmpInfo, 1, sizeof(BITMAPINFOHEADER), targetFile);

	/**
	 * step 2 : 完成空间的申请与分配的准备工作
	*/
	// 在这里因为待处理的图片宽度应当是4byte的倍数，因此我们首先要完成对宽度进行完成4byte的倍数化
	int l_width = WIDTHBYTES(width * bmpInfo.biBitCount);
	int write_width = WIDTHBYTES(writeBmpInfo.biWidth * writeBmpInfo.biBitCount);
	rotateX = width / 2;
	rotateY = height / 2;
	write_rotateX = dwidth / 2;
	write_rotateY = dheight / 2;

	// cout << "writeBmpInfo.biWidth : " << writeBmpInfo.biWidth << endl;
	// cout << "writeBmpInfo.biBitCount : " << writeBmpInfo.biBitCount << endl;
	// cout << "write_width" << write_width << endl;
	// cout << "writewidth" << writewidth << endl;


	// 准备工作完成之后，我们现在就要将bmp文件中的数据文件存放在一个数组中，因此我们需要申请空间创建数组来完成数据的存放
	BYTE* preData = (BYTE*)malloc(height * l_width);
	memset(preData, 0, height * l_width);

	BYTE* aftData = (BYTE*)malloc(dheight * writewidth);
	memset(aftData, 0, dheight * writewidth);


	// cout << "test!" << endl;
	int OriginalImg = l_width * height;
	int LaterImg = writewidth * dheight;

	fread(preData, 1, OriginalImg, file);

	/**
	 * step 3 : 完成将图像信息的迁移
	*/
	for (int hnum = 0; hnum < dheight; ++hnum) {
		// cout << "test " << hnum << endl;
		for (int wnum = 0; wnum < dwidth; ++wnum) {
			// 新数据的下标为index
			int index = hnum * writewidth + wnum * 3;
			// cout << "index " << index << endl;
			// 利用公式计算这个原来的点的地方
			double d_original_img_hnum = (wnum - write_rotateX) * sin(thelta) + (hnum - write_rotateY) * cos(thelta) + rotateY;
			double d_original_img_wnum = (wnum - write_rotateX) * cos(thelta) - (hnum - write_rotateY) * sin(thelta) + rotateX;

			if (d_original_img_hnum < 0 || d_original_img_hnum > height || d_original_img_wnum < 0 || d_original_img_wnum > width) {
				aftData[index] = 0; // 这个相当于是R
				aftData[index + 1] = 0; // 这个相当于是G
				aftData[index + 2] = 0;  // 这个相当于是B
				continue;
			}
			else {
				/**
				 * 我们在这里使用双线性插值法来完成对应
				*/
				int i_original_img_hnum = d_original_img_hnum;
				int i_original_img_wnum = d_original_img_wnum;
				double distance_to_a_X = d_original_img_wnum - i_original_img_wnum;
				double distance_to_a_Y = d_original_img_hnum - i_original_img_hnum;

				int original_point_A = i_original_img_hnum * l_width + i_original_img_wnum * 3;
				int original_point_B = i_original_img_hnum * l_width + (i_original_img_wnum + 1) * 3;
				int original_point_C = (i_original_img_hnum + 1) * l_width + i_original_img_wnum * 3;
				int original_point_D = (i_original_img_hnum + 1) * l_width + (i_original_img_wnum + 1) * 3;

				if (i_original_img_wnum == width - 1) {
					// cout << "test" << endl;
					original_point_A = original_point_B;
					original_point_C = original_point_D;
				}
				if (i_original_img_hnum == height - 1) {
					original_point_C = original_point_A;
					original_point_D = original_point_B;
				}

				aftData[index] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D];

				aftData[index + 1] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A + 1]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B + 1]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C + 1]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D + 1];

				aftData[index + 2] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * preData[original_point_A + 2]
					+ (1 - distance_to_a_X) * distance_to_a_Y * preData[original_point_B + 2]
					+ distance_to_a_X * (1 - distance_to_a_Y) * preData[original_point_C + 2]
					+ distance_to_a_X * distance_to_a_Y * preData[original_point_D + 2];
			}
		}
	}
	fwrite(aftData, 1, LaterImg, targetFile);
	fclose(file);
	fclose(targetFile);
	return 0;
}

int main()
{
	cout << "please input the bmp file's name : ";
	cin >> fileName;
	file = fopen(fileName, "rb");
	targetFile = fopen("RotImage.bmp", "wb");
	fread(&bfType, 1, sizeof(WORD), file);
	fwrite(&bfType, 1, sizeof(WORD), targetFile);
	if (0x4d42 != bfType) {
		cout << "wrong format!" << endl;
		return -1;
	}
	RotImage();
	file = fopen(fileName, "rb");
	targetFile = fopen("ScaImage.bmp", "wb");
	fread(&bfType, 1, sizeof(WORD), file);
	fwrite(&bfType, 1, sizeof(WORD), targetFile);
	ScaImage();
	return 0;
}