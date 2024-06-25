#include "stdafx.h"
#include "InputMappingContext.h"

std::array<BYTE, KEY_STATE_COUNT> InputMappingContext::key_state_;
std::array<bool, KEY_STATE_COUNT> InputMappingContext::togle_trigers_;
POINT InputMappingContext::old_cursor_position_;
POINT InputMappingContext::current_cursor_position_;
bool InputMappingContext::mouse_left_ = false;
bool InputMappingContext::mouse_right_ = false;

InputMappingContext::InputMappingContext()
{
	for (bool& togle_triger : togle_trigers_)
	{
		togle_triger = false;
	}
}

void InputMappingContext::UpdateKeyState()
{
	GetKeyboardState(key_state_.data());
	mouse_left_ = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	mouse_right_ = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

void InputMappingContext::UpdateCursorPosition()
{
	old_cursor_position_ = current_cursor_position_;
	GetCursorPos(&current_cursor_position_);
}

void InputMappingContext::UpdateInputValue()
{
	UpdateKeyState();
	UpdateCursorPosition();
}

bool InputMappingContext::IsTogle(int key)
{
	if (IsKeyDown(key) && !togle_trigers_[key])
	{
		togle_trigers_[key] = true;
		return true;
	}
	if (!IsKeyDown(key))
	{
		togle_trigers_[key] = false;
	}

	return false;
}

bool InputMappingContext::IsMouseLeft() const
{
	return mouse_left_;
} 

bool InputMappingContext::IsMouseRight() const
{
	return mouse_right_;
}
