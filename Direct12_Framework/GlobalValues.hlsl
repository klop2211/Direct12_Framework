// ���̴��� ����ϴ� �۷ι������� ������

// ī�޶�
cbuffer CameraInfo : register(b0)
{
    matrix view_matrix : packoffset(c0);
    matrix projection_matrix : packoffset(c4);
};

// ������Ʈ info
struct InstancedGameObjectInfo
{
    matrix world_matrix;
};

StructuredBuffer<InstancedGameObjectInfo> instanced_game_object_infos : register(t0);



