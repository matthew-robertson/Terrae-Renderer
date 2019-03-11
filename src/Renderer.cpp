#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "lodepng.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Geometry/Box.h"
#include "Geometry/BVHNode.h"
#include "Geometry/ConstantMedium.h"
#include "Geometry/Sphere.h"
#include "Geometry/HitableCollection.h"
#include "Geometry/Rect.h"
#include "Geometry/Transforms.h"
#include "Mats/LambertMaterial.h"
#include "Mats/MetalMaterial.h"
#include "Mats/DielectricMaterial.h"
#include "Mats/DiffuseLightMaterial.h"
#include "Math/Ray.h"
#include "Math/Vec3.h"
#include "Math/Utils.h"
#include "Textures/Texture.h"
#include "Textures/CheckerTexture.h"
#include "Textures/ImageTexture.h"

Vec3 getColour(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	if (world->checkIntersection(r, 0.001, DBL_MAX, rec)){
		Ray scattered;
		Vec3 attenuation;
		Vec3 emitted = rec.hitMat->emitted(rec.u, rec.v, rec.pos);
		if (depth < 50 && rec.hitMat->scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation*getColour(scattered, world, depth+1);
		} else {
			return emitted;
		}
	}
	else {
		return Vec3(0.0);
	}
}

Hitable *cornellBox() {
	Hitable **list = new Hitable*[8];
	int i = 0;
	Material *white = new LambertMaterial(new ConstantTexture(Vec3(0.73)));
	Material *red = new LambertMaterial(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material *green = new LambertMaterial(new ConstantTexture(Vec3(0.12, 0.43, 0.15)));
	Material *light = new DiffuseLightMaterial(new ConstantTexture(Vec3(7)));
	
	list[i++] = new FlipNormals(new YZRect(Vec3(555,277,277), Vec3(0,278,278), green));
	list[i++] = new YZRect(Vec3(0,277,277), Vec3(0,278,278), red);
	list[i++] = new XZRect(Vec3(278,554,280), Vec3(165,0,153), light);
	list[i++] = new FlipNormals(new XZRect(Vec3(277,555,277), Vec3(278,0,278), white));
	list[i++] = new XZRect(Vec3(277,0,277), Vec3(278,0,278), white);
	list[i++] = new FlipNormals(new XYRect(Vec3(277,277,555), Vec3(278,278,0), white));

	Hitable *b1 = new Translate(new RotateY(
					new Box(Vec3(0), Vec3(165,165,165), white),
					-18),
					Vec3(130,0,65));
	Hitable *b2 = new Translate(new RotateY(
					new Box(Vec3(0), Vec3(165,330,165), white),
					15),
					Vec3(265,0,295));

	list[i++] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vec3(1.0)));
	list[i++] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vec3(0.0)));

	return new HitableCollection(list, i);
}

Hitable *simpleLight() {
	Hitable **list = new Hitable*[4];

	Texture *green = new ConstantTexture(Vec3(0.1, 0.3, 0.2));
	Texture *white = new ConstantTexture(Vec3(0.9, 0.9, 0.9));
	Material *light = new DiffuseLightMaterial(new ConstantTexture(Vec3(4.0)));

	list[0] = new Sphere(Vec3(0, -1000, 0), Vec3(0, -1000, 0), 
						1000, 
						0.0, 1.0,
						new LambertMaterial(new CheckerTexture(green, white)));
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("../testScenes/earthmap.jpg", &nx, &ny, &nn, 0);
	Material *earth = new LambertMaterial(new ImageTexture(tex_data, nx, ny));
	list[1] = new Sphere(Vec3(0, 2, 0), Vec3(0, 2, 0), 
						2, 
						0.0, 1.0,
						earth
						/*new DielectricMaterial(1.5)*/);
	list[2] = new Sphere(Vec3(3, 7, 4), Vec3(3, 7, 4), 
						2, 
						0.0, 1.0,
						light);
	list[3] = new XYRect(Vec3(0, 2, -2), Vec3(1, 1, 0), light);

	return new HitableCollection(list, 4);
}

Hitable *randomScene() {
	int n = 500;
	Hitable **list = new Hitable*[n+1];
	Texture *green = new ConstantTexture(Vec3(0.1, 0.3, 0.2));
	Texture *white = new ConstantTexture(Vec3(0.9, 0.9, 0.9));
	list[0] = new Sphere(Vec3(0, -1000, 0), Vec3(0, -1000, 0), 1000, 0.0, 1.0, new LambertMaterial(new CheckerTexture(green, white)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			double chooseMat = randDouble();
			Vec3 center(a+0.9*randDouble(), .0, b+.9*randDouble());
			double randRadius = randDouble()*.2 + .1;
			Material *mat;
			if (chooseMat < 0.8) {
				Texture *c = new ConstantTexture(
					Vec3(randDouble()*randDouble(),
						randDouble()*randDouble(),
						randDouble()*randDouble()));
				mat = new LambertMaterial(c);
			} else if (chooseMat < 0.95) {
				Texture *c = new ConstantTexture(
					Vec3(0.5*(1.0 + randDouble()),
						0.5*(1.0 + randDouble()),
						0.5*(1.0 + randDouble())));
				mat = new MetalMaterial(c, 0.5*randDouble());
			} else {
				mat = new DielectricMaterial(1.5);
			}
			list[i++] = new Sphere(center+Vec3(0.0, randRadius, 0.0), center+Vec3(0.0, randRadius + randDouble() * .25, 0.0), randRadius, 0.0, 1.0, mat);
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), Vec3(0, 1, 0), 1.0, 0.0, 1.0, new DielectricMaterial(1.5));
    list[i++] = new Sphere(Vec3(-4, 1, 0), Vec3(-4, 1, 0), 1.0, 0.0, 1.0, new LambertMaterial(new ConstantTexture(Vec3(0.4, 0.2, 0.1))));
    list[i++] = new Sphere(Vec3(4, 1, 0), Vec3(4, 1, 0), 1.0, 0.0, 0.5, new MetalMaterial(new ConstantTexture(Vec3(0.7, 0.6, 0.5)), 0.0));

   
	//return new HitableCollection(list, i);
	return new BVHNode(list, i, 0, 1);
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
	Vec3 lookFrom = Vec3(278,278,-800);
	Vec3 lookAt = Vec3(278, 278, 0);
	double dist = 10.0;
	double ap = 0.00;
	Camera cam(lookFrom, lookAt, Vec3(0,1,0), 40, double(width)/double(height), ap, dist, 0.0, 1.0);

	// TODO: Parse the scene file
	//Hitable *world = randomScene();
	Hitable *world = simpleLight();
	std::cout << "Starting!" << std::endl;
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
			if (col[0] > 1.) col[0] = 1.0;
			if (col[1] > 1.) col[1] = 1.0;
			if (col[2] > 1.) col[2] = 1.0;

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