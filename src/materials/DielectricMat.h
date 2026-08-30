#ifndef DIELECTRIC_MAT_H
#define DIELECTRIC_MAT_H

#include "Material.h"
#include "Hittable.h"

class DielectricMat : public Material {
public:
    DielectricMat(double refractionIndex) : m_refractionIndex(refractionIndex) {}
    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        Vec3 unitDir = unit_vector(r_in.Direction());
        double ri = record.m_frontFace ? (1.0 / m_refractionIndex) : m_refractionIndex;

        double cosTheta = std::fmin(1.0, dot(-unitDir, record.normal));
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = (ri * sinTheta > 1.0);

        // Must reflect: When a ray enters a medium of lower index of refraction at a sufficiently glancing angle, 
        // it can refract with an angle greater than 90°
        if (cannotRefract || reflectance(cosTheta, m_refractionIndex) > RandomDouble()) { 
            Vec3 reflectedDirection = reflect(unitDir, record.normal);
            scattered = Ray(record.point, reflectedDirection);
        } else {
            Vec3 refractionDirection = refract(unitDir, record.normal, ri);
            scattered = Ray(record.point, refractionDirection, r_in.Time());
        }
        attenuation = Color(1.0, 1.0, 1.0);
        return true;
    }

private:
    double m_refractionIndex;

    // Schlick approximation
    double reflectance(double cosine, double refractionIndex) {
        double r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
    }
};

#endif