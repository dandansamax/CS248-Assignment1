#include <cmath>
#include <memory>
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

Matrix4f GeoObject::getInverseRotationMat(float angle, int axis)
{
    Matrix4f a = getInverseTranslationMat(center);
    Matrix4f rotation = Matrix4f();
    Matrix4f aIn = getInverseTranslationMat(-center);

    switch (axis)
    {
    // X-axis
    case 0:
        rotation.row[1].y = std::cos(-angle);
        rotation.row[1].z = -std::sin(-angle);
        rotation.row[2].y = std::sin(-angle);
        rotation.row[2].z = std::cos(-angle);
        break;
    // Y-axis
    case 1:
        rotation.row[2].z = std::cos(-angle);
        rotation.row[2].x = -std::sin(-angle);
        rotation.row[0].z = std::sin(-angle);
        rotation.row[0].x = std::cos(-angle);
        break;
    // Z-axis
    case 2:
        rotation.row[0].x = std::cos(-angle);
        rotation.row[0].y = -std::sin(-angle);
        rotation.row[1].x = std::sin(-angle);
        rotation.row[1].y = std::cos(-angle);
        break;
    default:
        return Matrix4f();
    }
    return aIn * rotation * a;
}

Matrix4f GeoObject::getInverseTranslationMat(const Vector3f &move)
{
    Matrix4f rnt = Matrix4f();
    rnt.row[0].w = -move.x;
    rnt.row[1].w = -move.y;
    rnt.row[2].w = -move.z;
    return rnt;
}

Matrix4f GeoObject::getInverseScaleMat(float factor)
{
    Matrix4f a = getInverseTranslationMat(center);
    Matrix4f scale = Matrix4f();
    Matrix4f aIn = getInverseTranslationMat(-center);
    scale.row[0].x = 1.0 / factor;
    scale.row[1].y = 1.0 / factor;
    scale.row[2].z = 1.0 / factor;

    return aIn * scale * a;
}

bool Sphere::getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const
{
    auto e = viewRay.getE3f();
    auto d = viewRay.getD3f();
    float A = d.dot(d);
    float B = 2 * d.dot(e - center);
    float C = (e - center).dot(e - center) - r * r;
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
        pushQueue(q, i, point, getNormal(e, point), (GeoObject *)this);
    }
    return flag;
}

bool Elipsoid::getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const
{
    auto e = viewRay.getE3f();
    auto d = viewRay.getD3f();
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
        pushQueue(q, i, point, getNormal(e, point), (GeoObject *)this);
    }
    return flag;
}

bool Plane::getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const
{
    auto e = viewRay.getE3f();
    auto d = viewRay.getD3f();
    float tmp = abc.dot(d);
    if (tmp == 0.0f)
    {
        return false;
    }

    float t = -(abc.dot(e) + cons) / tmp;
    if (t > 0)
    {
        Vector3f point = e + d * t;
        pushQueue(q, t, point, getNormal(e, point), (GeoObject *)this);
        return true;
    }
    else
    {
        return false;
    }
}

bool Circle::getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const
{
    auto e = viewRay.getE3f();
    auto d = viewRay.getD3f();
    auto tmp_q = std::make_shared<TQueue>();
    bool rnt = Plane::getIntersection(viewRay, tmp_q);
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

bool Cylinder::getIntersection(const Ray &viewRay, std::shared_ptr<TQueue> q) const
{
    auto e = viewRay.getE3f();
    auto d = viewRay.getD3f();
    auto tmp_q = std::make_shared<TQueue>();
    bool rnt1 = c1.getIntersection(viewRay, tmp_q);
    bool rnt2 = c2.getIntersection(viewRay, tmp_q);
    for (auto &rec : *tmp_q)
    {
        rec.target = (GeoObject *)this;
        pushQueue(q, rec);
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
        pushQueue(q, t0, p0, getNormal(e, p0), (GeoObject *)this);
        flag = true;
    }
    if (t1 > 0 && between2plane(p1))
    {
        pushQueue(q, t1, p1, getNormal(e, p1), (GeoObject *)this);
        flag = true;
    }
    return flag;
}