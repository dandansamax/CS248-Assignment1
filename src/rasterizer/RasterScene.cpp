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
            Triangle t = objPerMat * mesh->getIthTriangle(i);
            auto bounding = t.getBoundingBox();
            for (int x = std::max((int)bounding.x, 0); x <= std::min((int)bounding.y, ca->width - 1); x++)
                for (int y = std::max((int)bounding.z, 0); y <= std::min((int)bounding.w, ca->height - 1); y++)
                {
                    Vector3f p = Vector3f(x, y, 0);
                    auto bary = t.getBaryCoor(p);
                    if (bary.x > 0 && bary.y > 0 && bary.z > 0)
                    {
                        float depth = t.getDepth(p);
                        if (depth > ca->getZBuffer(x, y) && depth > -1 && depth < 1)
                        {
                            ca->setZBuffer(x, y, depth);
                            ca->setColor(x, y, Vector3f((depth + 1) / 2, 0.5f, 0.5f));
                        }
                    }
                }
        }
    }
    ca->setOfPixels(*pixels);
}

Matrix4f RasterScene::getTransformationMat()
{
    auto VPMat = ca->getViewportMat();
    auto ProjMat = ca->getProjectionMat();
    auto CamMat = ca->getCameraMat();
    return VPMat * (ProjMat * CamMat);
}