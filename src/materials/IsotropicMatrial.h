#ifndef ISOTROPICMATERIAL_H
#define ISOTROPICMATERIAL_H

#include "Material.h"
#include "Texture.h"
#include "SolidColor.h"
#include "Hittable.h"

class IsotropicMaterial : public Material {
public:
    IsotropicMaterial(std::shared_ptr<Texture> texture) : m_texture(texture) {}
    IsotropicMaterial(const Color& albedo) : m_texture(std::make_shared<SolidColor>(albedo)) {}

    bool Scatter(
        const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override
    {
        scattered = Ray(record.point, Vec3::RandomUnitVector(), r_in.Time());
        attenuation = m_texture->value(record.u, record.v, record.point);
        return true;
    }

private:
    std::shared_ptr<Texture> m_texture;
};


#endif
