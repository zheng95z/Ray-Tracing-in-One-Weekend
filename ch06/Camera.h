#pragma once
#include "ray.h"

class camera {
public:
	camera() {
		lower_left_corner = vec3(-2.0, -1.0, -1.0); // ×óÏÂ½Ç
		horizontal = vec3(4.0, 0.0, 0.0); // ºáÖá
		vertical = vec3(0.0, 2.0, 0.0); // ÊúÖá
		origin = vec3(0.0, 0.0, 0.0); // eye
	}
	ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical); }

	vec3 lower_left_corner;
	vec3 origin;
	vec3 horizontal;
	vec3 vertical;
};