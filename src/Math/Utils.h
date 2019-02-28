#ifndef UTILSH
#define UTILSH

double randDouble() {
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

#endif