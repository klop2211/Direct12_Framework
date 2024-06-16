#pragma once

struct IndexBufferInfo
{
	D3D12_INDEX_BUFFER_VIEW d3d12_index_buffer_view;
	int index_count;

	IndexBufferInfo(const D3D12_INDEX_BUFFER_VIEW& view, int count) : d3d12_index_buffer_view(view), index_count(count) {}
};

// 메쉬의 가장 작은 단위(단 1개의 메쉬를 저장하는 클래스)
// 간단한 Shape 메쉬를 만들 생각이면 이 메쉬를 상속 받으면 된다.
class SubMesh
{
public:
	SubMesh() {}
	virtual ~SubMesh() {}

	void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	void Render(ID3D12GraphicsCommandList* command_list, unsigned int instance_count);

protected:
	D3D12_PRIMITIVE_TOPOLOGY d3d12_primitive_topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ComPtr<ID3D12Resource> d3d12_position_buffer_ = nullptr;
	ComPtr<ID3D12Resource> d3d12_position_upload_buffer_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW d3d12_position_buffer_view_;

	std::vector<XMFLOAT3> position_buffer_;

	ComPtr<ID3D12Resource> d3d12_color_buffer_ = nullptr;
	ComPtr<ID3D12Resource> d3d12_color_upload_buffer_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW d3d12_color_buffer_view_;

	std::vector<XMFLOAT4> color_buffer_;

	//대부분의 메쉬는 1개의 인덱스 버퍼를 사용하지만 일부 메쉬의 경우 정점을 공유한채로 인덱스 버퍼가 여러개인 경우가 있음
	std::vector<ComPtr<ID3D12Resource>> d3d12_subset_index_buffers_;
	std::vector<ComPtr<ID3D12Resource>> d3d12_subset_index_upload_buffers_;
	std::vector<IndexBufferInfo> subset_index_buffer_infos;

	std::vector<std::vector<UINT>> subset_index_buffers_;

	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertex_buffer_views_;
};

