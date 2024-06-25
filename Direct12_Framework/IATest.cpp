#include "stdafx.h"
#include "IATest.h"

IATest* IATest::Instance()
{
	static IATest instance;
	return &instance;
}
