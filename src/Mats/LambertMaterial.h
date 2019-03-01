#ifndef LAMBERTH
#define LAMBERTH

class LambertMaterial : public Material {
	public:
		LambertMaterial(Vec3 alb): albedo(alb) {}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			Vec3 targetPoint = rec.pos +rec.normal + randomInUnitSphere();
			scattered = Ray(rec.pos, targetPoint - rec.pos, rIn.time());
			attenuation = albedo;
			return true;
		}

	private:
		Vec3 albedo;
};
#endif