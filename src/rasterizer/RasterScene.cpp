#include "RasterScene.h"

void RasterScene::render()
{
    ca->initBuffer();
    perMat = getTransformationMat();
    for (auto &mesh : meshes)
    {
        if (gouraud)
        {
            mesh->calGouraudColor(ca, lights, shader);
        }

        int size = mesh->getTriangleNum();
        for (int i = 0; i < size; i++)
        {
            Triangle originT = mesh->getIthTriangle(i, gouraud);
            Triangle t = perMat * originT;
            auto bounding = t.getBoundingBox();
            bounding.x = std::max((int)bounding.x, 0);
            bounding.y = std::min((int)bounding.y, ca->width - 1);
            bounding.z = std::max((int)bounding.z, 0);
            bounding.w = std::min((int)bounding.w, ca->height - 1);
            for (int x = bounding.x; x <= bounding.y; x++)
                for (int y = bounding.z; y <= bounding.w; y++)
                {
                    getPosColor(t, originT, x, y);
                }
        }
    }
    if (!gouraud)
    {
        pixelShade();
    }
    ca->setOfPixels(*pixels);
}

inline void RasterScene::getPosColor(Triangle &t, Triangle &originT, int x, int y)
{
    Vector3f p = Vector3f(x, y, 0);
    auto bary = t.getBaryCoor(p);
    if (bary.x > 0 && bary.y > 0 && bary.z > 0)
    {
        float depth = t.getDepth(bary);
        if (depth > ca->getZBuffer(x, y) && depth > -1 && depth < 1)
        {
            ca->setZBuffer(x, y, depth);
            if (gouraud)
            {
                ca->setColor(x, y, t.getGouraudColor(bary));
            }
            else
            {
                ca->setGBuffer(x, y, t.getNormal(bary), originT.getPosition(bary),
                               meshes[0]->getTextImageColor(t.getTextureColor(bary)));
            }
        }
    }
}

void RasterScene::pixelShade()
{
    for (int i = 0; i < ca->width; i++)
        for (int j = 0; j < ca->height; j++)
        {
            if (ca->getZBuffer(i, j) != -1.0f)
            {
                auto normal = ca->getNormal(i, j);
                auto interPos = ca->getPosition(i, j);
                auto color = ca->getTexture(i, j);
                auto rnt = shader->getPointColor(ca->position, interPos, normal, color, lights);
                ca->setColor(i, j, rnt);
            }
        }
}

Matrix4f RasterScene::getTransformationMat()
{
    auto VPMat = ca->getViewportMat();
    auto ProjMat = ca->getProjectionMat();
    auto CamMat = ca->getCameraMat();
    return VPMat * (ProjMat * CamMat);
}

inline Vector3f RasterScene::getArcballPos(int x, int y)
{
    float Ax = (float)x / ca->width * 2 - 1.0f;
    float Ay = (float)y / ca->height * 2 - 1.0f;
    if (Ax * Ax + Ay * Ay > 1.0f)
    {
        return Vector3f(Ax, Ay, 0.0f).normalize();
    }
    return Vector3f(Ax, Ay, std::sqrt(1 - Ax * Ax - Ay * Ay)).normalize();
}

void RasterScene::drag(int x, int y)
{
    Vector3f cur = getArcballPos(x, y);
    meshes[0]->curRotation = Matrix4f::getAxisAngleRotation(meshes[0]->dragSta, cur);
    meshes[0]->curInvert = Matrix4f::getAxisAngleRotation(cur, meshes[0]->dragSta);
}
void RasterScene::click(int x, int y) { meshes[0]->dragSta = getArcballPos(x, y); }
void RasterScene::release(int x, int y)
{
    meshes[0]->transformMat = meshes[0]->curRotation * meshes[0]->transformMat;
    meshes[0]->inverseMat = meshes[0]->inverseMat * meshes[0]->curInvert;
    meshes[0]->curRotation = Matrix4f();
    meshes[0]->curInvert = Matrix4f();
}