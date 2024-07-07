#pragma once
#include "Object.h"

struct CameraInfo
{
	XMFLOAT4X4 view_matrix;
	XMFLOAT4X4 projection_matrix;
	XMFLOAT3 camera_position;
};

class Camera : public Object
{
public:
	Camera();
	~Camera() { d3d12_camera_info_buffer_->Unmap(0, nullptr); }


	//setter
	void set_viewport(int top_left_x, int top_left_y, int width, int height, float min_z, float max_z);
	void set_scissor_rect(LONG left, LONG top, LONG right, LONG bottom);

	//getter
	virtual Mesh* mesh() const { return nullptr; }

	void CreateShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* command_list);

	void SetViewportAndScissorRect(ID3D12GraphicsCommandList* command_list);

	virtual void Update(float elapsed_time);

	// Ÿ�� �������� view matrix ����
	void CreateViewMatrix(const XMFLOAT3& camera_target_position);
	void CreateViewMatrix();

	// ���� ī�޶� world ��� �������� view matrix ������Ʈ
	void UpdateViewMatrix();

	void CreateProjectionMatrix(float near_plane_distance, float far_plane_distance, float aspect_ratio, float fov_angle);

protected:
	XMFLOAT4X4 view_matrix_ = Matrix4x4::Identity();
	XMFLOAT4X4 projection_matrix_ = Matrix4x4::Identity();

	D3D12_VIEWPORT d3d12_viewport_;
	D3D12_RECT d3d12_scissor_rect_;

	ComPtr<ID3D12Resource> d3d12_camera_info_buffer_;
	CameraInfo* mapped_camara_info_;

	// ī�޶� ����󿡼� �ٶ󺸴� ����
	XMFLOAT3 look_at_world_position_;

};

