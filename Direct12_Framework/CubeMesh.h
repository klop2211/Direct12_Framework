#pragma once
#include "StaticMesh.h"
class CubeMesh :
    public StaticMesh
{
public:
    CubeMesh(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

};

