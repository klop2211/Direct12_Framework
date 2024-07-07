#pragma once
#include "Scene.h"

enum class RootSignatureIndex{ Camera = 0, StaticMesh, Light, Material };


// 배치처리 및 인스턴싱을 이용한 렌더 구현
class DefaultScene :
    public Scene
{
public:
    DefaultScene(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

    virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

    virtual void CreateRootSignature(ID3D12Device* device);

    virtual void Update(float elapsed_time);
    virtual void Render(ID3D12GraphicsCommandList* command_list);

    void CreateLightsShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
    void UpdateLightsShaderVariable(ID3D12GraphicsCommandList* command_list);

    void CreateMaterialShaderVariable(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
    void UpdateMaterialShaderVariable(ID3D12GraphicsCommandList* command_list);

protected:
    void UpdateShaderRenderList();

protected:

};

