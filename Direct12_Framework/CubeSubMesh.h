#pragma once
#include "SubMesh.h"
class CubeSubMesh :
    public SubMesh
{
public:
    CubeSubMesh(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const XMFLOAT4& color);
    ~CubeSubMesh() {}
    
};

