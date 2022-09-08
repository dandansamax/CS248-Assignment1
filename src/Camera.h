#pragma once

#include "Vector.h"
#include "utility"

class Camera
{
private:
    Vector3f position;
    Vector3f u, v, w;
    bool perspective = false;
    float d = -1.0;
    int nx, ny;
    float width, height;

public:
    Camera(Vector3f position, Vector3f viewDirection, float width, float height, int nx, int ny);

    void setPerspective(float d)
    {
        perspective = true;
        this->d = d;
    }
    void setParallel()
    {
        perspective = false;
    }

    std::pair<Vector3f, Vector3f> getViewRay(int i, int j);
};
