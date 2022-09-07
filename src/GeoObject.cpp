#include "GeoObject.h"
#include <ofMatrix3x3.h>

static float solveSphere(ofVec3f e, ofVec3f d, ofVec3f c, float r)
{
    float A = d.dot(d);
    float B = 2 * d.dot(e - c);
    float C = (e - c).dot(e - c) - r * r;
    float delta = B * B - 4 * A * C;
    if (delta <= 0)
    {
        return NONE;
    }

    float t1 = (-B - std::sqrt(delta)) / A;
    float t2 = (-B + std::sqrt(delta)) / A;
    float maxt = std::max(t1, t2);
    float mint = std::min(t1, t2);
    if (mint > 0)
    {
        return mint;
    }
    else if (maxt > 0)
    {
        return maxt;
    }
    else
    {
        return NONE;
    }
}

float Sphere::getIntersectionT(ofVec3f e, ofVec3f d)
{
    return solveSphere(e, d, c, r);
}

float Elipsoid::getIntersectionT(ofVec3f e, ofVec3f d)
{
    ofVec3f scaleDown(1 / a, 1 / b, 1 / c);
    e *= scaleDown;
    d *= scaleDown;
    return solveSphere(e, d, ofVec3f(0, 0, 0), 1);
}