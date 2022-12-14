#pragma once

#include "BaseObject.h"
#include "BaseShader.h"
#include "Camera.h"
#include "Light.h"
#include "MarbleTexture.h"
#include "Triangle.h"
#include "ofImage.h"
#include <vector>

namespace tinyobj
{
    class attrib_t;
    class shape_t;
    class material_t;
    class index_t;
    bool LoadObj(attrib_t *attrib, std::vector<shape_t> *shapes, std::vector<material_t> *materials,
                 std::string *warn, std::string *err, const char *filename, const char *mtl_basedir,
                 bool triangulate, bool default_vcols_fallback);
}
class Mesh : public BaseObject
{
private:
    tinyobj::attrib_t *attrib;
    tinyobj::shape_t *shape;

    bool gouraud = false;
    std::vector<float> gouraudColor;

    Vector3f getTransformedPos(const Vector3f &v);
    Vector3f getTransformedNormal(const Vector3f &v);

public:
    ofImage textureImage;
    Matrix4f textureMat;
    MarbleTexture *marbleT;

    Matrix4f curRotation;
    Matrix4f curInvert;
    Vector3f dragSta;

    Mesh(const std::string &path, const Vector3f &center);
    ~Mesh();

    int getTriangleNum();

    Vector3f getIthVertex(int i);
    Vector3f getIthNormal(int i);
    void setIthNormal(int i, const Vector3f &normal);
    Vector3f getIthGouraudColor(int i);
    void setIthGouraudColor(int i, const Vector3f &color);
    Triangle getIthTriangle(int i, bool gouraud);
    inline Vector3f getSphericalCoor(const Vector3f &pos);
    Vector3f getTextImageColor(const Vector3f &coord);
    Vector3f getIthTextureCoor(int i);
    void calGouraudColor(std::unique_ptr<Camera> &ca,
                         const std::vector<std::shared_ptr<Light>> &lights,
                         const BaseShader &shader);
    void calVectexNormal();
    void calGouraudColor(std::unique_ptr<Camera> &ca,
                         const std::vector<std::shared_ptr<Light>> &lights,
                         const std::unique_ptr<BaseShader> &shader);

    void setTexureMapping(std::string path, const Vector3f &textureCenter);
};