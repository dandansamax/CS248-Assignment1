#include "Shader.h"

Vector3f LambertianShader::getColor(const std::vector<std::shared_ptr<Light>> &lights,
                                    const std::shared_ptr<GeoObject> target, Vector3f e, Vector3f d, float t)
{
    Vector3f p = e + t * d;
    Vector3f rnt = Vector3f();
    Vector3f normal = target->getNormal(p);

    for (auto light : lights)
    {
        Vector3f l = (light->position - p).normalize();
        rnt += kd * target->color * light->color * std::max(0.0f, normal.dot(l));
    }
    rnt += ka * ia * target->color;

    return rnt.truncate();
}

Vector3f PhongShader::getColor(const std::vector<std::shared_ptr<Light>> &lights,
                               const std::shared_ptr<GeoObject> target, Vector3f e, Vector3f d, float t)
{
    Vector3f p = e + t * d;
    Vector3f rnt = Vector3f();
    Vector3f normal = target->getNormal(p);
    Vector3f v = -d.normalize();

    for (auto light : lights)
    {
        Vector3f l = (light->position - p).normalize();
        Vector3f h = (v + l).normalize();
        rnt += kd * target->color * light->color * std::max(0.0f, normal.dot(l));
        rnt += ks * target->color * light->color * std::pow(std::max(0.0f, normal.dot(h)), phong_exponent);
    }
    rnt += ka * ia * target->color;

    return rnt.truncate();
}