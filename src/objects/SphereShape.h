#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "Hittable.h"
#include <cmath>
#include "Material.h"

class SphereShape : public Hittable {
public:
    SphereShape(Point3 center, double radius, std::shared_ptr<Material> material) : m_center(center), m_radius(std::fmax(0,radius)), m_material(material) {}

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        Vec3 oc = ray.Origin() - m_center;
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
        Vec3 outNormal = (hit_record.point - m_center) / m_radius;
        hit_record.SetFaceNormal(ray, outNormal);
        return true;
    }

private:
    Point3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
};


#endif