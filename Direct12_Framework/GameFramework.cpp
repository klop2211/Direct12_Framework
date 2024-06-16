#include "stdafx.h"
#include "GameFramework.h"
#include "Timer.h"
#include "Scene.h"
#include "DefaultScene.h"
#include "InputMappingContext.h"
#include "IMCTest.h"

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
	// direct 디바이스 생성 및 초기화
	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();

	CreateRenderTargetViews();
	CreateDepthStencilView();

	d3d12_command_list_->Reset(d3d12_command_allocator_.Get(), nullptr);

	timer_.reset(new Timer);
	scene_.reset(new DefaultScene(d3d12_device_.Get(), d3d12_command_list_.Get()));
	input_mapping_context_.reset(new IMCTest());

	d3d12_command_list_->Close();
	ID3D12CommandList* d3d12_command_lists[] = { d3d12_command_list_.Get() };
	d3d12_command_queue_->ExecuteCommandLists(1, d3d12_command_lists);

	WaitForGpuComplete();

}

void GameFramework::FrameAdvance()
{
	timer_->Tick(60.f);

	//입력 처리
	ProcessInput();

	//현재 씬 업데이트(충돌처리, 업데이트, 애니메이션)
	scene_->Update(timer_->elapsed_time());

	//렌더 사전 준비
	HRESULT hresult = d3d12_command_allocator_->Reset();
	hresult = d3d12_command_list_->Reset(d3d12_command_allocator_.Get(), NULL);

	D3D12_RESOURCE_BARRIER d3d12_resource_barrier;
	::ZeroMemory(&d3d12_resource_barrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3d12_resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3d12_resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3d12_resource_barrier.Transition.pResource = d3d12_swap_chain_back_buffers_[swap_chain_buffer_current_index_].Get();
	d3d12_resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	d3d12_resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3d12_resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	d3d12_command_list_->ResourceBarrier(1, &d3d12_resource_barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3d12_rtv_cpu_descriptor_handle = d3d12_render_target_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
	d3d12_rtv_cpu_descriptor_handle.ptr += (swap_chain_buffer_current_index_ * rtv_descriptor_increment_size_);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	d3d12_command_list_->ClearRenderTargetView(d3d12_rtv_cpu_descriptor_handle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = d3d12_depth_stencil_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
	d3d12_command_list_->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

	d3d12_command_list_->OMSetRenderTargets(1, &d3d12_rtv_cpu_descriptor_handle, TRUE, &d3dDsvCPUDescriptorHandle);

	//현재 씬 렌더
	scene_->Render(d3d12_command_list_.Get());

	//렌더 마무리 작업
	d3d12_resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3d12_resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	d3d12_resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	d3d12_command_list_->ResourceBarrier(1, &d3d12_resource_barrier);

	hresult = d3d12_command_list_->Close();

	ID3D12CommandList* d3d12_command_lists[] = { d3d12_command_list_.Get()};
	d3d12_command_queue_->ExecuteCommandLists(1, d3d12_command_lists);

	WaitForGpuComplete();

	dxgi_swap_chain_->Present(0, 0);

	MoveToNextFrame();

	// GameName (60 FPS)
	::SetWindowText(hwnd_, (game_name_ + L" (" + std::to_wstring(timer_->current_frame_rate()) + L" FPS)").c_str());

}

void GameFramework::ChangeSwapChainState()
{
	BOOL is_full_screen_state = false;
	dxgi_swap_chain_->GetFullscreenState(&is_full_screen_state, nullptr);
	dxgi_swap_chain_->SetFullscreenState(!is_full_screen_state, nullptr);

	DXGI_MODE_DESC dxgi_target_parameters;
	dxgi_target_parameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgi_target_parameters.Width = client_width_;
	dxgi_target_parameters.Height = client_height_;
	dxgi_target_parameters.RefreshRate.Numerator = 60;
	dxgi_target_parameters.RefreshRate.Denominator = 1;
	dxgi_target_parameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgi_target_parameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgi_swap_chain_->ResizeTarget(&dxgi_target_parameters);

	ResizeBackBuffer();

}

void GameFramework::ChangeInputMappingContext(InputMappingContext* new_imc)
{
	input_mapping_context_.reset(new_imc);
}

void GameFramework::WaitForGpuComplete()
{
	const UINT64 fence_value = ++fence_values_[swap_chain_buffer_current_index_];
	HRESULT hresult = d3d12_command_queue_->Signal(d3d12_fence_.Get(), fence_value);

	if (d3d12_fence_->GetCompletedValue() < fence_value)
	{
		hresult = d3d12_fence_->SetEventOnCompletion(fence_value, fence_event_handle_);
		::WaitForSingleObject(fence_event_handle_, INFINITE);
	}
}

void GameFramework::MoveToNextFrame()
{
	swap_chain_buffer_current_index_ = dxgi_swap_chain_->GetCurrentBackBufferIndex();

	WaitForGpuComplete();
}

void GameFramework::ResizeBackBuffer()
{
	WaitForGpuComplete();
	d3d12_command_list_->Reset(d3d12_command_allocator_.Get(), nullptr);

	// 기존 back buffer 리셋(삭제)
	for (auto& swap_chain_buffer : d3d12_swap_chain_back_buffers_)
	{
		swap_chain_buffer.Reset();
	}
	d3d12_depth_stencil_buffer_.Reset();

	// 스왑체인 resize
	DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_desc;
	dxgi_swap_chain_->GetDesc(&dxgi_swap_chain_desc);
	dxgi_swap_chain_->ResizeBuffers(
		swap_chain_buffer_count_, 
		client_width_, client_height_, 
		dxgi_swap_chain_desc.BufferDesc.Format, 
		dxgi_swap_chain_desc.Flags);

	swap_chain_buffer_current_index_ = 0;

	// back buffer 재생성
	CreateRenderTargetViews();
	CreateDepthStencilView();

	d3d12_command_list_->Close();

	ID3D12CommandList* d3d12_command_lists[] = { d3d12_command_list_.Get() };
	d3d12_command_queue_->ExecuteCommandLists(1, d3d12_command_lists);

	WaitForGpuComplete();

}

void GameFramework::ProcessInput()
{
	// 현재 프레임 워크에 등록된 IMC 실행
	if (input_mapping_context_)
	{
		input_mapping_context_->UpdateInputValue();
		input_mapping_context_->HandleInput();
	}

	// 개발 편의를 위한 예외 키
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);
	}
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
	HRESULT hresult = d3d12_device_->CreateDescriptorHeap(
		&d3d12_descriptor_heap_desc, 
		__uuidof(ID3D12DescriptorHeap), 
		(void**)(d3d12_render_target_descriptor_heap_.GetAddressOf()));

	d3d12_descriptor_heap_desc.NumDescriptors = 1;
	d3d12_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hresult = d3d12_device_->CreateDescriptorHeap(
		&d3d12_descriptor_heap_desc, 
		__uuidof(ID3D12DescriptorHeap), 
		(void**)(d3d12_depth_stencil_descriptor_heap_.GetAddressOf()));

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

	HRESULT hresult = dxgi_factory_->CreateSwapChain(d3d12_command_queue_.Get(), &dxgi_swap_chain_desc, (IDXGISwapChain**)dxgi_swap_chain_.GetAddressOf());

	if (!dxgi_swap_chain_)
	{
		MessageBox(NULL, L"Swap Chain Cannot be Created.", L"Error", MB_OK);
		::PostQuitMessage(0);
		return;
	}

	hresult = dxgi_factory_->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER);
	swap_chain_buffer_current_index_ = dxgi_swap_chain_->GetCurrentBackBufferIndex();
}

