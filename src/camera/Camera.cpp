#include "Camera.h"

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

    // Initialize the viewport parameters
    mFocalLength = 1.0;
    mViewportHeight = 2.0;
    mViewportWidth = mViewportHeight * ((double) mImageWidth / mImageHeight);
    mCameraCenter = Point3(0,0,0);

    // Initialize the viewport
    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewportU = Vec3(mViewportWidth, 0, 0);
    auto viewportV = Vec3(0, -mViewportHeight, 0);
    mViewportUpperLeft = mCameraCenter - viewportU / 2 - viewportV / 2 - Vec3(0, 0, mFocalLength);
    
    mPixelDeltaU = viewportU / mImageWidth;
    mPixelDeltaV = viewportV / mImageHeight;
    mPixel00Loc = mViewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);
}

Color Camera::RayColor(const Ray& r, const HittableObjects& scene) {
    // Get the color of the pixel
    HitRecord record;
    if (scene.Hit(r, Interval(0.0, G_INFINITY), record)) {
        return 0.5 * Color(record.normal + Color(1,1,1));
    }

    auto unit_direction = unit_vector(r.Direction());
    auto a = 0.5 * (1.0 + unit_direction.y());

    return Color(0.5,0.7,1) * a + Color(1,1,1) * (1 - a);
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
            auto pixel_center = mPixel00Loc + mPixelDeltaU * i + mPixelDeltaV * j;
            auto ray_direction = pixel_center - mCameraCenter;
            Ray r(mCameraCenter, ray_direction);

            Color pixel_color = RayColor(r, scene);

            WriteColor(out, pixel_color);
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << mImageWidth << "x" << mImageHeight << ").\n";

}
