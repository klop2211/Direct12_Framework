#pragma once

class Mesh;

// 모든 셰이더의 기초 클래스
class Shader
{
public:
	Shader() {}
	~Shader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout() = 0;
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob) = 0;
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob) = 0;

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR* file_name, LPCSTR shader_name, LPCSTR shader_profile, ID3DBlob** shader_blob);
	
	//TODO: 이 함수의 쓰임 공부 및 구현
	D3D12_SHADER_BYTECODE ReadCompiledShaderFromFile(WCHAR* file_name, ID3DBlob** shader_blob = NULL) {}

	virtual void CreateShader(ID3D12Device* device, ID3D12RootSignature* root_signature);

	void SetPiplineState(ID3D12GraphicsCommandList* command_list);

	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

	void AddRenderMesh(Mesh* mesh);
	void EraseRenderMesh(Mesh* mesh);
	void ClearRenderList() { render_list_.clear(); }

protected:
	ComPtr<ID3D12PipelineState> d3d12_pipeline_state_;

	std::list<Mesh*> render_list_;

};

