#ifndef TRANSFORMSH
#define TRANSFORMSH

#include <float.h>

class FlipNormals: public Hitable {
	public:
		FlipNormals(Hitable *p): _toFlip(p) {}
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
			if (_toFlip->checkIntersection(r, tMin, tMax, rec)) {
				rec.normal = -rec.normal;
				return true;
			} else {
				return false;
			}
		}
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			return _toFlip->boundingBox(t0,t1,box);
		}

	private:
		Hitable *_toFlip;
};

class Translate: public Hitable {
	public:
		Translate(Hitable *p, Vec3 displacement): _toTranslate(p), _offset(displacement) {}
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
			Ray moved_r(r.origin() - _offset, r.direction(), r.time());
			if (_toTranslate->checkIntersection(moved_r, tMin, tMax, rec)){
				rec.pos += _offset;
				return true;
			} else {
				return false;
			}
		}

		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			if (_toTranslate->boundingBox(t0,t1,box)){
				box = AABB(box.min() + _offset, box.max() + _offset);
				return true;
			} else {
				return false;
			}
		}

	private:
		Hitable *_toTranslate;
		Vec3 _offset;
};

class RotateY: public Hitable {
	public:
		RotateY(Hitable *p, double theta);
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			box = _bbox; return _hasBox;
		}

	private:
		Hitable *_toRotate;
		double _sinTheta;
		double _cosTheta;
		bool _hasBox;
		AABB _bbox;
};

RotateY::RotateY(Hitable *p, double theta): _toRotate(p) {
	double rads = (3.14159/180.) * theta;
	_sinTheta = sin(rads);
	_cosTheta = cos(rads);

	_hasBox = _toRotate->boundingBox(0,1,_bbox);
	
	Vec3 min = Vec3(DBL_MAX,DBL_MAX,DBL_MAX);
	Vec3 max = Vec3(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double x = i*_bbox.max().x() + (1-i)*_bbox.min().x();
				double y = j*_bbox.max().y() + (1-j)*_bbox.min().y();
				double z = k*_bbox.max().z() + (1-k)*_bbox.min().z();
				double newX = _cosTheta*x + _sinTheta*z;
				double newZ = -_sinTheta*x + _cosTheta*z;

				Vec3 test = Vec3(newX,y,newZ);
				for (int l = 0; l < 3; l++) {
					if (test[l] > max[l]) max[l] = test[l];
					if (test[l] < min[l]) min[l] = test[l];
				}
			}
		}
	}
	_bbox = AABB(min,max);
}

bool RotateY::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	Vec3 o = r.origin();
	Vec3 d = r.direction();
	o[0] = _cosTheta*r.origin().x() - _sinTheta*r.origin().z();
	o[2] = _sinTheta*r.origin().x() + _cosTheta*r.origin().z();
	d[0] = _cosTheta*r.direction().x() - _sinTheta*r.direction().z();
	d[2] = _sinTheta*r.direction().x() + _cosTheta*r.direction().z();	

	Ray rotatedR = Ray(o,d,r.time());
	if (_toRotate->checkIntersection(rotatedR, tMin, tMax, rec)) {
		Vec3 p = rec.pos;
		Vec3 n = rec.normal;
		p[0] = _cosTheta*rec.pos.x() + _sinTheta*rec.pos.z();
		p[2] = -_sinTheta*rec.pos.x() + _cosTheta*rec.pos.z();
		n[0] = _cosTheta*rec.normal.x() + _sinTheta*rec.normal.z();
		n[2] = -_sinTheta*rec.normal.x() + _cosTheta*rec.normal.z();

		rec.pos = p;
		rec.normal = n;
		return true;
	} else {
		return false;
	}
}

class RotateZ: public Hitable {
	public:
		RotateZ(Hitable *p, double theta);
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			box = _bbox; return _hasBox;
		}

	private:
		Hitable *_toRotate;
		double _sinTheta;
		double _cosTheta;
		bool _hasBox;
		AABB _bbox;
};

