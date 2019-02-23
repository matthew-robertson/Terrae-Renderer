#ifndef HITTABLECOLLECITONH
#define HITTABLECOLLECITONH

#include "Hitable.h"

class HitableCollection : public Hitable {
	public:
		HitableCollection() {}
		HitableCollection (Hitable **l, int n) { list = l; listSize = n; }
		virtual bool checkIntersection(const Ray& r, double tMin, double tMax, hit_record& rec) const;
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

#endif