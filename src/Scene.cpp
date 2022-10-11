#include "Scene.h"
#include "Utils.h"

Vector3f Scene::getColorByED(const Ray &viewRay)
{
    std::shared_ptr<TQueue> q = make_shared<TQueue>();
    // no intersection
    if (!getAllObjIntersections(objects, viewRay, q))
    {
        return Vector3f();
    }

    const TRecord &rec = sortAndGetMinK(q);

    Vector3f color = shader->getColor(lights, objects, viewRay, rec);
    if (selectedObj == rec.target)
    {
        color = color * 0.7 + Vector3f(0.3f, 0.3f, 0.3f);
    }

    if (rec.target->specular_reflection)
    {
        std::shared_ptr<TQueue> spec_q = make_shared<TQueue>();
        Vector3f d = (rec.inter_point - viewRay.getE3f()).normalize();
        Vector3f n = rec.normal;
        Vector3f r = d - 2 * d.dot(n) * n;
        Vector3f e = rec.inter_point + r * eps;
        color = (1 - rec.target->km) * color;
        if (getAllObjIntersections(objects, Ray(e, r), spec_q))
        {
            const TRecord &spec_rec = sortAndGetMinK(spec_q);
            color += rec.target->km * shader->getColor(lights, objects, Ray(e, r), spec_rec);
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
            Ray viewRay = ca->getViewRay(i, j);
            Vector3f color = getColorByED(viewRay);
            ca->setColor(i, j, color);
        }
    }
    ca->setOfPixels(*pixels);
}

void Scene::select(int x, int y)
{
    auto viewRay = ca->getViewRay(x, y);
    std::shared_ptr<TQueue> q = make_shared<TQueue>();
    if (!getAllObjIntersections(objects, viewRay, q))
    {
        selectedObj = nullptr;
        return;
    }

    const TRecord &rec = sortAndGetMinK(q);
    selectedObj = rec.target;
    return;
}

// 0: Left, 1: Up, 2: Right, 3: Down
void Scene::objMoveInCaSpace(float distance, int direction)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->translate(ca->getDirection(direction) * distance);
    return;
}

void Scene::objScale(float factor)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->scale(factor);
    return;
}

void Scene::objReset()
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->reset();
    return;
}

void Scene::objRotate(float angle, int axis)
{
    if (!selectedObj)
    {
        return;
    }
    selectedObj->rotate(angle, axis);
    return;
}