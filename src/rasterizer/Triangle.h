#pragma once

#include "Vector.h"

namespace rasterizer
{
    class Triangle
    {
    public:
        Vector3f t1, t2, t3;

    public:
        Triangle(Vector3f t1, Vector3f t2, Vector3f t3) : t1(t1), t2(t2), t3(t3) {}
    };
}