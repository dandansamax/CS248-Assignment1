#pragma once

#include <cmath>
#include <utility>

#include "Utils.h"
#include "Vector.h"
#include "ofPixels.h"

class Camera
{
private:
    Vector3f position;
    Vector3f u, v, w;
    float focalLength = 1.0f;

    Vector3f positionBack, uBack, vBack, wBack;
    float focalLengthBack = 1.0f;

    int size;
    std::unique_ptr<Vector3f[]> pixelBuffer;
    std::unique_ptr<Vector3f[]> normalBuffer;
    std::unique_ptr<Vector3f[]> positionBuffer;
    std::unique_ptr<Vector3f[]> colorBuffer;
    std::unique_ptr<float[]> zBuffer;

public:
    bool perspective = false;
    int width, height;
    float r, t;
    int Msaafactor = 1;
    Camera(Vector3f position, Vector3f viewDirection, float focalLength, int width, int height,
           float r, float t, int Msaafactor);

    void setPerspective() { perspective = true; }
    void setParallel() { perspective = false; }
    void setColor(int i, int j, const Vector3f &color) { pixelBuffer[height * i + j] = color; }

    void setOfPixels(ofPixels &pixels) const;
    Vector3f getMsaaAverageColor(int x, int y) const;

    Ray getViewRay(int i, int j);
    // 0: Left, 1: Up, 2: Right, 3: Down
    Vector3f getDirection(int direction);

    void pan(float distance, int direction);
    void dolly(float distance);
    void zoom(float duration);
    // 0: Left, 1: Up, 2: Right, 3: Down
    void orbit(float angle, float centerDis, int direction);

    void reset()
    {
        position = positionBack;
        u = uBack, v = vBack, w = wBack;
        focalLength = focalLengthBack;
    }

    Matrix4f getViewportMat();
    Matrix4f getProjectionMat();
    Matrix4f getCameraMat();

    void initBuffer()
    {
        for (int i = 0; i < size; i++)
        {
            pixelBuffer[i] = Vector3f();
            normalBuffer[i] = Vector3f();
            positionBuffer[i] = Vector3f();
            colorBuffer[i] = Vector3f();
            zBuffer[i] = -1;
        }
    }
    void setZBuffer(int i, int j, float depth) { zBuffer[height * i + j] = depth; }
    float getZBuffer(int i, int j) { return zBuffer[height * i + j]; }

    void setGBuffer(int i, int j, const Vector3f &normal, const Vector3f &position,
                    const Vector3f &color)
    {
        normalBuffer[height * i + j] = normal;
        positionBuffer[height * i + j] = position;
        colorBuffer[height * i + j] = color;
    }
    Vector3f getNormal(int i, int j) { return normalBuffer[height * i + j]; }
};
