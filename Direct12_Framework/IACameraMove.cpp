#include "stdafx.h"
#include "IACameraMove.h"

IACameraMove* IACameraMove::Instance()
{
    static IACameraMove instance;
    return &instance;
}
