#pragma once

#include <vector>

#include "Camera.h"
#include "GeoObject.h"
#include "Shader.h"
#include "ofMain.h"

class Scene
{
private:
    Vector3f getColorByED(Vector3f e, Vector3f d);

public:
    ofPixels *pixels;

    std::vector<std::shared_ptr<GeoObject>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    std::unique_ptr<Camera> ca;
    std::unique_ptr<Shader> shader;

    void render();
};