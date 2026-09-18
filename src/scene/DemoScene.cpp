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
#include "ConstantMedia.h"
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
    } else if (sceneIndex == 6) {
        CornellBox();
    } else if (sceneIndex == 7) {
        CornellSmoke();
    } else if (sceneIndex == 8) {
        ShowCaseScene01(1024, 1500, 40);
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
    world.AddObject(std::make_shared<SphereShape>(Point3(0,7,0), 2, difflight));
    world.AddObject(std::make_shared<QuadShape>(Point3(3,1,-2), Vec3(2,0,0), Vec3(0,2,0), difflight));

    Camera cam;

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

void DemoScene::CornellBox() {
    HittableObjects world;

    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLightMat>(Color(15, 15, 15));

    world.AddObject(std::make_shared<QuadShape>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.AddObject(std::make_shared<QuadShape>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.AddObject(std::make_shared<QuadShape>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

    // Adds two boxes
    std::shared_ptr<Hittable> box1 = CreateBox(Point3(0,0,0), Point3(165,330,165), white);
    box1 = std::make_shared<RotateY>(box1, 15.0);
    box1 = std::make_shared<Translate>(box1, Vec3(265,0,295));
    world.AddObject(box1);

    std::shared_ptr<Hittable> box2 = CreateBox(Point3(0,0,0), Point3(165,165,165), white);
    box2 = std::make_shared<RotateY>(box2, -18.0);
    box2 = std::make_shared<Translate>(box2, Vec3(130,0,65));
    world.AddObject(box2);

    Camera cam;

    cam.SetAspectRatio(1.0);
    cam.SetImageWidth(400);
    cam.SetSamplesPerPixel(200);
    cam.SetMaxDepth(50);

    cam.mBackgroundColor = Color(0,0,0);

    cam.mFOV     = 40;
    cam.mLookFrom = Point3(278, 278, -800);
    cam.mLookAt   = Point3(278, 278, 0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(world);
}

void DemoScene::CornellSmoke() {
    HittableObjects world;

    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLightMat>(Color(7, 7, 7));

    world.AddObject(std::make_shared<QuadShape>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.AddObject(std::make_shared<QuadShape>(Point3(113,554,127), Vec3(330,0,0), Vec3(0,0,305), light));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,555,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.AddObject(std::make_shared<QuadShape>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

    std::shared_ptr<Hittable> box1 = CreateBox(Point3(0,0,0), Point3(165,330,165), white);
    box1 = std::make_shared<RotateY>(box1, 15.0);
    box1 = std::make_shared<Translate>(box1, Vec3(265,0,295));

    std::shared_ptr<Hittable> box2 = CreateBox(Point3(0,0,0), Point3(165,165,165), white);
    box2 = std::make_shared<RotateY>(box2, -18.0);
    box2 = std::make_shared<Translate>(box2, Vec3(130,0,65));

    world.AddObject(std::make_shared<ConstantMedia>(box1, 0.01, Color(0,0,0)));
    world.AddObject(std::make_shared<ConstantMedia>(box2, 0.01, Color(1,1,1)));

    Camera cam;

    cam.SetAspectRatio(1.0);
    cam.SetImageWidth(600);
    cam.SetSamplesPerPixel(200);
    cam.SetMaxDepth(50);

    cam.mBackgroundColor = Color(0,0,0);

    cam.mFOV     = 40;
    cam.mLookFrom = Point3(278, 278, -800);
    cam.mLookAt   = Point3(278, 278, 0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(world);
}

void DemoScene::ShowCaseScene01(int imageWidth, int samplesPerPixel, int maxDDepth) {
    HittableObjects boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = RandomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.AddObject(CreateBox(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    HittableObjects world;

    world.AddObject(std::make_shared<BVHNode>(boxes1));

    auto light = std::make_shared<DiffuseLightMat>(Color(7, 7, 7));
    world.AddObject(std::make_shared<QuadShape>(
        Point3(123,554,147), Vec3(300,0,0), Vec3(0,0,265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    auto sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.AddObject(std::make_shared<SphereShape>(center1, center2, 50, sphere_material));

    world.AddObject(std::make_shared<SphereShape>(
        Point3(260, 150, 45), 50, std::make_shared<DielectricMat>(1.5)));
    world.AddObject(std::make_shared<SphereShape>(
        Point3(0, 150, 145), 50, std::make_shared<MetalMat>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<SphereShape>(
        Point3(360,150,145), 70, std::make_shared<DielectricMat>(1.5));
    world.AddObject(boundary);
    world.AddObject(std::make_shared<ConstantMedia>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<SphereShape>(
        Point3(0,0,0), 5000, std::make_shared<DielectricMat>(1.5));
    world.AddObject(std::make_shared<ConstantMedia>(boundary, .0001, Color(1,1,1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    world.AddObject(std::make_shared<SphereShape>(Point3(400,200,400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.2);
    world.AddObject(std::make_shared<SphereShape>(Point3(220,280,300), 80, std::make_shared<Lambertian>(pertext)));

    HittableObjects boxes2;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.AddObject(std::make_shared<SphereShape>(Point3::RandomVec3(0,165), 10, white));
    }

    world.AddObject(std::make_shared<Translate>(
        std::make_shared<RotateY>(
            std::make_shared<BVHNode>(boxes2), 15),
            Vec3(-100,270,395)
        )
    );

    Camera cam;

    cam.SetAspectRatio(1.0);
    cam.SetImageWidth(imageWidth);
    cam.SetSamplesPerPixel(samplesPerPixel);
    cam.SetMaxDepth(maxDDepth);

    cam.mBackgroundColor = Color(0,0,0);

    cam.mFOV     = 40;
    cam.mLookFrom = Point3(478, 278, -600);
    cam.mLookAt   = Point3(278, 278, 0);
    cam.mVUp      = Vec3(0,1,0);

    cam.mDefocusAngle = 0;

    cam.Render(world);
}

