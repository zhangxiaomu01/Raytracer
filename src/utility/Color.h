#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include "Interval.h"

#include <fstream>

using Color = Vec3;

// Convert linear color to gamma color
inline Color LinearToGamma(const Color& linear_color) {
    const double inverseGamma = 1.0 / 2.2;
    return Vec3(pow(linear_color.x(), inverseGamma), pow(linear_color.y(), inverseGamma), pow(linear_color.z(), inverseGamma));
}

inline void WriteColor(std::ofstream& out, const Color& pixel_color) {
    static const Interval validColorRange(0.0, 0.999);

    auto gammaColor = LinearToGamma(pixel_color);
    auto r = gammaColor.x();
    auto g = gammaColor.y();
    auto b = gammaColor.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(256 * validColorRange.Clamp(r));
    int gbyte = int(256 * validColorRange.Clamp(g));
    int bbyte = int(256 * validColorRange.Clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif