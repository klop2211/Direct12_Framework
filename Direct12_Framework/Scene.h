#pragma once

class Shader;
class Object;

struct LightInfoPackage;
struct MaterialInfo;

class Camera;
class Light;
class Material;

// �����ӿ�ũ�� ������Ʈ�ϰ� ������ �� ���� ���� Ŭ����
class Scene
{
public:
	Scene() {}
	~Scene();

	virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;

	// ��Ʈ �ñ׳�ó ����
	virtual void CreateRootSignature(ID3D12Device* device) = 0;

	// ���� ��� ������Ʈ�� �浹ó�� �� �ִϸ���Ʈ�� �̷������ �Լ�
	virtual void Update(float elapsed_time) = 0;
	// ���� ��� ������Ʈ�� �׸��� �Լ�
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

protected:
	ComPtr<ID3D12RootSignature> root_signature_;

	std::vector<Shader*> shaders_;

	//���� �����ϴ� ������Ʈ
	std::list<Object*> objects_;

	Camera* camera_ = nullptr;

	// �ѹ��� �����ϴ� ������ �ִ� ����(���� �����ϴ� ������ ������ �ƴ�!!)
	const int kMaxLight = MAX_LIGHT;

	// ���� ����
	ComPtr<ID3D12Resource> d3d12_light_info_;
	LightInfoPackage* mapped_light_info_;

	std::vector<Light*> lights_;
	XMFLOAT4 global_ambient_;

	// ���� ����
	std::vector<Material*> materials_;

};

