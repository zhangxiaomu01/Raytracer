#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>


// Constants
const double G_EPSILON = 1e-6;
const double G_PI = 3.14159265358979323846;
const double G_INFINITY = std::numeric_limits<double>::infinity();

// Utility Functions
inline double Clamp(double x, double min, double max) {
    return std::max(min, std::min(max, x));
}

inline double DgreeToRadians(double degrees) {
    return degrees * G_PI / 180.0;
}

inline double RadiansToDegrees(double radians) {
    return radians * 180.0 / G_PI;
}

// Common includes
#include "Color.h"
#include "Ray.h"
#include "Vec3.h"

#endif