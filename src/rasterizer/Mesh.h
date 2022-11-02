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

    Triangle getIthTriangle(int i)
    {
        Vector3f t1 = getIthVertex(mesh.Indices[i * 3]);
        Vector3f t2 = getIthVertex(mesh.Indices[i * 3 + 1]);
        Vector3f t3 = getIthVertex(mesh.Indices[i * 3 + 2]);
        return Triangle(t1, t2, t3);
    }
};