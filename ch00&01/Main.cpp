//*************************************
// book1(RT in 1 weekend) ch0&1
// 相较于原书，添加如下功能：
// *(ch00&01)导出为png格式
// *(ch00&01)渲染进度

//*************************************

// from https://github.com/nothings/stb.git
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main() {
	// width, height, channels of image
	int nx = 1920; // width
	int ny = 1080; // height
	int channels = 3;
	// 存储图像数据
	unsigned char *data = new unsigned char[nx*ny*channels];
	
	// 循环遍历图像nx*ny中的每个像素
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;
			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);
			// data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i] = ir;
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = ig;
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = ib;
		}
		// 计算渲染进度
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// 写出png图片
	stbi_write_png("ch00&01.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
}