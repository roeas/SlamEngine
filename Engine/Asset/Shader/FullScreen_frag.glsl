#version 460 core

// Input
layout(location = 0) in vec2 v_uv;

// Output
layout(location = 0) out vec4 o_color;

// Texture
layout(binding = 0) uniform sampler2D s_preResult;

void main()
{
    o_color = vec4(texture(s_preResult, v_uv).xyz, 1.0);
}
