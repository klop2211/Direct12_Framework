#include "stdafx.h"
#include "SubMesh.h"
#include "Material.h"

SubMesh::~SubMesh()
{

}

void SubMesh::CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	if (position_buffer_.size())
	{
		d3d12_position_buffer_.Attach(CreateBufferResource(device, command_list,
			position_buffer_.data(), sizeof(XMFLOAT3) * position_buffer_.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_position_upload_buffer_));

		//TODO: 만약 position 버퍼뷰를 따로 저장할 필요가 없다면 그냥 정점 버퍼뷰 벡터에 emplace하는 것은 어떠한가?
		d3d12_position_buffer_view_.BufferLocation = d3d12_position_buffer_->GetGPUVirtualAddress();
		d3d12_position_buffer_view_.StrideInBytes = sizeof(XMFLOAT3);
		d3d12_position_buffer_view_.SizeInBytes = sizeof(XMFLOAT3) * position_buffer_.size();

		vertex_buffer_views_.emplace_back(d3d12_position_buffer_view_);
	}
	if (color_buffer_.size())
	{
		d3d12_color_buffer_.Attach(CreateBufferResource(device, command_list,
			color_buffer_.data(), sizeof(XMFLOAT4) * color_buffer_.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_color_upload_buffer_));

		d3d12_color_buffer_view_.BufferLocation = d3d12_color_buffer_->GetGPUVirtualAddress();
		d3d12_color_buffer_view_.StrideInBytes = sizeof(XMFLOAT4);
		d3d12_color_buffer_view_.SizeInBytes = sizeof(XMFLOAT4) * color_buffer_.size();

		vertex_buffer_views_.emplace_back(d3d12_color_buffer_view_);
	}
	if (normal_buffer_.size())
	{
		d3d12_normal_buffer_.Attach(CreateBufferResource(device, command_list,
			normal_buffer_.data(), sizeof(XMFLOAT3) * normal_buffer_.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_normal_upload_buffer_));

		d3d12_normal_buffer_view_.BufferLocation = d3d12_normal_buffer_->GetGPUVirtualAddress();
		d3d12_normal_buffer_view_.StrideInBytes = sizeof(XMFLOAT3);
		d3d12_normal_buffer_view_.SizeInBytes = sizeof(XMFLOAT3) * normal_buffer_.size();

		vertex_buffer_views_.emplace_back(d3d12_normal_buffer_view_);

	}

	for (auto& index_buffer : subset_index_buffers_)
	{
		d3d12_subset_index_buffers_.emplace_back();
		d3d12_subset_index_upload_buffers_.emplace_back();

		d3d12_subset_index_buffers_.back().Attach(CreateBufferResource(device, command_list,
			index_buffer.data(), sizeof(UINT) * index_buffer.size(),
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			&d3d12_subset_index_upload_buffers_.back()));

		D3D12_INDEX_BUFFER_VIEW index_buffer_view;
		index_buffer_view.BufferLocation = d3d12_subset_index_buffers_.back()->GetGPUVirtualAddress();
		index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		index_buffer_view.SizeInBytes = sizeof(UINT) * index_buffer.size();

		subset_index_buffer_infos.emplace_back(index_buffer_view, index_buffer.size());
	}

}

void SubMesh::Render(ID3D12GraphicsCommandList* command_list, unsigned int instance_count)
{
	command_list->IASetPrimitiveTopology(d3d12_primitive_topology_);

	command_list->IASetVertexBuffers(0, vertex_buffer_views_.size(), vertex_buffer_views_.data());

	if (material_)
	{
		material_->UpdateShaderVariable(command_list);
	}

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

void SubMesh::CalculateVertexNormal()
{
	if (d3d12_primitive_topology_ == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		normal_buffer_.resize(position_buffer_.size());
		if (subset_index_buffers_.size())
		{
			for (auto& index_buffer : subset_index_buffers_)
			{
				for (int i = 0; i < position_buffer_.size(); ++i)
				{
					XMFLOAT3 normal{ 0,0,0 };
					int count = 0;
					for (int j = 0; j < index_buffer.size(); j += 3)
					{
						// 이 정점을 포함하는 폴리곤을 찾으면 그 평면의 노말을 계산하여 더한다.
						if (index_buffer[j] == i)
						{
							normal += Plane::PlaneNormal(
								position_buffer_[index_buffer[j]], 
								position_buffer_[index_buffer[j + 1]], 
								position_buffer_[index_buffer[j + 2]]);
							++count;
						}
						if (index_buffer[j + 1] == i)
						{
							normal += Plane::PlaneNormal(
								position_buffer_[index_buffer[j]],
								position_buffer_[index_buffer[j + 1]],
								position_buffer_[index_buffer[j + 2]]);
							++count;
						}
						if (index_buffer[j + 2] == i)
						{
							normal += Plane::PlaneNormal(
								position_buffer_[index_buffer[j]],
								position_buffer_[index_buffer[j + 1]],
								position_buffer_[index_buffer[j + 2]]);
							++count;
						}
					}
					normal_buffer_[i] = normal;
				}
			}
		}
		else
		{
			for (int i = 0; i < position_buffer_.size() - 2; i += 3)
			{
				XMFLOAT3 normal;
				normal = Plane::PlaneNormal(position_buffer_[i], position_buffer_[i + 1], position_buffer_[i + 2]);
				normal_buffer_[i] = normal_buffer_[i + 1] = normal_buffer_[i + 2] = normal;
			}
		}

		for (XMFLOAT3& normal : normal_buffer_)
		{
			normal = Vector3::Normalize(normal);
		}

	}
}