void GameFramework::CreateRenderTargetViews()
{
	D3D12_CPU_DESCRIPTOR_HANDLE d3d12_rtv_cpu_descriptor_handle = d3d12_render_target_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < swap_chain_buffer_count_; ++i)
	{
		dxgi_swap_chain_->GetBuffer(i, __uuidof(ID3D12Resource), (void**)d3d12_swap_chain_back_buffers_[i].GetAddressOf());
		d3d12_device_->CreateRenderTargetView(d3d12_swap_chain_back_buffers_[i].Get(), NULL, d3d12_rtv_cpu_descriptor_handle);
		d3d12_rtv_cpu_descriptor_handle.ptr += rtv_descriptor_increment_size_;
	}
}

void GameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3d12_resource_desc;
	d3d12_resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3d12_resource_desc.Alignment = 0;
	d3d12_resource_desc.Width = client_width_;
	d3d12_resource_desc.Height = client_height_;
	d3d12_resource_desc.DepthOrArraySize = 1;
	d3d12_resource_desc.MipLevels = 1;
	d3d12_resource_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3d12_resource_desc.SampleDesc.Count = (msaa_4x_enable_) ? 4 : 1;
	d3d12_resource_desc.SampleDesc.Quality = (msaa_4x_enable_) ? (msaa_4x_quality_levels_ - 1) : 0;
	d3d12_resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3d12_resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES d3d12_heap_properties;
	::ZeroMemory(&d3d12_heap_properties, sizeof(D3D12_HEAP_PROPERTIES));
	d3d12_heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
	d3d12_heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3d12_heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3d12_heap_properties.CreationNodeMask = 1;
	d3d12_heap_properties.VisibleNodeMask = 1;

	D3D12_CLEAR_VALUE d3d12_clear_value;
	d3d12_clear_value.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3d12_clear_value.DepthStencil.Depth = 1.0f;
	d3d12_clear_value.DepthStencil.Stencil = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = d3d12_depth_stencil_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
	d3d12_device_->CreateCommittedResource(
		&d3d12_heap_properties, 
		D3D12_HEAP_FLAG_NONE, 
		&d3d12_resource_desc, 
		D3D12_RESOURCE_STATE_DEPTH_WRITE, 
		&d3d12_clear_value, 
		__uuidof(ID3D12Resource), 
		(void**)d3d12_depth_stencil_buffer_.GetAddressOf());

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	::ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

	d3d12_device_->CreateDepthStencilView(d3d12_depth_stencil_buffer_.Get(), &d3dDepthStencilViewDesc, d3dDsvCPUDescriptorHandle);

}
