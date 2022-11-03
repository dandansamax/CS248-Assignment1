#pragma once

#include "Camera.h"

class PixelShader
{
public:
    virtual void shade(std::unique_ptr<Camera> &ca,
                       const std::vector<std::shared_ptr<Light>> &lights) = 0;
};

class NormalShader : public PixelShader
{
    void shade(std::unique_ptr<Camera> &ca, const std::vector<std::shared_ptr<Light>> &lights)
    {
        for (int i = 0; i < ca->width; i++)
            for (int j = 0; j < ca->height; j++)
            {
                if (ca->getZBuffer(i, j) != -1.0f)
                {
                    auto color = ca->getNormal(i, j);
                    color = (color + Vector3f(1, 1, 1)) / 2;
                    ca->setColor(i, j, color);
                }
            }
    }
};

class PhongPixelShader : public PixelShader
{
private:
    float ka = 0.2, kd = 0.6, ks = 0.2;
    float phong_exponent = 32;
    Vector3f ia = Vector3f(1, 1, 1);

public:
    void shade(std::unique_ptr<Camera> &ca, const std::vector<std::shared_ptr<Light>> &lights)
    {
        for (int i = 0; i < ca->width; i++)
            for (int j = 0; j < ca->height; j++)
            {
                if (ca->getZBuffer(i, j) != -1.0f)
                {
                    auto normal = ca->getNormal(i, j);
                    auto p = ca->getPosition(i, j);
                    auto color = ca->getTexture(i, j);
                    auto v = (ca->position - ca->getPosition(i, j)).normalize();
                    Vector3f rnt = Vector3f();

                    for (auto light : lights)
                    {
                        Vector3f l = (light->position - p).normalize();
                        Vector3f h = (v + l).normalize();
                        rnt += kd * color * light->color * std::max(0.0f, normal.dot(l));
                        rnt += ks * color * light->color *
                               std::pow(std::max(0.0f, normal.dot(h)), phong_exponent);
                    }
                    rnt += ka * ia * color;
                    ca->setColor(i, j, rnt);
                }
            }
    }
};