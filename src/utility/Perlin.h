#ifndef PERLIN_H
#define PERLIN_H

#include "Vec3.h"

class Perlin
{
public:
    Perlin() {
        for (int i = 0; i < POINT_COUNT; i++) {
            randonVectors[i] = unit_vector(Vec3::RandomVec3());
        }

        PerlinGeneratePerm(permX);
        PerlinGeneratePerm(permY);
        PerlinGeneratePerm(permZ);
    }
    ~Perlin() = default;

    double Noise(const Point3& p) const {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = int(std::floor(p.x()));
        auto j = int(std::floor(p.y()));
        auto k = int(std::floor(p.z()));
        Vec3 c[2][2][2];

        for (int l = 0; l < 2; l++) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    c[l][m][n] = randonVectors[
                        permX[(i + l) & 255] ^ permY[(j + m) & 255] ^ permZ[(k + n) & 255]
                    ];
                }
            }
        }

        return PerlinInterpolation(c, u, v, w);
    }

    double Turbulence(const Point3& p, int depth) const {
        double accum = 0.0;
        auto tempP = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight * Noise(tempP);
            tempP *= 2.0;
            weight *= 0.5;
        }
        return std::fabs(accum);
    }

private:
    static const int POINT_COUNT = 256;
    Vec3 randonVectors[POINT_COUNT];
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

    static double PerlinInterpolation(const Vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u * u * (3.0 - 2.0 * u);
        auto vv = v * v * (3.0 - 2.0 * v);
        auto ww = w * w * (3.0 - 2.0 * w);
        double accum = 0.0;
        for (int l = 0; l < 2; l++) {
            for (int m = 0; m < 2; m++) {
                for (int n = 0; n < 2; n++) {
                    Vec3 weightValue(u-l, v-m, w-n);
                    accum += (l * uu + (1 - l) * (1 - uu)) 
                    * (m * vv + (1 - m) * (1 - vv)) 
                    * (n * ww + (1 - n) * (1 - ww)) * dot(weightValue, c[l][m][n]);
                }
            }
        }
        return accum;
    }

};

#endif
