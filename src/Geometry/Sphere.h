#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"
#include "../Mats/Material.h"

void getSphereUVs(const Vec3& p, double& u, double& v) {
	double phi = atan2(p.z(), p.x());
	double theta = asin(p.y());
	u = 1. - (phi + 3.14159) / (2.0*3.14159);
	v = (theta + 3.14159/2.0) / 3.14159;
}

class Sphere: public Hitable {
	public:
		Sphere() {}
		Sphere(Vec3 ci, Vec3 cf, double r, double ti, double tf, Material *m) : 
			_radius(r), _centeri(ci), _centerf(cf), _ti(ti), _tf(tf), _mat(m) {};
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const;
		bool boundingBoxAtT(double t, AABB& box) const;

		Vec3 center(double t) const;

	private:
		Vec3 _centeri, _centerf;
		double _ti, _tf;
		double _radius;
		Material *_mat;
};

Vec3 Sphere::center(double t) const {
	return _centeri + (t - _ti)/(_tf - _ti) * (_centerf - _centeri);
}

bool Sphere::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	Vec3 ctr = center(r.time());
	Vec3 oc = r.origin() - ctr;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - _radius*_radius;
	double det = b*b - a*c; 

	if (det > 0) {
		double temp = (-b - sqrt(det))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.pos = r.pointAtParam(rec.t);
			rec.normal = (rec.pos - ctr) / _radius;
			rec.hitMat = _mat;
			getSphereUVs((rec.pos - ctr)/_radius, rec.u, rec.v);

			return true;
		}
		
		temp = (-b + sqrt(det))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.pos = r.pointAtParam(rec.t);
			rec.normal = (rec.pos - ctr) / _radius;
			rec.hitMat = _mat;
			getSphereUVs((rec.pos - ctr)/_radius, rec.u, rec.v);

			return true;
		}
	}

	return false;
}

bool Sphere::boundingBoxAtT(double t, AABB& box) const {
	box = AABB(center(t)-Vec3(_radius), center(t)+Vec3(_radius));
	return true;
}

bool Sphere::boundingBox(double t0, double t1, AABB& box) const {
	AABB b0, b1;
	boundingBoxAtT(t0, b0);
	boundingBoxAtT(t1, b1);

	box = surroundingBox(b0, b1);
	return true;
}
#endif