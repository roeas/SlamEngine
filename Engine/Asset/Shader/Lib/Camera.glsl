#ifndef SL_CAMERA_GLSL
#define SL_CAMERA_GLSL

#include "Shared.h"

layout(std140, binding = SL_BINDING_POINT_CAMERA) uniform UBCamera
{
    vec4 ub_cameraPos;
    mat4 ub_viewMat;
    mat4 ub_projectionMat;
};

vec3 GetCameraPos()
{
    return ub_cameraPos.xyz;
}

mat4 GetViewMat()
{
    return ub_viewMat;
}

mat4 GetViewMatWithoutTransform()
{
    return mat4(ub_viewMat[0], ub_viewMat[1], ub_viewMat[2], vec4(0, 0, 0, 1));
}

mat4 GetProjectionMat()
{
    return ub_projectionMat;
}

mat4 GetViewProjectionMat()
{
    return ub_projectionMat * ub_viewMat;
}

#endif
