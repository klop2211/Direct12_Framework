// 세이더가 사용하는 글로벌변수의 정리본

// 카메라
cbuffer CameraInfo : register(b0)
{
    matrix g_view_matrix : packoffset(c0);
    matrix g_projection_matrix : packoffset(c4);
    float3 g_camera_position : packoffset(c8);
};

// 오브젝트 info
struct InstancedGameObjectInfo
{
    matrix world_matrix;
};

StructuredBuffer<InstancedGameObjectInfo> g_instanced_game_object_infos : register(t0);

struct Light
{
    int type; // 0 == point, 1 == direction, 2 == spot
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 position;
    float3 direction;
    float range;
    float falloff;
    float3 attenuation; // x == 0, y == 1, z == 2 각 원소가 0,1,2에 해당하는 값 내적으로 간단하게 연산 가능
    float theta; // cos(theta) 값
    float phi; // cos(phi) 값
};

#define MAX_LIGHT 16

// 조명 info
cbuffer LightInfo : register(b1)
{
    Light g_lights[MAX_LIGHT];
    float4 g_global_ambient;
    int g_light_count;
}

cbuffer MaterialInfo : register(b2)
{
    float g_ambient_factor;
    float3 g_specular_color; // 만일 오브젝트 텍스처에 specular_map이 있다면 그것을 사용
    float g_specular_power;
}
