#ifndef LAMBERTIAN_MAT_H
#define LAMBERTIAN_MAT_H

#include "Material.h"
#include "Hittable.h"
#include "Texture.h"
#include "SolidColor.h"

class Lambertian : public Material {
public:
    Lambertian(const Color& a) : m_albedoTexture(std::make_shared<SolidColor>(a)) {}
    Lambertian(std::shared_ptr<Texture> abedoTexture) : m_albedoTexture(abedoTexture) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        Vec3 outDir = record.normal + Vec3::RandomUnitVector();
        if (outDir.NearZero()) { // Avoid reflection to the same direction
            outDir = record.normal;
        }
        outDir = unit_vector(outDir);
        scattered = Ray(record.point, outDir, r_in.Time());
        attenuation = m_albedoTexture->value(record.u, record.v, record.point);
        return true;
    }
    
private:
    std::shared_ptr<Texture> m_albedoTexture;
};


#endif