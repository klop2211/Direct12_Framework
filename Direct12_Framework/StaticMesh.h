#pragma once
#include "Mesh.h"

struct StaticMeshInstanceInfo
{
	XMFLOAT4X4 world_matrix;
};

class StaticMesh :
    public Mesh
{
public:
	StaticMesh() {}
	~StaticMesh() {}

	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list);

private:
	StaticMeshInstanceInfo* mapped_instance_info_ = nullptr;
};

