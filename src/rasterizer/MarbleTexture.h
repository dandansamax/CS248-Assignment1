#pragma once

#include "Vector.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

class MarbleTexture
{
private:
    float width, k1, k2;
    int n, turbulenceFactor;
    Vector3f s0, s1;
    std::vector<int> P;
    std::vector<Vector3f> G;

    float solidNoise(const Vector3f &xyz)
    {
        int x = xyz.x;
        int y = xyz.y;
        int z = xyz.z;

        float rnt = 0;
        for (int i = x; i <= x + 1; i++)
            for (int j = y; j <= y + 1; j++)
                for (int k = z; k <= z + 1; k++)
                {
                    rnt += bigOmega(Vector3f(xyz.x - i, xyz.y - j, xyz.z - k), i, j, k);
                }
        return rnt;
    }

    float bigOmega(const Vector3f &uvw, int i, int j, int k)
    {
        return smallOmega(uvw.x) * smallOmega(uvw.y) * smallOmega(uvw.z) * Gamma(i, j, k).dot(uvw);
    }

    float smallOmega(float t)
    {
        auto abs = std::abs(t);
        if (abs < 1)
        {
            return 2 * abs * abs * abs - 3 * abs * abs + 1;
        }
        else
        {
            return 0;
        }
    }

    Vector3f Gamma(int i, int j, int k) { return G[phi(i + phi(j + phi(k)))]; }

    int phi(int i) { return P[(i + n * 100000) % n]; }

    Vector3f randomUniVec()
    {
        for (;;)
        {
            float x = ((float)rand() / (RAND_MAX)) * 2 - 1;
            float y = ((float)rand() / (RAND_MAX)) * 2 - 1;
            float z = ((float)rand() / (RAND_MAX)) * 2 - 1;
            if (x * x + y * y + z * z < 1)
            {
                return Vector3f(x, y, z);
            }
        }
    }

    float turbulence(const Vector3f &x)
    {
        float sum = 0;
        for (int i = 0; i < turbulenceFactor; i++)
        {
            float p = std::pow(2, i);
            sum += std::abs(solidNoise(x * p)) / p;
        }
        return sum;
    }

public:
    MarbleTexture(float width, float k1, float k2, int n, int turbulenceFactor, Vector3f s0,
                  Vector3f s1)
        : width(width), k1(k1), k2(k2), n(n), turbulenceFactor(turbulenceFactor), s0(s0), s1(s1)
    {
        std::srand(std::time(0));
        for (int i = 0; i < n; i++)
        {
            P.push_back(i);
            G.push_back(randomUniVec());
        }
        std::random_shuffle(P.begin(), P.end());
    }

    Vector3f getColor(const Vector3f &point)
    {
        float t = (1 + std::sin((k1 * point.z + turbulence(k2 * point)) / width)) / 2;
        return t * s0 + (1 - t) * s1;
    }
};