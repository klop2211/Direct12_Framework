#pragma once
#include "InputAction.h"

// 카메라의 전후좌우 이동 입력 2D 축
class IACameraMove :
    public InputAction
{
public:
    static IACameraMove* Instance();
};

