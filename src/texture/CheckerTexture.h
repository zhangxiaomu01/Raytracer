#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H
#include "Texture.h"
#include "Color.h"
#include "SolidColor.h"

class CheckerTexture : public Texture
{
public:
    CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) : m_invScale(1.0 / scale), m_even(even), m_odd(odd) {}

    CheckerTexture(double scale, const Color& even, const Color& odd) : m_invScale(1.0 / scale), m_even(std::make_shared<SolidColor>(even)), m_odd(std::make_shared<SolidColor>(odd)) {}

    Color value(double u, double v, const Point3& p) const override {
        int xInt = int(std::floor(p.x() * m_invScale));
        int yInt = int(std::floor(p.y() * m_invScale));
        int zInt = int(std::floor(p.z() * m_invScale));

        bool bEven = (xInt + yInt + zInt) % 2 == 0;
        return bEven ? m_even->value(u, v, p) : m_odd->value(u, v, p);
    }
private:
    double m_invScale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
};


#endif