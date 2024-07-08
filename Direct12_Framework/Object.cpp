#include "stdafx.h"
#include "Object.h"
#include "Mesh.h"

XMFLOAT3 Object::position_vector() const
{
    return XMFLOAT3(transform_matrix_._41, transform_matrix_._42, transform_matrix_._43);
}

XMFLOAT3 Object::look_vector() const
{
    return XMFLOAT3(transform_matrix_._31, transform_matrix_._32, transform_matrix_._33);
}

XMFLOAT3 Object::right_vector() const
{
    return XMFLOAT3(transform_matrix_._11, transform_matrix_._12, transform_matrix_._13);
}

XMFLOAT3 Object::up_vector() const
{
    return XMFLOAT3(transform_matrix_._21, transform_matrix_._22, transform_matrix_._23);
}

void Object::AddRotate(float pitch, float yaw, float roll)
{
    if (IsZero(pitch) && IsZero(yaw) && IsZero(roll))
    {
        return;
    }

    pitch_ += pitch;
    yaw_ += yaw;
    roll_ += roll;

    Rotate(pitch, yaw, roll);
}

void Object::SetMeshAtShader()
{
    if (mesh_)
    {
        mesh_->SetMeshAtShader();
    }
}

BoundingOrientedBox Object::GetObjectObb() const
{
    if (mesh_)
    {
        BoundingOrientedBox temp;
        mesh_->obb().Transform(temp, XMLoadFloat4x4(&world_matrix_));
        return temp;
    }
    return BoundingOrientedBox();
}

void Object::UpdateWorldMatrix()
{
    world_matrix_ = parent_ ? transform_matrix_ * parent_->world_matrix_ : transform_matrix_;

    if (child_)
    {
        child_->UpdateWorldMatrix();
    }
    if (sibling_)
    {
        sibling_->UpdateWorldMatrix();
    }
}

void Object::Rotate(float pitch, float yaw, float roll)
{
    XMFLOAT3 x_axis{1,0,0}, y_axis{0,1,0}, z_axis{0,0,1};
    XMMATRIX pitch_rotate = XMMatrixRotationAxis(XMLoadFloat3(&right_vector()), XMConvertToRadians(pitch));
    XMMATRIX yaw_rotate = XMMatrixRotationAxis(XMLoadFloat3(&y_axis), XMConvertToRadians(yaw));
    XMMATRIX roll_rotate = XMMatrixRotationAxis(XMLoadFloat3(&look_vector()), XMConvertToRadians(roll));

    XMMATRIX rotate_matrix = XMMatrixMultiply(XMMatrixMultiply(pitch_rotate, yaw_rotate), roll_rotate);

    set_look_vector(Vector3::TransformNormal(look_vector(), rotate_matrix));
    set_right_vector(Vector3::CrossProduct(y_axis, look_vector(), true));
    set_up_vector(Vector3::CrossProduct(look_vector(), right_vector(), true));
}

Object::~Object()
{
    if (mesh_)
    {
        mesh_->Release(this);
    }
}

void Object::set_position_vector(const XMFLOAT3& value)
{
    set_position_vector(value.x, value.y, value.z);
}

void Object::set_look_vector(const XMFLOAT3& value)
{
    set_look_vector(value.x, value.y, value.z);
}

void Object::set_right_vector(const XMFLOAT3& value)
{
    set_right_vector(value.x, value.y, value.z);
}

void Object::set_up_vector(const XMFLOAT3& value)
{
    set_up_vector(value.x, value.y, value.z);
}

void Object::set_position_vector(float x, float y, float z)
{
    transform_matrix_._41 = x, transform_matrix_._42 = y, transform_matrix_._43 = z;
}

void Object::set_look_vector(float x, float y, float z)
{
    transform_matrix_._31 = x, transform_matrix_._32 = y, transform_matrix_._33 = z;
}

void Object::set_right_vector(float x, float y, float z)
{
    transform_matrix_._11 = x, transform_matrix_._12 = y, transform_matrix_._13 = z;
}

void Object::set_up_vector(float x, float y, float z)
{
    transform_matrix_._21 = x, transform_matrix_._22 = y, transform_matrix_._23 = z;
}

void Object::set_mesh(Mesh* value)
{
    if (mesh_)
    {
        mesh_->Release(this);
    }
    value->AddRef(this);
    mesh_ = value;
}
