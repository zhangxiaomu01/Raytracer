#include "Camera.h"

#include "CommonUtil.h"

Camera::Camera() {
    // Initialize the camera
    Initialize();
}

void Camera::Initialize() {
    // Initialize the camera parameters
    mAspectRatio = 16 / 9.0;
    mImageWidth = 400;
    mImageHeight = static_cast<int>(mImageWidth / mAspectRatio);
    mImageHeight = mImageHeight < 1 ? 1 : mImageHeight;
    mCameraCenter = mLookFrom;
    mLookAt = Point3(0,0,-1);
    mVUp = Vec3(0,1,0);
    w = unit_vector(mLookFrom - mLookAt); // w points to the negative direction of camera view
    u = unit_vector(cross(mVUp, w));
    v = cross(w, u);

       // Initialize the viewport parameters
    // mFocalLength = (mLookAt - mLookFrom).length();
    auto theta = DgreeToRadians(mFOV);
    auto h = std::tan(theta / 2.0);
    mViewportHeight = 2 * h * mFocalDistance;
    mViewportWidth = mViewportHeight * ((double) mImageWidth / mImageHeight);

    // Initialize the viewport
    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewportU = mViewportWidth * u;
    auto viewportV = mViewportHeight * -v;
    mViewportUpperLeft = mCameraCenter - viewportU / 2 - viewportV / 2 - w * mFocalDistance;
    
    mPixelDeltaU = viewportU / mImageWidth;
    mPixelDeltaV = viewportV / mImageHeight;
    mPixel00Loc = mViewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);

    // Initialize the samples
    mSamplesPerPixel = 10;
    mSampleRates = 1.0 / mSamplesPerPixel;
    mMaxDepth = 20;

    // Defocus radius
    auto defocusRadius = mFocalDistance * std::tan(DgreeToRadians(mDefocusAngle / 2.0));
    mDefocusDiskU = defocusRadius * u;
    mDefocusDiskV = defocusRadius * v;
}

Color Camera::RayColor(const Ray& r, const HittableObjects& scene, int depth) {
    if (depth <= 0) { // Max depth
        return Color(0,0,0);
    }

    // Get the color of the pixel
    HitRecord record;
    // 0.00001 is the minimum distance to avoid shadow acne
    if (scene.Hit(r, Interval(0.00001, G_INFINITY), record)) {
        Color attentunation = Color(0,0,0);
        Ray outRay;

        if (record.m_material->Scatter(r, record, attentunation, outRay)) {
            return attentunation * RayColor(outRay, scene, depth - 1);
        }

        return attentunation;
    }

    auto unit_direction = unit_vector(r.Direction());
    auto a = 0.5 * (1.0 + unit_direction.y());

    return Color(0.5,0.7,1) * a + Color(1,1,1) * (1 - a);
}

Ray Camera::GetRay(int x, int y) {

    Vec3 offset = SampleSquare(); // Sample in pixel square uniformly
    // Get the pixel center
    auto pixelCenter = mPixel00Loc 
        + mPixelDeltaU * (x + offset.x()) + mPixelDeltaV * (y + offset.y());

    auto rayOrigin = mDefocusAngle < G_EPSILON ? mCameraCenter : SampleDefocusDisk();
    auto rayDirection = pixelCenter - rayOrigin;

    // Ray time
    auto rayTime = RandomDouble();
    return Ray(rayOrigin, rayDirection, rayTime);
}

Ray Camera::GetOutRay(const Vec3& point, const Vec3& normal) {
    // 1. Sample uniformly in the hemisphere above the normal vector
    Vec3 randomUnitVector = Vec3::RandomUnitVector();
    if (dot(randomUnitVector, normal) > 0.0) {
        return Ray(point, randomUnitVector);
    } else {
        return Ray(point, -randomUnitVector);
    }
}

Vec3 Camera::SampleSquare() const {
    // Sample a point in the square
    return Vec3(RandomDouble() - 0.5, RandomDouble() * 0.5 - 0.5, 0);
}

Vec3 Camera::SampleDefocusDisk() const {
    // Sample a point in the disk
    auto p = Vec3::RandomInUnitDisk();
    return mCameraCenter + p.x() * mDefocusDiskU  + p.y() * mDefocusDiskV;
}

void Camera::Render(const HittableObjects& scene) {
    Initialize();

    std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
    if (!out) {
        std::cerr << "Error: cannot open image.ppm for writing.\n";
        return;
    }

    // Render the scene
    out << "P3\n" << mImageWidth << ' ' << mImageHeight << "\n255\n";

     for (int j = 0; j < mImageHeight; j++) {
        std::cout << "\rScanlines remaining: " << (mImageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < mImageWidth; i++) {
            // auto pixel_center = mPixel00Loc + mPixelDeltaU * i + mPixelDeltaV * j;
            // auto ray_direction = pixel_center - mCameraCenter;
            // Ray r(mCameraCenter, ray_direction);
            Color pixel_color = Color(0,0,0);
            for (int s = 0; s < mSamplesPerPixel; s++) {
                auto r = GetRay(i, j);
                pixel_color += RayColor(r, scene, mMaxDepth);
            }

            pixel_color *= mSampleRates;
            
            WriteColor(out, pixel_color);
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << mImageWidth << "x" << mImageHeight << ").\n";

}
