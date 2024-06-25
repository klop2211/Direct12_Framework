#include "stdafx.h"
#include "Shader.h"
#include "Mesh.h"

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3d12_rasterizer_desc;
	::ZeroMemory(&d3d12_rasterizer_desc, sizeof(D3D12_RASTERIZER_DESC));
	d3d12_rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
	d3d12_rasterizer_desc.CullMode = D3D12_CULL_MODE_BACK;
	d3d12_rasterizer_desc.FrontCounterClockwise = FALSE;
	d3d12_rasterizer_desc.DepthBias = 0;
	d3d12_rasterizer_desc.DepthBiasClamp = 0.0f;
	d3d12_rasterizer_desc.SlopeScaledDepthBias = 0.0f;
	d3d12_rasterizer_desc.DepthClipEnable = TRUE;
	d3d12_rasterizer_desc.MultisampleEnable = FALSE;
	d3d12_rasterizer_desc.AntialiasedLineEnable = FALSE;
	d3d12_rasterizer_desc.ForcedSampleCount = 0;
	d3d12_rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(d3d12_rasterizer_desc);
}

D3D12_BLEND_DESC Shader::CreateBlendState()
{
	D3D12_BLEND_DESC d3d12_blend_desc;
	::ZeroMemory(&d3d12_blend_desc, sizeof(D3D12_BLEND_DESC));
	d3d12_blend_desc.AlphaToCoverageEnable = FALSE;
	d3d12_blend_desc.IndependentBlendEnable = FALSE;
	d3d12_blend_desc.RenderTarget[0].BlendEnable = FALSE;
	d3d12_blend_desc.RenderTarget[0].LogicOpEnable = FALSE;
	d3d12_blend_desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3d12_blend_desc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3d12_blend_desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3d12_blend_desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3d12_blend_desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3d12_blend_desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3d12_blend_desc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3d12_blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3d12_blend_desc);
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3d12_depth_stencil_desc;
	::ZeroMemory(&d3d12_depth_stencil_desc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3d12_depth_stencil_desc.DepthEnable = TRUE;
	d3d12_depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3d12_depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3d12_depth_stencil_desc.StencilEnable = FALSE;
	d3d12_depth_stencil_desc.StencilReadMask = 0x00;
	d3d12_depth_stencil_desc.StencilWriteMask = 0x00;
	d3d12_depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3d12_depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3d12_depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(d3d12_depth_stencil_desc);
}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* file_name, LPCSTR shader_name, LPCSTR shader_profile, ID3DBlob** shader_blob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pd3dErrorBlob = NULL;
	HRESULT hResult = ::D3DCompileFromFile(file_name, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, shader_name, shader_profile, nCompileFlags, 0, shader_blob, &pd3dErrorBlob);
	//char *pErrorString = (char *)pd3dErrorBlob->GetBufferPointer();

	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = (*shader_blob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*shader_blob)->GetBufferPointer();

	return(d3dShaderByteCode);
}

void Shader::CreateShader(ID3D12Device* device, ID3D12RootSignature* root_signature)
{
	ID3DBlob* d3d_vertex_shader_blob = NULL, *d3d_pixel_shader_blob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3d12_pipeline_state_desc;
	::ZeroMemory(&d3d12_pipeline_state_desc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3d12_pipeline_state_desc.pRootSignature = root_signature;
	d3d12_pipeline_state_desc.VS = CreateVertexShader(&d3d_vertex_shader_blob);
	d3d12_pipeline_state_desc.PS = CreatePixelShader(&d3d_pixel_shader_blob);
	d3d12_pipeline_state_desc.RasterizerState = CreateRasterizerState();
	d3d12_pipeline_state_desc.BlendState = CreateBlendState();
	d3d12_pipeline_state_desc.DepthStencilState = CreateDepthStencilState();
	d3d12_pipeline_state_desc.InputLayout = CreateInputLayout();
	d3d12_pipeline_state_desc.SampleMask = UINT_MAX;
	d3d12_pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3d12_pipeline_state_desc.NumRenderTargets = 1;
	d3d12_pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3d12_pipeline_state_desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3d12_pipeline_state_desc.SampleDesc.Count = 1;
	d3d12_pipeline_state_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	HRESULT hResult = device->CreateGraphicsPipelineState(&d3d12_pipeline_state_desc, __uuidof(ID3D12PipelineState), (void**)&d3d12_pipeline_state_);

	if (d3d_vertex_shader_blob) d3d_vertex_shader_blob->Release();
	if (d3d_pixel_shader_blob) d3d_pixel_shader_blob->Release();

	if (d3d12_pipeline_state_desc.InputLayout.pInputElementDescs) delete d3d12_pipeline_state_desc.InputLayout.pInputElementDescs;
}

void Shader::SetPiplineState(ID3D12GraphicsCommandList* command_list)
{
	command_list->SetPipelineState(d3d12_pipeline_state_.Get());
}

void Shader::AddRenderMesh(Mesh* mesh)
{
	if (!mesh) return;

	//중복 검사
	if (std::find(render_list_.begin(), render_list_.end(), mesh) == render_list_.end())
	{
		render_list_.push_back(mesh);
	}
}

void Shader::EraseRenderMesh(Mesh* mesh)
{
	if (!mesh) return;

	auto& target_it = std::find(render_list_.begin(), render_list_.end(), mesh);
	render_list_.erase(target_it);
}
