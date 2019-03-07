#ifndef DIFFUSELIGHTMATERIALH
#define DIFFUSELIGHTMATERIALH

class DiffuseLightMaterial : public Material {
	public:
		DiffuseLightMaterial(Texture *t) : _emitTex(t) {}
		virtual bool scatter(const Ray& rIn, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
			return false;
		}
		virtual Vec3 emitted(double u, double v, const Vec3& p) const {
			return _emitTex->value(u,v,p);
		}

	private:
		Texture *_emitTex;
};

#endif