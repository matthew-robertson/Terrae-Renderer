#ifndef CONSTANTMEDIUMH
#define CONSTANTMEDIUMH

#include <float.h>
#include <iostream>

#include "Hitable.h"
#include "../Mats/IsotropicMaterial.h"
#include "../Math/Utils.h"

class ConstantMedium: public Hitable {
	public:
		ConstantMedium(Hitable *b, double d, Texture *a): _boundary(b), _density(d), _albedo(a) {
			_phaseFunction = new IsotropicMaterial(a);
		}
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			return _boundary->boundingBox(t0,t1,box);
		}

	private:
		Hitable *_boundary;
		double _density;
		Texture *_albedo;
		Material *_phaseFunction;
};

bool ConstantMedium::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	hit_record rec1, rec2;

	if(_boundary->checkIntersection(r,-DBL_MAX,DBL_MAX, rec1)) {
		if(_boundary->checkIntersection(r, rec1.t+0.0001, DBL_MAX, rec2)) {
			if (db) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
			if (rec1.t < tMin) rec1.t = tMin;
			if (rec2.t > tMax) rec2.t = tMax;
			if (rec1.t < 0.) rec1.t = 0;
			if (rec1.t >= rec2.t) return false;

			double distInBoundary = (rec2.t - rec1.t) * r.direction().length();
			double hitDistance = -(1.0/_density) * log(randDouble());
			if (hitDistance < distInBoundary) {
				rec.u = 0.0;
				rec.v = 0.0; 	
				rec.t = rec1.t + hitDistance/r.direction().length();
				rec.pos = r.pointAtParam(rec.t);
				rec.normal = Vec3(1.0,0,0);
				rec.hitMat = _phaseFunction;
				return true;
			}
		}
	}
	return false;
}
#endif