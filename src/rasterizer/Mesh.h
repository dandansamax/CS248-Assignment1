#pragma once

#include "Triangle.h"
#include "BaseObject.h"
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

public:
    Mesh(const std::string &path, const Vector3f &center);
    ~Mesh();

    int getTriangleNum();

    Vector3f getIthVertex(int i);
    Vector3f getIthNormal(int i);
    void setIthNormal(int i, Vector3f normal);
    Triangle getIthTriangle(int i);
    void calVectexNormal();
};