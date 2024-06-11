#pragma once

// ��� ������Ʈ�� ��� �޴� Ŭ����
class Object
{
public:
	bool is_live() const { return is_live_; }

	virtual void Update(float elapsed_time) = 0;
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

private:
	bool is_live_ = true;

};

