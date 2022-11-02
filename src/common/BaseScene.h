#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "BaseObject.h"
#include "Light.h"
#include "Camera.h"
#include "ofPixels.h"

class BaseScene
{
public:
    std::vector<std::shared_ptr<Light>> lights;
    std::unique_ptr<Camera> ca;

    ofPixels *pixels;
    BaseObject *selectedObj;
    int selectLight = 0;

    // 0: Left, 1: Up, 2: Right, 3: Down
    void objMoveInCaSpace(float distance, int direction);
    void objScale(float factor);
    void objRotate(float angle, int axis);
    void objReset();

    void switchLight();
    void moveLight(float distance, int direction);

    virtual void render() = 0;
};