#include "CommonUtil.h"
#include "HittableObjects.h"
#include "SphereShape.h"

Color ray_color(const Ray& r, const HittableObjects& scene) {
    HitRecord record;
    if (scene.Hit(r, 0.0, INFINITY, record)) {
        return 0.5 * Color(record.normal + Color(1,1,1));
    }

    auto unit_direction = unit_vector(r.Direction());
    auto a = 0.5 * (1.0 + unit_direction.y());

    return Color(0.5,0.7,1) * a + Color(1,1,1) * (1 - a);
}

int main() {
    // Image

    auto aspect_ratio = 16 / 9.0;
    int image_width = 400;
    auto image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = image_height < 1 ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * ((double) image_width / image_height);
    Vec3 camera_center(0, 0, 0);

    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);
    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    auto viewport_upper_left = camera_center - viewport_u / 2 - viewport_v / 2 - Vec3(0, 0, focal_length);
    auto pixel00_loc = viewport_upper_left + pixel_delta_u * 0.5 + pixel_delta_v * 0.5;

    // Scene
    auto scene = std::make_shared<HittableObjects>();
    // Add spheres to the scene
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,0,-1), 0.5));
    scene->AddObject(std::make_shared<SphereShape>(Point3(0,-100.5, -1), 100));

    // Render

    std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
    if (!out) {
        std::cerr << "Error: cannot open image.ppm for writing.\n";
        return 1;
    }

    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + pixel_delta_u * i + pixel_delta_v * j;
            auto ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            Color pixel_color = ray_color(r, *scene);

            WriteColor(out, pixel_color);
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << image_width << "x" << image_height << ").\n";
    return 0;
}
