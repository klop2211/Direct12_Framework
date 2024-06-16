#include "stdafx.h"
#include "Object.h"
#include "Mesh.h"

XMFLOAT3 Object::position_vector() const
{
    return XMFLOAT3(world_matrix_._41, world_matrix_._42, world_matrix_._43);
}

XMFLOAT3 Object::look_vector() const
{
    return XMFLOAT3(world_matrix_._31, world_matrix_._32, world_matrix_._33);
}

XMFLOAT3 Object::right_vector() const
{
    return XMFLOAT3(world_matrix_._11, world_matrix_._12, world_matrix_._13);
}

XMFLOAT3 Object::up_vector() const
{
    return XMFLOAT3(world_matrix_._21, world_matrix_._22, world_matrix_._23);
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
    world_matrix_._41 = x, world_matrix_._42 = y, world_matrix_._43 = z;
}

void Object::set_look_vector(float x, float y, float z)
{
    world_matrix_._31 = x, world_matrix_._32 = y, world_matrix_._33 = z;
}

void Object::set_right_vector(float x, float y, float z)
{
    world_matrix_._11 = x, world_matrix_._12 = y, world_matrix_._13 = z;
}

void Object::set_up_vector(float x, float y, float z)
{
    world_matrix_._21 = x, world_matrix_._22 = y, world_matrix_._23 = z;
}
