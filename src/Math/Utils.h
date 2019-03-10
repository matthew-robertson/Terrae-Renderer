#ifndef UTILSH
#define UTILSH

inline double randDouble() {
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

inline double ffmin(double d1, double d2) {
	return d1 < d2 ? d1 : d2;
}

inline double ffmax(double d1, double d2) {
	return d1 > d2 ? d1 : d2;
}
#endif