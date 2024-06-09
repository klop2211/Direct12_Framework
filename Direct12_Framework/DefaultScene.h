#pragma once
#include "Scene.h"
class DefaultScene :
    public Scene
{
public:
    virtual void Update(float elapsed_time) {}
    virtual void Render(ID3D12GraphicsCommandList* command_list) {}

};

