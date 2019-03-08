#ifndef IMAGETEXTUREH
#define IMAGETEXTUREH

class ImageTexture: public Texture {
	public:
		ImageTexture() {}
		ImageTexture(unsigned char *image, int w, int h): _pixels(image), _width(w), _height(h) {}

		virtual Vec3 value(double u, double v, const Vec3& p) const;

	private:
		unsigned char *_pixels;
		int _width;
		int _height;
};

Vec3 ImageTexture::value(double u, double v, const Vec3& p) const {
	int i = int((  u)*_width);
	int j = int((1-v)*_height-0.001);

	if (i < 0) i=0;
	if (i > _width-1) i=_width-1;
	if (j < 0) j=0;
	if (j > _height-1) j=_height-1;

	double r = int(_pixels[3*(i + j*_width)  ])/255.0;
	double g = int(_pixels[3*(i + j*_width)+1])/255.0;
	double b = int(_pixels[3*(i + j*_width)+2])/255.0;
	return Vec3(r,g,b);
}

#endif