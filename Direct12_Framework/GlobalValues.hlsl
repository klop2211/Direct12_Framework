// ���̴��� ����ϴ� �۷ι������� ������

// ī�޶�
cbuffer CameraInfo : register(b0)
{
    matrix g_view_matrix : packoffset(c0);
    matrix g_projection_matrix : packoffset(c4);
    float3 g_camera_position : packoffset(c8);
};

// ������Ʈ info
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
    float3 attenuation; // x == 0, y == 1, z == 2 �� ���Ұ� 0,1,2�� �ش��ϴ� �� �������� �����ϰ� ���� ����
    float theta; // cos(theta) ��
    float phi; // cos(phi) ��
};

#define MAX_LIGHT 16

// ���� info
cbuffer LightInfo : register(b1)
{
    Light g_lights[MAX_LIGHT];
    float4 g_global_ambient;
    int g_light_count;
}

cbuffer MaterialInfo : register(b2)
{
    float g_ambient_factor;
    float3 g_specular_color; // ���� ������Ʈ �ؽ�ó�� specular_map�� �ִٸ� �װ��� ���
    float g_specular_power;
}
