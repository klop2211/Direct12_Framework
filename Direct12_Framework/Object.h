#pragma once

class Mesh;

// 모든 오브젝트가 상속 받는 클래스
class Object
{
public:
	Object() {}
	virtual ~Object() {}

	//setter
	void set_position_vector(const XMFLOAT3& value);
	void set_look_vector(const XMFLOAT3& value);
	void set_right_vector(const XMFLOAT3& value);
	void set_up_vector(const XMFLOAT3& value);
	void set_position_vector(float x, float y, float z);
	void set_look_vector(float x, float y, float z);
	void set_right_vector(float x, float y, float z);
	void set_up_vector(float x, float y, float z);

	//getter
	XMFLOAT3 position_vector() const;
	XMFLOAT3 look_vector() const;
	XMFLOAT3 right_vector() const;
	XMFLOAT3 up_vector() const;
	bool is_live() const { return is_live_; }
	XMFLOAT4X4 world_matrix() const { return world_matrix_; }
	virtual Mesh* mesh() const = 0;

	virtual void Update(float elapsed_time) = 0;

protected:
	bool is_live_ = true;

	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

};

