#ifndef BOXH
#define BOXH

#include "HitableCollection.h"
#include "Rect.h"
#include "Transforms.h"

class Box: public Hitable {
	public:
		Box() {}
		Box(const Vec3& p0, const Vec3& p1, Material *mat);
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			box = AABB(_min,_max);
			return true;
		}

	private:
		Hitable *_planes;
		Vec3 _min,_max;
};

Box::Box(const Vec3& p0, const Vec3& p1, Material *mat) {
	_min = p0;
	_max = p1;

	Vec3 c = (p1 - p0)/2 + p0;
	Vec3 w = (p1 - p0)/2;

	Hitable **planes = new Hitable*[6];
	planes[0] = new XYRect(Vec3(c.x(), c.y(), p1.z()), Vec3(w.x(), w.y(), 0), mat);
	planes[1] = new FlipNormals(new XYRect(Vec3(c.x(), c.y(), p0.z()), Vec3(w.x(), w.y(), 0), mat));
	planes[2] = new XZRect(Vec3(c.x(), p1.y(), c.z()), Vec3(w.x(), 0, w.z()), mat);
	planes[3] = new FlipNormals(new XZRect(Vec3(c.x(), p0.y(), c.z()), Vec3(w.x(), 0, w.z()), mat));
	planes[4] = new YZRect(Vec3(p1.x(), c.y(), c.z()), Vec3(0, w.y(), w.z()), mat);
	planes[5] = new FlipNormals(new YZRect(Vec3(p0.x(), c.y(), c.z()), Vec3(0, w.y(), w.z()), mat));
	_planes = new HitableCollection(planes, 6);
}

bool Box::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const{
	return _planes->checkIntersection(r,tMin,tMax,rec);
}

#endif