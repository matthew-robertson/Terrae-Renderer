#ifndef METALH
#define METALH

class MetalMaterial : public Material {
	public:
		MetalMaterial(Vec3 alb, double f): albedo(alb) { if (f < 1.0) fuzz = f; else fuzz = 1.0;}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			Vec3 ref = reflect(normalize(rIn.direction()), rec.normal);
			scattered = Ray(rec.pos, ref + fuzz*randomInUnitSphere(), rIn.time());
			attenuation = albedo;
			return dot(scattered.direction(), rec.normal) > 0.0;
		}

	private:
		Vec3 albedo;
		double fuzz;
};
#endif