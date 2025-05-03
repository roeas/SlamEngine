#ifndef SL_CAMERA_GLSL
#define SL_CAMERA_GLSL

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

#endif
