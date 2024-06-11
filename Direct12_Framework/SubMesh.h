#pragma once

struct IndexBufferInfo
{
	D3D12_INDEX_BUFFER_VIEW d3d12_index_buffer_view;
	int index_count;
};

// �޽��� ���� ���� ����(�� 1���� �޽��� �����ϴ� Ŭ����)
// ������ Shape �޽��� ���� �����̸� �� �޽��� ��� ������ �ȴ�.
class SubMesh
{
public:

	void Render(ID3D12GraphicsCommandList* command_list, unsigned int instance_count);

private:
	D3D12_PRIMITIVE_TOPOLOGY d3d12_primitive_topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ComPtr<ID3D12Resource> d3d12_position_buffer_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW d3d12_position_buffer_view_;

	std::vector<XMFLOAT3> position_buffer_;

	//��κ��� �޽��� 1���� �ε��� ���۸� ��������� �Ϻ� �޽��� ��� ������ ������ä�� �ε��� ���۰� �������� ��찡 ����
	std::vector<ComPtr<ID3D12Resource>> d3d12_subset_index_buffers_;
	std::vector<IndexBufferInfo> subset_index_buffer_infos;

	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertex_buffer_views_;
};

