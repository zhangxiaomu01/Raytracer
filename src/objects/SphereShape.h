#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "Hittable.h"
#include <cmath>
#include "Material.h"

class SphereShape : public Hittable {
public:
    // Stationary sphere
    SphereShape(const Point3& staticCenter, double radius, std::shared_ptr<Material> material) : m_center(staticCenter, Vec3(0, 0, 0)), m_radius(std::fmax(0,radius)), m_material(material) {
        auto rvec = Vec3(radius, radius, radius);
        m_boundingBox = AABB(staticCenter - rvec, staticCenter + rvec);
    }

    // Moving sphere
    SphereShape(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> material) : m_center(center1, center2 - center1), m_radius(std::fmax(0,radius)), m_material(material) {
        auto rvec = Vec3(radius, radius, radius);
        AABB box0 = AABB(m_center.At(0) - rvec, m_center.At(0) + rvec);
        AABB box1 = AABB(m_center.At(1) - rvec, m_center.At(1) + rvec);
        m_boundingBox = AABB(box0, box1);
    }

    AABB BoundingBox() const override {
        return m_boundingBox;
    }

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        Vec3 currentCenter = m_center.At(ray.Time());
        Vec3 oc = ray.Origin() - currentCenter;
        auto a = ray.Direction().length_squared();
        auto h = dot(oc, ray.Direction()); // h = b / -2
        auto c = dot(oc, oc) - m_radius * m_radius;
        auto discriminant = h * h - a * c;

        if (discriminant < 0.0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        auto t = (-h - sqrtd) / a;
        if (!rayInterval.Surrounds(t)) {
            t = (-h + sqrtd) / a;
            if (!rayInterval.Surrounds(t)) {
                return false;
            }
        }

        hit_record.point = ray.At(t);
        hit_record.t = t;
        hit_record.m_material = m_material;
        Vec3 outNormal = (hit_record.point - currentCenter) / m_radius;
        hit_record.SetFaceNormal(ray, outNormal);
        GetSphereUVCoordinates(outNormal, hit_record.u, hit_record.v); // Get UV coordinates
        return true;
    }

    static void GetSphereUVCoordinates(const Point3& p, double& u, double& v) {
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + G_PI;

        u = phi / (2.0 * G_PI);
        v = theta / G_PI;
    }

private:
    Ray m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;

    AABB m_boundingBox;
};


#endif