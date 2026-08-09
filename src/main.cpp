#include "Color.h"
#include "Ray.h"
#include "Interval.h"
#include "HittableObjects.h"
#include "SphereShape.h"
#include "LambertMat.h"
#include "MetalMat.h"
#include "Camera.h"

int main() {
    // Camera
    Camera camera;
    camera.mFOV = 20;
    camera.mLookFrom = Point3(-2,2,1);
    camera.mLookAt = Point3(0,0,-1);
    camera.mVUp = Point3(0,1,0);
    camera.mDefocusAngle = 10.0;
    camera.mFocalDistance = 3.4;

    // Scene
    auto scene = std::make_shared<HittableObjects>();
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<MetalMat>(Color(0.8, 0.8, 0.8), 0.0);
    auto material_right  = std::make_shared<MetalMat>(Color(0.8, 0.6, 0.2), 0.3);

    scene->AddObject(std::make_shared<SphereShape>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    scene->AddObject(std::make_shared<SphereShape>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    scene->AddObject(std::make_shared<SphereShape>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    scene->AddObject(std::make_shared<SphereShape>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Render
    camera.Render(*scene);
    return 0;
}

