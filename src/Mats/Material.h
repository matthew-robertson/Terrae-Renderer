#ifndef MATERIALH
#define MATERIALH

#include "../Math/Ray.h"
#include "../Math/Vec3.h"

class Material {
	public:
		virtual bool scatter(const Ray& rIn, const hit_record& hitRec, Vec3& attenuation, Ray& scattered) const = 0;
};

#endif