#ifndef METAL_MAT_H
#define METAL_MAT_H

#include "Material.h"
#include "Hittable.h"

class MetalMat : public Material {
public:
    // fuzz shoul be in [0, 1]
    MetalMat(const Color& albedo, double fuzz = 0.0) : mAlbedo(albedo), mFuzzFactor(fuzz) {};
    ~MetalMat() override = default;

    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        Vec3 reflected = reflect(r_in.Direction(), record.normal) + mFuzzFactor * Vec3::RandomUnitVector();
        if (dot(reflected, record.normal) < 0.0) {
            return false;
        }
        attenuation = mAlbedo;
        scattered = Ray(record.point, reflected);
        return true;
    }

private:
    Color mAlbedo;
    double mFuzzFactor;
};

#endif