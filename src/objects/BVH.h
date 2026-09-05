#ifndef BVH_H
#define BVH_H

#include "Hittable.h"
#include "HittableObjects.h"

class BVHNode : public Hittable {
public:
    BVHNode(HittableObjects objects) : BVHNode(objects.GetObjects(), 0, objects.GetObjects().size()) {}
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        size_t objectSpan = end - start;

        if (objectSpan == 1) {
            m_left = m_right = objects[start];
        } else if (objectSpan == 2) {
            m_left = objects[start];
            m_right = objects[start + 1];
        } else {
            int axis = RandomInt(0,2);
            auto comparator = (axis == 0) ? boxXCompare
                            : (axis == 1) ? boxYCompare
                                        : boxZCompare;

            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            size_t mid = start + objectSpan / 2;
            m_left = std::make_shared<BVHNode>(objects, start, mid);
            m_right = std::make_shared<BVHNode>(objects, mid, end);
            
        }

        m_boundingBox = AABB(m_left->BoundingBox(), m_right->BoundingBox());

    }
    ~BVHNode() = default;

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override {
        if (!m_boundingBox.Hit(ray, rayInterval)) { // box 未求交，跳过
            return false;
        }

        bool hitLeft = m_left->Hit(ray, rayInterval, hit_record);
        // 不能因为hitLeft为真而提前返回，因为我们需要求到真正的hitRecord
        bool hitRight = m_right->Hit(
            ray, Interval(rayInterval.m_min, hitLeft ? hit_record.t : rayInterval.m_max), hit_record);

        return hitLeft || hitRight;
    }

    AABB BoundingBox() const override {
        return m_boundingBox;
    }

private:
    AABB m_boundingBox;
    std::shared_ptr<Hittable> m_left;
    std::shared_ptr<Hittable> m_right;

    static bool BoxCompare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axisIndex) {
        auto boxAAxis = a->BoundingBox().AxisInterval(axisIndex);
        auto boxBAxis = b->BoundingBox().AxisInterval(axisIndex);
        return boxAAxis.m_min < boxBAxis.m_min;
    }

    static bool boxXCompare (const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
        return BoxCompare(a, b, 0);
    }

    static bool boxYCompare (const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
        return BoxCompare(a, b, 1);
    }
    static bool boxZCompare (const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
        return BoxCompare(a, b, 2);
    }
};


#endif