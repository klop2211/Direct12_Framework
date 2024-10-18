#include "stdafx.h"
#include "IACameraRotate.h"

IACameraRotate* IACameraRotate::Instance()
{
	static IACameraRotate instance;
	return &instance;
}
