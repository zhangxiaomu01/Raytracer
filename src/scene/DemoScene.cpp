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
#include "DiffuseLightMat.h"
#include "QuadShape.h"
#include "CheckerTexture.h"
#include "ImageTexture.h"
#include "NoiseTexture.h"
#include "SolidColor.h"
#include "Camera.h"

void DemoScene::RenderScene(int sceneIndex) {
    if (sceneIndex == 0) {
        BoundingSphereScene();
    } else if (sceneIndex == 1) {
        CheckeredSpheresScene();
    } else if (sceneIndex == 2) {
        EarthScene();
    } else if (sceneIndex == 3) {
        PerlinSpheres();
    } else if (sceneIndex == 4) {
        QuadShapeScene();
    } else if (sceneIndex == 5) {
        SampleLight();
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

void DemoScene::EarthScene() {
    auto earthTexture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earthMaterial = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<SphereShape>(Point3(0,0,0), 2, earthMaterial);

    Camera cam;

    cam.SetAspectRatio(16.0 / 9.0);
    cam.SetImageWidth(400);
    cam.SetSamplesPerPixel(100);
    cam.SetMaxDepth(50);

    cam.SetFOV(20);
    cam.mLookFrom = Point3(0,0,12);
    cam.mLookAt   = Point3(0,0,0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(HittableObjects(globe));
}

void DemoScene::PerlinSpheres() {
    HittableObjects world;

    auto pertext = std::make_shared<NoiseTexture>(4.0);
    world.AddObject(std::make_shared<SphereShape>(Point3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    world.AddObject(std::make_shared<SphereShape>(Point3(0,2,0), 2, make_shared<Lambertian>(pertext)));

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

    cam.Render(world);
}

void DemoScene::QuadShapeScene() {
    HittableObjects world;

    // Materials
    auto left_red     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.AddObject(std::make_shared<QuadShape>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red));
    world.AddObject(std::make_shared<QuadShape>(Point3(-2,-2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.AddObject(std::make_shared<QuadShape>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.AddObject(std::make_shared<QuadShape>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.AddObject(std::make_shared<QuadShape>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal));

    Camera cam;

    cam.SetAspectRatio(1.0);
    cam.SetImageWidth(400);
    cam.SetSamplesPerPixel(100);
    cam.SetMaxDepth(50);

    cam.SetFOV(80);
    cam.mLookFrom = Point3(0,0,9);
    cam.mLookAt   = Point3(0,0,0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(world);
}

void DemoScene::SampleLight() {
    HittableObjects world;

    auto pertext = std::make_shared<NoiseTexture>(4);
    world.AddObject(std::make_shared<SphereShape>(Point3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    world.AddObject(std::make_shared<SphereShape>(Point3(0,2,0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = std::make_shared<DiffuseLightMat>(Color(4,4,4));
    world.AddObject(std::make_shared<QuadShape>(Point3(3,1,-2), Vec3(2,0,0), Vec3(0,2,0), difflight));

    Camera cam;
    cam.Initialize();

    cam.SetAspectRatio(16.0 / 9.0);
    cam.SetImageWidth(400);
    cam.SetSamplesPerPixel(100);
    cam.SetMaxDepth(50);

    cam.SetFOV(20);

    cam.mBackgroundColor = Color(0,0,0);
    cam.mLookFrom = Point3(26,3,6);
    cam.mLookAt   = Point3(0,2,0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(world);
}

