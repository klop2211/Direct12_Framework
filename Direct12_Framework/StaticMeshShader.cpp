#include "stdafx.h"
#include "StaticMeshShader.h"
#include "Mesh.h"

std::unique_ptr<StaticMeshShader> StaticMeshShader::instance_ = nullptr;

StaticMeshShader::StaticMeshShader()
{
	assert(instance_ == nullptr);
	instance_.reset(this);
}

StaticMeshShader* StaticMeshShader::Instance()
{
	return instance_.get();
}

D3D12_INPUT_LAYOUT_DESC StaticMeshShader::CreateInputLayout()
{
	UINT input_element_desc_count = 3;
	D3D12_INPUT_ELEMENT_DESC* d3d12_input_element_descs = new D3D12_INPUT_ELEMENT_DESC[input_element_desc_count];

	d3d12_input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	d3d12_input_element_descs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	d3d12_input_element_descs[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3d12_input_layout_desc;
	d3d12_input_layout_desc.pInputElementDescs = d3d12_input_element_descs;
	d3d12_input_layout_desc.NumElements = input_element_desc_count;

	return(d3d12_input_layout_desc);
}

D3D12_SHADER_BYTECODE StaticMeshShader::CreateVertexShader(ID3DBlob** shader_blob)
{
	return(CompileShaderFromFile(L"StaticMeshShader.hlsl", "VSIlluminated", "vs_5_1", shader_blob));
}

D3D12_SHADER_BYTECODE StaticMeshShader::CreatePixelShader(ID3DBlob** shader_blob)
{
	return(CompileShaderFromFile(L"StaticMeshShader.hlsl", "PSIlluminated", "ps_5_1", shader_blob));
}

void StaticMeshShader::Render(ID3D12GraphicsCommandList* command_list)
{
	SetPiplineState(command_list);

	for (auto& render_mesh : render_list_)
	{
		render_mesh->Render(command_list);
	}
}
