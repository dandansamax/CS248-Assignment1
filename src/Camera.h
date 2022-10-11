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
    float focal_length = 1.0f;

    std::unique_ptr<Vector3f[]> pixelBuffer;

public:
    bool perspective = false;
    int width, height;
    float r, t;
    int MSAA_factor = 1;
    Camera(Vector3f position, Vector3f viewDirection, int width, int height, float r, float t,
           int MSAA_factor);

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
    void setColor(int i, int j, const Vector3f &color) { pixelBuffer[height * i + j] = color; }

    void setOfPixels(ofPixels &pixels) const;
    Vector3f getMsaaAverageColor(int x, int y) const;

    Ray getViewRay(int i, int j);
    void movePosition(Vector3f duration);

    // 0: Left, 1: Up, 2: Right, 3: Down
    Vector3f getDirection(int direction);
};
