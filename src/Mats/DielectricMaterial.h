#ifndef DIELECTRICH
#define DIELECTRICH
#include <iostream>

#include "../Math/Utils.h"

double schlick(double cosine, double refIdx) {
	double r0 = (1.0 - refIdx) / (1.0 + refIdx);
	r0 = r0*r0;
	return r0 + (1.0 - r0)*pow((1.0-cosine), 5.0);
}

class DielectricMaterial : public Material {
	public:
		DielectricMaterial(double ri): refIdx(ri) {}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			Vec3 outwardNormal;
			Vec3 reflected = reflect(rIn.direction(), rec.normal);
			Vec3 refracted;

			double niOverNt;
			double reflectProb;
			double cosine;

			attenuation = Vec3(1.0);
			
			if (dot(rIn.direction(), rec.normal) > 0.0) {
				outwardNormal = -rec.normal;
				niOverNt = refIdx;
				cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
			} else {
				outwardNormal = rec.normal;
				niOverNt = 1.0 / refIdx;
				cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
			}

			if (refract(rIn.direction(), outwardNormal, niOverNt, refracted)){
				reflectProb = schlick(cosine, refIdx);
			} else {
				reflectProb = 1.0;
			}

			if (randDouble() < reflectProb) {
				scattered = Ray(rec.pos, reflected);
			} else {
				scattered = Ray(rec.pos, refracted);
			}
			return true;
		}

	private:
		double refIdx;
};

#endif