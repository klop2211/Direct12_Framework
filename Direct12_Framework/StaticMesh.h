#pragma once
#include "Mesh.h"

//TODO: 향후 조명 처리 및 재질 추가시 color는 삭제
struct StaticMeshInstanceInfo
{
	XMFLOAT4X4 world_matrix;
	XMFLOAT4 color;
};

class StaticMesh :
    public Mesh
{
public:
	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list);

private:
	StaticMeshInstanceInfo* mapped_instance_info_ = nullptr;
};

