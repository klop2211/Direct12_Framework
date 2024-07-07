#include "stdafx.h"
#include "Scene.h"
#include "Shader.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

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
	for (auto& material : materials_)
	{
		delete material;
	}
}
