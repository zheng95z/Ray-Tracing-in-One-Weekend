#pragma once

#include "Hitable.h"

Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

Vec3 RandomInUnitSphere()
{
	Vec3 p;
	do {
		p = 2.0f * Vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - Vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1.0f);

	return p;
}

class Material {
public:
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a):albedo(a){}
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	Vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 reflected = Reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*RandomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	Vec3 albedo;
	float fuzz;
};