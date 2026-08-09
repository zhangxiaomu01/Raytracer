#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"
#include "Color.h"

struct HitRecord;

class Material {
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) = 0;
};

#endif