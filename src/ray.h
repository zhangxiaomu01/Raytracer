#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    Ray() = default;
    Ray(const point3& origin, const vec3& direction) : m_origin(origin), m_direction(direction) {}

    const point3& Origin() const { return m_origin; }
    const vec3& Direction() const { return m_direction; }
    point3 At(double t) const { return m_origin + t * m_direction; }

private:
    point3 m_origin;
    vec3 m_direction;
};

#endif