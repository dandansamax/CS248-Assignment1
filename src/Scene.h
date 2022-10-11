#pragma once

#include <vector>

#include "Camera.h"
#include "GeoObject.h"
#include "Shader.h"
#include "ofMain.h"

class Scene
{
private:
    Vector3f getColorByED(const Ray &viewRay);

public:
    ofPixels *pixels;

    std::vector<std::shared_ptr<GeoObject>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    std::unique_ptr<Camera> ca;
    std::unique_ptr<Shader> shader;

    GeoObject *selectedObj;

    void render();
    void select(int x, int y);

    // 0: Left, 1: Up, 2: Right, 3: Down
    void objMoveInCaSpace(float distance, int direction);
    void objScale(float factor);
    void objRotate(float angle, int axis);
    void objReset();
};