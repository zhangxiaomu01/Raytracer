#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "HittableObjects.h"
#include "Color.h"

class Camera
{
public:
    double mFOV = 90.0;
    Point3 mLookFrom = Vec3(0,0,0);
    Point3 mLookAt = Vec3(0,0,-1);
    Vec3 mVUp = Vec3(0,1,0);

    Camera();
    ~Camera() = default;

    void Render(const HittableObjects& scene);

private:
    void Initialize();

    Color RayColor(const Ray& r, const HittableObjects& scene, int depth);

    Ray GetRay(int x, int y);

    Ray GetOutRay(const Vec3& point, const Vec3& normal);

    Vec3 SampleSquare() const;

    float mAspectRatio;
    int mImageWidth;
    int mImageHeight;

    // Camera parameters
    double mFocalLength;
    double mViewportHeight;
    double mViewportWidth;
    Vec3 mCameraCenter;
    Vec3 u, v, w;

    // Viewport parameters
    Point3 mViewportUpperLeft;
    Point3 mPixel00Loc;
    Vec3 mPixelDeltaU;
    Vec3 mPixelDeltaV;

    // Samples
    int mSamplesPerPixel;
    double mSampleRates;
    int mMaxDepth;

};

#endif