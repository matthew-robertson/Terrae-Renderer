#ifndef CAMERAH
#define CAMERAH
#include <iostream>

#include "Math/Ray.h"
#include "Math/Vec3.h"

class Camera {
	public:
		Camera (Vec3 lookFrom, Vec3 lookAt, Vec3 up, double vfov, double aspect, double aperature, double focalDist) {
			lensRadius = aperature/2.0;
			double theta = vfov*3.14159/180.0;
			double halfHeight = tan(theta/2.0);
			double halfWidth = halfHeight * aspect;

			origin = lookFrom;
			w = normalize(lookFrom - lookAt);
			u = normalize(cross(up, w));
			v = cross(w, u);

			lowerLeftCorner = origin - halfWidth*focalDist*u - halfHeight*focalDist*v - focalDist*w;
			horizontalAxis  = 2.0*halfWidth*focalDist*u;
			verticalAxis    = 2.0*halfHeight*focalDist*v;
		}

		Ray getRay(double s, double t) {
			Vec3 randomLensPoint = lensRadius * randomInUnitDisc();
			Vec3 offset = randomLensPoint.x()*u + randomLensPoint.y()*v;
			return Ray(origin + offset, lowerLeftCorner+ s*horizontalAxis + t*verticalAxis - origin - offset);
		}

		Vec3 lowerLeftCorner;
		Vec3 horizontalAxis;
		Vec3 verticalAxis;
		Vec3 u,w,v;
		Vec3 origin;
		double lensRadius;
};

#endif