#pragma once

#include "Vector.h"

struct Record
{
    Vector3f normal;
};

class GeoObject
{
public:
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
    Sphere(Vector3f c, float r) : c(c), r(r) {}
    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point)
    {
        return Vector3f(point - c).normalize();
    }
};

class Elipsoid : public GeoObject
{
private:
    float a, b, c;
    Vector3f normal_factor;
public:
    // x^2/a^2 + y^2/b^2 + z^2/c^2 - 1 = 0
    Elipsoid(float a, float b, float c) : a(a), b(b), c(c),normal_factor(Vector3f(2/a*a,2/b*b,2/c*c)) {}
    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point)
    {
        return Vector3f(point*normal_factor).normalize();
    }
};

class Plane : public GeoObject
{
private:
    Vector3f abc;
    float cons;

public:
    // ax + by + cz + cons = 0
    Plane(float a, float b, float c, float cons) : abc(Vector3f(a, b, c)), cons(cons) {}
    Plane(Vector3f p, Vector3f norm) : abc(norm), cons(-p.dot(norm)) {}

    bool getIntersection(Vector3f e, Vector3f d, float &t0, float &t1, Record &rec);
    Vector3f getNormal(Vector3f point){
        return abc.normalize();
    }
};