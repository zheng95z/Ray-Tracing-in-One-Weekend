//#ifdef RAYH
//#define RAYH
//#endif
#include "Vec3.h"

class Ray
{
public:
	Ray(){}
	Ray(const Vec3& a, const Vec3& b) { A = a; B = b; }

	//起点向量
	Vec3 origin() const { return A; }

	//方向向量
	Vec3 direction() const { return B; }

	//p(t) = A + t*B.
	Vec3 point_at_parameter(float t) const { return A + t*B; }

	Vec3 A;
	Vec3 B;
};
