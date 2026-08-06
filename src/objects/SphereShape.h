#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "Hittable.h"
#include "vec3.h"

class SphereShape : public Hittable {
public:
    SphereShape(point3 center, double radius) : m_center(center), m_radius(std::fmax(0,radius)) {}

    bool Hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& hit_record) const override {
        vec3 oc = ray.Origin() - m_center;
        auto a = ray.Direction().length_squared();
        auto h = dot(oc, ray.Direction()); // h = b / -2
        auto c = dot(oc, oc) - m_radius * m_radius;
        auto discriminant = h * h - a * c;

        if (discriminant < 0.0) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        auto t = (-h - sqrtd) / a;
        if (t >= ray_tmax || t <= ray_tmin) {
            t = (-h + sqrtd) / a;
            if (t >= ray_tmax || t <= ray_tmin) {
                return false;
            }
        }

        hit_record.point = ray.At(t);
        hit_record.normal = (hit_record.point - m_center) / m_radius;
        hit_record.t = t;
        return true;
    }

private:
    point3 m_center;
    double m_radius;
};


#endif