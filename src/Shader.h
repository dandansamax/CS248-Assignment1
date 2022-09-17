#pragma once

#include "GeoObject.h"
#include "Light.h"

class Shader
{
public:
    virtual Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                              const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e, const Vector3f &d,
                              const TRecord &record) = 0;
};

class NaiveShader : public Shader
{
public:
    NaiveShader() {}
    Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e, const Vector3f &d,
                      const TRecord &record)
    {
        return record.target->color;
    }
};

class NormShader : public Shader
{
public:
    NormShader() {}
    Vector3f getColor(const std::vector<std::shared_ptr<Light>> &lights,
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e, const Vector3f &d,
                      const TRecord &record)
    {
        return (record.normal + Vector3f(1, 1, 1)) * 0.5;
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
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e, const Vector3f &d,
                      const TRecord &record);
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
                      const std::vector<std::shared_ptr<GeoObject>> &objects, const Vector3f &e, const Vector3f &d,
                      const TRecord &record);
};