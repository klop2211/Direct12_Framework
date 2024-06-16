// 세이더가 사용하는 글로벌변수의 정리본

// 카메라
cbuffer CameraInfo : register(b0)
{
    matrix view_matrix : packoffset(c0);
    matrix projection_matrix : packoffset(c4);
    float3 camera_position : packoffset(c8);
};

// 오브젝트 info
struct InstancedGameObjectInfo
{
    matrix world_matrix;
};

StructuredBuffer<InstancedGameObjectInfo> instanced_game_object_infos : register(t0);



