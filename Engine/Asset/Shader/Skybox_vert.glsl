#version 460 core

#include <Camera.glsl>

// Input
layout(location = 0) in vec3 a_position;

// Output
layout(location = 0) out vec3 v_dir;

void main()
{
    v_dir = a_position;

    gl_Position = GetProjectionMat() * GetViewMatWithoutTransform() * vec4(a_position, 1.0);
}
