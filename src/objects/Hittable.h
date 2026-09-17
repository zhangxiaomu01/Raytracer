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
    // u, v coordinates for texture mapping.
    double u;
    double v;

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

class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable> object, const Vec3& offset) : m_object(object), m_offset(offset) {
        m_boundingBox = m_object->BoundingBox() + offset;
    }

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        // Offset Ray to the opposite position: basically means convert ray from world coordinate to object coordinate
        Ray offsetRay(ray.Origin() - m_offset, ray.Direction(), ray.Time());

        if (!m_object->Hit(offsetRay, rayInterval, hit_record)) {
            return false;
        }
        // Convert hit point back to world coordinate
        hit_record.point += m_offset;

        return true;
    }

    AABB BoundingBox() const override {
        return m_boundingBox;
    }

private:
    std::shared_ptr<Hittable> m_object;
    Vec3 m_offset;
    AABB m_boundingBox;
};

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle) : m_object(object) {
        auto radians = DgreeToRadians(angle);
        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
        m_boundingBox = m_object->BoundingBox();

        Point3 min( G_INFINITY,  G_INFINITY,  G_INFINITY);
        Point3 max(-G_INFINITY, -G_INFINITY, -G_INFINITY);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i*m_boundingBox.m_x.m_max + (1-i)*m_boundingBox.m_x.m_min;
                    auto y = j*m_boundingBox.m_y.m_max + (1-j)*m_boundingBox.m_y.m_min;
                    auto z = k*m_boundingBox.m_z.m_max + (1-k)*m_boundingBox.m_z.m_min;

                    auto newx =  cosTheta*x + sinTheta*z;
                    auto newz = -sinTheta*x + cosTheta*z;

                    Vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        m_boundingBox = AABB(min, max);
    }

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        // Transform the ray from world space to object space.

        auto origin = Point3(
            (cosTheta * ray.Origin().x()) - (sinTheta * ray.Origin().z()),
            ray.Origin().y(),
            (sinTheta * ray.Origin().x()) + (cosTheta * ray.Origin().z())
        );

        auto direction = Vec3(
            (cosTheta * ray.Direction().x()) - (sinTheta * ray.Direction().z()),
            ray.Direction().y(),
            (sinTheta * ray.Direction().x()) + (cosTheta * ray.Direction().z())
        );

        Ray rotateRay(origin, direction, ray.Time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!m_object->Hit(rotateRay, rayInterval, hit_record))
            return false;

        // Transform the intersection from object space back to world space.

        hit_record.point = Point3(
            (cosTheta * hit_record.point.x()) + (sinTheta * hit_record.point.z()),
            hit_record.point.y(),
            (-sinTheta * hit_record.point.x()) + (cosTheta * hit_record.point.z())
        );

        hit_record.normal = Vec3(
            (cosTheta * hit_record.normal.x()) + (sinTheta * hit_record.normal.z()),
            hit_record.normal.y(),
            (-sinTheta * hit_record.normal.x()) + (cosTheta * hit_record.normal.z())
        );

        return true;
    }

    AABB BoundingBox() const override {
        return m_boundingBox;
    }

private:
    std::shared_ptr<Hittable> m_object;
    double sinTheta;
    double cosTheta;
    AABB m_boundingBox;
};

#endif