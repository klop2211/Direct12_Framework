#include "GlobalValues.hlsl"

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||< Diffused >|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// 단순히 색상만을 가지는 셰이더

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VSDiffused(VS_INPUT input, uint instance_id : SV_InstanceID)
{
    VS_OUTPUT output;
    
    output.position = mul(mul(mul(float4(input.position, 1.f), instanced_game_object_infos[instance_id].world_matrix), view_matrix), projection_matrix);
    output.color = input.color;
    
    return output;
}

float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}