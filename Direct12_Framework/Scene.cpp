#include "stdafx.h"
#include "Scene.h"
#include "Shader.h"
#include "Object.h"

Scene::~Scene()
{
	for (auto& shader : shaders_)
	{
		delete shader;
	}
	for (auto& object : objects_)
	{
		delete object;
	}
}
