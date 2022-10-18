#pragma once

#include "Vector.h"

class Light
{
public:
    Vector3f position;
    Vector3f color;

    Light(Vector3f position, Vector3f color) : position(position), color(color) {}

    void move(float distance, int direction)
    {
        Matrix4f moveMat = Matrix4f();
        switch (direction)
        {
        case 0:
            moveMat = Matrix4f::getTranslationMat(Vector3f(-distance, 0, 0));
            break;
        case 1:
            moveMat = Matrix4f::getTranslationMat(Vector3f(0, distance, 0));
            break;
        case 2:
            moveMat = Matrix4f::getTranslationMat(Vector3f(distance, 0, 0));
            break;
        case 3:
            moveMat = Matrix4f::getTranslationMat(Vector3f(0, -distance, 0));
            break;
        case 4:
            moveMat = Matrix4f::getTranslationMat(Vector3f(0, 0, distance));
            break;
        case 5:
            moveMat = Matrix4f::getTranslationMat(Vector3f(0, 0, -distance));
            break;
        }
        position = (moveMat * position.toPoint4f()).getVector3f();
    }
};