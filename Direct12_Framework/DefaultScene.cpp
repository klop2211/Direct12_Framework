#include "stdafx.h"
#include "DefaultScene.h"
#include "StaticMeshShader.h"
#include "StaticMeshObject.h"
#include "CubeMesh.h"
#include "Object.h"
#include "Camera.h"

DefaultScene::DefaultScene(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	Initialize(device, command_list);
}

void DefaultScene::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	CreateRootSignature(device);

	// 쉐이더 생성
	int shader_count = 1;
	shaders_.reserve(shader_count);
	shaders_.emplace_back(new StaticMeshShader);

	for (auto& shader : shaders_)
	{
		shader->CreateShader(device, root_signature_.Get());
	}

	// 카메라 생성
	camera_ = new Camera;
	camera_->CreateViewMatrix(XMFLOAT3(0, 0, 1));
	camera_->CreateProjectionMatrix(1, 5000, float(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT)), 40);
	camera_->CreateShaderVariable(device, command_list);
	objects_.push_back(camera_);

	// 오브젝트 생성
	CubeMesh* cube_mesh = new CubeMesh(device, command_list);
	StaticMeshObject* cube = new StaticMeshObject;
	cube->set_mesh(cube_mesh);
	cube->set_position_vector(0, 20, 100);
	objects_.push_back(cube);

	for (int i = 0; i < 10; ++i)
	{
		cube = new StaticMeshObject;
		cube->set_mesh(cube_mesh);
		cube->set_position_vector(i * 10 - 50, 10, 100);
		objects_.push_back(cube);
	}

}

void DefaultScene::CreateRootSignature(ID3D12Device* device)
{
	D3D12_ROOT_PARAMETER d3d12_root_parameter[2];
	d3d12_root_parameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	d3d12_root_parameter[0].Descriptor.ShaderRegister = 0; //Camera
	d3d12_root_parameter[0].Descriptor.RegisterSpace = 0; // b0
	d3d12_root_parameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3d12_root_parameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	d3d12_root_parameter[1].Descriptor.ShaderRegister = 0; // StaticMeshObjectInfos
	d3d12_root_parameter[1].Descriptor.RegisterSpace = 0; // t0
	d3d12_root_parameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	D3D12_ROOT_SIGNATURE_FLAGS d3d12_root_signature_flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3d12_root_signature_desc;
	::ZeroMemory(&d3d12_root_signature_desc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3d12_root_signature_desc.NumParameters = _countof(d3d12_root_parameter);
	d3d12_root_signature_desc.pParameters = d3d12_root_parameter;
	d3d12_root_signature_desc.NumStaticSamplers = 0;
	d3d12_root_signature_desc.pStaticSamplers = nullptr;
	d3d12_root_signature_desc.Flags = d3d12_root_signature_flags;

	ID3DBlob* d3d_signature_blob = NULL;
	ID3DBlob* d3d_error_blob = NULL;
	D3D12SerializeRootSignature(&d3d12_root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &d3d_signature_blob, &d3d_error_blob);
	device->CreateRootSignature(0, d3d_signature_blob->GetBufferPointer(), d3d_signature_blob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&root_signature_);
	if (d3d_signature_blob) d3d_signature_blob->Release();
	if (d3d_error_blob) d3d_error_blob->Release();
}

void DefaultScene::Render(ID3D12GraphicsCommandList* command_list)
{
	command_list->SetGraphicsRootSignature(root_signature_.Get());

	UpdateShaderRenderList();

	// 카메라 info set
	camera_->SetViewportAndScissorRect(command_list);
	camera_->UpdateShaderVariable(command_list);

	// light info set

	for (auto& shader : shaders_)
	{
		shader->Render(command_list);
	}
}

void DefaultScene::UpdateShaderRenderList()
{
	for (auto& shader : shaders_)
	{
		shader->ClearRenderList();
		for (auto& object : objects_)
		{
			shader->AddRenderMesh(object->mesh());
		}
	}
}
