#ifndef HITABLEH
#define HITABLEH

#include "../Math/Ray.h"

struct hit_record {
	double t;
	Vec3 pos;
	Vec3 normal;
};

class Hitable {
	public:
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const = 0;
};
#endif