#pragma once

#include <vector>

#include "BaseScene.h"
#include "BaseShader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ofMain.h"

class RasterScene : public BaseScene
{
private:
    Matrix4f perMat;
    Matrix4f getTransformationMat();

    Matrix4f current_rotation;
    Vector3f dragSta;

public:
    RasterScene() { current_rotation = Matrix4f(); }
    bool gouraud = false;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::unique_ptr<BaseShader> shader;

    Vector3f getArcballPos(int x, int y);
    void drag(int x, int y);
    void click(int x, int y);
    void release(int x, int y);

    void render();
    void getPosColor(Triangle &t, Triangle &originT, int x, int y);
    void pixelShade();
};