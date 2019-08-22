#pragma once
#include "hitable.h"

class sphere :public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) { center = cen; radius = r; mat_ptr = m; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 A_C = r.origin() - center;
	vec3 B = r.direction();
	float a = dot(B, B);
	float b = dot(A_C, B);
	float c = dot(A_C, A_C) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}