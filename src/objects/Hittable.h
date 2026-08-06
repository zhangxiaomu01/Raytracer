#ifndef HITTABLE_H
#define HITTABLE_H

#include "CommonUtil.h"

struct HitRecord {
    Point3 point;
    Vec3 normal;
    double t;
    bool m_frontFace;

    // Normal should be unit vector.
    void SetFaceNormal(const Ray& ray, const Vec3& outNormal) {

        m_frontFace = dot(ray.Direction(), outNormal) < 0.0;
        normal = m_frontFace ? outNormal : -outNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    // Check if the ray intersects the object.
    virtual bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const = 0;
};

#endif