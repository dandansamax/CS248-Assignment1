#pragma once

#include <vector>

#include "BaseScene.h"
#include "Camera.h"
#include "Mesh.h"
#include "PixelShader.h"
#include "Shader.h"
#include "ofMain.h"

class RasterScene : public BaseScene
{
private:
    Matrix4f perMat;
    Matrix4f getTransformationMat();

public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::unique_ptr<PixelShader> shader;
    void render();
    void getPosColor(Triangle &t, Triangle &originT, int x, int y);
};