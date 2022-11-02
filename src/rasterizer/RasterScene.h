#pragma once

#include <vector>

#include "BaseScene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "ofMain.h"

class RasterScene : public BaseScene
{
private:
    Matrix4f perMat;
    Matrix4f getTransformationMat();
    Matrix4f getModelMat();

public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    void render();
};