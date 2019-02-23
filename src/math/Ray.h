#ifndef RAYH
#define RAYH
#include "Vec3.h"

class Ray {
	public:
		Ray () {}
		Ray (const Vec3 &o, const Vec3 &d) { _o = o; _d = d; }
		Vec3 direction() const { return _d; }
		Vec3 origin() const { return _o; }
		Vec3 pointAtParam(const double t) const { return _o + t*_d;}

	private:
		Vec3 _o;
		Vec3 _d;
};
#endif