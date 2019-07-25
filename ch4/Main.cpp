#include <iostream>
#include <fstream>
#include "Ray.h"
using namespace std;

bool hit_sphere(const Vec3& center, float radius, const Ray& r)
{
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f*dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discrimiant = b*b - 4.0f*a*c;
	return (discrimiant > 0.0f);
}

Vec3 Color(const Ray& r)
{
	if (hit_sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f, r))
		return Vec3(1.0f, 0.0f, 0.0f);

	Vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f*(unit_direction.y() + 1.0f);

	//(1-t)*白色+t*蓝色，结果是一个蓝白的渐变
	return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{

	ofstream outfile;
	outfile.open("ch4Image.ppm");

	int nx = 200;
	int ny = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Vec3 vertical(0.0f, 2.0f, 0.0f);
	Vec3 origin(0.0f, 0.0f, 0.0f);


	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			Vec3 col = Color(r);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}