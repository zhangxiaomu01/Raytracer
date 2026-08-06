#include <fstream>
#include <iostream>

#include "Color.h"
#include "Ray.h"

// Formula: t^2 - 2*t*oc + oc^2 - radius^2 = 0
// t = (-b + sqrt(b^2 - 4*a*c)) / (2*a)
double hit_sphere(const point3& center, double radius, const Ray& r) {
    vec3 oc = r.Origin() - center;
    auto a = dot(r.Direction(), r.Direction());
    auto h = dot(oc, r.Direction()); // h = b / -2
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < 0.0) {
        return -1.0;
    }

    return (-h - std::sqrt(discriminant)) /  a;
}

Color ray_color(const Ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.At(t) - point3(0,0,-1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
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
            Ray r(camera_center, ray_direction);

            Color pixel_color = ray_color(r);

            WriteColor(out, pixel_color);
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << image_width << "x" << image_height << ").\n";
    return 0;
}
