#version 460 core

#include <Camera.glsl>

// Input
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_uv0;

// Output
layout(location = 0) out vec3 v_worldPos;
layout(location = 1) out vec3 v_normal;
layout(location = 2) out vec3 v_tangent;
layout(location = 3) out vec3 v_bitangent;
layout(location = 4) out vec2 v_uv0;

// Uniform
layout(location = SL_LOCATION_MODEL) uniform mat4 u_model;
layout(location = SL_LOCATION_MODEL_INV_TRANS) uniform mat4 u_modelInvTrans;

void main()
{
    v_worldPos = (u_model * vec4(a_position, 1.0)).xyz;
    v_normal = normalize((u_modelInvTrans * vec4(a_normal, 0.0)).xyz);
    v_tangent = normalize((u_modelInvTrans * vec4(a_tangent, 0.0)).xyz);
    v_bitangent = normalize(cross(v_tangent, v_normal));
    v_uv0 = a_uv0;

    gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
}
