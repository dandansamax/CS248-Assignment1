#pragma once

#include "Vector.h"
#include <algorithm>
#include <vector>

class MarbleTexture
{
private:
    float width, k1, k2;
    int n;
    std::vector<int> P;

    float solidNoise(const Vector3f &xyz)
    {
        // TODO
        return 0.0f;
    }

    float bigOmega(const Vector3f &uvw, const Vector3f &ijk)
    {
        // TODO
        return 0.0f;
    }

    float smallOmega(float t)
    {
        auto abs = std::abs(t);
        if (abs < 1)
        {
            return 2 * t * t * t - 3 * t * t + 1;
        }
        else
        {
            return 0;
        }
    }

public:
    MarbleTexture(float width, float k1, float k2, int n) : width(width), k1(k1), k2(k2), n(n)
    {
        for (int i = 0; i < n; i++)
        {
            P.push_back(i);
        }
        std::random_shuffle(P.begin(), P.end());
    }

    Vector3f getColor(const Vector3f &point)
    {
        // TODO
        return Vector3f();
    }
};