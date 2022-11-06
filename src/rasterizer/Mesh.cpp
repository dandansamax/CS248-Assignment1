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

    attrib = new tinyobj::attrib_t();

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

Mesh::~Mesh()
{
    delete attrib;
    delete shape;
}

int Mesh::getTriangleNum() { return shape->mesh.num_face_vertices.size(); }

Vector3f Mesh::getTransformedPos(const Vector3f &v)
{
    return (curRotation * transformMat * v.toPoint4f()).getVector3f();
}
Vector3f Mesh::getTransformedNormal(const Vector3f &v)
{

    return (curInvert.T() * inverseMat.T() * v.toDirection4f()).getVector3f();
}

Vector3f Mesh::getIthVertex(int i)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    float x = attrib->vertices[3 * size_t(idx.vertex_index) + 0];
    float y = attrib->vertices[3 * size_t(idx.vertex_index) + 1];
    float z = attrib->vertices[3 * size_t(idx.vertex_index) + 2];
    return getTransformedPos(Vector3f(x, y, z));
}

Vector3f Mesh::getIthNormal(int i)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    float x = attrib->normals[3 * size_t(idx.vertex_index) + 0];
    float y = attrib->normals[3 * size_t(idx.vertex_index) + 1];
    float z = attrib->normals[3 * size_t(idx.vertex_index) + 2];
    return getTransformedNormal(Vector3f(x, y, z));
}

void Mesh::setIthNormal(int i, const Vector3f &normal)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    attrib->normals[3 * size_t(idx.vertex_index) + 0] = normal.x;
    attrib->normals[3 * size_t(idx.vertex_index) + 1] = normal.y;
    attrib->normals[3 * size_t(idx.vertex_index) + 2] = normal.z;
}

Vector3f Mesh::getIthGouraudColor(int i)
{
    tinyobj::index_t idx = shape->mesh.indices[i];
    float x = gouraudColor[3 * size_t(idx.vertex_index) + 0];
    float y = gouraudColor[3 * size_t(idx.vertex_index) + 1];
    float z = gouraudColor[3 * size_t(idx.vertex_index) + 2];
    return Vector3f(x, y, z);
}

Vector3f Mesh::getIthTexture(int i) { return Vector3f(0.7f, 0.7f, 0.7f); }

Triangle Mesh::getIthTriangle(int i, bool gouraud)
{
    Vector3f n1, n2, n3;
    Vector3f t1 = getIthVertex(i * 3);
    Vector3f t2 = getIthVertex(i * 3 + 1);
    Vector3f t3 = getIthVertex(i * 3 + 2);
    if (gouraud)
    {
        n1 = getIthGouraudColor(i * 3);
        n2 = getIthGouraudColor(i * 3 + 1);
        n3 = getIthGouraudColor(i * 3 + 2);
    }
    else
    {
        n1 = getIthNormal(i * 3);
        n2 = getIthNormal(i * 3 + 1);
        n3 = getIthNormal(i * 3 + 2);
    }
    return Triangle(t1, t2, t3, n1, n2, n3);
}

void Mesh::calVectexNormal()
{
    int size = getTriangleNum();
    attrib->normals.resize(attrib->vertices.size() * sizeof(attrib->normals[0]));
    for (auto &n : attrib->normals)
    {
        n = 0.0f;
    }
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
    size = attrib->normals.size();
    for (int i = 0; i < size; i += 3)
    {
        Vector3f n = Vector3f(attrib->normals[i], attrib->normals[i + 1], attrib->normals[i + 2]);
        n = n.normalize();
        attrib->normals[i] = n.x, attrib->normals[i + 1] = n.y, attrib->normals[i + 2] = n.z;
    }
}

void Mesh::calGouraudColor(std::unique_ptr<Camera> &ca,
                           const std::vector<std::shared_ptr<Light>> &lights,
                           const std::unique_ptr<BaseShader> &shader)
{
    int size = attrib->vertices.size();
    gouraudColor.resize(size * sizeof(gouraudColor[0]));
    for (int i = 0; i < size; i += 3)
    {
        auto inter =
            Vector3f(attrib->vertices[i], attrib->vertices[i + 1], attrib->vertices[i + 2]);
        inter = getTransformedPos(inter);

        auto normal = Vector3f(attrib->normals[i], attrib->normals[i + 1], attrib->normals[i + 2]);
        normal = getTransformedNormal(normal);

        auto color = Vector3f(0.7f, 0.7f, 0.7f);
        auto rnt = shader->getPointColor(ca->position, inter, normal, color, lights);
        gouraudColor[i] = rnt.x, gouraudColor[i + 1] = rnt.y, gouraudColor[i + 2] = rnt.z;
    }
}