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

	// ������Ʈ�� �ش� ��(degree)��ŭ ȸ�� �߰�
	void AddRotate(float pitch, float yaw, float roll);

	// �޽��� ����ϴ� ���̴��� ��������Ʈ�� �޽� set
	void SetMeshAtShader();
	
	// ������Ʈ�� ���� �޽��� obb�� ��������� �����Ͽ� ��ȯ
	BoundingOrientedBox GetObjectObb() const;

	void UpdateWorldMatrix();

	// ���������� �߰��ϴ� �Լ�
	void AddChild(Object* value);
	void AddSibling(Object* value);

	// ���������� ��ȸ�ϸ� �ش�Ǵ� ������Ʈ�� �˻�
	Object* FindObjectFrame(const std::string& name);
	Object* FindObjectFrame(Object* object);


protected:
	// ���� ȸ���� �̷������ �Լ�
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
	// �� �޽� ��������� �ݵ�� set_mesh�� �����ؾ���
	Mesh* mesh_ = nullptr;

	// �� ������Ʈ�� ���庯ȯ ���, �ݵ�� UpdateWorldMatrix�� ���� ������Ʈ �Ǿ�� ��
	XMFLOAT4X4 world_matrix_ = Matrix4x4::Identity();

};

