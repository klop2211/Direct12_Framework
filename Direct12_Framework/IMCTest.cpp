#include "stdafx.h"
#include "IMCTest.h"
#include "GameFramework.h"
#include "IACameraRotate.h"
#include "IACameraMove.h"

void IMCTest::HandleInput()
{
	if (IsTogle('1'))
	{
		GameFramework::Instance()->ChangeSwapChainState();
	}

	if (IsMouseLeft())
	{
		POINT delta = GetCursorDelta();
		IACameraRotate* ia_test = IACameraRotate::Instance();
		ia_test->set_action_value(delta.x, -delta.y);
	}

	IACameraMove::Instance()->Reset();
	if (IsKeyDown('W'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(0,0,1);
	}
	if (IsKeyDown('S'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(0, 0, -1);
	}
	if (IsKeyDown('A'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(-1);
	}
	if (IsKeyDown('D'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(1);
	}
	if (IsKeyDown('Q'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(0, -1);
	}
	if (IsKeyDown('E'))
	{
		IACameraMove* ia_camera_move = IACameraMove::Instance();
		ia_camera_move->AddActionValue(0, 1);
	}

}
