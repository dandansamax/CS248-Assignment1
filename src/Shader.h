#include "GeoObject.h"
#include "Light.h"

class Shader
{
public:
    virtual Vector3f getColor(const std::vector<Light> &lights, const GeoObject &target, Vector3f e, Vector3f d,
                              float t) = 0;
};

class PhongShader : public Shader
{
private:
    float ka = 0.2, kd = 0.6, ks = 0.2;

public:
    PhongShader(float ka, float kd, float ks) : ka(ka), kd(kd), ks(ks) {}
    Vector3f getColor(const std::vector<Light> &lights, const GeoObject &target, Vector3f e, Vector3f d,
                      float t);
};

class LambertianShader : public Shader
{
private:
    float ka = 0.2, kd = 0.8;

public:
    LambertianShader(float ka, float kd) : ka(ka), kd(kd) {}
    Vector3f getColor(const std::vector<Light> &lights, const GeoObject &target, Vector3f e, Vector3f d,
                      float t);
};