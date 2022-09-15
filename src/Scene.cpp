#include "Scene.h"

Vector3f Scene::getColorByED(Vector3f e, Vector3f d)
{
    std::shared_ptr<TQueue> q = make_shared<TQueue>();
    for (auto obj : objects)
    {
        obj->getIntersection(e, d, q);
    }

    // no intersection
    if (q->empty())
    {
        return Vector3f();
    }

    std::sort(q->begin(), q->end());

    return shader->getColor(lights, objects, e, d, q->back());
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