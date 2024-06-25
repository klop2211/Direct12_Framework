#include "stdafx.h"
#include "IMCTest.h"
#include "GameFramework.h"
#include "IATest.h"

void IMCTest::HandleInput()
{
	if (IsTogle('1'))
	{
		GameFramework::Instance()->ChangeSwapChainState();
	}

	if (IsMouseLeft())
	{
		POINT delta = GetCursorDelta();
		IATest* ia_test = IATest::Instance();
		ia_test->set_action_value(delta.x, -delta.y);
	}
}
