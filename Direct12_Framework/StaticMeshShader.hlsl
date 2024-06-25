#include "LightFunc.hlsl"

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||< Diffused >|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// �ܼ��� ������ ������ ���̴�

struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSDiffused(VS_DIFFUSED_INPUT input, uint instance_id : SV_InstanceID)
{
    VS_DIFFUSED_OUTPUT output;
    
    output.position = mul(mul(mul(float4(input.position, 1.f), g_instanced_game_object_infos[instance_id].world_matrix), g_view_matrix), g_projection_matrix);
    output.color = input.color;
    
    return output;
}

float4 PSDiffused(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
    return input.color;
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||< Illuminated >|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// color ������ Ȱ���Ͽ� ����ó���� �ϴ� ���̴�

struct VS_ILLUMINATED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct VS_ILLUMINATED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 world_position : POSITION;
    float3 world_normal : NORMAL;
};

VS_ILLUMINATED_OUTPUT VSIlluminated(VS_ILLUMINATED_INPUT input, uint instance_id : SV_InstanceID)
{
    VS_ILLUMINATED_OUTPUT output;
    
    output.world_position = mul(float4(input.position, 1.f), g_instanced_game_object_infos[instance_id].world_matrix).xyz;
    output.position = mul(mul(float4(output.world_position, 1.f), g_view_matrix), g_projection_matrix);
    output.color = input.color;
    output.world_normal = mul(input.normal, (float3x3)g_instanced_game_object_infos[instance_id].world_matrix);
    
    return output;
}

float4 PSIlluminated(VS_ILLUMINATED_OUTPUT input) : SV_TARGET
{
    float3 world_normal = normalize(input.world_normal);
    float4 illuminated_color = Lighting(input.color, input.world_position, world_normal);
    
    return illuminated_color;
}