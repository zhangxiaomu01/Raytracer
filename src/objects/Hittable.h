#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct HitRecord {
    point3 point;
    vec3 normal;
    double t;
};

class Hittable {
public:
    virtual ~Hittable() = default;

    // Check if the ray intersects the object.
    virtual bool Hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& hit_record) const = 0;
};

#endif