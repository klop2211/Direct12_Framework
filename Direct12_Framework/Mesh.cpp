#include "stdafx.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Object.h"

Mesh::~Mesh()
{
	for (auto& sub_mesh : sub_meshes_)
	{
		delete sub_mesh;
	}
}

void Mesh::AddRef(Object* object)
{
	object_list_.push_back(object);
	++instance_count_;
}

void Mesh::Release(Object* object)
{
	//TODO: ������ �� ������Ʈ�� ����Ʈ���� �����ϴ� �ڵ� �߰�
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
