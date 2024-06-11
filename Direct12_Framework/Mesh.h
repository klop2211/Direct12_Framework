#pragma once

class SubMesh;
class Object;

// ���� ���������� �׸� �޽��� ���� Ŭ���� 
// �ν��Ͻ��� ����Ͽ� �����Ѵ�.
class Mesh
{
public:
	~Mesh();

	void AddRef(Object* object);
	void Release(Object* object);

	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list) = 0;

	void Render(ID3D12GraphicsCommandList* command_list);

	void AddSubMesh(SubMesh* sub_mesh);

protected:
	//TODO: ������ ������ ���� �� �����ϱ�
	// Create �Ǵ� instance info�� ����, �� ���� count�� ������ ���̴� value�� �ٽ� �����ؾ��Ѵ�.
	unsigned int instance_info_count_ = 100;

	// �� �޽��� �׷����� �ϴ� ����
	unsigned int instance_count_ = 0;

	// �ν��Ͻ� ����(ex: world_matrix)
	ComPtr<ID3D12Resource> d3d12_instaced_object_info_ = nullptr;

	// �� �޽��� ����޽� (�޽��� ���� �޽��� �ƴҼ� ����)
	std::list<SubMesh*> sub_meshes_;

	// �� �޽��� �����ϰ� �ִ� object ����Ʈ. ���� �� �� ������Ʈ���� info�� ���ͼ� �ν��Ͻ� ������ ä���
	std::list<Object*> object_list_;
};

