#pragma once

#include "Light.h"
#include "Vector.h"
#include <memory>
#include <vector>

class BaseShader
{
public:
    virtual Vector3f getPointColor(const Vector3f &viewPos, const Vector3f &interPos,
                                   const Vector3f &normal, const Vector3f &color,
                                   const std::vector<std::shared_ptr<Light>> &lights) = 0;
};

class NormalShader : public BaseShader
{
public:
    Vector3f getPointColor(const Vector3f &viewPos, const Vector3f &interPos,
                           const Vector3f &normal, const Vector3f &color,
                           const std::vector<std::shared_ptr<Light>> &lights)
    {
        return normal;
    }
};

class PhongBaseShader : public BaseShader
{
private:
    float ka = 0.2, kd = 0.6, ks = 0.2;
    float phong_exponent = 32;
    Vector3f ia = Vector3f(1, 1, 1);

public:
    Vector3f getPointColor(const Vector3f &viewPos, const Vector3f &interPos,
                           const Vector3f &normal, const Vector3f &color,
                           const std::vector<std::shared_ptr<Light>> &lights)
    {
        auto v = (viewPos - interPos).normalize();
        Vector3f rnt = Vector3f();

        for (auto light : lights)
        {
            Vector3f l = (light->position - interPos).normalize();
            Vector3f h = (v + l).normalize();
            rnt += kd * color * light->color * std::max(0.0f, normal.dot(l));
            rnt +=
                ks * color * light->color * std::pow(std::max(0.0f, normal.dot(h)), phong_exponent);
        }
        rnt += ka * ia * color;
        return rnt;
    }
};