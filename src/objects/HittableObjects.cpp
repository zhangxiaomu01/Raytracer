#include "HittableObjects.h"

bool HittableObjects::Hit(const Ray& ray, Interval rayInterval, HitRecord& hit_record) const {
    if (m_objects.empty()) {
        return false;
    }

    HitRecord temp_hit_record;
    double closest_t = rayInterval.m_max;
    bool hit = false;

    // Find the closest hit object
    for (const auto& object : m_objects) {
        if (object->Hit(ray, Interval(rayInterval.m_min, closest_t), temp_hit_record)) {
            closest_t = temp_hit_record.t;
            hit_record = temp_hit_record;
            hit = true;
        }
    }

    return hit;
}
