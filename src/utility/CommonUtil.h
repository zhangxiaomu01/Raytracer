#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>


// Constants
const inline double G_EPSILON = 1e-6;
const inline double G_PI = 3.14159265358979323846;
const inline double G_INFINITY = std::numeric_limits<double>::infinity();

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

// Generate a random double in the range [0, 1) uniformly
inline double RandomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double minVal, double maxVal) {
    return minVal + RandomDouble() * (maxVal - minVal);
}

#endif