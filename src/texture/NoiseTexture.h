#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#include "Texture.h"
#include "Perlin.h"


class NoiseTexture : public Texture
{
public:
    NoiseTexture() = default;
    NoiseTexture(double scale) : m_scale(scale) {}
    ~NoiseTexture() = default;
    Color value(double u, double v, const Point3& p) const override {
        return Color(0.5, 0.5, 0.5) * (1.0 + std::sin(m_scale * p.z() + 10 * perlin.Turbulence(p, 7)));
    }

private:
    Perlin perlin;
    double m_scale;
};


#endif
