#ifndef SL_CAMERA_GLSL
#define SL_CAMERA_GLSL

#include "Shared/Standard.h"

layout(std140, binding = SL_BINDING_POINT_CAMERA) uniform UBCamera
{
    vec4 ub_cameraPos;
    mat4 ub_viewMat;
    mat4 ub_projectionMat;
    mat4 ub_viewProjctionMat;
    mat4 ub_viewMatWithoutTransform;
};

vec3 GetCameraPos()
{
    return ub_cameraPos.xyz;
}

mat4 GetViewMat()
{
    return ub_viewMat;
}

mat4 GetProjectionMat()
{
    return ub_projectionMat;
}

mat4 GetViewProjectionMat()
{
    return ub_viewProjctionMat;
}

mat4 GetViewMatWithoutTransform()
{
    return ub_viewMatWithoutTransform;
}

#endif
