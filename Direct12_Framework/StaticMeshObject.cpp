#include "stdafx.h"
#include "StaticMeshObject.h"
#include "StaticMesh.h"
#include "Mesh.h"

StaticMeshObject::~StaticMeshObject()
{
	mesh_->Release(this);
}

void StaticMeshObject::set_mesh(StaticMesh* value)
{
	value->AddRef(this);
	mesh_ = value;
}

void StaticMeshObject::Update(float elapsed_time)
{

}
