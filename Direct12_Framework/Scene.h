#pragma once

// 프레임워크가 업데이트하고 렌더를 할 씬의 기초 클래스
class Scene
{
public:
	Scene() {}
	~Scene() {}

	// 씬의 모든 오브젝트의 충돌처리 및 애니메이트가 이루어지는 함수
	virtual void Update(float elapsed_time) = 0;
	// 씬의 모든 오브젝트를 그리는 함수
	virtual void Render(ID3D12GraphicsCommandList* command_list) = 0;

private:
	
};

