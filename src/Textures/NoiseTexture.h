#ifndef NOISETEXTUREH
#define NOISETEXTUREH

#include "Texture.h"
#include "../Math/Perlin.h"

class NoiseTexture: public Texture {
	public:
		NoiseTexture(double sc): _scale(sc) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			//return Vec3(1.)*noise.turb(_scale*p);
			return Vec3(1.)*.5*(1. + sin(_scale*p.z() + 10.*noise.turb(p)));
		}

	private:
		Perlin noise;
		double _scale;
};

#endif