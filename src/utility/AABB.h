#ifndef AABB_H
#define AABB_H

#include "Interval.h"
#include "Vec3.h"
#include "Ray.h"

class AABB {
public:
    Interval m_x, m_y, m_z;

    AABB() = default;
    AABB(const Interval& x, const Interval& y, const Interval& z) : m_x(x), m_y(y), m_z(z) {}

    AABB(const Point3& a, const Point3& b) {
        m_x = a[0] <= b[0] ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        m_y = a[1] <= b[1] ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        m_z = a[2] <= b[2] ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }

    AABB(const AABB& a, const AABB& b) {
        m_x = Interval(a.m_x, b.m_x);
        m_y = Interval(a.m_y, b.m_y);
        m_z = Interval(a.m_z, b.m_z);
    }

    const Interval& AxisInterval(int n) const {
        if (n == 1) return m_y;
        if (n == 2) return m_z;
        return m_x;
    }

    bool Hit(const Ray& ray, Interval rayInterval) const {
        const Point3& rayOrigin = ray.Origin();
        const Vec3&   rayDirection  = ray.Direction();

        for (int n = 0; n < 3; n++) {
            const Interval& axisInterval = AxisInterval(n);

            // Parallel to this slab: avoid 0 * inf = NaN when origin is exactly on the boundary.
            if (rayDirection[n] == 0.0) {
                if (rayOrigin[n] < axisInterval.m_min || rayOrigin[n] > axisInterval.m_max) {
                    return false;
                }
                continue;
            }

            const double adinv = 1.0 / rayDirection[n];

            const double t0 = (axisInterval.m_min - rayOrigin[n]) * adinv;
            const double t1 = (axisInterval.m_max - rayOrigin[n]) * adinv;

            if (t0 < t1) {
                if (t0 > rayInterval.m_min) {
                    rayInterval.m_min = t0;
                }
                if (t1 < rayInterval.m_max) {
                    rayInterval.m_max = t1;
                }
            } else {
                if (t1 > rayInterval.m_min) {
                    rayInterval.m_min = t1;
                }
                if (t0 < rayInterval.m_max) {
                    rayInterval.m_max = t0;
                }
            }

            if (rayInterval.m_max <= rayInterval.m_min) {
                return false;
            }
        }
        return true;
    }


};

#endif