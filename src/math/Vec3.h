#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>
class Vec3 {
	public:
		Vec3 () {}
  		Vec3(const double x, const double y, const double z){ elements[0] = x; elements[1] = y; elements[2] = z; }
  		inline double x() const { return elements[0]; }
  		inline double y() const { return elements[1]; }
  		inline double z() const { return elements[2]; }
  		inline double r() const { return elements[0]; }
  		inline double g() const { return elements[1]; }
  		inline double b() const { return elements[1]; }

  		inline const Vec3& operator+() const { return *this; }
  		inline Vec3 operator-() const { return Vec3(-elements[0], -elements[1], -elements[2]); }
  		inline double operator[](int i) const { return elements[i]; }
  		inline double& operator[](int i) { return elements[i]; };

  		inline Vec3& operator+=(const Vec3 &v2);
  		inline Vec3& operator-=(const Vec3 &v2);
  		inline Vec3& operator*=(const Vec3 &v2);
  		inline Vec3& operator/=(const Vec3 &v2);
  		inline Vec3& operator*=(const double i);
  		inline Vec3& operator/=(const double i);

  		inline double length() const {
  			return sqrt(elements[0]*elements[0] + elements[1]*elements[1] + elements[2]*elements[2]);
  		}

  		inline double squaredLength() const {
  			return elements[0]*elements[0] + elements[1]*elements[1] + elements[2]*elements[2];
  		}

  		inline void normalize();

	  	double elements[3];
};

inline std::istream& operator>>(std::istream &is, Vec3 &v) {
	is >> v.elements[0] >> v.elements[1] >> v.elements[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &v) {
	os << "X: " << v.elements[0] << " Y: " << v.elements[1] << " Z: " << v.elements[2];
	return os;
}

inline void Vec3::normalize() {
	double iL = 1.0 / sqrt(elements[0]*elements[0] + elements[1]*elements[1] + elements[2]*elements[2]);
	elements[0] *= iL;
	elements[1] *= iL;
	elements[2] *= iL;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1[0] + v2[0],
				v1[1] + v2[1],
				v1[2] + v2[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1[0] - v2[0],
				v1[1] - v2[1],
				v1[2] - v2[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1[0] * v2[0],
				v1[1] * v2[1],
				v1[2] * v2[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1[0] / v2[0],
				v1[1] / v2[1],
				v1[2] / v2[2]);
}

inline Vec3 operator*(const double t, const Vec3 &v) {
	return Vec3(t*v[0], t*v[1], t*v[2]);
}

inline Vec3 operator/(const Vec3 &v, const double t) {
	return Vec3(v[0]/t, v[1]/t, v[2]/t);
}

inline double dot(const Vec3 &v1, const Vec3 &v2) {
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3&v2) {
	return Vec3(
			(v1[1]*v2[2] - v1[2]*v2[1]),
			-(v1[0]*v2[2] - v1[2]*v2[0]),
			(v1[0]*v2[1] - v1[1]*v2[0]));
}

inline Vec3& Vec3::operator+=(const Vec3 &v) {
	elements[0] += v[0];
	elements[1] += v[1];
	elements[2] += v[2];

	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v) {
	elements[0] -= v[0];
	elements[1] -= v[1];
	elements[2] -= v[2];

	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v) {
	elements[0] *= v[0];
	elements[1] *= v[1];
	elements[2] *= v[2];

	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v) {
	elements[0] /= v[0];
	elements[1] /= v[1];
	elements[2] /= v[2];

	return *this;
}

inline Vec3& Vec3::operator*=(const double t) {
	elements[0] *= t;
	elements[1] *= t;
	elements[2] *= t;

	return *this;
}

inline Vec3& Vec3::operator/=(const double t) {
	double i = 1.0/t;
	elements[0] *= i;
	elements[1] *= i;
	elements[2] *= i;

	return *this;
}

inline Vec3 normalize(Vec3 v) {
	return v / v.length();
}
#endif