#pragma once

// 인풋 정보를 저장 클래스. 이 클래스를 상속 받아 사용하며 IA 접두사를 사용한다.
class InputAction
{
public:
	void set_action_value(float x, float y = 0.f, float z = 0.f) { action_value_ = XMFLOAT3(x, y, z); }

	XMFLOAT3 action_value() const { return action_value_; }

	virtual InputAction* Instance() = 0;

private:
	XMFLOAT3 action_value_{ 0,0,0 };

};

