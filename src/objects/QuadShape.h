#ifndef QUADSHAPE_H
#define QUADSHAPE_H

#include "Hittable.h"

class QuadShape : public Hittable {
public:
    QuadShape(const Point3& Q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material) 
    : m_Q(Q), m_u(u), m_v(v), m_material(material) {
        auto n = cross(m_u, m_v);
        m_normal = unit_vector(n);
        m_distance = dot(m_Q, m_normal);
        m_w = n / dot(n, n);

        SetBoundingBox();
    }
    ~QuadShape() = default;

    virtual void SetBoundingBox() {
        auto bboxDiagonal1 = AABB(m_Q, m_Q + m_u + m_v);
        auto bboxDiagonal2 = AABB(m_Q + m_u, m_Q + m_v);
        m_bounding_box = AABB(bboxDiagonal1, bboxDiagonal2);
    }

    AABB BoundingBox() const override {
        return m_bounding_box;
    }

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        auto denom = dot(m_normal, ray.Direction());

        // Ray is parallel to the plane
        if (std::fabs(denom) < 1e-8) {
            return false;
        }

        auto t = (m_distance - dot(m_normal, ray.Origin())) / denom;
        if (!rayInterval.Contains(t)) {
            return false;
        }

        auto intersection = ray.At(t);
        // Determine whether the point lies in the quad
        Vec3 planHitVector = intersection - m_Q;
        auto alpha = dot(m_w, cross(planHitVector, m_v));
        auto beta = dot(m_w, cross(m_u,planHitVector));

        // Check if the point lies in the quad
        if (!IsInterior(alpha, beta, hit_record)) {
            return false;
        }

        hit_record.t = t;
        hit_record.point = intersection;
        hit_record.normal = m_normal;
        hit_record.m_material = m_material;
        hit_record.SetFaceNormal(ray, m_normal);
        return true;
    }

    virtual bool IsInterior(double alpha, double beta, HitRecord& record) const {
        Interval unitInterval(0.0, 1.0);

        if (!unitInterval.Contains(alpha) || !unitInterval.Contains(beta)) {
            return false;
        }

        record.u = alpha;
        record.v = beta;
        return true;
    }

private:
    Point3 m_Q;
    Vec3 m_u, m_v;
    Vec3 m_w;
    std::shared_ptr<Material> m_material;
    AABB m_bounding_box;
    Vec3 m_normal;
    double m_distance;
};

#endif