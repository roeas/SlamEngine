#version 460 core

#include "Shared.h"

// Input
layout(location = 0) in vec3 v_dir;

// Output
layout(location = 0) out vec4 o_color;

// Texture
layout(binding = 0) uniform samplerCube s_skybox;

// Uniform
layout(location = SL_LOCATION_SKY_FACTOR) uniform float u_skyFactor;

void main()
{
    o_color = vec4(texture(s_skybox, v_dir).xyz * vec3(u_skyFactor), 1.0);
}
