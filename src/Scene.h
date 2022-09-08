#pragma once

#include <vector>

#include "GeoObject.h"
#include "Camera.h"
#include "ofMain.h"

class Scene
{
private:
    Vector3f getColor(Vector3f e, Vector3f d);

public:
    std::vector<GeoObject> objects;
    Camera ca;

    Scene(Camera ca) : ca(ca) {}

    void render(ofPixels &color);
};