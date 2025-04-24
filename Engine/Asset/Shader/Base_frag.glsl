#version 460 core

// Input
layout(location = 0) in vec2 v_uv0;

// Output
layout(location = 0) out vec4 o_color;

// Uniform
layout(binding = 0) uniform sampler2D tex_debugUV;

void main()
{
    o_color = texture(tex_debugUV, v_uv0);
}
