#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"

class Sphere: public Hitable {
	public:
		Sphere() {}
		Sphere(Vec3 c, double r) : center(c), radius(r) {};
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;

		Vec3 center;
		double radius;
};

bool Sphere::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	Vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double det = b*b - a*c; 

	if (det > 0) {
		double temp = (-b - sqrt(det))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.pos = r.pointAtParam(rec.t);
			rec.normal = (rec.pos - center) / radius;
			return true;
		}
		
		temp = (-b + sqrt(det))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.pos = r.pointAtParam(rec.t);
			rec.normal = (rec.pos - center) / radius;
			return true;
		}
	}

	return false;
}

#endif