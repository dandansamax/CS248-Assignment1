#include <cmath>

#include "GeoObject.h"

static bool solveSphere(Vector3f e, Vector3f d, Vector3f c, float r, float &t0, float &t1)
{
    float A = d.dot(d);
    float B = 2 * d.dot(e - c);
    float C = (e - c).dot(e - c) - r * r;
    float delta = B * B - 4 * A * C;
    if (delta <= 0)
    {
        return false;
    }

    float t0 = (-B - std::sqrt(delta)) / A;
    float t1 = (-B + std::sqrt(delta)) / A;
    if (t0 > t1)
    {
        std::swap(t0, t1);
    }
    if (t1 > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Sphere::getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec)
{
    return solveSphere(e, d, c, r, t0, t1);
}

bool Elipsoid::getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec)
{
    Vector3f scaleDown(1 / a, 1 / b, 1 / c);
    Vector3f et = e * scaleDown;
    Vector3f dt = d * scaleDown;
    return solveSphere(et, dt, Vector3f(0, 0, 0), 1, t0, t1);
}

bool Plane::getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec)
{
    t0 = -1.0f;
    float t = abc.dot(d);
    if (t == 0.0f)
    {
        return false;
    }

    t1 = -(abc.dot(e) + cons) / t;
    if (t1 > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}