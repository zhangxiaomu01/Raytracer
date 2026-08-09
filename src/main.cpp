#include "Color.h"
#include "Ray.h"
#include "Interval.h"
#include "HittableObjects.h"
#include "SphereShape.h"
#include "LambertMat.h"
#include "Camera.h"

int main() {
    // Camera
    Camera camera;
    // Material
    auto material = std::make_shared<Lambertian>(Color(0.8,0.3,0.3));

    // Scene
    auto scene = std::make_shared<HittableObjects>();
    // Add spheres to the scene
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,0,-1), 0.5, material));
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,-100.5, -1), 100, material));

    // Render
    camera.Render(*scene);
    return 0;
}

