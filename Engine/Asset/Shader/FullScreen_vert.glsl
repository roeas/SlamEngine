#version 460 core

// Input
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_uv;

// Output
layout(location = 0) out vec2 v_uv;

void main()
{
    v_uv = a_uv;

    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
}
