#include "stdafx.h"
#include "SubMesh.h"

void SubMesh::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	if (position_buffer_.size())
	{
		d3d12_position_buffer_ = CreateBufferResource(device, command_list,
			position_buffer_.data(), sizeof(XMFLOAT3) * position_buffer_.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_position_upload_buffer_);

		//TODO: 만약 position 버퍼뷰를 따로 저장할 필요가 없다면 그냥 정점 버퍼뷰 벡터에 emplace하는 것은 어떠한가?
		d3d12_position_buffer_view_.BufferLocation = d3d12_position_buffer_->GetGPUVirtualAddress();
		d3d12_position_buffer_view_.StrideInBytes = sizeof(XMFLOAT3);
		d3d12_position_buffer_view_.SizeInBytes = sizeof(XMFLOAT3) * position_buffer_.size();

		vertex_buffer_views_.emplace_back(d3d12_position_buffer_view_);
	}
	if (color_buffer_.size())
	{
		d3d12_color_buffer_ = CreateBufferResource(device, command_list,
			color_buffer_.data(), sizeof(XMFLOAT4) * color_buffer_.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_color_upload_buffer_);

		d3d12_color_buffer_view_.BufferLocation = d3d12_color_buffer_->GetGPUVirtualAddress();
		d3d12_color_buffer_view_.StrideInBytes = sizeof(XMFLOAT4);
		d3d12_color_buffer_view_.SizeInBytes = sizeof(XMFLOAT4) * color_buffer_.size();

		vertex_buffer_views_.emplace_back(d3d12_color_buffer_view_);
	}

	for (auto& index_buffer : subset_index_buffers_)
	{
		d3d12_subset_index_buffers_.emplace_back();
		d3d12_subset_index_upload_buffers_.emplace_back();

		d3d12_subset_index_buffers_.back() = CreateBufferResource(device, command_list,
			index_buffer.data(), sizeof(UINT) * index_buffer.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_subset_index_upload_buffers_.back());

		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		index_buffer_view.BufferLocation = d3d12_subset_index_buffers_.back()->GetGPUVirtualAddress();
		index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		index_buffer_view.SizeInBytes = sizeof(UINT);

		subset_index_buffer_infos.emplace_back(index_buffer_view, index_buffer.size());
	}

}

void SubMesh::Render(ID3D12GraphicsCommandList* command_list, unsigned int instance_count)
{
	command_list->IASetPrimitiveTopology(d3d12_primitive_topology_);

	command_list->IASetVertexBuffers(0, vertex_buffer_views_.size(), vertex_buffer_views_.data());

	if (d3d12_subset_index_buffers_.size())
	{
		for (auto& index_buffer_info : subset_index_buffer_infos)
		{
			command_list->IASetIndexBuffer(&index_buffer_info.d3d12_index_buffer_view);
			command_list->DrawIndexedInstanced(index_buffer_info.index_count, instance_count, 0, 0, 0);
		}
	}
	else
	{
		command_list->DrawInstanced(position_buffer_.size(), instance_count, 0, 0);
	}

}
