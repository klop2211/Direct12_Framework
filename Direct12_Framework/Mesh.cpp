#include "stdafx.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Object.h"
#include "Shader.h"
#include "Material.h"

Mesh::~Mesh()
{
	for (auto& sub_mesh : sub_meshes_)
	{
		delete sub_mesh;
	}

	d3d12_instaced_object_info_->Unmap(0, nullptr);
}

void Mesh::AddRef(Object* object)
{
	object_list_.push_back(object);
	++instance_count_;
}

void Mesh::Release(Object* object)
{
	auto& target_it = std::find(object_list_.begin(), object_list_.end(), object);
	object_list_.erase(target_it);

	--instance_count_;
	if (instance_count_ <= 0)
	{
		delete this;	
	}
}

void Mesh::Render(ID3D12GraphicsCommandList* command_list)
{
	UpdateShaderVariables(command_list);

	for (auto& sub_mesh : sub_meshes_)
	{
		sub_mesh->Render(command_list, instance_count_);
	}
}

void Mesh::AddSubMesh(SubMesh* sub_mesh)
{
	sub_meshes_.push_back(sub_mesh);
}

void Mesh::SetMeshAtShader()
{
	if (shader_)
	{
		shader_->AddRenderMesh(this);
	}
}

void Mesh::SetMaterialAtSubMesh(int sub_mesh_index, Material* material)
{
	sub_meshes_[sub_mesh_index]->set_material(material);
}
