#ifndef PERLIN_H
#define PERLIN_H

#include "Vec3.h"

class Perlin
{
public:
    Perlin() {
        for (int i = 0; i < POINT_COUNT; i++) {
            randDoubles[i] = RandomDouble();
        }

        PerlinGeneratePerm(permX);
        PerlinGeneratePerm(permY);
        PerlinGeneratePerm(permZ);
    }
    ~Perlin() = default;

    double Noise(const Point3& p) const {
        auto i = int(4 * p.x()) & 255;
        auto j = int(4 * p.y()) & 255;
        auto k = int(4 * p.z()) & 255;
        return randDoubles[permX[i] ^ permY[j] ^ permZ[k]];
    }

private:
    static const int POINT_COUNT = 256;
    double randDoubles[POINT_COUNT];
    int permX[POINT_COUNT];
    int permY[POINT_COUNT];
    int permZ[POINT_COUNT];

    static void PerlinGeneratePerm(int* p) {
        for (int i = 0; i < POINT_COUNT; i++) {
            p[i] = i;
        }

        Permute(p, POINT_COUNT);
    }

    static void Permute(int* p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = RandomInt(0, i);
            std::swap(p[i], p[target]);
        }
    }
};

#endif
