#pragma once

#include "GeoObject.h"
#include "Light.h"

class Shader
{
public:
    virtual Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                              const std::vector<std::shared_ptr<GeoObject>> &objects,
                              const std::shared_ptr<GeoObject> target, Vector3f e, Vector3f d, float t) = 0;
};

class NaiveShader : public Shader
{
public:
    NaiveShader() {}
    Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const std::shared_ptr<GeoObject> target,
                      Vector3f e, Vector3f d, float t)
    {
        // std::cout<<target
        return target->color;
    }
};

class PhongShader : public Shader
{
private:
    float ka = 0.2, kd = 0.6, ks = 0.2;
    float phong_exponent = 32;
    Vector3f ia = Vector3f(1, 1, 1);

public:
    PhongShader() {}
    PhongShader(float ka, float kd, float ks) : ka(ka), kd(kd), ks(ks) {}
    Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const std::shared_ptr<GeoObject> target,
                      Vector3f e, Vector3f d, float t);
};

class LambertianShader : public Shader
{
private:
    float ka = 0.2, kd = 0.8;
    Vector3f ia = Vector3f(1, 1, 1);

public:
    LambertianShader() {}
    LambertianShader(float ka, float kd) : ka(ka), kd(kd) {}
    Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const std::shared_ptr<GeoObject> target,
                      Vector3f e, Vector3f d, float t);
};