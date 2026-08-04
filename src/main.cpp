#include <fstream>
#include <iostream>

int main() {
    // Image

    int image_width = 256;
    int image_height = 256;

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
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    out.close();
    std::cerr << "Done. Wrote image.ppm (" << image_width << "x" << image_height << ").\n";
    return 0;
}
