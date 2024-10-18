#pragma once
#include "InputAction.h"

// 카메라 상하좌우 회전 입력 2D 축
class IACameraRotate :
    public InputAction
{
public:
    static IACameraRotate* Instance();

};

