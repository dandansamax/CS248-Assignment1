#pragma once

#include <algorithm>

#include "ofColor.h"
#include "GeoObject.h"
#include "Vector.h"
#include "Ray.h"


static inline bool getAllObjIntersections(const std::vector<std::shared_ptr<GeoObject>> &objects, const Ray &viewRay,
                                    std::shared_ptr<TQueue> q)
{
    bool flag = false;
    for (auto obj : objects)
    {
        if (obj->getTransformIntersection(viewRay, q))
        {
            flag = true;
        }
    }
    return flag;
}

static inline const TRecord &sortAndGetMinK(std::shared_ptr<TQueue> q)
{
    std::sort(q->begin(), q->end());
    return q->front();
}

static inline ofColor getOfColor(const Vector3f &v) { return ofColor(v.x * 255, v.y * 255, v.z * 255); }