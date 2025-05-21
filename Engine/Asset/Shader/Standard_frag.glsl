#version 460 core

#include <Camera.glsl>
#include <Light.glsl>
#include <Material.glsl>

// Input
layout(location = 0) in vec3 v_worldPos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec3 v_bitangent;
layout(location = 4) in vec2 v_uv0;

// Output
layout(location = 0) out vec4 o_color;

void main()
{
    vec3 cameraPos = GetCameraPos();
    Material material = GetMaterial(v_uv0, v_normal, v_tangent, v_bitangent);

    // Direct
    vec3 directColor = material.albedo;
    directColor *= vec3(material.occlusion);

    // Environment
    vec3 environmentColor = vec3(0.01);

    vec3 finalColor = directColor + environmentColor + material.emissive;
    o_color = vec4(finalColor, 1.0);
}
