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
    std::vector<GeoObject> objects;
    std::vector<Light> lights;
    Camera &ca;
    ofPixels &pixels;
    Shader &shader;

    Scene(ofPixels &pixels, Camera &ca, Shader &shader) : ca(ca), pixels(pixels), shader(shader) {}

    void render();
};