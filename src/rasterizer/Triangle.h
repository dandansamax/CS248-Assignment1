#pragma once

#include "Constant.h"
#include "Vector.h"

class Triangle
{
public:
    Vector3f t1, t2, t3;
    Vector3f n1, n2, n3;

public:
    Triangle(Vector3f t1, Vector3f t2, Vector3f t3, Vector3f n1, Vector3f n2, Vector3f n3)
        : t1(t1), t2(t2), t3(t3), n1(n1), n2(n2), n3(n3)
    {
    }

    Vector3f getBaryCoor(Vector3f p)
    {
        auto x = Vector3f(t1.x, t1.y, 0);
        auto y = Vector3f(t2.x, t2.y, 0);
        auto z = Vector3f(t3.x, t3.y, 0);
        auto n = (y - x).cross(z - x);
        auto na = (z - y).cross(p - y);
        auto nb = (x - z).cross(p - z);
        float alpha = n.dot(na) / (n.dot(n));
        float beta = n.dot(nb) / (n.dot(n));
        return Vector3f(alpha, beta, 1.0f - alpha - beta);
    }

    float getDepth(const Vector3f &coor)
    {
        return coor.x * t1.z + coor.y * t2.z + coor.z * t3.z;
    }

    Vector3f getNormal(const Vector3f &coor)
    {
        return coor.x * n1 + coor.y * n2 + coor.z * n3;
    }

    Vector3f getPosition(const Vector3f &coor)
    {
        return coor.x * t1 + coor.y * t2 + coor.z * t3;
    }

    // x_min, x_max, y_min, y_max
    Vector4f getBoundingBox()
    {
        auto x_min = std::floor(std::min(std::min(t1.x, t2.x), t3.x));
        auto x_max = std::ceil(std::max(std::max(t1.x, t2.x), t3.x));
        auto y_min = std::floor(std::min(std::min(t1.y, t2.y), t3.y));
        auto y_max = std::ceil(std::max(std::max(t1.y, t2.y), t3.y));
        return Vector4f(x_min, x_max, y_min, y_max);
    }

    friend Triangle operator*(const Matrix4f &mat, const Triangle &t)
    {
        auto a = (mat * t.t1.toPoint4f()).getVector3f();
        auto b = (mat * t.t2.toPoint4f()).getVector3f();
        auto c = (mat * t.t3.toPoint4f()).getVector3f();
        return Triangle(a, b, c, t.n1, t.n2, t.n3);
    }
};