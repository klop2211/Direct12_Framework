#pragma once

class SubMesh;
class Object;
class Shader;
class Material;

// ���� ���������� �׸� �޽��� ���� Ŭ���� 
// �ν��Ͻ��� ����Ͽ� �����Ѵ�.
class Mesh
{
public:
	~Mesh();

	void AddRef(Object* object);
	void Release(Object* object);

	//setter
	void set_shader(Shader* value) { shader_ = value; }

	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list) = 0;

	void Render(ID3D12GraphicsCommandList* command_list);

	void AddSubMesh(SubMesh* sub_mesh);

	//TODO: ���� ������Ʈ���� �� �Լ��� ���� ���̴� ��������Ʈ�� ������Ʈ�ϰ� ����
	// �� �޽��� ����� ���̴��� ��������Ʈ�� �� �޽��� set
	void SetMeshAtShader();

	void SetMaterialAtSubMesh(int sub_mesh_index, Material* material);

protected:
	//TODO: ������ ������ ���� �� �����ϱ�
	// Create �Ǵ� instance info�� ����, �� ���� count�� ������ ���̴� value�� �ٽ� �����ؾ��Ѵ�.
	unsigned int instance_info_count_ = 10000;

	// �� �޽��� �׷����� �ϴ� ����
	unsigned int instance_count_ = 0;

	// �ν��Ͻ� ����(ex: world_matrix)
	ComPtr<ID3D12Resource> d3d12_instaced_object_info_ = nullptr;

	// �� �޽��� ����޽� (�޽��� ���� �޽��� �ƴҼ� ����)
	std::vector<SubMesh*> sub_meshes_;

	// �� �޽��� �����ϰ� �ִ� object ����Ʈ. ���� �� �� ������Ʈ���� info�� ���ͼ� �ν��Ͻ� ������ ä���
	std::list<Object*> object_list_;

	// �� �޽��� �׸��� ���̴�
	Shader* shader_ = nullptr;
};

