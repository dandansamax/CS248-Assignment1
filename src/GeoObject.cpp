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

bool Sphere::getIntersection(const Vector3f &e, const Vector3f &d, std::shared_ptr<TQueue> q) const
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
        push_queue(q, i, point, getNormal(e, point), (GeoObject *)this);
    }
    return flag;
}

bool Elipsoid::getIntersection(const Vector3f &e, const Vector3f &d, std::shared_ptr<TQueue> q) const
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
        push_queue(q, i, point, getNormal(e, point), (GeoObject *)this);
    }
    return flag;
}

bool Plane::getIntersection(const Vector3f &e, const Vector3f &d, std::shared_ptr<TQueue> q) const
{

    float tmp = abc.dot(d);
    if (tmp == 0.0f)
    {
        return false;
    }

    float t = -(abc.dot(e) + cons) / tmp;
    if (t > 0)
    {
        Vector3f point = e + d * t;
        push_queue(q, t, point, getNormal(e, point), (GeoObject *)this);
        return true;
    }
    else
    {
        return false;
    }
}

bool Circle::getIntersection(const Vector3f &e, const Vector3f &d, std::shared_ptr<TQueue> q) const
{

    auto tmp_q = std::make_shared<TQueue>();
    bool rnt = Plane::getIntersection(e, d, tmp_q);
    if (!rnt)
    {
        return false;
    }
    auto &rec = tmp_q->at(0);
    Vector3f l = e + d * rec.t - c;
    if (l.dot(l) < r * r)
    {
        rec.target = (GeoObject *)this;
        q->push_back(rec);
        return true;
    }
    else
    {
        return false;
    }
}

bool Cylinder::getIntersection(const Vector3f &e, const Vector3f &d, std::shared_ptr<TQueue> q) const
{
    auto tmp_q = std::make_shared<TQueue>();
    bool rnt1 = c1.getIntersection(e, d, tmp_q);
    bool rnt2 = c2.getIntersection(e, d, tmp_q);
    for (auto &rec : *tmp_q)
    {
        rec.target = (GeoObject *)this;
        push_queue(q, rec);
    }
    if (rnt1 && rnt2)
    {
        return true;
    }
    float dv = d.dot(direct);
    Vector3f ea = e - center;
    float eav = ea.dot(direct);
    float A = dv * dv - d.dot(d);
    float B = 2 * dv * eav - 2 * d.dot(ea);
    float C = eav * eav - ea.dot(ea) + r * r;

    bool flag = false;
    float t0, t1;
    if (!solveQuadratic(A, B, C, t0, t1))
    {
        return false;
    }
    Vector3f p0 = e + t0 * d;
    Vector3f p1 = e + t1 * d;
    if (t0 > 0 && between2plane(p0))
    {
        push_queue(q, t0, p0, getNormal(e, p0), (GeoObject *)this);
        flag = true;
    }
    if (t1 > 0 && between2plane(p1))
    {
        push_queue(q, t1, p1, getNormal(e, p1), (GeoObject *)this);
        flag = true;
    }
    return flag;
}