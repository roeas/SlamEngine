#version 460 core

// Input
layout(location = 0) in vec3 v_worldPos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tangent;
layout(location = 3) in vec3 v_bitangent;
layout(location = 4) in vec2 v_uv0;

// Output
layout(location = 0) out vec4 o_color;

#include "Shared/PBRMaterial.h"

#include "Lib/Camera.glsl"
#include "Lib/Material.glsl"

void main()
{
    vec3 cameraPos = GetCameraPos();
    PBRMaterial material = GetPBRMaterial(v_uv0, v_normal, v_tangent, v_bitangent);

    vec3 directColor = material.albedo;
    directColor *= material.occlusion;

    vec3 environmentColor = vec3(0.01);

    // Post processing
    vec3 finalColor = directColor + environmentColor + material.emissive;
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    o_color = vec4(finalColor, 1.0);
}
