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
        // Linear interpolation
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());
        // Hermite cubic interpolation
        u = u * u * (3.0 - 2.0 * u);
        v = v * v * (3.0 - 2.0 * v);
        w = w * w * (3.0 - 2.0 * w);

        auto i = int(std::floor(p.x()));
        auto j = int(std::floor(p.y()));
        auto k = int(std::floor(p.z()));
        double c[2][2][2];

        for (int l = 0; l < 2; l++) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    c[l][m][n] = randDoubles[
                        permX[(i + l) & 255] ^ permY[(j + m) & 255] ^ permZ[(k + n) & 255]
                    ];
                }
            }
        }

        return TrilinearInterpolation(c, u, v, w);
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

    static double TrilinearInterpolation(double c[2][2][2], double u, double v, double w) {
        double accum = 0.0;
        for (int l = 0; l < 2; l++) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    accum += (l * u + (1 - l) * (1 - u)) 
                    * (m * v + (1 - m) * (1 - v)) 
                    * (n * w + (1 - n) * (1 - w)) * c[l][m][n];
                }
            }
        }
        return accum;
    }

};

#endif
