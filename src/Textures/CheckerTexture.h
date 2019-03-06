#ifndef CHECKERTEXTUREH
#define CHECKERTEXTUREH

#include "Texture.h"

class CheckerTexture : public Texture {
	public:
		CheckerTexture() {}
		CheckerTexture(Texture *t1, Texture *t2): _even(t1), _odd(t2) {}
		virtual Vec3 value(double u, double v, const Vec3& p) const {
			double sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());

			if (sines < 0) {
				return _odd->value(u,v,p);
			} else {
				return _even->value(u,v,p);
			}
		}

	private:
		Texture *_odd;
		Texture *_even;
};

#endif