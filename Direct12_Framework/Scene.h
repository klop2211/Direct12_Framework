#pragma once

class Shader;
class Object;

// 프레임워크가 업데이트하고 렌더를 할 씬의 기초 클래스
class Scene
{
public:
	Scene() {}
	~Scene();

	virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;

	// 루트 시그너처 생성
	virtual void CreateRootSignature(ID3D12Device* device) = 0;

	// 씬의 모든 오브젝트의 충돌처리 및 애니메이트가 이루어지는 함수
	virtual void Update(float elapsed_time) = 0;
	// 씬의 모든 오브젝트를 그리는 함수
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

protected:
	ComPtr<ID3D12RootSignature> root_signature_;

	std::vector<Shader*> shaders_;

	std::list<Object*> objects_;

};

