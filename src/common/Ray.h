#pragma once

#include "Vector.h"

struct Ray
{
    Vector4f e, d;
    Ray(const Vector4f &e, const Vector4f &d) : e(e), d(d){};
    Ray(const Vector3f &e, const Vector3f &d) : e(e.toPoint4f()), d(d.toDirection4f()){};
    Vector3f getE3f() const { return e.getVector3f(); }
    Vector3f getD3f() const { return d.getVector3f(); }
};