RotateZ::RotateZ(Hitable *p, double theta): _toRotate(p) {
	double rads = (3.14159/180.) * theta;
	_sinTheta = sin(rads);
	_cosTheta = cos(rads);

	_hasBox = _toRotate->boundingBox(0,1,_bbox);
	
	Vec3 min = Vec3(DBL_MAX,DBL_MAX,DBL_MAX);
	Vec3 max = Vec3(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double x = i*_bbox.max().x() + (1-i)*_bbox.min().x();
				double y = j*_bbox.max().y() + (1-j)*_bbox.min().y();
				double z = k*_bbox.max().z() + (1-k)*_bbox.min().z();
				double newX = _cosTheta*x - _sinTheta*y;
				double newY = _sinTheta*x + _cosTheta*y;

				Vec3 test = Vec3(newX,newY,z);
				for (int l = 0; l < 3; l++) {
					if (test[l] > max[l]) max[l] = test[l];
					if (test[l] < min[l]) min[l] = test[l];
				}
			}
		}
	}
	_bbox = AABB(min,max);
}

bool RotateZ::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	Vec3 o = r.origin();
	Vec3 d = r.direction();
	o[0] = _cosTheta*r.origin().x() + _sinTheta*r.origin().y();
	o[1] = -_sinTheta*r.origin().x() + _cosTheta*r.origin().y();
	d[0] = _cosTheta*r.direction().x() + _sinTheta*r.direction().y();
	d[1] = -_sinTheta*r.direction().x() + _cosTheta*r.direction().y();	

	Ray rotatedR = Ray(o,d,r.time());
	if (_toRotate->checkIntersection(rotatedR, tMin, tMax, rec)) {
		Vec3 p = rec.pos;
		Vec3 n = rec.normal;
		p[0] = _cosTheta*rec.pos.x() - _sinTheta*rec.pos.y();
		p[1] = _sinTheta*rec.pos.x() + _cosTheta*rec.pos.y();
		n[0] = _cosTheta*rec.normal.x() - _sinTheta*rec.normal.y();
		n[1] = _sinTheta*rec.normal.x() + _cosTheta*rec.normal.y();

		rec.pos = p;
		rec.normal = n;
		return true;
	} else {
		return false;
	}
}

class RotateX: public Hitable {
	public:
		RotateX(Hitable *p, double theta);
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const {
			box = _bbox; return _hasBox;
		}

	private:
		Hitable *_toRotate;
		double _sinTheta;
		double _cosTheta;
		bool _hasBox;
		AABB _bbox;
};

RotateX::RotateX(Hitable *p, double theta): _toRotate(p) {
	double rads = (3.14159/180.) * theta;
	_sinTheta = sin(rads);
	_cosTheta = cos(rads);

	_hasBox = _toRotate->boundingBox(0,1,_bbox);
	
	Vec3 min = Vec3(DBL_MAX,DBL_MAX,DBL_MAX);
	Vec3 max = Vec3(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double x = i*_bbox.max().x() + (1-i)*_bbox.min().x();
				double y = j*_bbox.max().y() + (1-j)*_bbox.min().y();
				double z = k*_bbox.max().z() + (1-k)*_bbox.min().z();
				double newY = _cosTheta*y - _sinTheta*z;
				double newZ = _sinTheta*y + _cosTheta*z;

				Vec3 test = Vec3(x,newY,newZ);
				for (int l = 0; l < 3; l++) {
					if (test[l] > max[l]) max[l] = test[l];
					if (test[l] < min[l]) min[l] = test[l];
				}
			}
		}
	}
	_bbox = AABB(min,max);
}

bool RotateX::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	Vec3 o = r.origin();
	Vec3 d = r.direction();
	o[1] = _cosTheta*r.origin().y() + _sinTheta*r.origin().z();
	o[2] = -_sinTheta*r.origin().y() + _cosTheta*r.origin().z();
	d[1] = _cosTheta*r.direction().y() + _sinTheta*r.direction().z();
	d[2] = -_sinTheta*r.direction().y() + _cosTheta*r.direction().z();	

	Ray rotatedR = Ray(o,d,r.time());
	if (_toRotate->checkIntersection(rotatedR, tMin, tMax, rec)) {
		Vec3 p = rec.pos;
		Vec3 n = rec.normal;
		p[1] = _cosTheta*rec.pos.y() - _sinTheta*rec.pos.z();
		p[2] = _sinTheta*rec.pos.y() + _cosTheta*rec.pos.z();
		n[1] = _cosTheta*rec.normal.y() - _sinTheta*rec.normal.z();
		n[2] = _sinTheta*rec.normal.y() + _cosTheta*rec.normal.z();

		rec.pos = p;
		rec.normal = n;
		return true;
	} else {
		return false;
	}
}
#endif