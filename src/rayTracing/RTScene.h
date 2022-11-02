#pragma once

#include <vector>

#include "BaseScene.h"
#include "Camera.h"
#include "GeoObject.h"
#include "Shader.h"
#include "ofMain.h"

class RTScene: public BaseScene
{
private:
    Vector3f getColorByED(const Ray &viewRay);

public:

    std::vector<std::shared_ptr<GeoObject>> objects;
    std::unique_ptr<Shader> shader;

    void render();
    void select(int x, int y);
};