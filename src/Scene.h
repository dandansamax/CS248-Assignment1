#pragma once

#include <vector>

#include "GeoObject.h"
#include "Camera.h"
#include "ofMain.h"

class Scene
{
public:
    std::vector<GeoObject> Objects;
    Camera ca;

    Scene(Camera ca) : ca(ca) {}

    void render(ofPixels &color);
};