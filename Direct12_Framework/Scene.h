#pragma once

class Shader;
class Object;

struct LightInfoPackage;
struct MaterialInfo;

class Camera;
class Light;
class Material;

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

	//씬을 구성하는 오브젝트
	std::list<Object*> objects_;

	Camera* camera_ = nullptr;

	// 한번에 연산하는 조명의 최대 개수(씬에 존재하는 조명의 개수가 아님!!)
	const int kMaxLight = MAX_LIGHT;

	// 조명 관리
	ComPtr<ID3D12Resource> d3d12_light_info_;
	LightInfoPackage* mapped_light_info_;

	std::vector<Light*> lights_;
	XMFLOAT4 global_ambient_;

	// 재질 관리
	std::vector<Material*> materials_;

};

