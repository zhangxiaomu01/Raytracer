#ifndef LAMBERTIAN_MAT_H
#define LAMBERTIAN_MAT_H

#include "Material.h"
#include "Hittable.h"

class Lambertian : public Material {
public:
    Lambertian(const Color& a) : m_albedo(a) {}
    virtual bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        attenuation = m_albedo;
        Vec3 outDir = record.normal + Vec3::RandomUnitVector();
        if (outDir.NearZero()) { // Avoid reflection to the same direction
            outDir = record.normal;
        }
        outDir = unit_vector(outDir);
        scattered = Ray(record.point, outDir);
        return true;
    }
    
private:
    Color m_albedo;
};


#endif