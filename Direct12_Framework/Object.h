#pragma once

class Mesh;

// ��� ������Ʈ�� ��� �޴� Ŭ����
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

	//getter
	XMFLOAT3 position_vector() const;
	XMFLOAT3 look_vector() const;
	XMFLOAT3 right_vector() const;
	XMFLOAT3 up_vector() const;
	bool is_live() const { return is_live_; }
	XMFLOAT4X4 world_matrix() const { return world_matrix_; }
	Mesh* mesh() const { return mesh_; }

	virtual void Update(float elapsed_time) = 0;

	// ������Ʈ�� �ش� ��(degree)��ŭ ȸ�� �߰�
	void AddRotate(float pitch, float yaw, float roll);

	// �޽��� ����ϴ� ���̴��� ��������Ʈ�� �޽� set
	void SetMeshAtShader();

protected:
	// ���� ȸ���� �̷������ �Լ�
	void Rotate(float pitch, float yaw, float roll);

protected:
	bool is_live_ = true;

	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

	float pitch_ = 0.f;
	float yaw_ = 0.f;
	float roll_ = 0.f;

private:
	// �� �޽� ��������� �ݵ�� set_mesh�� �����ؾ���
	Mesh* mesh_ = nullptr;

};

