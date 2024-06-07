#pragma once
class GameFramework
{
public:
	GameFramework(const HINSTANCE& hinstance, const HWND& hwnd);
	~GameFramework();

	void Initialize();

	void FrameAdvance();

private:
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateSwapChain();

private:
	static GameFramework* game_framework_;

	HINSTANCE hinstance_;
	HWND hwnd_;

	int client_width_;
	int client_height_;

	ComPtr<IDXGIFactory4> dxgi_factory_;
	ComPtr<IDXGISwapChain3> dxgi_swap_chain_;
	ComPtr<ID3D12Device> d3d12_device_;
	ComPtr<ID3D12Fence> d3d12_fence_;

	ComPtr<ID3D12CommandAllocator> d3d12_command_allocator_;
	ComPtr<ID3D12CommandQueue> d3d12_command_queue_;
	ComPtr<ID3D12GraphicsCommandList> d3d12_command_list_;

	static const UINT swap_chain_buffer_count_ = 2;
	ComPtr<ID3D12DescriptorHeap> d3d12_render_target_descriptor_heap_;
	UINT swap_chain_buffer_current_index_;

	ComPtr<ID3D12DescriptorHeap> d3d12_depth_stencil_descriptor_heap_;

	std::array<UINT64, swap_chain_buffer_count_> fence_values_;

	HANDLE fence_event_handle_;

	UINT rtv_descriptor_increment_size_;
	UINT dsv_descriptor_increment_size_;
	UINT cbv_srv_uav_descriptor_increment_size_;

	UINT msaa_4x_quality_levels_;
	bool msaa_4x_enable_;
};

