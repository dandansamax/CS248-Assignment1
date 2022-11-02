#pragma once

#include <memory>
#include <queue>

#include "Ray.h"
#include "Vector.h"

class BaseObject
{
public:
    Matrix4f transformMat, inverseMat;
    Vector3f center, originCenter;

    BaseObject(const Vector3f &center) : center(center), originCenter(center) {}

    // Rotate along 0:x/1:y/2:z axis (in radian measure) (right hand)
    void rotate(float angle, int axis)
    {
        transformMat = Matrix4f::getRotationMat(angle, center, axis) * transformMat;
        inverseMat = inverseMat * Matrix4f::getRotationMat(-angle, center, axis);
    }

    // Tranlate in 3 dimension
    void translate(const Vector3f &move)
    {
        center += move;
        transformMat = Matrix4f::getTranslationMat(move) * transformMat;
        inverseMat = inverseMat * Matrix4f::getTranslationMat(-move);
    }

    void scale(float factor)
    {
        transformMat = Matrix4f::getScaleMat(factor, center) * transformMat;
        inverseMat = inverseMat * Matrix4f::getScaleMat(1 / factor, center);
    }

    void reset()
    {
        center = originCenter;
        transformMat = Matrix4f();
        inverseMat = Matrix4f();
    }
};