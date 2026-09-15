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
        // Normalize to [0, 1]
        return Color(1, 1, 1) * 0.5 * (1.0 + perlin.Noise(p * m_scale));
    }

private:
    Perlin perlin;
    double m_scale;
};


#endif
