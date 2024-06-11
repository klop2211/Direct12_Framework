#pragma once

class SubMesh;

// 실제 렌더러에서 그릴 메쉬의 기초 클래스 
// 인스턴싱을 사용하여 렌더한다.
class Mesh
{
public:
	virtual void CreateShaderVariables(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* command_list) = 0;

	void Render(ID3D12GraphicsCommandList* command_list);

protected:
	// 이 메쉬가 그려져야 하는 갯수
	unsigned int instance_count_ = 0;

	// 인스턴스 정보(ex: world_matrix)
	ComPtr<ID3D12Resource> instaced_object_infos_ = nullptr;

	// 이 메쉬의 서브메쉬 (메쉬는 단일 메쉬가 아닐수 있음)
	std::list<std::unique_ptr<SubMesh>> sub_meshes_;

};

