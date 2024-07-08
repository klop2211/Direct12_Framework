#pragma once

class Mesh;

// 모든 오브젝트가 상속 받는 클래스
class Object
{
public:
	Object() {}
	~Object();

	//setter
	void set_position_vector(const XMFLOAT3& value);
	void set_look_vector(const XMFLOAT3& value);
	void set_right_vector(const XMFLOAT3& value);
	void set_up_vector(const XMFLOAT3& value);
	void set_position_vector(float x, float y, float z);
	void set_look_vector(float x, float y, float z);
	void set_right_vector(float x, float y, float z);
	void set_up_vector(float x, float y, float z);
	void set_mesh(Mesh* value);
	void set_is_render_(bool value) { is_render_ = value; }

	//getter
	XMFLOAT3 position_vector() const;
	XMFLOAT3 look_vector() const;
	XMFLOAT3 right_vector() const;
	XMFLOAT3 up_vector() const;
	bool is_live() const { return is_live_; }
	XMFLOAT4X4 world_matrix() const { return world_matrix_; }
	Mesh* mesh() const { return mesh_; }
	bool is_render() const { return is_render_; }

	virtual void Update(float elapsed_time) = 0;

	// 오브젝트의 해당 값(degree)만큼 회전 추가
	void AddRotate(float pitch, float yaw, float roll);

	// 메쉬가 사용하는 셰이더의 렌더리스트에 메쉬 set
	void SetMeshAtShader();
	
	// 오브젝트가 가진 메쉬의 obb에 월드행렬을 적용하여 반환
	BoundingOrientedBox GetObjectObb() const;

	void UpdateWorldMatrix();

protected:
	// 실제 회전이 이루어지는 함수
	void Rotate(float pitch, float yaw, float roll);

protected:
	bool is_live_ = true;
	bool is_render_ = true;

	XMFLOAT4X4 transform_matrix_ = Matrix4x4::Identity();

	float pitch_ = 0.f;
	float yaw_ = 0.f;
	float roll_ = 0.f;

	std::string name_ = "None";

	Object* child_ = nullptr;
	Object* sibling_ = nullptr;
	Object* parent_ = nullptr;

private:
	// 이 메쉬 멤버변수는 반드시 set_mesh로 접근해야함
	Mesh* mesh_ = nullptr;

	// 이 오브젝트의 월드변환 행렬, 반드시 UpdateWorldMatrix에 의해 업데이트 되어야 함
	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

};

