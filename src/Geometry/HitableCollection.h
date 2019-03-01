#ifndef HITTABLECOLLECITONH
#define HITTABLECOLLECITONH

#include "Hitable.h"

class HitableCollection : public Hitable {
	public:
		HitableCollection() {}
		HitableCollection (Hitable **l, int n) { list = l; listSize = n; }
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
		virtual bool boundingBox(double t0, double t1, AABB& box) const;
		Hitable **list;
		int listSize;
};

bool HitableCollection::checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const {
	hit_record temp;
	bool hitAnything = false;
	double closestSoFar = tMax;

	for (int i = 0; i < listSize; i++) {
		if (list[i]->checkIntersection(r, tMin, closestSoFar, temp)) {
			hitAnything = true;
			closestSoFar = temp.t;
			rec = temp;
		}
	}

	return hitAnything;
}

bool HitableCollection::boundingBox(double t0, double t1, AABB& box) const {
	if (listSize < 1) return false;

	AABB tempBox;
	bool firstHasBox = list[0]->boundingBox(t0, t1, tempBox);
	if (!firstHasBox) {
		return false;
	} else {
		box = tempBox;
	}

	for (int i = 1; i < listSize; i++) {
		if (list[i]->boundingBox(t0,t1,tempBox)) {
			box = surroundingBox(box, tempBox);
		} else {
			return false;
		}
	}
	return true;
}

#endif