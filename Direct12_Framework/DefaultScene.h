#pragma once
#include "Scene.h"

// ��ġó�� �� �ν��Ͻ��� �̿��� ���� ����
class DefaultScene :
    public Scene
{
public:
    virtual void Update(float elapsed_time) {}
    virtual void Render(ID3D12GraphicsCommandList* command_list) {}

};

