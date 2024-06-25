#pragma once

class SubMesh;
class Object;
class Shader;

// 실제 렌더러에서 그릴 메쉬의 기초 클래스 
// 인스턴싱을 사용하여 렌더한다.
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

	//TODO: 씬과 업데이트에서 이 함수를 통해 셰이더 렌더리스트를 업데이트하게 구현
	// 이 메쉬가 사용할 셰이더의 렌더리스트에 이 메쉬를 set
	void SetMeshAtShader();

protected:
	//TODO: 적절한 개수가 몇일 지 생각하기
	// Create 되는 instance info의 개수, 이 수를 count가 넘으면 쉐이더 value를 다시 생성해야한다.
	unsigned int instance_info_count_ = 100;

	// 이 메쉬가 그려져야 하는 갯수
	unsigned int instance_count_ = 0;

	// 인스턴스 정보(ex: world_matrix)
	ComPtr<ID3D12Resource> d3d12_instaced_object_info_ = nullptr;

	// 이 메쉬의 서브메쉬 (메쉬는 단일 메쉬가 아닐수 있음)
	std::list<SubMesh*> sub_meshes_;

	// 이 메쉬를 참조하고 있는 object 리스트. 렌더 시 이 오브젝트들의 info를 얻어와서 인스턴싱 정보를 채운다
	std::list<Object*> object_list_;

	// 이 메쉬를 그리는 셰이더
	Shader* shader_ = nullptr;
};

