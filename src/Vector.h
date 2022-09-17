#pragma once

#include <cmath>
#include <ofColor.h>
class Vector3f
{
public:
    float x, y, z;
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

    bool operator==(const Vector3f &b) { return x == b.x && y == b.y && z == b.z; }

    Vector3f operator-() { return Vector3f(-x, -y, -z); }

    Vector3f operator+(const Vector3f &b) const { return Vector3f(x + b.x, y + b.y, z + b.z); }
    Vector3f operator-(const Vector3f &b) const { return Vector3f(x - b.x, y - b.y, z - b.z); }
    Vector3f operator*(const Vector3f &b) const { return Vector3f(x * b.x, y * b.y, z * b.z); }
    Vector3f operator/(const Vector3f &b) const { return Vector3f(x / b.x, y / b.y, z / b.z); }
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
        return Vector3f(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }
    float dot(const Vector3f &b) const { return x * b.x + y * b.y + z * b.z; }

    Vector3f operator*(const float &b) const { return Vector3f(x * b, y * b, z * b); }
    Vector3f operator/(const float &b) const { return Vector3f(x / b, y / b, z / b); }
    Vector3f &operator*=(const float &b)
    {
        x = x * b;
        y = y * b;
        z = z * b;
        return *this;
    }
    friend Vector3f operator*(const float &b, const Vector3f &vec) { return vec * b; }
    friend Vector3f operator/(const float &b, const Vector3f &vec) { return Vector3f(b / vec.x, b / vec.y, b / vec.z); }

    Vector3f normalize() const { return *this * (1 / this->norm()); }
    Vector3f truncate() const { return Vector3f(std::min(1.0f, x), std::min(1.0f, y), std::min(1.0f, z)); }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    float sum() const { return x + y + z; }

    friend std::ostream &operator<<(std::ostream &o, const Vector3f &v)
    {
        o << v.x << " "
          << " " << v.y << " " << v.z;
        return o;
    }

    ofColor getOfColor() const { return ofColor(x * 255, y * 255, z * 255); }
};