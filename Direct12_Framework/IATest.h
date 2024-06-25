#pragma once
#include "InputAction.h"

class IATest :
    public InputAction
{
public:
    static IATest* Instance();

};

