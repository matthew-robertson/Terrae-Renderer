#ifndef CAMERAH
#define CAMERAH
#include <iostream>

#include "Math/Ray.h"
#include "Math/Vec3.h"

class Camera {
	public:
		Camera () {
			lowerLeftCorner = Vec3(-2.0, -1.0, -1.0);
			horizontalAxis  = Vec3( 4.0,  0.0,  0.0);
			verticalAxis    = Vec3( 0.0,  2.0,  0.0);
			origin          = Vec3( 0.0,  0.0,  0.0);
		}

		Ray getRay(double u, double v) {
			return Ray(origin, lowerLeftCorner+ u*horizontalAxis + v*verticalAxis - origin);
		}

		Vec3 lowerLeftCorner;
		Vec3 horizontalAxis;
		Vec3 verticalAxis;
		Vec3 origin;
};

#endif