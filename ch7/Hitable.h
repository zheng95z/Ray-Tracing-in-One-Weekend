#pragma once  
#include "Ray.h"

//撞击点处信息
struct hit_record
{
	//射线参数t
	float t;
	//撞击点位置向量p
	Vec3 p;
	//撞击点处法向量N
	Vec3 normal;
};

//所有能被射线撞击的物体的父类
class Hitable
{
public:
	//hit()在此被声明为虚函数，则hitable为抽象类。抽象类的子类中必须实现其虚函数
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};
