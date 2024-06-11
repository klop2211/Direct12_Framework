#include "stdafx.h"
#include "Mesh.h"
#include "SubMesh.h"

void Mesh::Render(ID3D12GraphicsCommandList* command_list)
{
	UpdateShaderVariables(command_list);

	for (auto& sub_mesh : sub_meshes_)
	{
		sub_mesh->Render(command_list, instance_count_);
	}
}
