#pragma once

// ��� ������Ʈ�� ��� �޴� Ŭ����
class Object
{
public:
	//getter
	bool is_live() const { return is_live_; }
	XMFLOAT4X4 world_matrix() const { return world_matrix_; }

	virtual void Update(float elapsed_time) = 0;

protected:
	bool is_live_ = true;

	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

};

