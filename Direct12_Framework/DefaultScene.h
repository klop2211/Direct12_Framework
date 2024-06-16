#pragma once
#include "Scene.h"

enum class RootSignatureIndex{ Camera = 0, StaticMesh };

class Camera;

// ��ġó�� �� �ν��Ͻ��� �̿��� ���� ����
class DefaultScene :
    public Scene
{
public:
    DefaultScene(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

    virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

    virtual void CreateRootSignature(ID3D12Device* device);

    virtual void Update(float elapsed_time) {}
    virtual void Render(ID3D12GraphicsCommandList* command_list);


protected:
    void UpdateShaderRenderList();

protected:
    Camera* camera_ = nullptr;

};

