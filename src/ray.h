#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray {
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction, double time) : m_origin(origin), m_direction(direction), m_time(time) {}

    Ray(const Point3& origin, const Vec3& direction) : m_origin(origin), m_direction(direction), m_time(0.0) {}

    const Point3& Origin() const { return m_origin; }
    const Vec3& Direction() const { return m_direction; }
    Point3 At(double t) const { return m_origin + t * m_direction; }

    // Record the time of the ray
    double Time() const { return m_time; }

private:
    Point3 m_origin;
    Vec3 m_direction;
    double m_time;
};

#endif