//*************************************
// book1(RT in 1 weekend) ch4
// 相较于原书 添加如下功能：
// -(ch00&01)导出为png格式
// -(ch00&01)渲染进度
// -(ch04)图像的长宽比要与 horizontal/vertical 一致，否则会扭曲变形。这里可以选择修改图像长宽或 h/v 的值，我们这里选择修改图像长宽

//*************************************

// stb图像库 from https://github.com/nothings/stb.git
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include "ray.h"

bool hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 A_C = r.origin() - center;
	vec3 B = r.direction();
	float a = dot(B, B);
	float b = 2.0 * dot(A_C, B);
	float c = dot(A_C, A_C) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	return (discriminant > 0);
}

vec3 color(const ray& r) {
	// 如果 ray r 击中球体的话，返回球体的颜色
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);
	// 否则，计算背景色并返回
	vec3 unit_direction = unit_vector(r.direction());
	float t = (0.5 * unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	// width, height, channels of image
	int nx = 2000; // width
	int ny = 1000; // height
	int channels = 3;
	// 存储图像数据
	unsigned char *data = new unsigned char[nx*ny*channels];

	// camera相关
	vec3 lower_left_corner(-2.0, -1.0, -1.0); // 左下角
	vec3 horizontal(4.0, 0.0, 0.0); // 横轴
	vec3 vertical(0.0, 2.0, 0.0); // 竖轴
	vec3 origin(0.0, 0.0, 0.0); // eye
	
	// 循环遍历图像nx*ny中的每个像素
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			// 确定 ray r
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			// 计算 ray r 返回的颜色
			vec3 col = color(r);

			// 写入图像数据 data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i + 0] = int(255.99*col[0]);
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = int(255.99*col[1]);
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = int(255.99*col[2]);
		}
		// print渲染进度
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// 写出png图片
	stbi_write_png("ch04.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
	return 0;
}