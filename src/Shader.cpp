#include "Shader.h"
#include "Utils.h"

static inline bool inShadow(const std::vector<std::shared_ptr<GeoObject>> &objects, std::shared_ptr<Light> light,
                            const Vector3f &point)
{
    Vector3f shadowRay = (light->position - point).normalize();
    return getIntersections(objects, point + eps * shadowRay, shadowRay, nullptr);
}

Vector3f LambertianShader::getColor(const std::vector<std::shared_ptr<Light>> &lights,
                                    const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e,
                                    const Vector3f &d, const TRecord &record)
{
    Vector3f p = record.inter_point;
    Vector3f rnt = Vector3f();
    Vector3f normal = record.normal;
    Vector3f color = record.target->color;

    for (auto light : lights)
    {
        if (inShadow(objects, light, p))
        {
            continue;
        }
        Vector3f l = (light->position - p).normalize();
        rnt += kd * color * light->color * std::max(0.0f, normal.dot(l));
    }
    rnt += ka * ia * color;

    return rnt.truncate();
}

Vector3f PhongShader::getColor(const std::vector<std::shared_ptr<Light>> &lights,
                               const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e,
                               const Vector3f &d, const TRecord &record)
{
    Vector3f p = record.inter_point;
    Vector3f rnt = Vector3f();
    Vector3f normal = record.normal;
    Vector3f color = record.target->color;
    Vector3f v = -d.normalize();

    for (auto light : lights)
    {
        if (inShadow(objects, light, p))
        {
            continue;
        }
        Vector3f l = (light->position - p).normalize();
        Vector3f h = (v + l).normalize();
        rnt += kd * color * light->color * std::max(0.0f, normal.dot(l));
        rnt += ks * color * light->color * std::pow(std::max(0.0f, normal.dot(h)), phong_exponent);
    }
    rnt += ka * ia * color;

    return rnt.truncate();
}