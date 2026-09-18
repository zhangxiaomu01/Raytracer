#ifndef CONSTANTMEDIA_H
#define CONSTANTMEDIA_H

#include "Hittable.h"
#include "Texture.h"
#include "Material.h"
#include "IsotropicMatrial.h"

class ConstantMedia : public Hittable {
public:
    ConstantMedia(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> texture)
        : m_boundary(boundary), m_inverseDensity(1.0 / density), 
        m_phaseFunction(std::make_shared<IsotropicMaterial>(texture)) {}

    ConstantMedia(std::shared_ptr<Hittable> boundary, double density, const Color& albedo)
        : m_boundary(boundary), m_inverseDensity(1.0 / density), 
        m_phaseFunction(std::make_shared<IsotropicMaterial>(albedo)) {}
    
    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& record) const override {
        HitRecord record1, record2;

        // Ray intersects boundary: entry point
        // Note the limit is Universal, because ray can be inside the boundary, 
        // so record1.t can be negative.
        if (!m_boundary->Hit(ray, Interval::UNIVERSAL, record1)) {
            return false;
        }

        // Ray intersects boundary: exit point
        if (!m_boundary->Hit(ray, Interval(record1.t + 0.0001, G_INFINITY), record2)) {
            return false;
        }

        if (record1.t < rayInterval.m_min) record1.t = rayInterval.m_min;
        if (record2.t > rayInterval.m_max) record2.t = rayInterval.m_max;

        if (record1.t >= record2.t) {
            return false;
        }

        if (record1.t < 0) record1.t = 0;

        auto rayLength = ray.Direction().length();
        auto distanceInsideBoundary = (record2.t - record1.t) * rayLength;
        auto hitDistance = - m_inverseDensity * std::log(RandomDouble());

        if (hitDistance > distanceInsideBoundary) {
            return false;
        }

        record.t = record1.t + hitDistance / rayLength;
        record.point = ray.At(record.t);
        record.normal = Vec3(1.0, 0.0, 0.0); // Arbitrary normal
        record.m_frontFace = true; // Arbitrary
        record.m_material = m_phaseFunction;
        return true;
    }

    AABB BoundingBox() const override {
        return m_boundary->BoundingBox();
    }

private:
    std::shared_ptr<Hittable> m_boundary;
    double m_inverseDensity;
    std::shared_ptr<Material> m_phaseFunction;
};

#endif