#pragma once
#include "Object.h"

enum class LightType { Point = 0, Directional, Spot };

// hlsl 상수버퍼 패킹에 유의할 것
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
    XMFLOAT3 attenuation; // x == 0, y == 1, z == 2 각 원소가 0,1,2에 해당하는 값 내적으로 간단하게 연산 가능
    float theta; // cos(theta) 값
    float phi; // cos(phi) 값
    XMFLOAT2 pad; // hlsl 패킹에 의한 더미 값
};

// 셰이더에서 실제로 사용하는 조명 정보(여러조명을 일괄처리)
struct LightInfoPackage
{
    LightInfo lights[MAX_LIGHT];
    XMFLOAT4 global_ambient;
    int light_count; // 실제 사용하는 조명의 개수(최대 조명의 개수보다 조명이 적을 수 있음)
};

// 조명 기초클래스 
//TODO: 이 클래스를 상속받아 점 조명, 방향 조명등을 구현
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

