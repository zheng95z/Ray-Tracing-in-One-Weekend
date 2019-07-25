#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"

using namespace std;

Vec3 Color(const Ray& r, Hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5f*Vec3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
	}
	else
	{
		//绘制背景
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		//(1-t)*白色+t*蓝色，结果是一个蓝白的渐变
		return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{

	ofstream outfile;
	outfile.open("ch5_2Image.ppm");

	int nx = 200;
	int ny = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Vec3 vertical(0.0f, 2.0f, 0.0f);
	Vec3 origin(0.0f, 0.0f, 0.0f);

	Hitable *list[2];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
	Hitable *world = new HitableList(list, 2);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			Ray r(origin, lower_left_corner + u*horizontal + v*vertical);

			Vec3 p = r.point_at_parameter(2.0);
			Vec3 col = Color(r,world);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}