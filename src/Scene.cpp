#include "Scene.h"

Vector3f Scene::getColor(Vector3f e, Vector3f d)
{
    for (GeoObject &obj : objects)
    {
        float t0, t1;
        Record rec;
        obj.getIntersection(e, d, t0, t1, rec);
        // TODO
    }
    return Vector3f(0,0,0);
}

void Scene::render(ofPixels &pixels)
{
    for (int i = 0; i < ca.nx; i++)
    {
        for (int j = 0; j < ca.ny; j++)
        {

            auto [e, d] = ca.getViewRay(i, j);
            Vector3f color = getColor(e, d);
            pixels.setColor(i, j, color.getOfcolor());
        }
    }
    return;
}