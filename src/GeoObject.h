#pragma once

#include "Vector.h"

struct Record
{
    Vector3f normal;
};

class GeoObject
{
public:
    Vector3f color;
    GeoObject(Vector3f color) : color(color) {}
    // virtual ~GeoObject() {}

    // make sure that t0 is less than t1
    virtual bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec) = 0;
    virtual Vector3f getNormal(Vector3f point) = 0;
};

class Sphere : public GeoObject
{
private:
    Vector3f c;
    float r;

public:
    // (p-c)^2 - r^2 = 0
    Sphere(Vector3f c, float r, Vector3f color) : GeoObject(color), c(c), r(r) {}
    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point) { return Vector3f(point - c).normalize(); }
};

class Elipsoid : public GeoObject
{
private:
    Vector3f abc, uvw;
    Vector3f normal_factor;

public:
    // (x-u)^2/a^2 + (y-v)^2/b^2 + (z-w)^2/c^2 - 1 = 0
    Elipsoid(Vector3f abc, Vector3f uvw, Vector3f color)
        : GeoObject(color), abc(abc), uvw(uvw), normal_factor(2 / (abc * abc))
    {
    }
    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point) { return Vector3f((point - uvw) * normal_factor).normalize(); }
};

class Plane : public GeoObject
{
private:
    Vector3f abc;
    float cons;

public:
    // ax + by + cz + cons = 0
    Plane(float a, float b, float c, float cons, Vector3f color) : GeoObject(color), abc(Vector3f(a, b, c)), cons(cons)
    {
    }
    Plane(Vector3f p, Vector3f norm, Vector3f color) : GeoObject(color), abc(norm), cons(-p.dot(norm)) {}

    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point) { return abc.normalize(); }
};