#pragma once

// ��ǲ ������ �����ϴ� Ŭ����. �� Ŭ������ ��� �޾� ����ϸ� IA ���λ縦 ����Ѵ�.
class InputAction
{
public:
	void set_action_value(float x, float y = 0.f, float z = 0.f) { action_value_ = XMFLOAT3(x, y, z); }

	XMFLOAT3 action_value() const { return action_value_; }

	void AddActionValue(float x, float y = 0, float z = 0) { action_value_ += XMFLOAT3(x, y, z); }
	void Reset() { action_value_ = XMFLOAT3(0, 0, 0); }


private:
	XMFLOAT3 action_value_{ 0,0,0 };

};

