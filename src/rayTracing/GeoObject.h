#pragma once

#include <memory>
#include <queue>

#include "BaseObject.h"
#include "Ray.h"
#include "Vector.h"

class GeoObject;

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

    bool operator<(const TRecord &b) const { return t < b.t; }
};

using TQueue = std::vector<TRecord>;
class GeoObject : public BaseObject
{
public:
    Vector3f color;
    bool specular_reflection = false;
    float km = 0.3f;

    GeoObject(const Vector3f &color, const Vector3f &center) : BaseObject(center), color(color) {}

    virtual Vector3f getNormal(const Vector3f &view, const Vector3f &point) const = 0;

    void pushQueue(std::shared_ptr<TQueue> q, float t, const Vector3f &inter_point,
                   const Vector3f &normal, GeoObject *target) const
    {
        if (!q)
        {
            return;
        }
        q->push_back(TRecord(t, inter_point, normal, target));
    }
    void pushQueue(std::shared_ptr<TQueue> q, const TRecord &rec) const
    {
        if (!q)
        {
            return;
        }
        q->push_back(rec);
    }

    GeoObject &set_specular(float km)
    {
        this->specular_reflection = true;
        this->km = km;
        return *this;
    }

    bool getTransformIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q)
    {
        Ray tmp_viewRay = Ray(inverseMat * viewRay.e, inverseMat * viewRay.d);
        if (!q)
        {
            return getIntersection(tmp_viewRay, nullptr);
        }
        auto tmp_q = std::make_shared<TQueue>();
        bool rnt = getIntersection(tmp_viewRay, tmp_q);
        for (auto record : *tmp_q)
        {
            record.inter_point = (transformMat * record.inter_point.toPoint4f()).getVector3f();
            record.normal =
                (inverseMat.T() * record.normal.toDirection4f()).getVector3f().normalize();
            q->push_back(record);
        }
        return rnt;
    }

private:
    virtual bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const = 0;
};

class Sphere : public GeoObject
{
private:
    float r;

public:
    // (p-c)^2 - r^2 = 0
    Sphere(Vector3f c, float r, Vector3f color) : GeoObject(color, c), r(r) {}
    bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(const Vector3f &view, const Vector3f &point) const
    {
        return Vector3f(point - center).normalize();
    }
};

class Elipsoid : public GeoObject
{
private:
    Vector3f abc, uvw;
    Vector3f normal_factor;

public:
    // (x-u)^2/a^2 + (y-v)^2/b^2 + (z-w)^2/c^2 - 1 = 0
    Elipsoid(Vector3f abc, Vector3f uvw, Vector3f color)
        : GeoObject(color, uvw), abc(abc), uvw(uvw), normal_factor(2 / (abc * abc))
    {
    }
    bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(const Vector3f &view, const Vector3f &point) const
    {
        return Vector3f((point - uvw) * normal_factor).normalize();
    }
};

class Plane : public GeoObject
{
public:
    Vector3f abc;
    float cons;
    Plane(Vector3f p, Vector3f norm, Vector3f color)
        : GeoObject(color, p), abc(norm), cons(-p.dot(norm))
    {
    }

    bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(const Vector3f &view, const Vector3f &point) const
    {
        // Vector3f n = abc.normalize();
        // Vector3f np = Vector3f(-cons / abc.x, 0, 0) + n;
        // bool b1 = side(np);
        // bool b2 = side(view);
        // return b1 == b2 ? n : -n;
        return abc.normalize();
    }

    bool side(const Vector3f &point) const { return abc.dot(point) + cons > 0; }
};

class Circle : public Plane
{
private:
    Vector3f c;
    float r;

public:
    Circle(Vector3f center, float r, Vector3f norm, Vector3f color)
        : Plane(center, norm, color), c(center), r(r)
    {
    }
    bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const;
};

class Cylinder : public GeoObject
{
private:
    Circle c1, c2;
    Vector3f direct;
    float r;

    bool between2plane(const Vector3f &point) const { return !c1.side(point) && !c2.side(point); }

public:
    // up_bound must be bigger than low_bound
    Cylinder(Vector3f center, Vector3f direct, float r, float up_bound, float low_bound,
             Vector3f color)
        : GeoObject(color, center),
          c1(Circle(center + direct * up_bound, r, direct.normalize(), color)),
          c2(Circle(center + direct * low_bound, r, -direct.normalize(), color)),
          direct(direct.normalize()), r(r)

    {
    }
    bool getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const;
    Vector3f getNormal(const Vector3f &view, const Vector3f &point) const
    {
        Vector3f v = point - center;
        Vector3f n = (point - (center + (direct * v.dot(direct)))).normalize();
        return n;
    }
};