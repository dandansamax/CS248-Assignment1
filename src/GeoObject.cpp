#include <cmath>
#include <utility>

#include "GeoObject.h"

static bool solveQuadratic(float a, float b, float c, float &ans0, float &ans1)
{
    float delta = b * b - 4 * a * c;
    if (delta <= 0)
    {
        return false;
    }

    ans0 = (-b - std::sqrt(delta)) / (2 * a);
    ans1 = (-b + std::sqrt(delta)) / (2 * a);
    return true;
}

bool Sphere::getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec)
{
    float A = d.dot(d);
    float B = 2 * d.dot(e - c);
    float C = (e - c).dot(e - c) - r * r;
    if (!solveQuadratic(A, B, C, t0, t1))
    {
        return false;
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

bool Elipsoid::getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec)
{
    float A = ((d * d) / (abc * abc)).sum();
    float B = ((2 * (e - uvw) * d) / (abc * abc)).sum();
    float C = (((e - uvw) * (e - uvw)) / (abc * abc)).sum() - 1;
    if (!solveQuadratic(A, B, C, t0, t1))
    {
        return false;
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