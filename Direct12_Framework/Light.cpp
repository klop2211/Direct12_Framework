#include "stdafx.h"
#include "Light.h"

Light::Light()
{
}

void Light::set_light_info(const LightInfo& value)
{
	light_info_ = value;
	set_position_vector(value.position);
}

void Light::Update(float elapsed_time)
{
	light_info_.position = position_vector();
}
