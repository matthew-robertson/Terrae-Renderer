#ifndef METALH
#define METALH

#include "../Textures/Texture.h"

class MetalMaterial : public Material {
	public:
		MetalMaterial(Texture *t, double f): _albedo(t) { if (f < 1.0) _fuzz = f; else _fuzz = 1.0;}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			Vec3 ref = reflect(normalize(rIn.direction()), rec.normal);
			scattered = Ray(rec.pos, ref + _fuzz*randomInUnitSphere(), rIn.time());
			attenuation = _albedo->value(rec.u, rec.v, rec.pos);
			return dot(scattered.direction(), rec.normal) > 0.0;
		}

	private:
		Texture *_albedo;
		double _fuzz;
};
#endif