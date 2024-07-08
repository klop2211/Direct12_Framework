#pragma once
#include "Object.h"

enum class LightType { Point = 0, Direction, Spot };

// hlsl ������� ��ŷ�� ������ ��
struct LightInfo
{
    XMFLOAT4 diffuse;
    XMFLOAT4 ambient;
    XMFLOAT4 specular;
    XMFLOAT3 position;
    int type;
    XMFLOAT3 direction;
    float range;
    float falloff;
    XMFLOAT3 attenuation; // x == 0, y == 1, z == 2 �� ���Ұ� 0,1,2�� �ش��ϴ� �� �������� �����ϰ� ���� ����
    float theta; // cos(theta) ��
    float phi; // cos(phi) ��
    XMFLOAT2 pad; // hlsl ��ŷ�� ���� ���� ��
};

// ���̴����� ������ ����ϴ� ���� ����(���������� �ϰ�ó��)
struct LightInfoPackage
{
    LightInfo lights[MAX_LIGHT];
    XMFLOAT4 global_ambient;
    int light_count; // ���� ����ϴ� ������ ����(�ִ� ������ �������� ������ ���� �� ����)
};

// ���� ����Ŭ���� 
//TODO: �� Ŭ������ ��ӹ޾� �� ����, ���� ������� ����
class Light :
    public Object
{
public:
    Light();
    ~Light() {}

    void set_light_info(const LightInfo& value);

    LightInfo light_info() const { return light_info_; }

    virtual void Update(float elapsed_time);

protected:
    LightInfo light_info_;

};

