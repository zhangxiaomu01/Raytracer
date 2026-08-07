#include "Color.h"
#include "Ray.h"
#include "Interval.h"
#include "HittableObjects.h"
#include "SphereShape.h"
#include "Camera.h"

int main() {
    // Camera
    Camera camera;
    // Scene
    auto scene = std::make_shared<HittableObjects>();
    // Add spheres to the scene
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,0,-1), 0.5));
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,-100.5, -1), 100));

    // Render
    camera.Render(*scene);
    return 0;
}
