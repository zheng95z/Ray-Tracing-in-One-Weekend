#pragma once

#include "Hitable.h"

float Schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

//计算折射光线的方向向量。ni_over_nt 为入射介质的折射指数和折射介质的折射指数的比值。
bool Refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	//根号里面的内容小于零，说明折射光线的方向向量无实根，即没有折射光线，即出现全反射。所以，折射光线函数return false
	else
		return false;
}

//通过入射光线，计算反射光线
Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

//生成随机方向的标准向量
Vec3 RandomInUnitSphere()
{
	Vec3 p;
	do {
		p = 2.0f * Vec3((rand() % 100 / float(100)), (rand() % 100 / float(100)), (rand() % 100 / float(100))) - Vec3(1.0f, 1.0f, 1.0f);
	} while (dot(p, p) >= 1.0f);

	return p;
}

//抽象出的材质类
class Material {
public:
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

//漫反射材质
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

//镜面反射材质
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

class Dielectric : public Material {
public:
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outward_normal;
		Vec3 reflected = Reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			//cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = sqrt(1 - ref_idx*ref_idx*(1 - cosine*cosine));
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (Refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
			reflect_prob = Schlick(cosine, ref_idx);
		else
			reflect_prob = 1.0;
		if ((rand() % (100) / (float)(100)) < reflect_prob)
			scattered = Ray(rec.p, reflected);
		else
			scattered = Ray(rec.p, refracted);
		return true;
	}

	float ref_idx;
};