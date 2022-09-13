#pragma once

#include "Vector.h"
#include "utility"

class Camera
{
private:
    Vector3f position;
    Vector3f u, v, w;
    float focal_length = 1.0f;

public:
    bool perspective = false;
    int width, height;
    float r, t;
    Camera(Vector3f position, Vector3f viewDirection, int width, int height, float r, float t);

    void setPerspective(float focal_length)
    {
        perspective = true;
        this->focal_length = focal_length;
        position += focal_length * w;
    }
    void setParallel()
    {
        perspective = false;
        position -= focal_length * w;
    }

    std::pair<Vector3f, Vector3f> getViewRay(int i, int j);
    void movePosition(Vector3f duration);
};
