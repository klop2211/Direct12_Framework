#include "GlobalValues.hlsl"

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

float4 PointLight(int light_index, float3 vertex_position, float3 vertex_normal, float3 vertex_to_camera, float4 albedo_color)
{
    float4 return_color = float4(0, 0, 0, 0);
    if (g_lights[light_index].range < distance(vertex_position, g_lights[light_index].position))
        return return_color;
    
    float4 ambient_color = float4(0, 0, 0, 0);
    float4 diffuse_color = float4(0, 0, 0, 0);
    float4 specular_color = float4(0, 0, 0, 0);
    
    float3 vertex_to_light = g_lights[light_index].position - vertex_position;
    float distance_vertex_to_light = length(vertex_to_light);
    vertex_to_light = normalize(vertex_to_light);
    // Laf = 1 / dot(attenuation, float3(1, d, d^2))
    float attenuation_factor = 1.f / dot(g_lights[light_index].attenuation, float3(1.f, distance_vertex_to_light, distance_vertex_to_light * distance_vertex_to_light));
    // A = Ma * (Ga + La * Laf) 이지만 Ga * Ma의 연산은 Lighting 함수에서 최종적으로 함
    // 즉, Ma * (La * Laf) 임
    ambient_color = albedo_color * g_ambient_factor * (g_lights[light_index].ambient * attenuation_factor);
    
    // D = Md * (Ld * (dot(Vn * Ldir) * Laf))
    diffuse_color = albedo_color * (g_lights[light_index].diffuse * (dot(vertex_normal, vertex_to_light) * attenuation_factor));
    
    // S = Ms * (Ls * (dot(Vn * H(i))^p * Laf))
    float3 half_vector = normalize(normalize(vertex_to_camera) + vertex_to_light);
    specular_color = float4(g_specular_color, 1.f) * (g_lights[light_index].specular * pow(max(dot(vertex_normal, half_vector), 0.0), g_specular_power) * attenuation_factor);
    
    return_color = ambient_color + diffuse_color + specular_color;
    return return_color;
}

float4 DirectionalLight(int light_index, float3 vertex_position, float3 vertex_normal, float3 vertex_to_camera, float4 albedo_color)
{
    float4 return_color = float4(0, 0, 0, 0);
    
    float4 ambient_color = float4(0, 0, 0, 0);
    float4 diffuse_color = float4(0, 0, 0, 0);
    float4 specular_color = float4(0, 0, 0, 0);
    
    // Ma * (Ga + La(i))
    ambient_color = (albedo_color * g_ambient_factor) * g_lights[light_index].ambient;
    
    // Md * Ld(i) * (dot(n, Ldir(i)))
    diffuse_color = albedo_color * g_lights[light_index].diffuse * max(dot(vertex_normal, -g_lights[light_index].direction), 0.f);
    
    // Ms * Ls(i) * (dot(n, H(i))^p)
    float3 half_vector = normalize(normalize(vertex_to_camera) + -g_lights[light_index].direction);
    specular_color = float4(g_specular_color, 1.f) * g_lights[light_index].specular * pow(max(dot(vertex_normal, half_vector), 0.f), g_specular_power);

    return_color = ambient_color + diffuse_color + specular_color;
    
    return return_color;
}


float4 Lighting(float4 albedo_color, float3 vertex_position, float3 vertex_normal)
{
    float3 vertex_to_camera = g_camera_position - vertex_position;
    
    float4 return_color = float4(0.f, 0.f, 0.f, 0.f);
    for (int i = 0; i < g_light_count; ++i)
    {
        if (g_lights[i].type == POINT_LIGHT)
        {
            return_color += PointLight(i, vertex_position, vertex_normal, vertex_to_camera, albedo_color);
        }
        if (g_lights[i].type == DIRECTIONAL_LIGHT)
        {
            return_color += DirectionalLight(i, vertex_position, vertex_normal, vertex_to_camera, albedo_color);
        }
    }
    return_color += g_global_ambient * g_ambient_factor;
    return return_color;
}
