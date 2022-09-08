#pragma once

#include <cmath>
#include <ofColor.h>
class Vector3f
{
public:
    float x, y, z;
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3f operator+(const Vector3f &b) const
    {
        return Vector3f(x + b.x, y + b.y, z + b.z);
    }
    Vector3f operator-(const Vector3f &b) const
    {
        return Vector3f(x - b.x, y - b.y, z - b.z);
    }
    Vector3f operator*(const Vector3f &b) const
    {
        return Vector3f(x * b.x, y * b.y, z * b.z);
    }
    Vector3f operator/(const Vector3f &b) const
    {
        return Vector3f(x / b.x, y / b.y, z / b.z);
    }
    Vector3f &operator+=(const Vector3f &b)
    {
        x += b.x, y += b.y, z += b.z;
        return *this;
    }
    Vector3f &operator-=(const Vector3f &b)
    {
        x -= b.x, y -= b.y, z -= b.z;
        return *this;
    }
    Vector3f &operator*=(const Vector3f &b)
    {
        x *= b.x, y *= b.y, z *= b.z;
        return *this;
    }
    Vector3f &operator/=(const Vector3f &b)
    {
        x /= b.x, y /= b.y, z /= b.z;
        return *this;
    }

    Vector3f cross(const Vector3f &b) const
    {
        return Vector3f(y * b.z - z * b.y,
                        z * b.x - x * b.z,
                        x * b.y - y * b.x);
    }
    float dot(const Vector3f &b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }

    Vector3f operator*(const float &b) const
    {
        return Vector3f(x * b, y * b, z * b);
    }
    Vector3f &operator*=(const float &b)
    {
        x = x * b;
        y = y * b;
        z = z * b;
        return *this;
    }
    friend Vector3f operator*(const float &b, const Vector3f &vec)
    {
        return vec * b;
    }

    Vector3f normalize() const
    {
        float length = std::sqrt(x * x + y * y + z * z);
        return *this * (1 / length);
    }

    ofColor getOfColor() const
    {
        return ofColor(x * 255, y * 255, z * 255);
    }
};