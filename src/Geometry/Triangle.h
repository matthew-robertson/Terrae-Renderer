#ifndef TRIANGLEH
#define TRIANGLEH

#include "Hitable.h"

class Triangle: public Hitable {
	public:
		Triangle() {}
		Triangle(Vec3 p0, Vec3 p1, Vec3 p2, Material *mat): _p0(p0), _p1(p1), _p2(p2), _mat(mat) {}

		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			Vec3 _min = min(min(_p0,_p1), _p2);
			Vec3 _max = max(max(_p0,_p1), _p2);
			box = AABB(_min, _max);
			return true;
		}

	private:
		Vec3 _p0,_p1,_p2;
		Material *_mat;
};

bool Triangle::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	double EPSILON = 0.00001;
	Vec3 e1,e2,h,s,q;
	double det,f,u,v;

	e1 = _p1 - _p0;
	e2 = _p2 - _p0;
	h = cross(r.direction(), e2);
	det = dot(e1, h);

	if (det <= EPSILON) return false;

	double invDet = 1.0/det;
	Vec3 tvec = r.origin() - _p0;

	rec.u = dot(tvec, h) * invDet; 
    if (rec.u < 0 || rec.u > 1) return false; 
 
    Vec3 qvec = cross(tvec, e1); 
    rec.v = dot(r.direction(), qvec) * invDet; 
    if (rec.v < 0 || rec.u + rec.v > 1) return false; 
 
 	rec.normal = cross(e1,e2);
	rec.hitMat = _mat;
    rec.t = dot(e2, qvec) * invDet; 

	return true;
}

#endif