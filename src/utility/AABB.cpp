#include "AABB.h"

const AABB AABB::EMPTY_AABB = AABB(Interval::EMPTY, Interval::EMPTY, Interval::EMPTY);
const AABB AABB::UNIVERSAL_AABB = 
    AABB(Interval::UNIVERSAL, Interval::UNIVERSAL, Interval::UNIVERSAL);

const Interval& AABB::AxisInterval(int n) const {
    if (n == 1) return m_y;
    if (n == 2) return m_z;
    return m_x;
}


bool AABB::Hit(const Ray& ray, Interval rayInterval) const {
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


int AABB::LongestAxis() const {
    if (m_x.Size() > m_y.Size() && m_x.Size() > m_z.Size()) {
        return 0;
    }
    if (m_y.Size() > m_z.Size()) {
        return 1;
    }
    return 2;
}