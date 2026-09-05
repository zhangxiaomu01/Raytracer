#ifndef HITTABLE_OBJECTS_H
#define HITTABLE_OBJECTS_H

#include <vector>
#include <memory>

#include "Hittable.h"

class HittableObjects : public Hittable {
public:
    HittableObjects() = default;
    HittableObjects(std::shared_ptr<Hittable> object) : m_objects({object}) {}
    ~HittableObjects() = default;

    inline void ClearObjects() {
        m_objects.clear();
    }

    inline void AddObject(std::shared_ptr<Hittable> object) {
        m_objects.emplace_back(object);
        m_boundingBox = AABB(m_boundingBox, object->BoundingBox());
    }

    AABB BoundingBox() const override {
        return m_boundingBox;
    }

    bool Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
    AABB m_boundingBox;

};


#endif