#ifndef CAMERAH
#define CAMERAH
#include <iostream>

#include "Math/Ray.h"
#include "Math/Vec3.h"

class Camera {
	public:
		Camera (Vec3 lookFrom, Vec3 lookAt, Vec3 up, double vfov, double aspect) {
			Vec3 u,w,v;
			double theta = vfov*3.14159/180.0;
			double halfHeight = tan(theta/2.0);
			double halfWidth = halfHeight * aspect;

			origin = lookFrom;

			w = normalize(lookFrom - lookAt);
			u = normalize(cross(up, w));
			v = cross(w, u);

			lowerLeftCorner = origin - halfWidth*u - halfHeight*v - w;
			horizontalAxis  = 2.0*halfWidth*u;
			verticalAxis    = 2.0*halfHeight*v;
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