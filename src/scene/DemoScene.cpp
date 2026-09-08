#include "DemoScene.h"

#include "Color.h"
#include "Ray.h"
#include "BVH.h"
#include "Interval.h"
#include "HittableObjects.h"
#include "SphereShape.h"
#include "LambertMat.h"
#include "DielectricMat.h"
#include "MetalMat.h"
#include "CheckerTexture.h"
#include "SolidColor.h"
#include "Camera.h"

void DemoScene::RenderScene(int sceneIndex) {
    if (sceneIndex == 0) {
        BoundingSphereScene();
    } else if (sceneIndex == 1) {
        CheckeredSpheresScene();
    }
}

void DemoScene::BoundingSphereScene() {
    HittableObjects scene;

    // Camera
    auto checkerTexture = std::make_shared<CheckerTexture>(
        0.32,
        Color(0.2, 0.3, 0.1),
        Color(0.9, 0.9, 0.9));
    scene.AddObject(
        std::make_shared<SphereShape>(Point3(0,-1000,0), 1000, 
        std::make_shared<Lambertian>(checkerTexture)));

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

    scene = HittableObjects(std::make_shared<BVHNode>(scene));

    Camera camera;
    camera.mFOV     = 20;
    camera.mLookFrom = Point3(13,2,3);
    camera.mLookAt   = Point3(0,0,0);
    camera.mVUp      = Vec3(0,1,0);

    camera.mDefocusAngle = 0.6;
    camera.mFocalDistance    = 10.0;

    // Render
    camera.Render(scene);
}

void DemoScene::CheckeredSpheresScene() {
    HittableObjects scene;

    auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

    scene.AddObject(std::make_shared<SphereShape>(Point3(0,-10, 0), 10, std::make_shared<Lambertian>(checker)));
    scene.AddObject(std::make_shared<SphereShape>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));
    Camera cam;

    cam.SetAspectRatio(16.0 / 9.0);
    cam.SetImageWidth(400);
    cam.SetSamplesPerPixel(100);
    cam.SetMaxDepth(50);

    cam.SetFOV(20);
    cam.mLookFrom = Point3(13,2,3);
    cam.mLookAt   = Point3(0,0,0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(scene);
}

