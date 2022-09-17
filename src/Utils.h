#pragma once

#include <algorithm>

#include "GeoObject.h"
#include "Vector.h"

const float eps = 0.001f;

static inline bool getIntersections(const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e,
                                    const Vector3f &d, std::shared_ptr<TQueue> q)
{
    bool flag = false;
    for (auto obj : objects)
    {
        if (obj->getIntersection(e, d, q))
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