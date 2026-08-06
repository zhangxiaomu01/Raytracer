#include "HittableObjects.h"

bool HittableObjects::Hit(const Ray& ray, double ray_tmin, double ray_tmax, HitRecord& hit_record) const {
    if (m_objects.empty()) {
        return false;
    }

    HitRecord temp_hit_record;
    double closest_t = ray_tmax;
    bool hit = false;

    // Find the closest hit object
    for (const auto& object : m_objects) {
        if (object->Hit(ray, ray_tmin, closest_t, temp_hit_record)) {
            closest_t = temp_hit_record.t;
            hit_record = temp_hit_record;
            hit = true;
        }
    }

    return hit;
}
