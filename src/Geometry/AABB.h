#ifndef AABBH
#define AABBH

#include <algorithm>
#include <iostream>

#include "../Math/Utils.h"

class AABB {
	public:
		AABB() {}
		AABB(const Vec3& a, const Vec3& b) { _min = a; _max = b; }
		
		Vec3 max() const { return _max; }
		Vec3 min() const { return _min; }

		bool hit(const Ray& r, double tmin, double tmax) const;

	private:
		Vec3 _min;
		Vec3 _max;
};

inline bool AABB::hit(const Ray& r, double tmin, double tmax) const {
	for (int i = 0; i < 3; i++) {
		double invd = 1.0/r.direction()[i];
		double t0 = (min()[i] - r.origin()[i]) * invd;
		double t1 = (max()[i] - r.origin()[i]) * invd;

		if (invd < 0.0) {
			std::swap(t0, t1);
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if (tmax <= tmin) return false;
	}
	return true;
}

inline std::ostream& operator<<(std::ostream &os, const AABB &b) {
	os << "AABB(Min: " << b.min() << ", Max: " << b.max() << ")";
	return os;
}

AABB surroundingBox(AABB b0, AABB b1) {
	return AABB(min(b0.min(), b1.min()), max(b0.max(), b1.max()));
}
#endif