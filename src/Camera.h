#pragma once

#include "Vector.h"
#include "utility"

class Camera
{
private:
    Vector3f position;
    Vector3f u, v, w;
    float focal_length = -1.0;

public:
    bool perspective = false;
    int width, height;
    float r, t;
    Camera(Vector3f position, Vector3f viewDirection, int width, int height, float r, float t);

    void setPerspective(float focal_length)
    {
        perspective = true;
        this->focal_length = focal_length;
    }
    void setParallel() { perspective = false; }

    std::pair<Vector3f, Vector3f> getViewRay(int i, int j);
};
