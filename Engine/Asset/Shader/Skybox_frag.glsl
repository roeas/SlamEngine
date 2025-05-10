#version 460 core

// Input
layout(location = 0) in vec3 v_dir;

// Output
layout(location = 0) out vec4 o_color;

// Texture
layout(binding = 0) uniform samplerCube s_sky;

void main()
{
    o_color = vec4(texture(s_sky, v_dir).xyz, 1.0);
}
