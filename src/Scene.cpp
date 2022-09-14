#include "Scene.h"

Vector3f Scene::getColorByED(Vector3f e, Vector3f d)
{
    std::shared_ptr<GeoObject> target = nullptr;
    float t = HUGE_VAL_F32;
    for (auto &obj : objects)
    {
        float t0, t1;
        Record rec;
        bool intersected = obj->getIntersection(e, d, t0, t1, rec);
        if (intersected)
        {
            float tmpT = (t0 > 0 ? t0 : t1);
            if (tmpT < t)
            {
                t = tmpT;
                target = obj;
            }
        }
    }

    // no intersection
    if (target == nullptr)
    {
        return Vector3f();
    }

    return shader->getColor(lights, objects, target, e, d, t);
}

void Scene::render()
{
    for (int i = 0; i < ca->width; i++)
    {
        for (int j = 0; j < ca->height; j++)
        {
            Vector3f e, d;
            std::tie(e, d) = ca->getViewRay(i, j);
            Vector3f color = getColorByED(e, d);
            pixels->setColor(i, j, color.getOfColor());
        }
    }
}