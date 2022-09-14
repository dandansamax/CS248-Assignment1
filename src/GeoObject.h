#pragma once

#include "Vector.h"

#include <queue>

struct TRecord
{
    TRecord(float t, Vector3f inter_point, Vector3f normal, GeoObject *target)
        : t(t), inter_point(inter_point), normal(normal), target(target)
    {
    }

    float t;
    Vector3f inter_point;
    Vector3f normal;
    GeoObject *target;

    bool operator<(const TRecord &b) const { return t > b.t; }
};

using TQueue = std::vector<TRecord>;

class GeoObject
{
public:
    Vector3f color;
    GeoObject(Vector3f color) : color(color) {}
    // virtual ~GeoObject() {}

    // make sure that t0 is less than t1
    virtual bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const = 0;
    virtual Vector3f getNormal(Vector3f view, Vector3f point) const = 0;
};

class Sphere : public GeoObject
{
private:
    Vector3f c;
    float r;

public:
    // (p-c)^2 - r^2 = 0
    Sphere(Vector3f c, float r, Vector3f color) : GeoObject(color), c(c), r(r) {}
    bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(Vector3f view, Vector3f point) const { return Vector3f(point - c).normalize(); }
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
    bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(Vector3f view, Vector3f point) const { return Vector3f((point - uvw) * normal_factor).normalize(); }
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

    bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(Vector3f view, Vector3f point) const { return abc.normalize(); }

    bool side(Vector3f point) const { return abc.dot(point) + cons > 0; }
};

class Circle : public Plane
{
private:
    Vector3f c;
    float r;

public:
    Circle(Vector3f center, float r, Vector3f norm, Vector3f color) : Plane(center, norm, color), r(r), c(c) {}
    bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const;
};

class Cylinder : public GeoObject
{
private:
    Circle c1, c2;
    Vector3f center, direct;
    float r;

public:
    Cylinder(Vector3f center, Vector3f direct, float r, Vector3f bound1, Vector3f bound2, Vector3f color)
        : GeoObject(color), center(center.normalize()), direct(direct.normalize()), r(r),
          c1(Circle(bound1, r, (bound1 - bound2).normalize(), color)),
          c2(Circle(bound2, r, (bound2 - bound1).normalize(), color))
    {
    }
    bool getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const;
};