#include <cmath>
#include <utility>

#include "GeoObject.h"

static inline bool solveQuadratic(float a, float b, float c, float &ans0, float &ans1)
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

bool Sphere::getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const
{
    float A = d.dot(d);
    float B = 2 * d.dot(e - c);
    float C = (e - c).dot(e - c) - r * r;
    float t[2];
    if (!solveQuadratic(A, B, C, t[0], t[1]))
    {
        return false;
    }
    bool flag = false;
    for (auto i : t)
    {
        if (i < 0)
            continue;
        flag = true;
        Vector3f point = e + d * i;
        q->push_back(TRecord(i, point, getNormal(e, point), (GeoObject *)this));
    }
    return flag;
}

bool Elipsoid::getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const
{
    float A = ((d * d) / (abc * abc)).sum();
    float B = ((2 * (e - uvw) * d) / (abc * abc)).sum();
    float C = (((e - uvw) * (e - uvw)) / (abc * abc)).sum() - 1;
    float t[2];
    if (!solveQuadratic(A, B, C, t[0], t[1]))
    {
        return false;
    }
    bool flag = false;
    for (auto i : t)
    {
        if (i < 0)
            continue;
        flag = true;
        Vector3f point = e + d * i;
        q->push_back(TRecord(i, point, getNormal(e, point), (GeoObject *)this));
    }
    return flag;
}

bool Plane::getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const
{

    float t = abc.dot(d);
    if (t == 0.0f)
    {
        return false;
    }

    float t = -(abc.dot(e) + cons) / t;
    if (t > 0)
    {
        Vector3f point = e + d * t;
        q->push_back(TRecord(t, point, getNormal(e, point), (GeoObject *)this));
        return true;
    }
    else
    {
        return false;
    }
}

bool Circle::getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const
{

    auto tmp_q = std::make_shared<TQueue>();
    bool rnt = Plane::getIntersection(e, d, tmp_q);
    if (!rnt)
    {
        return false;
    }
    float t = tmp_q->at(0).t;
    Vector3f l = e + d * t - c;
    if (l.dot(l) < r * r)
    {
        q->push_back(tmp_q->at(0));
        return true;
    }
    else
    {
        return false;
    }
}

static inline bool between2plane(Vector3f point, const Plane &plane1, const Plane &plane2)
{
    return plane1.side(point) != plane2.side(point);
}

bool Cylinder::getIntersection(Vector3f e, Vector3f d, std::shared_ptr<TQueue> q) const
{
    auto tmp_q = std::make_shared<TQueue>();
    bool rnt1 = c1.getIntersection(e, d, tmp_q);
    bool rnt2 = c2.getIntersection(e, d, tmp_q);
    if (rnt1 && rnt2)
    {
        q->insert(q->end(), tmp_q->begin(), tmp_q->end());
        return true;
    }
    float dv = d.dot(direct);
    Vector3f ea = e - center;
    float eav = ea.dot(direct);
    float A = dv * dv - d.dot(d);
    float B = 2 * dv * eav - 2 * d.dot(ea);
    float C = eav * eav - ea.dot(ea) + r * r;

    float tmp0, tmp1;
    if (!solveQuadratic(A, B, C, tmp0, tmp1))
    {
        return false;
    }
    Vector3f p0 = e + tmp0 * d;
    Vector3f p1 = e + tmp1 * d;
    if (between2plane(p0, c1, c2))
    {
        // TODO
    }
}