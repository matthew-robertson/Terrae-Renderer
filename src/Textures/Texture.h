#ifndef TEXTUREH
#define TEXTUREH

#include "../Math/Vec3.h"

class Texture {
	public:
		virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class ConstantTexture: public Texture {
	public:
		ConstantTexture() {}
		ConstantTexture(Vec3 c): _colour(c) {}

		virtual Vec3 value(double u, double v, const Vec3& p) const { return _colour; }

	private:
		Vec3 _colour;
};

#endif