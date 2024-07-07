#include "stdafx.h"
#include "DefaultScene.h"
#include "Material.h"

Material::~Material()
{

}

void Material::CreateShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	UINT element_byte = ((sizeof(MaterialInfo) + 255) & ~255); //256ÀÇ ¹è¼ö
	d3d12_material_info_.Attach(CreateBufferResource(device, command_list, 
		&material_info_, 
		element_byte,
		D3D12_HEAP_TYPE_DEFAULT, 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, 
		&d3d12_material_info_upload_buffer_));
}

void Material::UpdateShaderVariable(ID3D12GraphicsCommandList* command_list)
{
	command_list->SetGraphicsRootConstantBufferView((int)RootSignatureIndex::Material, d3d12_material_info_->GetGPUVirtualAddress());
}
