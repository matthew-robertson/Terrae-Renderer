#ifndef CAMERAH
#define CAMERAH
#include <iostream>

#include "Math/Ray.h"
#include "Math/Vec3.h"
#include "Math/Utils.h"

class Camera {
	public:
		Camera (Vec3 lookFrom, Vec3 lookAt, Vec3 up, double vfov, double aspect,
				double aperature, double focalDist, double ti, double tf) {
			_lensRadius = aperature/2.0;
			double theta = vfov*3.14159/180.0;
			double halfHeight = tan(theta/2.0);
			double halfWidth = halfHeight * aspect;

			_ti = ti;
			_tf = tf;

			origin = lookFrom;
			_w = normalize(lookFrom - lookAt);
			_u = normalize(cross(up, _w));
			_v = cross(_w, _u);

			_lowerLeftCorner = origin - halfWidth*focalDist*_u - halfHeight*focalDist*_v - focalDist*_w;
			_horizontalAxis  = 2.0*halfWidth*focalDist*_u;
			_verticalAxis    = 2.0*halfHeight*focalDist*_v;
		}

		Ray getRay(double s, double t) {
			Vec3 randomLensPoint = _lensRadius * randomInUnitDisc();
			Vec3 offset = randomLensPoint.x()*_u + randomLensPoint.y()*_v;
			double time = _ti + randDouble()*(_tf-_ti);

			return Ray(origin + offset, _lowerLeftCorner+ s*_horizontalAxis + t*_verticalAxis - origin - offset, time);
		}

		Vec3 origin;

	private: 
		Vec3 _lowerLeftCorner;
		Vec3 _horizontalAxis;
		Vec3 _verticalAxis;
		Vec3 _u,_w,_v;
		double _lensRadius;
		double _ti;
		double _tf;
};

#endif