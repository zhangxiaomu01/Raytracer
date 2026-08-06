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
    }

    bool Hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& hit_record) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;

};


#endif