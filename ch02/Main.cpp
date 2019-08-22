//*************************************
// book1(RT in 1 weekend) ch2
// 相较于原书 添加如下功能：
// -(ch00&01)导出为png格式
// -(ch00&01)渲染进度
// 

//*************************************

// stb图像库 from https://github.com/nothings/stb.git
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include "vec3.h"

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
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2f);
			// 写入图像数据 data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i + 0] = int(255.99*col[0]);
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = int(255.99*col[1]);
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = int(255.99*col[2]);
		}
		// print渲染进度
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// 写出png图片
	stbi_write_png("ch02.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
	return 0;
}