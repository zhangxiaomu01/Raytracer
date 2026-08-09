#ifndef METAL_MAT_H
#define METAL_MAT_H

#include "Material.h"
#include "Hittable.h"

class MetalMat : public Material {
public:
    MetalMat(const Color& albedo) : mAlbedo(albedo) {};
    ~MetalMat() override = default;

    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        attenuation = mAlbedo;
        Vec3 reflected = reflect(r_in.Direction(), record.normal);
        scattered = Ray(record.point, reflected);
        return true;
    }

private:
    Color mAlbedo;
};

#endif