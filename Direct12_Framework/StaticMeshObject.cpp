#include "stdafx.h"
#include "StaticMeshObject.h"
#include "StaticMesh.h"
#include "Mesh.h"
#include "IATest.h"

StaticMeshObject::StaticMeshObject(StaticMesh* mesh)
{
	set_mesh(mesh);
}

void StaticMeshObject::Update(float elapsed_time)
{
}
