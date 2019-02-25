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
	Hitable *list[5];
	list[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5, new LambertMaterial(Vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100., new LambertMaterial(Vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(Vec3(1.0, 0.0, -1.0), 0.5, new MetalMaterial(Vec3(0.8, 0.6, 0.3), 0.3));
	list[3] = new Sphere(Vec3(-1.0, 0.0, -1.0), 0.5, new DielectricMaterial(1.5));
	list[4] = new Sphere(Vec3(-1.0, 0.0, -1.0), -0.45, new DielectricMaterial(1.5));
	Hitable *world = new HitableCollection(list, 5);

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
				col += getColour(r, world, 0);
			}
			col/= nAAR;
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			image.push_back(255 * col[0]);
			image.push_back(255 * col[1]);
			image.push_back(255 * col[2]);
			image.push_back(255);
		}
		if (j % 50 == 0){
			std::cout << "Done: " << j << std::endl;
		}
	}

	unsigned error = lodepng::encode(outputFile, image, width, height);
	std::cout << "Outputted to file: " << outputFile << std::endl;
} 