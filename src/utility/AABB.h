#ifndef AABB_H
#define AABB_H

#include "Interval.h"
#include "Vec3.h"
#include "Ray.h"

class AABB {
public:
    Interval m_x, m_y, m_z;

    AABB() = default;
    AABB(const Interval& x, const Interval& y, const Interval& z) : m_x(x), m_y(y), m_z(z) {
        PadMinimums();
    }

    AABB(const Point3& a, const Point3& b) {
        m_x = a[0] <= b[0] ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        m_y = a[1] <= b[1] ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        m_z = a[2] <= b[2] ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
        PadMinimums();
    }

    AABB(const AABB& a, const AABB& b) {
        m_x = Interval(a.m_x, b.m_x);
        m_y = Interval(a.m_y, b.m_y);
        m_z = Interval(a.m_z, b.m_z);
        PadMinimums();
    }

    const Interval& AxisInterval(int n) const;

    bool Hit(const Ray& ray, Interval rayInterval) const;

    int LongestAxis() const;

    static const AABB EMPTY_AABB, UNIVERSAL_AABB;

private:
    void PadMinimums();
};

inline AABB operator+(const AABB& bbox, const Vec3& offset) {
    return AABB(bbox.m_x + offset.x(), bbox.m_y + offset.y(), bbox.m_z + offset.z());
}

inline AABB operator+(const Vec3& offset, const AABB& bbox) {
    return bbox + offset;
}

#endif