#pragma once

#include "Ray.h"

class Camera 
{
public:
	Camera()
	{
		lower_left_corner = Vec3(-2.0f, -1.0f, -1.0f);
		horizontal = Vec3(4.0f, 0.0f, 0.0f);
		vertical = Vec3(0.0f, 2.0f, 0.0f);
		origin = Vec3(0.0f, 0.0f, 0.0f);
	}

	Ray getRay(float u, float v) 
	{
		return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	}

	Vec3 lower_left_corner;
	Vec3 origin;
	Vec3 horizontal;
	Vec3 vertical;
};