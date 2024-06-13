#pragma once
#include "Shader.h"

class StaticMeshShader :
    public Shader
{
public:
    virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

    virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob);
    virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob);

    virtual void Render(ID3D12GraphicsCommandList* command_list);

};

