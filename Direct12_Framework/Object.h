#pragma once

// 모든 오브젝트가 상속 받는 클래스
class Object
{
public:
	//getter
	bool is_live() const { return is_live_; }
	XMFLOAT4X4 world_matrix() const { return world_matrix_; }

	virtual void Update(float elapsed_time) = 0;
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

protected:
	bool is_live_ = true;

	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

};

