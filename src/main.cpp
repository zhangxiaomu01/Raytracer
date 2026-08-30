#include "Color.h"
#include "Ray.h"
#include "Interval.h"
#include "HittableObjects.h"
#include "SphereShape.h"
#include "LambertMat.h"
#include "DielectricMat.h"
#include "MetalMat.h"
#include "Camera.h"

int main() {
    HittableObjects scene;

    // Camera
    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.AddObject(std::make_shared<SphereShape>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomDouble();
            Point3 center(a + 0.9*RandomDouble(), 0.2, b + 0.9*RandomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::RandomVec3() * Color::RandomVec3();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    Vec3 center2 = center + Vec3(0.0, RandomDouble(0.0 , 0.5), 0.0);
                    scene.AddObject(std::make_shared<SphereShape>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::RandomVec3(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphere_material = std::make_shared<MetalMat>(albedo, fuzz);
                    scene.AddObject(std::make_shared<SphereShape>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<DielectricMat>(1.5);
                    scene.AddObject(std::make_shared<SphereShape>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<DielectricMat>(1.5);
    scene.AddObject(std::make_shared<SphereShape>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.AddObject(std::make_shared<SphereShape>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<MetalMat>(Color(0.7, 0.6, 0.5), 0.0);
    scene.AddObject(std::make_shared<SphereShape>(Point3(4, 1, 0), 1.0, material3));

    Camera camera;
    camera.mFOV     = 20;
    camera.mLookFrom = Point3(13,2,3);
    camera.mLookAt   = Point3(0,0,0);
    camera.mVUp      = Vec3(0,1,0);

    camera.mDefocusAngle = 0.6;
    camera.mFocalDistance    = 10.0;

    // Render
    camera.Render(scene);
    return 0;
}

