#pragma once  
#include "Hitable.h"
#include "Material.h"
class Sphere : public Hitable {
public:
	Sphere() {}
	//此处为使用初始化列表的构造函数来初始化成员变量
	Sphere(Vec3 cen, float r, Material *m) : center(cen), radius(r),ma(m){};
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	Vec3 center;
	float radius;
	Material *ma;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = (b*b - 4.0f*a*c);
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (2.0f*a);
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = ma;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / (2.0f*a);
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = ma;
			return true;
		}
	}
	return false;
}