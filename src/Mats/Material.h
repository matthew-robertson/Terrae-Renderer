#ifndef MATERIALH
#define MATERIALH

#include "../Math/Ray.h"
#include "../Math/Vec3.h"

class Material {
	public:
		virtual bool scatter(const Ray& rIn, const hit_record& hitRec, Vec3& attenuation, Ray& scattered) const = 0;
		virtual Vec3 emitted(double u, double v, const Vec3& p) const {
			return Vec3(0.0);
		}
};

#endif