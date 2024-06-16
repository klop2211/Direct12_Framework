#include "stdafx.h"
#include "CubeMesh.h"
#include "CubeSubMesh.h"

CubeMesh::CubeMesh(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	SubMesh* sub_mesh = new CubeSubMesh(device, command_list, XMFLOAT4(1, 0, 0, 0));

	AddSubMesh(sub_mesh);

	CreateShaderVariables(device, command_list);
}
