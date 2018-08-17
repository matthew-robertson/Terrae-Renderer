#include <iostream>  
#include <stdlib.h>
#include <vector>
#include "lodepng.h"
using namespace std;  

void main(int argc, char *argv[])  
{  
	if (argc  != 5 ){
		cout << "Invalid arguments. Call this the following way: renderer.exe X Y sceneName outputName" << endl;
		return 1;
	}

	const int width = atoi(argv[1]);
	const int height = atoi(argv[2]);
	const char *sceneFile = argv[3];
	const char *outputFile = argv[4];

	// TODO: Parse the scene file

	// TODO: Path trace some shit
	vector<unsigned char> image;
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			image.push_back(255 * (i%3 == 0));
			image.push_back(255 * (i%3 == 1));
			image.push_back(255 * (i%3 == 2));
			image.push_back(255);
		}
	}

	unsigned error = lodepng::encode(outputFile, image, width, height);
	cout << "Outputted to file: " << outputFile << endl;
} 