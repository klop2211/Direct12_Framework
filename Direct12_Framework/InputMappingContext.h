#pragma once

// Ű���� 256��, ���콺 ���ʿ����� 2��
#define KEY_STATE_COUNT 256
#define MOUSE_KEY_STATE_COUNT 2

// �����ӿ��� ����� ��ǲó�� �ý��� Ŭ�����̸� �̸� ����ؼ� ����� ���� IMC ���λ縦 �ٿ����Ѵ�. ex) IMCPlayer
class InputMappingContext
{
public:
	InputMappingContext();
	~InputMappingContext() {}

	virtual void HandleInput() = 0;

	void UpdateInputValue();

protected:
	bool IsKeyDown(int key) const { return key_state_[key] & 0xF0; }
	bool IsTogle(int key);

	POINT GetCursorDelta() const { return current_cursor_position_ - old_cursor_position_; }

	bool IsMouseLeft() const; 
	bool IsMouseRight() const;

private:
	void UpdateKeyState();
	void UpdateCursorPosition();


	static std::array<BYTE, KEY_STATE_COUNT> key_state_;

	// �� Ű�� ��۷� ����ϱ� ���� ����. true�� Ű�� ����ߴٴ� ��
	static std::array<bool, KEY_STATE_COUNT> togle_trigers_;

	static POINT current_cursor_position_;
	static POINT old_cursor_position_;

	static bool mouse_left_;
	static bool mouse_right_;
};

