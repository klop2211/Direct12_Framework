#pragma once

struct MaterialInfo
{
	float ambient_factor;
	XMFLOAT3 specular_color;
	float specular_power;
};

class Material
{
public:
	Material() {}
	~Material();

	void set_material_info(const MaterialInfo& value) { material_info_ = value; }

	void CreateShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
	void UpdateShaderVariable(ID3D12GraphicsCommandList* command_list);

private:
	MaterialInfo material_info_;

	ComPtr<ID3D12Resource> d3d12_material_info_;
	ComPtr<ID3D12Resource> d3d12_material_info_upload_buffer_;

	//texture 추가시 사용할 코드(변경가능성 있음)
	/*
	Texture* albedo_map_ = nullptr
	*/

};

