#ifndef XYRECTH
#define XYRECTH

#include <iostream>
class XYRect : public Hitable {
	public:
		XYRect() {}
		XYRect(Vec3 c, Vec3 wh, Material *mat): _center(c), _dims(wh), _mat(mat) {}

		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			box = AABB(Vec3(_center.x() - _dims.x(), _center.y() - _dims.y(), _center.z() - 0.0001),
						Vec3(_center.x() + _dims.x(), _center.y() + _dims.y(), _center.z() + 0.0001));
			return true;
		}

	private:
		Vec3 _center;
		Vec3 _dims;
		Material *_mat;

};

bool XYRect::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	if (r.direction().z() == 0.0) {
		return false;
	}

	double t = (_center.z() - r.origin().z())/ r.direction().z();
	if (t < tMin || t > tMax) {
		return false;
	}

	Vec3 hitPoint = r.pointAtParam(t);
	if (hitPoint.x() < _center.x() - _dims.x() || hitPoint.x() > _center.x() + _dims.x() ||
		hitPoint.y() < _center.y() - _dims.y() || hitPoint.y() > _center.y() + _dims.y()) {
		return false;
	}

	rec.t = t;
	rec.pos = r.pointAtParam(t);
	rec.normal = Vec3(0.0, 0., 1.0);
	rec.hitMat = _mat;
	return true;

}

#endif