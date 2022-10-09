#pragma once

#include <cmath>
#include <iostream>

#include "Constant.h"

class Vector4f;

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
        o << v.x << " " << v.y << " " << v.z;
        return o;
    }

    Vector4f toPoint4f() const;
    Vector4f toDirection4f() const;
};

class Vector4f
{
public:
    float x, y, z, w;
    Vector4f() : x(0), y(0), z(0), w(0) {}
    Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    bool operator==(const Vector4f &b) { return x == b.x && y == b.y && z == b.z && w == b.w; }

    Vector4f operator+(const Vector4f &b) const { return Vector4f(x + b.x, y + b.y, z + b.z, w + b.w); }
    Vector4f operator-(const Vector4f &b) const { return Vector4f(x - b.x, y - b.y, z - b.z, w - b.w); }
    Vector4f operator*(const Vector4f &b) const { return Vector4f(x * b.x, y * b.y, z * b.z, w * b.w); }
    Vector4f &operator+=(const Vector4f &b)
    {
        x += b.x, y += b.y, z += b.z, w += b.w;
        return *this;
    }
    Vector4f &operator-=(const Vector4f &b)
    {
        x -= b.x, y -= b.y, z -= b.z, w -= b.w;
        return *this;
    }
    Vector4f &operator*=(const Vector4f &b)
    {
        x *= b.x, y *= b.y, z *= b.z, w *= b.w;
        return *this;
    }
    float dot(const Vector4f &b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }

    Vector3f getVector3f() const;

    friend std::ostream &operator<<(std::ostream &o, const Vector4f &v)
    {
        o << v.x << " " << v.y << " " << v.z << " " << v.w;
        return o;
    }
};

class Matrix4f
{
public:
    Vector4f row[4];
    Matrix4f() : row{Vector4f(1, 0, 0, 0), Vector4f(0, 1, 0, 0), Vector4f(0, 0, 1, 0), Vector4f(0, 0, 0, 1)} {}
    Matrix4f(const Vector4f &a, const Vector4f &b, const Vector4f &c, const Vector4f &d) : row{a, b, c, d} {}

    Matrix4f T() const
    {
        return Matrix4f(
            Vector4f(row[0].x, row[1].x, row[2].x, row[3].x), Vector4f(row[0].y, row[1].y, row[2].y, row[3].y),
            Vector4f(row[0].z, row[1].z, row[2].z, row[3].z), Vector4f(row[0].w, row[1].w, row[2].w, row[3].w));
    }

    Vector4f operator*(const Vector4f &vec) const
    {
        return Vector4f(vec.dot(row[0]), vec.dot(row[1]), vec.dot(row[2]), vec.dot(row[3]));
    }

    Matrix4f operator*(const Matrix4f &b) const
    {
        Matrix4f bT = b.T();
        Matrix4f rnt = Matrix4f();
        for (int r = 0; r < 4; r++)
        {
            rnt.row[r] =
                Vector4f(row[r].dot(bT.row[0]), row[r].dot(bT.row[1]), row[r].dot(bT.row[2]), row[r].dot(bT.row[3]));
        }
        return rnt;
    }

    friend std::ostream &operator<<(std::ostream &o, const Matrix4f &m)
    {
        for (auto x : m.row)
        {
            o << x << std::endl;
        }
        return o;
    }
};

inline Vector4f Vector3f::toPoint4f() const { return Vector4f(x, y, z, 1); }
inline Vector4f Vector3f::toDirection4f() const { return Vector4f(x, y, z, 0); }

inline Vector3f Vector4f::getVector3f() const { return w < eps ? Vector3f(x, y, z) : Vector3f(x / w, y / w, z / w); }