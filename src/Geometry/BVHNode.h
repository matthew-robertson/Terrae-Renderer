#ifndef BVHNODEH
#define BVHNODEH

#include <iostream>
#include <stdlib.h>

#include "Hitable.h"
#include "../Math/Utils.h"

int box_x_compare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hitable *ah = *(Hitable **)a;
	Hitable *bh = *(Hitable **)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor" << std::endl;
	}

	if (boxLeft.min().x() - boxRight.min().x() < 0.0) {
		return -1;
	} else {
		return 1;
	}
}

int box_y_compare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hitable *ah = *(Hitable **)a;
	Hitable *bh = *(Hitable **)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor" << std::endl;
	}

	if (boxLeft.min().y() - boxRight.min().y() < 0.0) {
		return -1;
	} else {
		return 1;
	}
}

int box_z_compare(const void *a, const void *b) {
	AABB boxLeft, boxRight;
	Hitable *ah = *(Hitable **)a;
	Hitable *bh = *(Hitable **)b;

	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor" << std::endl;
	}

	if (boxLeft.min().z() - boxRight.min().z() < 0.0) {
		return -1;
	} else {
		return 1;
	}
}

class BVHNode : public Hitable {
	public:
		BVHNode() {}
		BVHNode (Hitable **l, int n, double t0, double t1);
		virtual bool checkIntersection(const Ray& r, double tmin, double tmax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const;

		Hitable *_left;
		Hitable *_right;
		AABB _box;
};

bool BVHNode::boundingBox(double t0, double t1, AABB& box) const {
	box = _box;
	return true;
}

bool BVHNode::checkIntersection(const Ray& r, double tmin, double tmax, hit_record& rec) const {
	if (_box.hit(r, tmin, tmax)) {
		hit_record lRec, rRec;
		bool hitLeft = _left->checkIntersection(r, tmin, tmax, lRec);
		bool hitRight = _right->checkIntersection(r, tmin, tmax, rRec);

		if (hitLeft && hitRight) {
			if (lRec.t < rRec.t) {
				rec = lRec;
			} else {
				rec = rRec;
			}
			return true;
		} else if (hitLeft) {
			rec = lRec;
			return true;
		} else if (hitRight) {
			rec = rRec;
			return true;
		} else {
			return false;
		}

	} else {
		return false;
	}
}

BVHNode::BVHNode(Hitable **l, int n, double t0, double t1) {
	int axis = int(3*randDouble());
	if (axis == 0) {
		std::qsort(l, n, sizeof(Hitable *), box_x_compare);
	} else if (axis == 1) {
		std::qsort(l, n, sizeof(Hitable *), box_y_compare);
	} else {
		std::qsort(l, n, sizeof(Hitable *), box_z_compare);
	}

	if (n == 1) {
		_left = _right = l[0];
	} else if (n == 2) {
		_left  = l[0];
		_right = l[1];
	} else {
		_left = new BVHNode(l, n/2, t0, t1);
		_right = new BVHNode(l + n/2, n - n/2, t0, t1);
	}

	AABB boxLeft, boxRight;
	if (!_left->boundingBox(t0, t1, boxLeft) || !_right->boundingBox(t0, t1, boxRight)) {
		std::cerr << "No bounding box in BVHNode constructor" << std::endl;
	}

	_box = surroundingBox(boxLeft, boxRight);
}

#endif