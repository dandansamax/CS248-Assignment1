#pragma once

#include "Vector.h"

class Light
{
public:
    Vector3f position;
    Vector3f color;

    Light(Vector3f position, Vector3f color): position(position), color(color) {}
};