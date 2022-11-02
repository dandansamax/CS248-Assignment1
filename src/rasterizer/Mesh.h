#pragma once

#include "OBJ_Loader.h"
#include "Triangle.h"

class Mesh : public BaseObject
{
public:
    objl::Mesh mesh;

    Mesh(objl::Mesh &&mesh, const Vector3f &center) : BaseObject(center), mesh(mesh) {}

    Mesh(const std::string &path, const Vector3f &center) : BaseObject(center)
    {
        objl::Loader Loader;
        bool loadout = Loader.LoadFile(path);
        if (!loadout)
        {
            std::cerr << "Error on loading obj: " << path << std::endl;
            return;
        }
        else
        {
            std::cout << "Successfully load obj: " << path << std::endl;
        }
        mesh = Loader.LoadedMeshes[0];
    }

    int getTriangleNum() { return mesh.Indices.size() / 3; }

    Vector3f getIthVertex(int i)
    {
        return Vector3f(mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y,
                        mesh.Vertices[i].Position.Z);
    }

    Vector3f getIthNormal(int i)
    {
        return Vector3f(mesh.Vertices[i].Normal.X, mesh.Vertices[i].Normal.Y,
                        mesh.Vertices[i].Normal.Z);
    }

    Triangle getIthTriangle(int i)
    {
        Vector3f t1 = getIthVertex(mesh.Indices[i * 3]);
        Vector3f t2 = getIthVertex(mesh.Indices[i * 3 + 1]);
        Vector3f t3 = getIthVertex(mesh.Indices[i * 3 + 2]);
        Vector3f n1 = getIthNormal(mesh.Indices[i * 3]);
        Vector3f n2 = getIthNormal(mesh.Indices[i * 3 + 1]);
        Vector3f n3 = getIthNormal(mesh.Indices[i * 3 + 2]);
        return Triangle(t1, t2, t3, n1, n2, n3);
    }

    void calVectexNormal()
    {
        for (auto &v : mesh.Vertices)
        {
            v.Normal = objl::Vector3();
        }
        for (size_t i = 0; i < mesh.Indices.size(); i += 3)
        {
            auto t1 = mesh.Vertices[mesh.Indices[i * 3]];
            auto t2 = mesh.Vertices[mesh.Indices[i * 3 + 1]];
            auto t3 = mesh.Vertices[mesh.Indices[i * 3 + 2]];

            auto normal = objl::math::CrossV3(t2.Position - t1.Position, t3.Position - t1.Position);
            t1.Normal = t1.Normal + normal;
            t2.Normal = t2.Normal + normal;
            t3.Normal = t3.Normal + normal;
        }
        for (auto &v : mesh.Vertices)
        {
            v.Normal = v.Normal / objl::math::MagnitudeV3(v.Normal); 
        }
    }
};