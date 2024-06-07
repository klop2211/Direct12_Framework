#include "stdafx.h"
#include "GameFramework.h"

GameFramework* GameFramework::game_framework_ = nullptr;

GameFramework::GameFramework(const HINSTANCE& hinstance, const HWND& hwnd) : hinstance_(hinstance), hwnd_(hwnd)
{
	assert(game_framework_ == nullptr);
	game_framework_ = this;
}

GameFramework::~GameFramework()
{
}

void GameFramework::Initialize()
{
	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
}

void GameFramework::FrameAdvance()
{
	

	//입력 처리

	//충돌 처리

	//업데이트

	//애니메이션

	//렌더
}

void GameFramework::CreateDirect3DDevice()
{
	HRESULT hresult;

	UINT dxgi_farctory_flags = 0;
#if defined(_DEBUG)
	ID3D12Debug* d3d_debug_layer = NULL;
	hresult = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&d3d_debug_layer);
	if (d3d_debug_layer)
	{
		d3d_debug_layer->EnableDebugLayer();
		d3d_debug_layer->Release();
	}
	dxgi_farctory_flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hresult = ::CreateDXGIFactory2(dxgi_farctory_flags, __uuidof(IDXGIFactory4), (void**)&dxgi_factory_);

	ComPtr<IDXGIAdapter1> dxgi_adapter;

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != dxgi_factory_->EnumAdapters1(i, &dxgi_adapter); i++)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
		dxgi_adapter->GetDesc1(&dxgiAdapterDesc);
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), (void**)&d3d12_device_))) break;
	}

	if (!d3d12_device_)
	{
		hresult = dxgi_factory_->EnumWarpAdapter(_uuidof(IDXGIAdapter1), (void**)&dxgi_adapter);
		hresult = D3D12CreateDevice(dxgi_adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), (void**)&d3d12_device_);
	}

	if (!d3d12_device_)
	{
		MessageBox(NULL, L"Direct3D 12 Device Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	rtv_descriptor_increment_size_ = d3d12_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsv_descriptor_increment_size_ = d3d12_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	cbv_srv_uav_descriptor_increment_size_ = d3d12_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;
	hresult = d3d12_device_->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	msaa_4x_quality_levels_ = d3dMsaaQualityLevels.NumQualityLevels;
	msaa_4x_enable_ = (msaa_4x_quality_levels_ > 1) ? true : false;

	hresult = d3d12_device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&d3d12_fence_);

	for (UINT i = 0; i < swap_chain_buffer_count_; i++) fence_values_[i] = 1;
	fence_event_handle_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);

}

void GameFramework::CreateCommandQueueAndList()
{
	D3D12_COMMAND_QUEUE_DESC d3d12_command_queue_desc{};
	d3d12_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	d3d12_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hresult = d3d12_device_->CreateCommandQueue(&d3d12_command_queue_desc, _uuidof(ID3D12CommandQueue), (void**)&d3d12_command_queue_);

	hresult = d3d12_device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&d3d12_command_allocator_);

	hresult = d3d12_device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3d12_command_allocator_.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&d3d12_command_list_);
	
	hresult = d3d12_command_list_->Close();
}

void GameFramework::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3d12_descriptor_heap_desc{};
	d3d12_descriptor_heap_desc.NumDescriptors = swap_chain_buffer_count_;
	d3d12_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	d3d12_descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3d12_descriptor_heap_desc.NodeMask = 0;
	HRESULT hResult = d3d12_device_->CreateDescriptorHeap(&d3d12_descriptor_heap_desc, __uuidof(ID3D12DescriptorHeap), (void**)&d3d12_render_target_descriptor_heap_);

	d3d12_descriptor_heap_desc.NumDescriptors = 1;
	d3d12_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = d3d12_device_->CreateDescriptorHeap(&d3d12_descriptor_heap_desc, __uuidof(ID3D12DescriptorHeap), (void**)&d3d12_depth_stencil_descriptor_heap_);

}

void GameFramework::CreateSwapChain()
{
	RECT rcClient;
	::GetClientRect(hwnd_, &rcClient);
	client_width_ = rcClient.right - rcClient.left;
	client_height_ = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_desc{};
	dxgi_swap_chain_desc.BufferCount = swap_chain_buffer_count_;
	dxgi_swap_chain_desc.BufferDesc.Width = client_width_;
	dxgi_swap_chain_desc.BufferDesc.Height = client_height_;
	dxgi_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgi_swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	dxgi_swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	dxgi_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgi_swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgi_swap_chain_desc.OutputWindow = hwnd_;
	dxgi_swap_chain_desc.SampleDesc.Count = (msaa_4x_enable_) ? 4 : 1;
	dxgi_swap_chain_desc.SampleDesc.Quality = (msaa_4x_enable_) ? (msaa_4x_quality_levels_ - 1) : 0;
	dxgi_swap_chain_desc.Windowed = TRUE;
	dxgi_swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = dxgi_factory_->CreateSwapChain(d3d12_command_queue_.Get(), &dxgi_swap_chain_desc, (IDXGISwapChain**)dxgi_swap_chain_.GetAddressOf());

	if (!dxgi_swap_chain_)
	{
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	hResult = dxgi_factory_->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER);
	swap_chain_buffer_current_index_ = dxgi_swap_chain_->GetCurrentBackBufferIndex();
}
