#include <iostream>
#include <fstream>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction()); // y: -1~1
		float t = 0.5f * (unit_direction.y() + 1.0f); // t: 0~1
		return (1 - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}	
}

hitable* random_scene()
{
	std::default_random_engine reng;
	std::uniform_real_distribution<float> uni_dist(0.0f, 1.0f);
	int n = 500;
	hitable **list = new hitable *[n + 1];
	list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = uni_dist(reng);
			vec3 center(a + 0.9f*uni_dist(reng), 0.2f, b + 0.9f*uni_dist(reng));
			if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
			{
				if (choose_mat < 0.8f)
				{
					list[i++] = new sphere(center, 0.2f,
						new lambertian(vec3(uni_dist(reng) * uni_dist(reng), uni_dist(reng) * uni_dist(reng), uni_dist(reng) * uni_dist(reng))));
				}
				else if (choose_mat < 0.95f)
				{
					list[i++] = new sphere(center, 0.2f,
						new metal(vec3(0.5f * (1 + uni_dist(reng)), 0.5f * (1 + uni_dist(reng)), 0.5f * (1 + uni_dist(reng))), 0.5f * uni_dist(reng)));
				}
				else
				{
					list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	return new hitable_list(list, i);
}

int main() 
{
	std::ofstream outfile;
	outfile.open("ch12_2000x1000.ppm");

	int nx = 2000;
	int ny = 1000;
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	int count = 0;
	int total = nx * ny;
	//-----------------------------------------------------

	hitable *world = random_scene();
	vec3 lookfrom(13.0f, 2.0f, 3.0f);
	vec3 lookat(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.1f;

	camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), 20.0f, float(nx)/float(ny), aperture, dist_to_focus);
	
	std::default_random_engine reng;
	std::uniform_real_distribution<float> uni_dist(0.0f, 1.0f);

	//------------------------------------------------------
	for (int j = ny - 1; j >= 0; j--) 
	{
		for (int i = 0; i < nx; i++) 
		{
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + uni_dist(reng)) / float(nx);
				float v = float(j + uni_dist(reng)) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_paramater(2.0f);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			outfile << ir << " " << ig << " " << ib << "\n";
		}
		count+=nx;
		if (count % (10*nx) == 0)
		{
			std::cout << count / (float)total * 100.0f << "%\n";
		}
	}
	outfile.close();
	return 0;
}