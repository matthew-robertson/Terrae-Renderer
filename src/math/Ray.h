#ifndef RAYH
#define RAYH
#include "Vec3.h"

class Ray {
	public:
		Ray () {}
		Ray (const Vec3 &o, const Vec3 &d, const double t = 0.0) { _o = o; _d = d; _time = t; }
		Vec3 direction() const { return _d; }
		Vec3 origin() const { return _o; }
		double time() const { return _time; }
		Vec3 pointAtParam(const double t) const { return _o + t*_d;}

	private:
		Vec3 _o;
		Vec3 _d;
		double _time;
};
#endif