#include <iostream>  
#include <stdlib.h>
#include <vector>

#include "lodepng.h"

#include "Camera.h"
#include "Geometry/Sphere.h"
#include "Geometry/HitableCollection.h"
#include "Math/Ray.h"
#include "Math/Vec3.h"

Vec3 randomInUnitSphere() {
	Vec3 p;
	do {
		double r1 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double r2 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double r3 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

		p = 2.0*Vec3(r1, r2, r3) - Vec3(0.0);
	} while (p.squaredLength() >= 1.0);
	return p;
}

Vec3 getColour(const Ray& r, Hitable *world) {
	hit_record rec;

	if (world->checkIntersection(r, 0.001, 32000.0, rec)){
		Vec3 targetPoint = rec.pos +rec.normal + randomInUnitSphere();
		return 0.5*getColour(Ray(rec.pos, targetPoint - rec.pos), world);
	}
	else {
		Vec3 nRD = normalize(r.direction());
		double t = .5 * (nRD.y() + 1.0);
		return (1.0-t)*Vec3(1.0,1.0,1.0) + t*Vec3(0.5, 0.1, 0.7);
	}
}


void main(int argc, char *argv[])  
{  
	if (argc  != 6 ){
		std::cout << "Invalid arguments. Call this the following way: renderer.exe X Y AARays sceneName outputName" << std::endl;
		return 1;
	}

	//Camera setup
	Camera cam;

	const int width = std::atoi(argv[1]);
	const int height = std::atoi(argv[2]);
	const int nAAR = std::atoi(argv[3]);
	const char *sceneFile = argv[4];
	const char *outputFile = argv[5];

	// TODO: Parse the scene file
	Hitable *list[2];
	list[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5);
	list[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.);
	Hitable *world = new HitableCollection(list, 2);

	// TODO: Path trace some shit
	std::vector<unsigned char> image;
	for (int j = height-1; j >= 0; j--){
		for (int i = 0; i < width; i++){
			Vec3 col = Vec3(0.0);
			for (int x = 0; x < nAAR; x++){
				double r1 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
				double r2 = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

				double u = (double(i) + r1)/double(width);
				double v = (double(j) + r2)/double(height);

				Ray r = cam.getRay(u,v);
				col += getColour(r, world);
			}
			col/= nAAR;
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			image.push_back(255 * col[0]);
			image.push_back(255 * col[1]);
			image.push_back(255 * col[2]);
			image.push_back(255);
		}
	}

	unsigned error = lodepng::encode(outputFile, image, width, height);
	std::cout << "Outputted to file: " << outputFile << std::endl;
} 