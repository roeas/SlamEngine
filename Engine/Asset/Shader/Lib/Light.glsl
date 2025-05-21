#ifndef SL_LIGHT_GLSL
#define SL_LIGHT_GLSL

#include "Shared.h"

struct Light
{
    int type;
    float range;
    float intensity;
    float scale;
    float offset;
    vec3 color;
    vec3 position;
    vec3 direction;
};

layout(std140, binding = SL_BINDING_POINT_LIGHT) uniform UBLights
{
    SL_LightUniformBuffer ub_lights[SL_LIGHT_MAX_COUNT];
    int ub_lightCount;
};

#endif
