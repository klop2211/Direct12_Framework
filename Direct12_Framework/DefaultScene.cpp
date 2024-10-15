#include "stdafx.h"
#include "DefaultScene.h"
#include "StaticMeshShader.h"
#include "StaticMeshObject.h"
#include "CubeMesh.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

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
	Shader* temp = new StaticMeshShader;
	shaders_.push_back(temp);

	for (auto& shader : shaders_)
	{
		shader->CreateShader(device, root_signature_.Get());
	}

	// 카메라 생성
	camera_ = new Camera;
	camera_->CreateViewMatrix(XMFLOAT3(0, 0, 1));
	camera_->CreateProjectionMatrix(1, 1000, float(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT)), 40);
	camera_->CreateShaderVariable(device, command_list);
	objects_.push_back(camera_);

	// 조명 생성
	global_ambient_ = XMFLOAT4(0.1, 0.1, 0.1, 1);
	lights_.reserve(kMaxLight);
	LightInfo test_light{
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{0, 10, 10},
		(int)LightType::Directional,
		{0, -1, 0},
		100.f,
		0.f,
		{1, 0.01, 0.001},
		0,
		0
	};
	lights_.push_back(new Light);
	lights_.push_back(new Light);
	lights_.push_back(new Light);
	lights_.push_back(new Light);
	lights_[0]->set_light_info(test_light);
	test_light = {
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{0, 10, 10},
		(int)LightType::Point,
		{0, 0, 0},
		100.f,
		0.f,
		{1, 0.01, 0.001},
		0,
		0
	};
	lights_[1]->set_light_info(test_light);

	for (auto& light : lights_)
	{
		objects_.push_back(light);
	}
	CreateLightsShaderVariable(device, command_list);

	// 재질 생성
	Material* test_material = new Material;
	test_material->set_material_info(MaterialInfo
		{
			0.1,
		{0.8,0.8,0.8},
			400
		});
	test_material->CreateShaderVariable(device, command_list);
	materials_.push_back(test_material);

	// 오브젝트 생성
	CubeMesh* cube_mesh = new CubeMesh(device, command_list);
	cube_mesh->SetMaterialAtSubMesh(0, test_material);
	StaticMeshObject* cube = new StaticMeshObject(cube_mesh);
	cube->set_position_vector(0, 20, 20);
	objects_.push_back(cube);

	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			cube = new StaticMeshObject(cube_mesh);
			cube->set_position_vector(i * 2 - 100, -2, j * 2 - 100);
			objects_.push_back(cube);
		}
	}

}

void DefaultScene::CreateRootSignature(ID3D12Device* device)
{
	D3D12_ROOT_PARAMETER d3d12_root_parameter[4];
	d3d12_root_parameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	d3d12_root_parameter[0].Descriptor.ShaderRegister = 0; //Camera
	d3d12_root_parameter[0].Descriptor.RegisterSpace = 0;  // b0
	d3d12_root_parameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3d12_root_parameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	d3d12_root_parameter[1].Descriptor.ShaderRegister = 0; // StaticMeshObjectInfos
	d3d12_root_parameter[1].Descriptor.RegisterSpace = 0;  // t0
	d3d12_root_parameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3d12_root_parameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	d3d12_root_parameter[2].Descriptor.ShaderRegister = 1; //Lights
	d3d12_root_parameter[2].Descriptor.RegisterSpace = 0; // b1
	d3d12_root_parameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	d3d12_root_parameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	d3d12_root_parameter[3].Descriptor.ShaderRegister = 2; //Material
	d3d12_root_parameter[3].Descriptor.RegisterSpace = 0;  // b2
	d3d12_root_parameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


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

void DefaultScene::Update(float elapsed_time)
{
	for (auto& object : objects_)
	{
		object->Update(elapsed_time);
		object->UpdateWorldMatrix();
	}
}

void DefaultScene::Render(ID3D12GraphicsCommandList* command_list)
{
	command_list->SetGraphicsRootSignature(root_signature_.Get());

	UpdateShaderRenderList();

	// 카메라 info set
	camera_->SetViewportAndScissorRect(command_list);
	camera_->UpdateShaderVariable(command_list);

	// light info set
	UpdateLightsShaderVariable(command_list);

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
	}
	for (auto& object : objects_)
	{
		// 카메라 절두체 컬링
		if (camera_->IsInViewFrustum(object->GetObjectObb()))
		{
			object->set_is_render_(true);

			object->SetMeshAtShader();
		}
		else
		{
			object->set_is_render_(false);
		}
	}
}

void DefaultScene::CreateLightsShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
{
	UINT element_byte = ((sizeof(LightInfoPackage) + 255) & ~255); //256의 배수
	d3d12_light_info_.Attach(CreateBufferResource(device, command_list, nullptr, element_byte));
	d3d12_light_info_->Map(0, nullptr, (void**)&mapped_light_info_);
}

void DefaultScene::UpdateLightsShaderVariable(ID3D12GraphicsCommandList* command_list)
{
	//TODO: 카메라 위치와 방향을 기준으로 유효한 조명들만 추가하도록 구현.
	// 현재는 씬의 모든 조명을 추가하는 방식
	for (int i = 0; i < lights_.size(); ++i)
	{
		memcpy(&mapped_light_info_->lights[i], &lights_[i]->light_info(), sizeof(LightInfo));
	}

	memcpy(&mapped_light_info_->global_ambient, &global_ambient_, sizeof(XMFLOAT4));

	int light_count = lights_.size();
	memcpy(&mapped_light_info_->light_count, &light_count, sizeof(int));

	command_list->SetGraphicsRootConstantBufferView((int)RootSignatureIndex::Light, d3d12_light_info_->GetGPUVirtualAddress());
}

