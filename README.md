# Image

**如何使用**

	按照对话框的提示输入文件路径、图片的旋转角度以及目标图片的分辨率后即可在文件目录下获得对应的输出文件。

**核心代码**
```cpp
//旋转部分
for (int hnum = 0; hnum < newHeight; ++hnum) {
		for (int wnum = 0; wnum < newWidth; ++wnum) {
			int index = hnum * writewidth + wnum * 3;
			double d_original_img_hnum = (wnum - write_rotateX) * sin(thelta) + (hnum - write_rotateY) * cos(thelta) + rotateY;
			double d_original_img_wnum = (wnum - write_rotateX) * cos(thelta) - (hnum - write_rotateY) * sin(thelta) + rotateX;

			if (d_original_img_hnum < 0 || d_original_img_hnum > height || d_original_img_wnum < 0 || d_original_img_wnum > width) {
				aftData[index] = 0; // 这个相当于是R
				aftData[index + 1] = 0; // 这个相当于是G
				aftData[index + 2] = 0;  // 这个相当于是B
				continue;
			}
			else {
				int i_original_img_hnum = d_original_img_hnum;
				int i_original_img_wnum = d_original_img_wnum;
				double distance_to_a_X = d_original_img_wnum - i_original_img_wnum;
				double distance_to_a_Y = d_original_img_hnum - i_original_img_hnum;
				int original_point_A = i_original_img_hnum * l_width + i_original_img_wnum * 3;
				int original_point_B = i_original_img_hnum * l_width + (i_original_img_wnum + 1) * 3;
				int original_point_C = (i_original_img_hnum + 1) * l_width + i_original_img_wnum * 3;
				int original_point_D = (i_original_img_hnum + 1) * l_width + (i_original_img_wnum + 1) * 3;
				if (i_original_img_wnum == width - 1) {
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
//缩放部分
for (i = 0; i < dheight; i++)
	{
		dwsrcY = i * height / dheight;
		pucDest = dest_data + i * dwidth * 3;
		pucSrc = src_data + dwsrcY * width * 3;
		for (j = 0; j < dwidth; j++)
		{
			dwsrcX = j * width / dwidth;
			memcpy(pucDest + j * 3, pucSrc + dwsrcX * 3, 3);//数据拷贝
		}
	}
```