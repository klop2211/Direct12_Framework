#include "stdafx.h"
#include "StaticMesh.h"
#include "Object.h"
#include "DefaultScene.h"

void StaticMesh::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	if (instance_info_count_ < instance_count_)
	{
		instance_info_count_ = instance_count_;
	}
	UINT element_byte = (((sizeof(StaticMeshInstanceInfo) * instance_info_count_) + 255) & ~255); //256ÀÇ ¹è¼ö
	d3d12_instaced_object_info_.Attach(CreateBufferResource(device, command_list, nullptr, element_byte));
	d3d12_instaced_object_info_->Map(0, nullptr, (void**)&mapped_instance_info_);
}

void StaticMesh::UpdateShaderVariables(ID3D12GraphicsCommandList* command_list)
{
	command_list->SetGraphicsRootShaderResourceView((int)RootSignatureIndex::StaticMesh, d3d12_instaced_object_info_->GetGPUVirtualAddress());

	for (auto& object : object_list_)
	{
		mapped_instance_info_->world_matrix = object->world_matrix();
	}
}
