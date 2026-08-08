#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include "Interval.h"

#include <fstream>

using Color = Vec3;

inline void WriteColor(std::ofstream& out, const Color& pixel_color) {
    static const Interval validColorRange(0.0, 0.999);

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(256 * validColorRange.Clamp(r));
    int gbyte = int(256 * validColorRange.Clamp(g));
    int bbyte = int(256 * validColorRange.Clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif