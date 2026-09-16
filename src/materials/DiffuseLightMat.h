#ifndef DIFFUSELIGHTMAT_H
#define DIFFUSELIGHTMAT_H

#include "Material.h"
#include "Texture.h"
#include "SolidColor.h"

class DiffuseLightMat : public Material
{
public:
    DiffuseLightMat(std::shared_ptr<Texture> texture) : m_texture(texture) {}
    DiffuseLightMat(const Color& color) : m_texture(std::make_shared<SolidColor>(color)) {}
    ~DiffuseLightMat() = default;

    virtual bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered) override {
        return false;
    }

    Color Emit(double u, double v, const Point3& p) const override {
        return m_texture->value(u, v, p);
    }
private:
    std::shared_ptr<Texture> m_texture;
};


#endif

