#include "stdafx.h"
#include "SubMesh.h"

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
