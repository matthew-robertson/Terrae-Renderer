#include <iostream>
#include <stdlib.h>
#include <vector>

#include "lodepng.h"

#include "Camera.h"
#include "Geometry/Sphere.h"
#include "Geometry/HitableCollection.h"
#include "Mats/LambertMaterial.h"
#include "Mats/MetalMaterial.h"
#include "Mats/DielectricMaterial.h"
#include "Math/Ray.h"
#include "Math/Vec3.h"

double randDouble() {
	return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}

Vec3 getColour(const Ray& r, Hitable *world, int depth) {
	hit_record rec;

	if (world->checkIntersection(r, 0.001, 32000.0, rec)){
		Ray scattered;
		Vec3 attenuation;

		if (depth < 50 && rec.hitMat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * getColour(scattered, world, depth+1);
		} else {
			return Vec3(0.0);
		}
	}
	else {
		Vec3 nRD = normalize(r.direction());
		double t = .5 * (nRD.y() + 1.0);
		return (1.0-t)*Vec3(1.0,1.0,1.0) + t*Vec3(0.2, 0.5, 0.8);
	}
}

Hitable *randomScene() {
	int n = 500;
	Hitable **list = new Hitable*[n+1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new LambertMaterial(Vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double chooseMat = randDouble();
			Vec3 center(a+0.9*randDouble(), .0, b+.9*randDouble());
			if ((center - Vec3(4,0,0)).length() > 0.9) {
				double randRadius = randDouble()*.3 + .1;
				Material *mat;
				if (chooseMat < 0.8) {					
					mat = new LambertMaterial(Vec3(randDouble()*randDouble(),randDouble()*randDouble(),randDouble()*randDouble()));
				} else if (chooseMat < 0.95) {
					mat = new MetalMaterial(Vec3(0.5*(1 + randDouble()), 0.5*(1 + randDouble()), 0.5*(1 + randDouble())), 0.5*(1 + randDouble()));
				} else {
					mat = new DielectricMaterial(1.5);
				}
				list[i++] = new Sphere(center+Vec3(0.0, randRadius, 0.0), randRadius, mat);
			}

		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new DielectricMaterial(1.5));
    list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new LambertMaterial(Vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new MetalMaterial(Vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableCollection(list, i);
}


void main(int argc, char *argv[])  
{  
	if (argc  != 6 ){
		std::cout << "Invalid arguments. Call this the following way: renderer.exe X Y AARays sceneName outputName" << std::endl;
		return 1;
	}


	const int width = std::atoi(argv[1]);
	const int height = std::atoi(argv[2]);
	const int nAAR = std::atoi(argv[3]);
	const char *sceneFile = argv[4];
	const char *outputFile = argv[5];

	//Camera setup
	Vec3 lookFrom = Vec3(13,2,3);
	Vec3 lookAt = Vec3(0);
	double dist = 10.0;
	double ap = 0.1;
	Camera cam(lookFrom, lookAt, Vec3(0,1,0), 20, double(width)/double(height), ap, dist);

	// TODO: Parse the scene file
	Hitable *world = randomScene();

	// TODO: Path trace some shit
	std::vector<unsigned char> image;
	for (int j = height-1; j >= 0; j--){
		for (int i = 0; i < width; i++){
			Vec3 col = Vec3(0.0);
			for (int x = 0; x < nAAR; x++){
				double u = (double(i) + randDouble())/double(width);
				double v = (double(j) + randDouble())/double(height);

				Ray r = cam.getRay(u,v);
				col += getColour(r, world, 0);
			}
			col/= nAAR;
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			image.push_back(255 * col[0]);
			image.push_back(255 * col[1]);
			image.push_back(255 * col[2]);
			image.push_back(255);
		}
		if (j % 20 == 0){
			std::cout << "Done: " << j << std::endl;
		}
	}

	unsigned error = lodepng::encode(outputFile, image, width, height);
	std::cout << "Outputted to file: " << outputFile << std::endl;
} 