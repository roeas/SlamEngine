#version 460 core

#include <Camera.glsl>

// Input
layout(location = 0) in vec3 a_position;

// Uniform
layout(location = 0) uniform mat4 u_model;

void main()
{
    gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
