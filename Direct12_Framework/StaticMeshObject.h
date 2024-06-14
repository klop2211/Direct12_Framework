#pragma once
#include "Object.h"

class StaticMesh;

class StaticMeshObject :
    public Object
{
public:
    StaticMeshObject() {}
    StaticMeshObject(StaticMesh* mesh) : mesh_(mesh) {}
    ~StaticMeshObject();

    //getter
    virtual Mesh* mesh() const { return (Mesh*)mesh_; }

    //setter
    void set_mesh(StaticMesh* value);

    void Update(float elapsed_time);

private:
    StaticMesh* mesh_ = nullptr;
};

