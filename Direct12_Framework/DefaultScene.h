#pragma once
#include "Scene.h"

// 배치처리 및 인스턴싱을 이용한 렌더 구현
class DefaultScene :
    public Scene
{
public:
    virtual void Update(float elapsed_time) {}
    virtual void Render(ID3D12GraphicsCommandList* command_list);

};

