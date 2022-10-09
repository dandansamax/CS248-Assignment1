#include "Camera.h"

Camera::Camera(Vector3f position, Vector3f viewDirection, int width, int height, float r, float t, int MSAA_factor)
    : pixelBuffer(std::make_unique<Vector3f[]>(width * height * MSAA_factor * MSAA_factor)), width(width * MSAA_factor),
      height(height * MSAA_factor), r(r), t(t), MSAA_factor(MSAA_factor)
{
    this->position = position;

    w = -viewDirection.normalize();
    Vector3f up(0, -1, 0);
    u = up.cross(w).normalize();
    v = w.cross(u);
}

// normalization not guaranteed
Ray Camera::getViewRay(int i, int j)
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
    return Ray(e, d);
}

void Camera::movePosition(Vector3f duration) { position += duration; }

void Camera::setOfPixels(ofPixels &pixels) const
{
    for (int i = 0; i < width / MSAA_factor; i++)
    {
        for (int j = 0; j < height / MSAA_factor; j++)
        {
            pixels.setColor(i, j, getOfColor(getMsaaAverageColor(i, j)));
        }
    }
}

Vector3f Camera::getMsaaAverageColor(int x, int y) const
{
    Vector3f sum;
    x *= MSAA_factor;
    y *= MSAA_factor;
    for (int i = x; i < x + MSAA_factor; i++)
    {
        for (int j = y; j < y + MSAA_factor; j++)
        {
            sum += pixelBuffer[i * height + j];
        }
    }
    return sum / (MSAA_factor * MSAA_factor);
}