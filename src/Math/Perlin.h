#ifndef PERLINH
#define PERLINH

#include <math.h>

#include "Utils.h"

inline double perlinInterp(Vec3 c[2][2][2], double u, double v, double w) {
	double uu = u*u*(3-2*u);
	double vv = v*v*(3-2*v);
	double ww = w*w*(3-2*w);

	double accum = 0.0;
	for (int i=0; i<2; i++) {
		for (int j=0; j<2; j++) {
			for (int k=0; k<2; k++) {
				Vec3 weightV(u-i, v-j, w-k);
				accum += (i*uu + (1-i)*(1-uu)) *
						(j*vv + (1-j)*(1-vv)) *
						(k*ww + (1-k)*(1-ww)) * dot(c[i][j][k], weightV);
			}
		}
	}
	return accum;
}

class Perlin {
	public:
		double noise(const Vec3& p) const {
			double u = p.x() - floor(p.x());
			double v = p.y() - floor(p.y());
			double w = p.z() - floor(p.z());
			int i = int(floor(p.x()));
			int j = int(floor(p.y()));
			int k = int(floor(p.z()));

			Vec3 c[2][2][2];
			for (int di = 0; di < 2; di++) {
				for (int dj = 0; dj < 2; dj++) {
					for (int dk = 0; dk < 2; dk++) {
						c[di][dj][dk] = _ranvec[_permX[(i+di) &255] ^
												_permY[(j+dj) &255] ^
												_permZ[(k+dk) &255]];
					}
				}
			}

			return perlinInterp(c,u,v,w);
		}

		double turb(const Vec3& p, int depth=7) const {
			double acc = 0.0;
			Vec3 tempP = p;
			double weight = 1.0;
			for (int i = 0; i < depth; i++) {
				acc += weight*noise(tempP);
				weight *= 0.5;
				tempP *= 2.;
			}
			return fabs(acc);
		}

	private:
		static Vec3 *_ranvec;
		static int *_permX;
		static int *_permY;
		static int *_permZ;
};

static Vec3 *perlinGenerate() {
	Vec3 *p = new Vec3[256];
	for (int i = 0; i < 256; ++i) {
		p[i] = normalize(
				Vec3(2.*randDouble() - 1.,
					2.*randDouble() - 1.,
					2.*randDouble() - 1.));
	}
	return p;
}

void permute(int *p, int n) {
	for (int i = n-1; i >= 0; i--) {
		int target = int(randDouble() * (i+1));
		int temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
	return;
}

static int* perlinGeneratePerm() {
	int *p = new int[256];
	for (int i = 0; i < 256; i++) {
		p[i] = i;
	}
	permute(p, 256);
	return p;
}

Vec3 *Perlin::_ranvec = perlinGenerate();
int *Perlin::_permX = perlinGeneratePerm();
int *Perlin::_permY = perlinGeneratePerm();
int *Perlin::_permZ = perlinGeneratePerm();

#endif