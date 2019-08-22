#pragma once
#include "hitable.h"

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0f * vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	vec3 albedo;
};

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}

class metal : public material {
public:
	metal(const vec3& a, float f) : albedo(a), fuzz(f) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0); // 反射出的光线若与 normal 夹角大于90度，则不正确
	}
	vec3 albedo;
	float fuzz;
};

