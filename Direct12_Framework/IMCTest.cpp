#include "stdafx.h"
#include "IMCTest.h"
#include "GameFramework.h"

void IMCTest::HandleInput()
{
	if (IsTogle('1'))
	{
		GameFramework::Instance()->ChangeSwapChainState();
	}
}
