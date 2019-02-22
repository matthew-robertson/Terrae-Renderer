#include <iostream>  
#include <stdlib.h>
#include <vector>

#include "lodepng.h"
#include "Math/Ray.h"
#include "Math/Vec3.h"

Vec3 getColour(const Ray& r) {
	Vec3 nRD = normalize(r.direction());
	double t = .5 * (nRD.y() + 1.0);
	return (1.0-t)*Vec3(1.0,1.0,1.0) + t*Vec3(0.5, 0.1, 0.7);
}

struct Camera {
	Vec3 origin;
	Vec3 up;
	Vec3 right;
};

void main(int argc, char *argv[])  
{  
	if (argc  != 5 ){
		std::cout << "Invalid arguments. Call this the following way: renderer.exe X Y sceneName outputName" << std::endl;
		return 1;
	}

	//Camera setup
	struct Camera cam;
	cam.origin = Vec3(0.0,0.0,0.0);
	cam.right = Vec3(4.0,0.0,0.0);
	cam.up = Vec3(0.0,2.0,0.0);

	Vec3 upperLeft(-2.0, -1.0, -1.0);

	const int width = std::atoi(argv[1]);
	const int height = std::atoi(argv[2]);
	const char *sceneFile = argv[3];
	const char *outputFile = argv[4];

	// TODO: Parse the scene file

	// TODO: Path trace some shit
	std::vector<unsigned char> image;
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			double u = double(i)/double(width);
			double v = double(j)/double(height);
			Ray r(cam.origin, upperLeft + u*cam.right + v*cam.up);

			Vec3 col = getColour(r);
			image.push_back(255 * col[0]);
			image.push_back(255 * col[1]);
			image.push_back(255 * col[2]);
			image.push_back(255);
		}
	}

	unsigned error = lodepng::encode(outputFile, image, width, height);
	std::cout << "Outputted to file: " << outputFile << std::endl;
} 