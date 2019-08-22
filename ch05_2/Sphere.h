#pragma once
#include "hitable.h"

class sphere :public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r) { center = cen; radius = r; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	vec3 center;
	float radius;
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
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}