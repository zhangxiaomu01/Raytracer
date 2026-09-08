#ifndef TEXTURE_H
#define TEXTURE_H

#include "Color.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point3& p) const = 0;
};

#endif