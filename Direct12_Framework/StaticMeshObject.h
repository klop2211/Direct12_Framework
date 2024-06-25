#pragma once
#include "Object.h"

class StaticMesh;

class StaticMeshObject :
    public Object
{
public:
    StaticMeshObject() {}
    StaticMeshObject(StaticMesh* mesh); 

    void Update(float elapsed_time);

};

