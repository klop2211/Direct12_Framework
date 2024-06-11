#pragma once

class SubMesh;

// ���� ���������� �׸� �޽��� ���� Ŭ���� 
// �ν��Ͻ��� ����Ͽ� �����Ѵ�.
class Mesh
{
public:
	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list) = 0;

	void Render(ID3D12GraphicsCommandList* command_list);

protected:
	// �� �޽��� �׷����� �ϴ� ����
	unsigned int instance_count_ = 0;

	// �ν��Ͻ� ����(ex: world_matrix)
	ComPtr<ID3D12Resource> instaced_object_infos_ = nullptr;

	// �� �޽��� ����޽� (�޽��� ���� �޽��� �ƴҼ� ����)
	std::list<std::unique_ptr<SubMesh>> sub_meshes_;

};

