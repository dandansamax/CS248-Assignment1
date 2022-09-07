#pragma once

#include <ofVec3f.h>

const static float NONE = -1.0f;

class GeoObject
{
public:
    virtual float getIntersectionT(ofVec3f e, ofVec3f d) = 0;
};

class Sphere : public GeoObject
{
public:
    ofVec3f c;
    float r;
    // (p-c)^2 - r^2 = 0
    Sphere(ofVec3f c, float r) : c(c), r(r) {}
    float getIntersectionT(ofVec3f e, ofVec3f d);
};

class Elipsoid : public GeoObject
{
public:
    float a, b, c;
    // x^2/a^2 + y^2/b^2 + z^2/c^2 - 1 = 0
    Elipsoid(float a, float b, float c) : a(a), b(b), c(c) {}
    float getIntersectionT(ofVec3f e, ofVec3f d);
};

class Plane : public GeoObject
{
public:
    float a, b, c, d;
    // ax + by + cz + d = 0
    Plane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
    Plane(ofVec3f p, ofVec3f norm);

    float getIntersectionT(ofVec3f e, ofVec3f d);
};