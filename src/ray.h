#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction) : m_origin(origin), m_direction(direction) {}

    const Point3& Origin() const { return m_origin; }
    const Vec3& Direction() const { return m_direction; }
    Point3 At(double t) const { return m_origin + t * m_direction; }

private:
    Point3 m_origin;
    Vec3 m_direction;
};

#endif