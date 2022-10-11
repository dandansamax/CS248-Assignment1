#pragma once

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

    std::unique_ptr<Vector3f[]> pixelBuffer;

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
    
    void movePosition(Vector3f duration);
    void zoom(float duration);

    void reset()
    {
        position = positionBack;
        u = uBack, v = vBack, w = wBack;
        focalLength = focalLengthBack;
    }
};
