#include "Camera.h"

Camera::Camera(Vector3f position, Vector3f viewDirection, float focalLength, int width, int height,
               float r, float t, int Msaafactor)
    : position(position), focalLength(focalLength), positionBack(position),
      focalLengthBack(focalLength), size(width * height * Msaafactor * Msaafactor),
      pixelBuffer(std::make_unique<Vector3f[]>(size)),
      normalBuffer(std::make_unique<Vector3f[]>(size)),
      positionBuffer(std::make_unique<Vector3f[]>(size)),
      textureBuffer(std::make_unique<Vector3f[]>(size)), zBuffer(std::make_unique<float[]>(size)),
      width(width * Msaafactor), height(height * Msaafactor), r(r), t(t), Msaafactor(Msaafactor)
{
    w = -viewDirection.normalize();
    Vector3f up(0, 1, 0);
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

void Camera::setOfPixels(ofPixels &pixels) const
{
    for (int i = 0; i < width / Msaafactor; i++)
    {
        for (int j = 0; j < height / Msaafactor; j++)
        {
            pixels.setColor(i, height / Msaafactor - j - 1, getOfColor(getMsaaAverageColor(i, j)));
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

void Camera::orbit(float angle, float centerDis, int direction)
{
    Vector3f center = position + centerDis * (-w);
    Matrix4f rotationMat;
    switch (direction)
    {
    case 2:
        angle = -angle;
    case 0:
        rotationMat = Matrix4f::getRotationMat(angle, center, 1);
        break;
    case 1:
        angle = -angle;
    case 3:
    {
        float theta = std::atan(w.x / w.z);
        theta = Vector3f(0, 0, 1).dot(w) > 0 ? theta : pi + theta;
        rotationMat = Matrix4f::getRotationMat(theta, center, 1) *
                      Matrix4f::getRotationMat(angle, center, 0) *
                      Matrix4f::getRotationMat(-theta, center, 1);
        break;
    }
    }
    w = (rotationMat * w.toDirection4f()).getVector3f();
    u = (rotationMat * u.toDirection4f()).getVector3f();
    v = (rotationMat * v.toDirection4f()).getVector3f();
    position = (rotationMat * position.toPoint4f()).getVector3f();
}

void Camera::pan(float distance, int direction) { position += distance * getDirection(direction); }
void Camera::dolly(float distance) { position += distance * (-w); }

Matrix4f Camera::getViewportMat()
{
    float nx = width;
    float ny = height;

    auto rnt = Matrix4f();
    rnt.row[0].x = nx / 2;
    rnt.row[0].w = (nx - 1) / 2;
    rnt.row[1].y = ny / 2;
    rnt.row[1].w = (ny - 1) / 2;
    return rnt;
}

Matrix4f Camera::getProjectionMat()
{
    float l = -r;
    float b = -t;
    float n = -focalLength;
    float f = n * 5;

    auto orthMat = Matrix4f();
    auto P = Matrix4f();

    orthMat.row[0].x = 2 / (r - l);
    orthMat.row[0].w = -(r + l) / (r - l);
    orthMat.row[1].y = 2 / (t - b);
    orthMat.row[1].w = -(t + b) / (t - b);
    orthMat.row[2].z = 2 / (n - f);
    orthMat.row[2].w = -(n + f) / (n - f);

    if (perspective)
    {
        P.row[0].x = n;
        P.row[1].y = n;
        P.row[2].z = n + f;
        P.row[2].w = -f * n;
        P.row[3].z = 1;
        P.row[3].w = 0;
    }
    return orthMat * P;
}

Matrix4f Camera::getCameraMat()
{
    auto rotMat =
        Matrix4f(u.toDirection4f(), v.toDirection4f(), w.toDirection4f(), Vector4f(0, 0, 0, 1));
    auto transMat = Matrix4f();
    transMat.row[0].w = -position.x;
    transMat.row[1].w = -position.y;
    transMat.row[2].w = -position.z;
    return rotMat * transMat;
}