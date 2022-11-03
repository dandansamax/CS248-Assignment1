#include "RasterScene.h"

void RasterScene::render()
{
    ca->initBuffer();
    perMat = getTransformationMat();
    for (auto mesh : meshes)
    {
        auto objPerMat = perMat * mesh->transformMat;
        int size = mesh->getTriangleNum();
        for (int i = 0; i < size; i++)
        {
            Triangle originT = mesh->getIthTriangle(i);
            Triangle t = objPerMat * originT;
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
    shader->shade(ca, lights);
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
            ca->setGBuffer(x, y, t.getNormal(bary), originT.getPosition(bary),
                           Vector3f(0.7f, 0.7f, 0.7f));
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