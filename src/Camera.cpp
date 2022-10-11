#include "Camera.h"

Camera::Camera(Vector3f position, Vector3f viewDirection, float focalLength, int width, int height,
               float r, float t, int Msaafactor)
    : position(position), focalLength(focalLength), positionBack(position),
      focalLengthBack(focalLength),
      pixelBuffer(std::make_unique<Vector3f[]>(width * height * Msaafactor * Msaafactor)),
      width(width * Msaafactor), height(height * Msaafactor), r(r), t(t), Msaafactor(Msaafactor)
{
    w = -viewDirection.normalize();
    Vector3f up(0, -1, 0);
    u = up.cross(w).normalize();
    v = w.cross(u);

    uBack = u, vBack = v, wBack = w;
}

// normalization not guaranteed
Ray Camera::getViewRay(int i, int j)
{
    Vector3f e, d;
    float u_scalar = -r + 2 * r * ((float)i + 0.5) / width;
    float v_scalar = -t + 2 * t * ((float)j + 0.5) / height;
    if (perspective)
    {
        d = -focalLength * w + u_scalar * u + v_scalar * v;
        e = position;
    }
    else
    {
        d = -w;
        e = -focalLength * w + position + u_scalar * u + v_scalar * v;
    }
    return Ray(e, d);
}

void Camera::movePosition(Vector3f duration) { position += duration; }

void Camera::setOfPixels(ofPixels &pixels) const
{
    for (int i = 0; i < width / Msaafactor; i++)
    {
        for (int j = 0; j < height / Msaafactor; j++)
        {
            pixels.setColor(i, j, getOfColor(getMsaaAverageColor(i, j)));
        }
    }
}

Vector3f Camera::getMsaaAverageColor(int x, int y) const
{
    Vector3f sum;
    x *= Msaafactor;
    y *= Msaafactor;
    for (int i = x; i < x + Msaafactor; i++)
    {
        for (int j = y; j < y + Msaafactor; j++)
        {
            sum += pixelBuffer[i * height + j];
        }
    }
    return sum / (Msaafactor * Msaafactor);
}

// 0: Left, 1: Up, 2: Right, 3: Down
Vector3f Camera::getDirection(int direction)
{
    switch (direction)
    {
    case 0:
        return -u;
        break;
    case 1:
        return -v;
        break;
    case 2:
        return u;
        break;
    case 3:
        return v;
        break;
    }
    return Vector3f();
}

void Camera::zoom(float duration) { focalLength += focalLength + duration > eps ? duration : 0; }