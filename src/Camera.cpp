#include "Camera.h"

Camera::Camera(Vector3f position, Vector3f viewDirection, int width, int height, float r, float t)
    : width(width), height(height), r(r), t(t)
{
    this->position = position;

    w = -viewDirection.normalize();
    Vector3f up(0, -1, 0);
    u = up.cross(w).normalize();
    v = w.cross(u);
}

// normalization not guaranteed
std::pair<Vector3f, Vector3f> Camera::getViewRay(int i, int j)
{
    Vector3f e, d;
    float u_scalar = -r + 2 * r * ((float)i + 0.5) / width;
    float v_scalar = -t + 2 * t * ((float)j + 0.5) / height;
    if (perspective)
    {
        d = -focal_length * w + u_scalar * u + v_scalar * v;
        e = position;
    }
    else
    {
        d = -w;
        e = position + u_scalar * u + v_scalar * v;
    }
    return std::make_pair(e, d);
}

void Camera::movePosition(Vector3f duration) { position += duration; }