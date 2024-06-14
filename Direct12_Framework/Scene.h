#pragma once

class Shader;
class Object;

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

	std::list<Object*> objects_;

};

