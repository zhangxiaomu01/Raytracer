#ifndef SOLID_COLOR_H
#define SOLID_COLOR_H
#include "Texture.h"

class SolidColor : public Texture
{
public:
    SolidColor(const Color& c) : m_albedo(c) {}
    SolidColor(double r, double g, double b) : m_albedo(r, g, b) {}

    Color value(double u, double v, const Point3& p) const override { return m_albedo; }
private:
    Color m_albedo;
};

#endif