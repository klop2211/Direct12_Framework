#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"
#include "DefaultScene.h"
#include "IATest.h"

Camera::Camera() : Object()
{
	d3d12_viewport_ = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT, 0.0f, 1.0f };
	d3d12_scissor_rect_ = { 0, 0, FRAME_BUFFER_WIDTH , FRAME_BUFFER_HEIGHT };
}

void Camera::set_viewport(int top_left_x, int top_left_y, int width, int height, float min_z, float max_z)
{
	d3d12_viewport_.TopLeftX = top_left_x, d3d12_viewport_.TopLeftY = top_left_y;

	d3d12_viewport_.Width = width, d3d12_viewport_.Height = height;

	d3d12_viewport_.MinDepth = min_z, d3d12_viewport_.MaxDepth = max_z;
}

void Camera::set_scissor_rect(LONG left, LONG top, LONG right, LONG bottom)
{
	d3d12_scissor_rect_.left = left;
	d3d12_scissor_rect_.top = top;
	d3d12_scissor_rect_.right = right;
	d3d12_scissor_rect_.bottom = bottom;
}

void Camera::CreateShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	UINT element_byte = ((sizeof(CameraInfo) + 255) & ~255); //256의 배수
	d3d12_camera_info_buffer_.Attach(CreateBufferResource(device, command_list, nullptr, element_byte));

	d3d12_camera_info_buffer_->Map(0, NULL, (void**)&mapped_camara_info_);
}

void Camera::UpdateShaderVariable(ID3D12GraphicsCommandList* command_list)
{
	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMLoadFloat4x4(&view_matrix_)));
	memcpy(&mapped_camara_info_->view_matrix, &temp, sizeof(XMFLOAT4X4));

	XMStoreFloat4x4(&temp, XMMatrixTranspose(XMLoadFloat4x4(&projection_matrix_)));
	memcpy(&mapped_camara_info_->projection_matrix, &temp, sizeof(XMFLOAT4X4));

	memcpy(&mapped_camara_info_->camera_position, &position_vector(), sizeof(XMFLOAT3));

	command_list->SetGraphicsRootConstantBufferView((int)RootSignatureIndex::Camera, d3d12_camera_info_buffer_->GetGPUVirtualAddress());
}

void Camera::SetViewportAndScissorRect(ID3D12GraphicsCommandList* command_list)
{
	command_list->RSSetViewports(1, &d3d12_viewport_);
	command_list->RSSetScissorRects(1, &d3d12_scissor_rect_);
}

void Camera::Update(float elapsed_time)
{
	//TODO: 임시 카메라 조작을 추후 카메라를 상속받는 클래스로 구현
	XMFLOAT3 temp = IATest::Instance()->action_value();
	AddRotate(-temp.y, temp.x, 0);
	IATest::Instance()->set_action_value(0, 0, 0);

	UpdateViewMatrix();
}

void Camera::CreateViewMatrix(const XMFLOAT3& camera_target_position)
{
	look_at_world_position_ = camera_target_position;
	CreateViewMatrix();
}

void Camera::CreateViewMatrix()
{
	view_matrix_ = Matrix4x4::LookAtLH(position_vector(), look_at_world_position_, up_vector());
}

void Camera::UpdateViewMatrix()
{
	set_look_vector(Vector3::Normalize(look_vector()));
	set_right_vector(Vector3::CrossProduct(up_vector(), look_vector(), true));
	set_up_vector(Vector3::CrossProduct(look_vector(), right_vector(), true));

	XMFLOAT3 look = look_vector(), right = right_vector(), up = up_vector(), position = position_vector();
	view_matrix_._11 = right.x; view_matrix_._12 = up.x; view_matrix_._13 = look.x;
	view_matrix_._21 = right.y; view_matrix_._22 = up.y; view_matrix_._23 = look.y;
	view_matrix_._31 = right.z; view_matrix_._32 = up.z; view_matrix_._33 = look.z;
	view_matrix_._41 = -Vector3::DotProduct(position, right);
	view_matrix_._42 = -Vector3::DotProduct(position, up);
	view_matrix_._43 = -Vector3::DotProduct(position, look);
}

void Camera::CreateProjectionMatrix(float near_plane_distance, float far_plane_distance, float aspect_ratio, float fov_angle)
{
	projection_matrix_ = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fov_angle), aspect_ratio, near_plane_distance, far_plane_distance);
}
