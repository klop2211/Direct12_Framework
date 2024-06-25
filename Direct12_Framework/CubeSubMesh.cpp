#include "stdafx.h"
#include "CubeSubMesh.h"

CubeSubMesh::CubeSubMesh(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const XMFLOAT4& color)
{
	// 정점 8개
	position_buffer_.reserve(8);
	color_buffer_.reserve(8);

	// Left Top Front 
	position_buffer_.emplace_back(-1, 1, -1);  //0
	// R T F
	position_buffer_.emplace_back(1, 1, -1);   //1
	// L B F
	position_buffer_.emplace_back(-1, -1, -1); //2
	// R B F
	position_buffer_.emplace_back(1, -1, -1);  //3
	// L T B
	position_buffer_.emplace_back(-1, 1, 1);   //4
	// R T B
	position_buffer_.emplace_back(1, 1, 1);    //5
	// L B B
	position_buffer_.emplace_back(-1, -1, 1);  //6
	// R B B
	position_buffer_.emplace_back(1, -1, 1);   //7

	for (int i = 0; i < color_buffer_.capacity(); ++i)
	{
		color_buffer_.emplace_back(color);
	}

	//subset 1개, 인덱스 36개
	subset_index_buffers_.resize(1);
	auto& index_buffer = subset_index_buffers_.back();

	index_buffer.reserve(36);

	//front
	index_buffer.emplace_back(0);
	index_buffer.emplace_back(1);
	index_buffer.emplace_back(3);
	index_buffer.emplace_back(0);
	index_buffer.emplace_back(3);
	index_buffer.emplace_back(2);

	//back
	index_buffer.emplace_back(4);
	index_buffer.emplace_back(7);
	index_buffer.emplace_back(5);
	index_buffer.emplace_back(4);
	index_buffer.emplace_back(6);
	index_buffer.emplace_back(7);

	//left
	index_buffer.emplace_back(4);
	index_buffer.emplace_back(0);
	index_buffer.emplace_back(6);
	index_buffer.emplace_back(0);
	index_buffer.emplace_back(2);
	index_buffer.emplace_back(6);

	//right
	index_buffer.emplace_back(1);
	index_buffer.emplace_back(5);
	index_buffer.emplace_back(7);
	index_buffer.emplace_back(1);
	index_buffer.emplace_back(7);
	index_buffer.emplace_back(3);

	//top
	index_buffer.emplace_back(4);
	index_buffer.emplace_back(5);
	index_buffer.emplace_back(1);
	index_buffer.emplace_back(4);
	index_buffer.emplace_back(1);
	index_buffer.emplace_back(0);

	//bottom
	index_buffer.emplace_back(6);
	index_buffer.emplace_back(3);
	index_buffer.emplace_back(7);
	index_buffer.emplace_back(6);
	index_buffer.emplace_back(2);
	index_buffer.emplace_back(3);

	CalculateVertexNormal();

	CreateShaderVariables(device, command_list);

}
