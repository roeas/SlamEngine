#version 460 core

// Camera
layout(std140, binding = 0) uniform UBCamera
{
    vec4 ub_cameraPos;
    mat4 ub_viewProjection;
};
vec3 GetCameraPos()
{
    return ub_cameraPos.xyz;
}
mat4 GetViewProjectionMat()
{
    return ub_viewProjection;
}

// Input
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv0;

// Uniform
layout(location = 0) uniform mat4 u_model;

void main()
{
    gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
