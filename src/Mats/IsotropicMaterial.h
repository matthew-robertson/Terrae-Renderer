#ifndef ISOTROPICH
#define ISOTROPICH

#include "../Textures/Texture.h"
#include "Material.h"

class IsotropicMaterial : public Material {
	public:
		IsotropicMaterial(Texture * t): albedo(t) {}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			scattered = Ray(rec.pos, normalize(randomInUnitSphere()));
			attenuation = albedo->value(rec.u, rec.v, rec.pos);
			return true;
		}

	private:
		Texture *albedo;
};
#endif