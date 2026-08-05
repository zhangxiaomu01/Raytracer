#include <fstream>
#include <iostream>

#include "color.h"
#include "ray.h"

color ray_color(const ray& r) {
    return color(0,0,0);
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
    vec3 camera_center(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);
    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    auto viewport_upper_left = camera_center - viewport_u / 2 - viewport_v / 2 - vec3(0, 0, focal_length);
    auto pixel00_loc = viewport_upper_left + pixel_delta_u * 0.5 + pixel_delta_v * 0.5;

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
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);

            write_color(out, pixel_color);
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << image_width << "x" << image_height << ").\n";
    return 0;
}
