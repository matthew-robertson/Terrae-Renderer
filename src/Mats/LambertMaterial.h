#ifndef LAMBERTH
#define LAMBERTH

#include "../Textures/Texture.h"

class LambertMaterial : public Material {
	public:
		LambertMaterial(Texture * t): albedo(t) {}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			Vec3 targetPoint = rec.pos +rec.normal + randomInUnitSphere();
			scattered = Ray(rec.pos, targetPoint - rec.pos, rIn.time());
			attenuation = albedo->value(rec.u, rec.v, rec.pos);
			return true;
		}

	private:
		Texture *albedo;
};
#endif