#include "Scene.h"
#include "Utils.h"

Vector3f Scene::getColorByED(const Vector3f &e, const Vector3f &d)
{
    std::shared_ptr<TQueue> q = make_shared<TQueue>();
    // no intersection
    if (!getIntersections(objects, e, d, q))
    {
        return Vector3f();
    }

    const TRecord &rec = sortAndGetMinK(q);

    Vector3f color = shader->getColor(lights, objects, e, d, rec);

    if (rec.target->specular_reflection)
    {
        std::shared_ptr<TQueue> spec_q = make_shared<TQueue>();
        Vector3f d = (rec.inter_point - e).normalize();
        Vector3f n = rec.normal;
        Vector3f r = d - 2 * d.dot(n) * n;
        Vector3f e = rec.inter_point + r * eps;
        color = (1 - rec.target->km) * color;
        if (getIntersections(objects, e, r, spec_q))
        {
            const TRecord &spec_rec = sortAndGetMinK(spec_q);
            color += rec.target->km * shader->getColor(lights, objects, e, r, spec_rec);
        }
    }

    return color;
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
            ca->setColor(i, j, color);
        }
    }
    ca->setOfPixels(*pixels);
}