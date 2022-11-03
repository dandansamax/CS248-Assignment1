#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.h"
#include "BaseObject.h"
#include "tiny_obj_loader.h"

Mesh::Mesh(const std::string &path, const Vector3f &center) : BaseObject(center)
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!ret)
    {
        std::cerr << "Error on loading obj: " << path << std::endl;
        return;
    }
    else
    {
        std::cout << "Successfully load obj: " << path << std::endl;
    }
    if (!warn.empty())
    {
        std::cout << warn << std::endl;
    }

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    shape = new tinyobj::shape_t(shapes[0]);
}

int Mesh::getTriangleNum() { return shape->mesh.num_face_vertices.size(); }

Vector3f Mesh::getIthVertex(int i)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    float x = attrib->vertices[3 * size_t(idx.vertex_index) + 0];
    float y = attrib->vertices[3 * size_t(idx.vertex_index) + 1];
    float z = attrib->vertices[3 * size_t(idx.vertex_index) + 2];
    return Vector3f(x, y, z);
}

Vector3f Mesh::getIthNormal(int i)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    float x = attrib->vertices[3 * size_t(idx.normal_index) + 0];
    float y = attrib->vertices[3 * size_t(idx.normal_index) + 1];
    float z = attrib->vertices[3 * size_t(idx.normal_index) + 2];
    return Vector3f(x, y, z);
}

void Mesh::setIthNormal(int i, Vector3f normal)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    attrib->vertices[3 * size_t(idx.normal_index) + 0] = normal.x;
    attrib->vertices[3 * size_t(idx.normal_index) + 1] = normal.y;
    attrib->vertices[3 * size_t(idx.normal_index) + 2] = normal.z;
}

Triangle Mesh::getIthTriangle(int i)
{
    Vector3f t1 = getIthVertex(i * 3);
    Vector3f t2 = getIthVertex(i * 3 + 1);
    Vector3f t3 = getIthVertex(i * 3 + 2);
    Vector3f n1 = getIthNormal(i * 3).normalize();
    Vector3f n2 = getIthNormal(i * 3 + 1).normalize();
    Vector3f n3 = getIthNormal(i * 3 + 2).normalize();
    return Triangle(t1, t2, t3, n1, n2, n3);
}

void Mesh::calVectexNormal()
{
    for (auto &v : attrib->vertices)
    {
        v = 0.0f;
    }
    int size = getTriangleNum();
    for (int i = 0; i < size; i++)
    {
        Vector3f t1 = getIthVertex(i * 3);
        Vector3f t2 = getIthVertex(i * 3 + 1);
        Vector3f t3 = getIthVertex(i * 3 + 2);

        auto normal = (t2 - t1).cross(t3 - t1);
        Vector3f n1 = getIthNormal(i * 3);
        Vector3f n2 = getIthNormal(i * 3 + 1);
        Vector3f n3 = getIthNormal(i * 3 + 2);
        setIthNormal(i * 3, n1 + normal);
        setIthNormal(i * 3 + 1, n2 + normal);
        setIthNormal(i * 3 + 2, n3 + normal);
    }
}