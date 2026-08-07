#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "HittableObjects.h"
#include "Color.h"

class Camera
{
public:
    Camera();
    ~Camera() = default;

    void Render(const HittableObjects& scene);

private:
    void Initialize();

    Color RayColor(const Ray& r, const HittableObjects& scene);

    float mAspectRatio;
    int mImageWidth;
    int mImageHeight;

    // Camera parameters
    double mFocalLength;
    double mViewportHeight;
    double mViewportWidth;
    Vec3 mCameraCenter;

    // Viewport parameters
    Point3 mViewportUpperLeft;
    Point3 mPixel00Loc;
    Vec3 mPixelDeltaU;
    Vec3 mPixelDeltaV;
};

#endif