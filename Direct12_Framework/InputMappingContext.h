#pragma once

#define KEY_STATE_COUNT 256

// 프레임에서 사용할 인풋처리 시스템 클래스이며 이를 상속해서 사용할 때는 IMC 접두사를 붙여야한다. ex) IMCPlayer
class InputMappingContext
{
public:
	InputMappingContext();
	~InputMappingContext() {}

	virtual void HandleInput() = 0;

	void UpdateInputValue();

protected:
	bool IsKeyDown(int key) { return key_state_[key] & 0xF0; }
	bool IsTogle(int key);

	POINT GetCursorDelta() const { return current_cursor_position_ - old_cursor_position_; }

private:
	void UpdateKeyState();
	void UpdateCursorPosition();

	static std::array<BYTE, KEY_STATE_COUNT> key_state_;

	// 각 키를 토글로 사용하기 위한 변수. true면 키를 사용했다는 뜻
	static std::array<bool, KEY_STATE_COUNT> togle_trigers_;

	static POINT current_cursor_position_;
	static POINT old_cursor_position_;

};

