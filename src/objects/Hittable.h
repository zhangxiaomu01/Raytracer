#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"
#include "Material.h"
#include "AABB.h"

class Material;

struct HitRecord {
    Point3 point;
    Vec3 normal;
    double t;
    bool m_frontFace;
    std::shared_ptr<Material> m_material;

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

    virtual AABB BoundingBox() const = 0;
};

#endif