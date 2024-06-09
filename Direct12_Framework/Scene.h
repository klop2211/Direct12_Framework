#pragma once

// �����ӿ�ũ�� ������Ʈ�ϰ� ������ �� ���� ���� Ŭ����
class Scene
{
public:
	Scene() {}
	~Scene() {}

	// ���� ��� ������Ʈ�� �浹ó�� �� �ִϸ���Ʈ�� �̷������ �Լ�
	virtual void Update(float elapsed_time) = 0;
	// ���� ��� ������Ʈ�� �׸��� �Լ�
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

private:
	
};

